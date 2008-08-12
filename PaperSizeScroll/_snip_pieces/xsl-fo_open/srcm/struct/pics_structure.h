/*******************************************************************************
 * class SPics
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
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

/* pic structure and flash structure in qtextdocument */

#ifndef PICS_STRUCTURE_H
#define PICS_STRUCTURE_H
#include <QtGui>
#include <QtCore>
#include <QPixmap>
#include <QSvgRenderer>
#include <QTextDocumentFragment>
#include <QTextCharFormat>
#include <QAbstractTextDocumentLayout> // for QTextObjectInterface
#include <QDomDocument>
#include "Basic_Config.h"
#include "Fop_Leader_Element.h"


#define FOPIMAGEDIR "Pictures/"



static const int ObjectName = 853;
static const int FlashReferenceID = 322;

#define _LINK_COLOR_ \
             QColor("#dc0000")
             
#define _DEFAULT_TXT_COLOR_ \
             QColor("#000088")   /* cursor color palette from text now is cursor color */

#define _DOCUMENT_TXT_COLOR_ \
             QColor("#717171")    /* real txt palette */





static inline  QIcon createColorIcon( QColor color )
{
    QPixmap pixmap(50, 50);
    pixmap.fill(color);
    return QIcon(pixmap);
}


/* contains link on block */
static inline bool HavingLink(  const QTextBlock para )
{
        QTextBlock::iterator li;
				for (li = para.begin(); !(li.atEnd()); ++li) {
					      QTextFragment lifrag = li.fragment();
                if (lifrag.isValid()) {
                       const QTextCharFormat format = lifrag.charFormat();
                       if (format.isAnchor()) {
                        return true;
                       }
								}
				}
        return false;
}








static inline QPixmap TestImage( QString txt )
{
            QPixmap pError = QPixmap(500, 300 );
            pError.fill( Qt::red  );
            QPainter pter( &pError );
            pter.setFont( QFont( "Helvetica", 8 ) );
            pter.setBrush( Qt::green );
            pter.drawText( 5, 12 , txt );
            return pError;
}




static inline QTextBlockFormat default_block_style( QTextBlockFormat pf = QTextBlockFormat() )
{
 /* i not find on qt to remove default 12point default space !!! */
 pf.setBottomMargin(0);
 pf.setTopMargin(0);
 pf.setRightMargin(0);
 pf.setIndent(0);
 pf.setLeftMargin(0);
 pf.setProperty(FlashReferenceID,0);
 pf.setProperty(ObjectName,0);
 return pf;
}

static inline QTextCharFormat default_char_style( QTextCharFormat pf = QTextCharFormat() )
{
	  QFont font = QApplication::font();
    font.setPointSize(10);
	  pf.setFont ( font );
		pf.setProperty(FlashReferenceID,0);
    pf.setProperty(ObjectName,0);
	  pf.setVerticalAlignment ( QTextCharFormat::AlignTop );
    return pf;
}



static inline QRectF M_PagesizeMake( QPrinter::PageSize psize , bool landscape = true )
{
	QPrinter *print = new QPrinter(QPrinter::HighResolution);
	print->setFullPage(true);
	print->setPageSize(psize);
	if (landscape) {
  print->setOrientation(QPrinter::Landscape);
	} else {
	print->setOrientation(QPrinter::Portrait);
	}
	QRectF pagere = print->pageRect ( QPrinter::Point );
	delete print;
	return pagere;
}


class M_PageSize
{
    public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };
    M_PageSize() {
		landscape = false;
    name = "A4 (210 x 297 mm, 8.26 x 11.7 inches)";
    P_rect = QPrinter::A4;
    G_regt = M_PagesizeMake(P_rect,landscape);
		const qreal mr = FopInt("1cm");
		P_margin = QRectF(mr,mr,mr,mr);  
    }
		M_PageSize& operator=( const M_PageSize& d )
    {
      name = d.name;
      landscape = d.landscape;
      G_regt = d.G_regt;
      P_rect = d.P_rect;
			P_margin = d.P_margin;
      return *this;
    }
		
		operator QVariant() const {
    return QVariant::fromValue(*this);
    }
		QString HName()
		{
			QString sep(" /");
			QString reg = name;
			if (landscape) {
			reg.append(sep+QT_TR_NOOP("Landscape"));
			} else {
			reg.append(sep+QT_TR_NOOP("Portrait"));
			}
			return reg;
		}
		qreal faktor()
		{
     return qMax(G_regt.width(),G_regt.height()) / qMin(G_regt.width(),G_regt.height());
		}
		void Register( QString n , QPrinter::PageSize pp , bool La )
		{
			G_regt = M_PagesizeMake(P_rect,La);
			//////qDebug() << "### Register page  name." << n << " G_regt ->" << G_regt; 
			name = n;
			landscape = La;
			P_rect = pp;
			
		}
		void SetMargin( QRectF rectp )
		{
			P_margin = rectp;
		}
		/*  edit modus */
		qreal HandleDocument( QTextDocument *doc )
		{
			const qreal RightMargin = P_margin.y();
			const qreal LeftMargin = P_margin.height();
			const qreal LargeDoc = G_regt.width() - RightMargin  - LeftMargin;
			///////.toSize()
			doc->setPageSize ( G_regt.size() );
			////////////doc->setTextWidth ( G_regt.width() * 1.5 );
			QTextFrame  *Tframe = doc->rootFrame();
	    QTextFrameFormat Ftf = Tframe->frameFormat();
	    //////Ftf.setLeftMargin(P_margin.height());
	    //////Ftf.setBottomMargin(P_margin.width());
	    //////Ftf.setTopMargin(P_margin.x());
			///////Ftf.setRightMargin(P_margin.y());
	    ///////Ftf.setPadding ( 0);
			Ftf.setLeftMargin(0);
	    Ftf.setBottomMargin(0);
	    Ftf.setTopMargin(0);
			Ftf.setRightMargin(0);
	    Ftf.setPadding (0);
			Ftf.setWidth ( G_regt.width() * 1.5 );
			Tframe->setFrameFormat(Ftf);
			
			doc->adjustSize();
			
			return LargeDoc;
		}
			qreal HandlePrint( QTextDocument *doc )
		{
			const qreal RightMargin = P_margin.y();
			const qreal LeftMargin = P_margin.height();
			const qreal LargeDoc = G_regt.width() - RightMargin  - LeftMargin;
			///////.toSize()
			doc->setPageSize ( G_regt.size() );
			//////doc->setTextWidth ( G_regt.width() * 1.5 );
			QTextFrame  *Tframe = doc->rootFrame();
	    QTextFrameFormat Ftf = Tframe->frameFormat();
	    Ftf.setLeftMargin(P_margin.height());
	    Ftf.setBottomMargin(P_margin.width());
	    Ftf.setTopMargin(P_margin.x());
			Ftf.setRightMargin(P_margin.y());
	    Ftf.setPadding ( 0);
			Tframe->setFrameFormat(Ftf);
			return LargeDoc;
		}
		void ReportPage( QDomElement e )
		{
		const qreal TopMargin = Pointo(P_margin.x(),"mm");
		const qreal RightMargin = Pointo(P_margin.y(),"mm");
		const qreal BottomMargin = Pointo(P_margin.width(),"mm");
		const qreal LeftMargin = Pointo(P_margin.height(),"mm");
		e.setAttribute ("margin-top",QString("%1mm").arg(TopMargin));
    e.setAttribute ("margin-bottom",QString("%1mm").arg(BottomMargin));
    e.setAttribute ("margin-left",QString("%1mm").arg(LeftMargin));
    e.setAttribute ("margin-right",QString("%1mm").arg(RightMargin));
    e.setAttribute ("page-width",QString("%1mm").arg(Pointo(G_regt.width(),"mm")));
    e.setAttribute ("page-height",QString("%1mm").arg(Pointo(G_regt.height(),"mm")));
		}
		
/////////  MarginPage = QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
QPrinter::PageSize P_rect;
QRectF G_regt;
QRectF P_margin;
QString name;
bool landscape;
};

Q_DECLARE_METATYPE(M_PageSize);


class SPics
{
    public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 1 };
    SPics() {
    name = "error";
    info = "Image Description to blind people";
    extension = QByteArray("PNG");
    }
    
    QString MimeHard() const
    {
      if (extension == "PNG") {
        return QString("image/png");
      } else if (extension == "APNG") {
        return QString("image/png");
      } else if (extension == "JPG") {
        return QString("image/jpg");
      } else {
        return QString("text/plain");
      }
      
    }
    
    SPics& operator=( const SPics& d )
    {
      name = d.name;
      info = d.info;
      extension = d.extension;
      data = d.data;
      return *this;
    }
    bool operator!=( const SPics& d )
    {
        if (name != d.name ) {
        return false;
        } else {
        return true;
        }
    }
    operator QVariant() const {
    return QVariant::fromValue(*this);
    }
    QPixmap erno_pix() {
        
            QPixmap pError = QPixmap(20, 20 );
            pError.fill( Qt::red  );
            QPainter pter( &pError );
            pter.setFont( QFont( "Helvetica", 8 ) );
            pter.setBrush( Qt::green );
            pter.drawText( 5, 12 , "0" );
            return pError;
    }
    QString web()
    {
         QString flusches;
         if (data.size() < 1) {
         return flusches;
         }
         QByteArray daunq = qUncompress( data );
         return daunq.toBase64();
    }
		bool FopSaveImage( QDomElement e )
		{
			const QString dirref = QString("./%1%2").arg(FOPIMAGEDIR).arg(FileName());
			e.setAttribute ("src",dirref); /* current path from export now! */
			QFileInfo gosave(dirref);
			Cache( gosave.absolutePath() );
			  QFile f( gosave.absoluteFilePath() );
        if ( f.open( QIODevice::WriteOnly ) )
        {
          f.write ( qUncompress( data ) );
          f.close();   
          return true;					
        }
			return false;	
		}
    
    QPixmap pix() {
      if (data.size() < 1) {
      return erno_pix();
      }
      QPixmap resultimage;
      QByteArray daunq = qUncompress( data ); 
	    resultimage.loadFromData( daunq );
          if (resultimage.isNull()) {
            return erno_pix();
          }
          
	   return resultimage;
    }
    
    QString FileName()
    {
    return Imagename(name) + "." + QString(extension.data()).toLower();
    }
    void SavePixThread( QString dir = QString() )
    {
        QString fullpath = dir + FileName();
        QDir dira(dir);
        if ( !dira.mkpath(dir) ) {
         return;
        }
        QFile f( fullpath );
        if ( f.open( QIODevice::WriteOnly ) )
        {
          f.write ( qUncompress( data ) );
          f.close();          
        }
    }
    void set_pics( const QPixmap * barcode )
    {
      if (barcode->isNull()) {
        return;
      }
      QByteArray bytes;
      QBuffer buffer(&bytes);
      buffer.open(QIODevice::WriteOnly);
      barcode->save(&buffer,extension.data());
      data = qCompress(bytes,9);
    }
    void set_pics( QPixmap barcode )
    {
      if (barcode.isNull()) {
        return;
      }
      QByteArray bytes;
      QBuffer buffer(&bytes);
      buffer.open(QIODevice::WriteOnly);
      barcode.save(&buffer,extension.data());
      data = qCompress(bytes,9);
    }
    void set_pics( const QByteArray chunk )
    {
       data = qCompress(chunk,9);
    }
    QByteArray streams()
    {
      return qUncompress( data ); 
    }
    int picskilo() {
    return data.size();
    }
    QUrl indoc()
    {
      const int grep = name.size() -  name.indexOf(".");
      QString webname = name.left(grep);
      return QUrl(QString("./%1.%2").arg(name).arg(QString(extension.data()).toLower()));  
    }
    
    
    /* vars permanent */
    QString name;
    QString info;
    QByteArray extension;
    QByteArray data;  /* qCompress */
};

Q_DECLARE_METATYPE(SPics);

inline QDebug operator<<(QDebug debug, SPics& udoc)
{
	debug.nospace() << "SPics(name." 
		<< udoc.name << "," 
    << udoc.info << ",size()"
    << udoc.data.size() << ",type()"
    << udoc.extension << ")";
	return debug.space();
}

inline QDataStream& operator<<(QDataStream& out, const SPics& udoc)
{
    out << udoc.name;
    out << udoc.info;
    out << udoc.extension;
    out << udoc.data;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, SPics& udoc)
{
    in >> udoc.name;
    in >> udoc.info;
    in >> udoc.extension;
    in >> udoc.data;
    return in;
}




inline QString SaveImageGroup( QList<SPics> li  )
{
   if (li.size() < 1) {
     return QString();  
   }
	QByteArray bytes;
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::WriteOnly)) {
		return QString();
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds << (quint32)SPics::MAGICNUMBER;
	ds << (quint32)SPics::VERSION;
	/* place header */
             ///////QApplication::setOverrideCursor(Qt::WaitCursor);
             for (int i=0; i<li.size(); i++) {
                 SPics conni = li[i];
                 ds << conni;
             }   
             //////QApplication::restoreOverrideCursor();
	buffer.close();
	return bytes.toBase64();
}

/* decoded base64 stream to put on mysql row , file or network streams */
inline QList<SPics> OpenImageGroup( const QString datastream_base64   )
{
  
  
  QList<SPics> li;
	QByteArray xcode("");
	xcode.append(datastream_base64);
	quint32 magic, version;
	QByteArray bytes(QByteArray::fromBase64(xcode));   /* decoded base64 string to QByteArray */
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::ReadOnly)) {
		return li;
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds >> magic;
	if ( (quint32)SPics::MAGICNUMBER != magic ) {
    qDebug() << "######## SPics::MAGICNUMBER not ok  ";
		buffer.close();
		return li;
	}
	ds >> version;
	if ( (quint32)SPics::VERSION != version ) {
		qDebug() << "######## SPics::VERSION not ok  ";
		buffer.close();
		return li;
	}
  SPics appoint;
	/* place header */
    while (!ds.atEnd()) {
         ds >> appoint;
         li.append(appoint);
    }
	buffer.close();
	return li;
}



static inline QPixmap RenderPixmapFromSvgByte(  QByteArray streams ) 
{
        QSvgRenderer  svgRenderer( streams );
        QPixmap pix(  svgRenderer.defaultSize() );
        pix.fill(Qt::transparent);
        QPainter paint(&pix);
        svgRenderer.render(&paint);
        return pix;
}


static inline QPixmap RenderPixmapFromSvgByte(  const QString & filename ) 
{
        QSvgRenderer  svgRenderer;
				if ( svgRenderer.load(filename)) {
        QPixmap pix(  svgRenderer.defaultSize() );
        pix.fill(Qt::transparent);
        QPainter paint(&pix);
        svgRenderer.render(&paint);
        return pix;
				 } else {
					  SPics xx;
					  return xx.erno_pix();
				 }
}



#endif // PICS_STRUCTURE_H

