/*******************************************************************************
 * class OO_Handler
 *******************************************************************************
 * Copyright (C) 2008 by Peter Hohl  Porting to QT4
  * e-Mail: ppkciz@gmail.com   www.qmake.net
 * Copyright (C) 2006 by Tobias Koenig <tokoe@kde.org>  KDE Okular
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/


#include "oo_handler.h"

using namespace OOO;


OO_Handler::OO_Handler( const QString filedoc )
 : havingdoc(false),mTextDocument( 0 ), mCursor( 0 ),
    mStyleInformation( new StyleInformation )
{
	OpenFile(filedoc);
	///////qDebug() << "### Start open file -> " << filedoc;
	qDebug() << "### ooFile.size() " << ooFile.size();
	if (havingdoc) {
		/* parse start */
		///////ParseFiles();
	}
}

bool OO_Handler::ParseFiles()
{
	 Document oooDocument;
	 if (!filelist.contains("content.xml")) {
		setError( tr( "Invalid document structure (content.xml is missing)" ) );
		return false;
	 }
	oooDocument.mContent = ooFile["content.xml"];
	 
	if ( !filelist.contains( "styles.xml" ) ) {
    setError( tr( "Invalid document structure (styles.xml is missing)" ) );
    return false;
  }
	oooDocument.mStyles = ooFile["styles.xml"];
	
	if ( !filelist.contains( "meta.xml" ) ) {
    setError( tr( "Invalid document structure (meta.xml is missing)" ) );
    return false;
  }
	oooDocument.mMeta = ooFile["meta.xml"];
	
	
	mTextDocument = new QTextDocument();
  mCursor = new QTextCursor( mTextDocument );
	
	QXmlSimpleReader reader;
  QXmlInputSource source;
  source.setData( ooFile["content.xml"] );
	QString errorMsg;
  QDomDocument document;
  if ( !document.setContent( &source, &reader, &errorMsg ) ) {
    setError( tr( "Invalid XML document: %1").arg( errorMsg ));
    delete mCursor;
    return false;
  }
	
	StyleParser styleParser( &oooDocument, document, mStyleInformation );
	
	
	//////log.append(document.toString(1));
	/* append images */
  QMapIterator<QString, QByteArray> it( ooFile );
  while ( it.hasNext() ) {
     it.next();
     if (it.key().contains( "Pictures" )) {
     mTextDocument->addResource( QTextDocument::ImageResource, QUrl( it.key() ), QImage::fromData( it.value() ) );
     ooimage.insert(it.key(),it.value());
		 }
  }
	
	/**
   * Set the correct page size
	*/
  const QString masterLayout = mStyleInformation->masterPageName();
  const PageFormatProperty property = mStyleInformation->pageProperty( masterLayout );
  mTextDocument->setPageSize( QSize( qRound( property.width() ), qRound( property.height() ) ) );

  QTextFrameFormat frameFormat;
  frameFormat.setMargin( qRound( property.margin() ) );

  QTextFrame *rootFrame = mTextDocument->rootFrame();
  rootFrame->setFrameFormat( frameFormat );
	
	
  /**
   * Parse the content of the document
   */
  const QDomElement documentElement = document.documentElement();

  QDomElement element = documentElement.firstChildElement();
  while ( !element.isNull() ) {
    if ( element.tagName() == QLatin1String( "body" ) ) {
      if ( !convertBody( element ) ) {
        setError( tr( "Unable to convert document content" ));
        delete mCursor;
        return false;
      }
    }

    element = element.nextSiblingElement();
  }
  /*
  MetaInformation::List metaInformation = mStyleInformation->metaInformation();
  for ( int i = 0; i < metaInformation.count(); ++i ) {
    emit addMetaData( metaInformation[ i ].key(),
                      metaInformation[ i ].value(),
                      metaInformation[ i ].title() );
  }
  */
  delete mCursor;
	
	//////log.append(mTextDocument->toHtml());
	 
	 /*
	 
	 mimetype
Configurations2/accelerator/current.xml
content.xml
styles.xml
meta.xml
Thumbnails/thumbnail.png
settings.xml
META-INF/manifest.xml
	 
	 */
	 return true;
}


bool OO_Handler::convertBody( const QDomElement &element )
{
  QDomElement child = element.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == QLatin1String( "text" ) ) {
      if ( !convertText( child ) )
        return false;
    }

    child = child.nextSiblingElement();
  }

  return true;
}








bool OO_Handler::convertText( const QDomElement &element )
{
  QDomElement child = element.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == QLatin1String( "p" ) ) {
      mCursor->insertBlock();
      if ( !convertParagraph( mCursor, child ) )
        return false;
    } else if ( child.tagName() == QLatin1String( "h" ) ) {
      mCursor->insertBlock();
      if ( !convertHeader( mCursor, child ) )
        return false;
    } else if ( child.tagName() == QLatin1String( "list" ) ) {
      if ( !convertList( child ) )
        return false;
    } else if ( child.tagName() == QLatin1String( "table" ) ) {
      if ( !convertTable( child ) )
        return false;
    }

    child = child.nextSiblingElement();
  }

  return true;
}

bool OO_Handler::convertHeader( QTextCursor *cursor, const QDomElement &element )
{
  const QString styleName = element.attribute( "style-name" );
  const StyleFormatProperty property = mStyleInformation->styleProperty( styleName );

  QTextBlockFormat blockFormat;
  QTextCharFormat textFormat;
  property.applyBlock( &blockFormat );
  property.applyText( &textFormat );

  cursor->setBlockFormat( blockFormat );

  QDomNode child = element.firstChild();
  while ( !child.isNull() ) {
    if ( child.isElement() ) {
      const QDomElement childElement = child.toElement();
      if ( childElement.tagName() == QLatin1String( "span" ) ) {
        if ( !convertSpan( cursor, childElement, textFormat ) )
          return false;
      }
    } else if ( child.isText() ) {
      const QDomText childText = child.toText();
      if ( !convertTextNode( cursor, childText, textFormat ) )
        return false;
    }

    child = child.nextSibling();
  }

  /////////////emit addTitle( element.attribute( "outline-level", 0 ).toInt(), element.text(), cursor->block() );

  return true;
}

bool OO_Handler::convertParagraph( QTextCursor *cursor, const QDomElement &element, const QTextBlockFormat &parentFormat, bool merge )
{
  const QString styleName = element.attribute( "style-name" );
  const StyleFormatProperty property = mStyleInformation->styleProperty( styleName );

  QTextBlockFormat blockFormat( parentFormat );
  QTextCharFormat textFormat;
  property.applyBlock( &blockFormat );
  property.applyText( &textFormat );

  if ( merge )
    cursor->mergeBlockFormat( blockFormat );
  else
    cursor->setBlockFormat( blockFormat );

  QDomNode child = element.firstChild();
  while ( !child.isNull() ) {
    if ( child.isElement() ) {
      const QDomElement childElement = child.toElement();
      if ( childElement.tagName() == QLatin1String( "span" ) ) {
        if ( !convertSpan( cursor, childElement, textFormat ) )
          return false;
      } else if ( childElement.tagName() == QLatin1String( "tab" ) ) {
        mCursor->insertText( "    " );
      } else if ( childElement.tagName() == QLatin1String( "s" ) ) {
        QString spaces;
        spaces.fill( ' ', childElement.attribute( "c" ).toInt() );
        mCursor->insertText( spaces );
      } else if ( childElement.tagName() == QLatin1String( "frame" ) ) {
        if ( !convertFrame( childElement ) )
          return false;
      } else if ( childElement.tagName() == QLatin1String( "a" ) ) {
        if ( !convertLink( cursor, childElement, textFormat ) )
          return false;
      } else if ( childElement.tagName() == QLatin1String( "annotation" ) ) {
        if ( !convertAnnotation( cursor, childElement ) )
          return false;
      }
    } else if ( child.isText() ) {
      const QDomText childText = child.toText();
      if ( !convertTextNode( cursor, childText, textFormat ) )
        return false;
    }

    child = child.nextSibling();
  }

  return true;
}

bool OO_Handler::convertTextNode( QTextCursor *cursor, const QDomText &element, const QTextCharFormat &format )
{
  cursor->insertText( element.data(), format );

  return true;
}

bool OO_Handler::convertSpan( QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format )
{
  const QString styleName = element.attribute( "style-name" );
  const StyleFormatProperty property = mStyleInformation->styleProperty( styleName );

  QTextCharFormat textFormat( format );
  property.applyText( &textFormat );

  QDomNode child = element.firstChild();
  while ( !child.isNull() ) {
    if ( child.isText() ) {
      const QDomText childText = child.toText();
      if ( !convertTextNode( cursor, childText, textFormat ) )
        return false;
    }

    child = child.nextSibling();
  }

  return true;
}

bool OO_Handler::convertList( const QDomElement &element )
{
  const QString styleName = element.attribute( "style-name" );
  const ListFormatProperty property = mStyleInformation->listProperty( styleName );

  QTextListFormat format;

  if ( mCursor->currentList() ) { // we are in a nested list
    format = mCursor->currentList()->format();
    format.setIndent( format.indent() + 1 );
  }

  property.apply( &format, 0 );

  QTextList *list = mCursor->insertList( format );

  QDomElement itemChild = element.firstChildElement();
  int loop = 0;
  while ( !itemChild.isNull() ) {
    if ( itemChild.tagName() == QLatin1String( "list-item" ) ) {
      loop++;

      QDomElement childElement = itemChild.firstChildElement();
      while ( !childElement.isNull() ) {

        QTextBlock prevBlock;

        if ( childElement.tagName() == QLatin1String( "p" ) ) {
          if ( loop > 1 )
            mCursor->insertBlock();

          prevBlock = mCursor->block();

          if ( !convertParagraph( mCursor, childElement, QTextBlockFormat(), true ) )
            return false;

        } else if ( childElement.tagName() == QLatin1String( "list" ) ) {
          prevBlock = mCursor->block();

          if ( !convertList( childElement ) )
            return false;
        }

        list->add( prevBlock );

        childElement = childElement.nextSiblingElement();
      }
    }

    itemChild = itemChild.nextSiblingElement();
  }

  return true;
}

bool OO_Handler::convertTable( const QDomElement &element )
{
  /**
   * Find out dimension of the table
   */
  QDomElement rowElement = element.firstChildElement();

  int rowCounter = 0;
  int columnCounter = 0;
  while ( !rowElement.isNull() ) {
    if ( rowElement.tagName() == QLatin1String( "table-row" ) ) {
      rowCounter++;

      int counter = 0;
      QDomElement columnElement = rowElement.firstChildElement();
      while ( !columnElement.isNull() ) {
        if ( columnElement.tagName() == QLatin1String( "table-cell" ) ) {
          counter++;
        }
        columnElement = columnElement.nextSiblingElement();
      }

      columnCounter = qMax( columnCounter, counter );
    }

    rowElement = rowElement.nextSiblingElement();
  }

  /**
   * Create table
   */
  QTextTable *table = mCursor->insertTable( rowCounter, columnCounter );

  /**
   * Fill table
   */
  rowElement = element.firstChildElement();

  QTextTableFormat tableFormat;

  rowCounter = 0;
  while ( !rowElement.isNull() ) {
    if ( rowElement.tagName() == QLatin1String( "table-row" ) ) {

      int columnCounter = 0;
      QDomElement columnElement = rowElement.firstChildElement();
      while ( !columnElement.isNull() ) {
        if ( columnElement.tagName() == QLatin1String( "table-cell" ) ) {
          const StyleFormatProperty property = mStyleInformation->styleProperty( columnElement.attribute( "style-name" ) );

          QTextBlockFormat format;
          property.applyTableCell( &format );

          QDomElement paragraphElement = columnElement.firstChildElement();
          while ( !paragraphElement.isNull() ) {
            if ( paragraphElement.tagName() == QLatin1String( "p" ) ) {
              QTextTableCell cell = table->cellAt( rowCounter, columnCounter );
              QTextCursor cursor = cell.firstCursorPosition();
              cursor.setBlockFormat( format );

              if ( !convertParagraph( &cursor, paragraphElement, format ) )
                return false;
            }

            paragraphElement = paragraphElement.nextSiblingElement();
          }
          columnCounter++;
        }
        columnElement = columnElement.nextSiblingElement();
      }

      rowCounter++;
    }

    if ( rowElement.tagName() == QLatin1String( "table-column" ) ) {
      const StyleFormatProperty property = mStyleInformation->styleProperty( rowElement.attribute( "style-name" ) );
      property.applyTableColumn( &tableFormat );
    }

    rowElement = rowElement.nextSiblingElement();
  }

  table->setFormat( tableFormat );

  return true;
}

bool OO_Handler::convertFrame( const QDomElement &element )
{
  QDomElement child = element.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == QLatin1String( "image" ) ) {
      const QString href = child.attribute( "href" );
      QTextImageFormat format;
      format.setWidth( StyleParser::convertUnit( element.attribute( "width" ) ) );
      format.setHeight( StyleParser::convertUnit( element.attribute( "height" ) ) );
      format.setName( href );

      mCursor->insertImage( format );
    }

    child = child.nextSiblingElement();
  }

  return true;
}

bool OO_Handler::convertLink( QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format )
{
  int startPosition = cursor->position();

  QDomNode child = element.firstChild();
  while ( !child.isNull() ) {
    if ( child.isElement() ) {
      const QDomElement childElement = child.toElement();
      if ( childElement.tagName() == QLatin1String( "span" ) ) {
        if ( !convertSpan( cursor, childElement, format ) )
          return false;
      }
    } else if ( child.isText() ) {
      const QDomText childText = child.toText();
      if ( !convertTextNode( cursor, childText, format ) )
        return false;
    }

    child = child.nextSibling();
  }

  int endPosition = cursor->position();
  /* not needed on qtextbrowser */
  ////////Okular::Action *action = new Okular::BrowseAction( element.attribute( "href" ) );
  ////////////emit addAction( action, startPosition, endPosition );

  return true;
}

bool OO_Handler::convertAnnotation( QTextCursor *cursor, const QDomElement &element )
{
  QStringList contents;
  QString creator;
  QDateTime dateTime;

  int position = cursor->position();

  QDomElement child = element.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == QLatin1String( "creator" ) ) {
      creator = child.text();
    } else if ( child.tagName() == QLatin1String( "date" ) ) {
      dateTime = QDateTime::fromString( child.text(), Qt::ISODate );
    } else if ( child.tagName() == QLatin1String( "p" ) ) {
        contents.append( child.text() );
    }

    child = child.nextSiblingElement();
  }

  ///////Okular::TextAnnotation *annotation = new Okular::TextAnnotation;
  ///////annotation->setAuthor( creator );
  //////annotation->setContents( contents.join( "\n" ) );
  /////annotation->setCreationDate( dateTime );
  ////////annotation->style().setColor( QColor( "#ffff00" ) );
  //////annotation->style().setOpacity( 0.5 );

  //////////emit addAnnotation( annotation, position, position + 3 );

  return true;
}





void OO_Handler::setError( QString msg )
{
	log.append(msg);
	emit errorFound(msg);
}
QString OO_Handler::text()
{
	return log;
}


void OO_Handler::OpenFile( const QString filedoc )
{
	  ooFile.clear();
	  QuaZip zip(filedoc);
	  log = "";
    zip.open(QuaZip::mdUnzip);
    QuaZipFile file(&zip);
	  for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile()) {
			     file.open(QIODevice::ReadOnly);
           const QString name = zip.getCurrentFileName();
			     if (!name.endsWith("/")) {
			     log.append(name+"\n");
					 filelist.append(name);
					 QByteArray fean = file.readAll();
					 ooFile.insert(name,fean);
					 ///////////qDebug() << "### name " << name << "-" << fean.size();
					 }
			file.close();
		}
		zip.close();
		if (ooFile.size() > 0) {
			havingdoc = true;
		}
}












