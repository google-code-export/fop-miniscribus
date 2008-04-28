#ifndef RICHDOC_STRUCTURE_H
#define RICHDOC_STRUCTURE_H


#include <QtGui>
#include <QtCore>





class RichDoc
{
    public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };
    RichDoc() {
    html = QByteArray("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Your text</p>");
		style = QString("min-height:300px;padding:0px 0px 0px 0px;background-color:#ffffff;");
    }
    RichDoc& operator=( const RichDoc& d )
    {
      resource.clear();
      resource = d.resource;
      html = d.html;
      style = d.style;
    }
    operator QVariant() const {
    return QVariant::fromValue(*this);
    }
    
    void Register( QTextDocument *doc , QMap<QString,SPics> list , QString s )
    {
      style = s;
      html.clear();
      html.append(doc->clone()->toHtml("utf-8"));
      resource.clear();
			if (list.size() > 0) {
      resource = list;
			}
    }
		
		void Register( const QString xhtml , QMap<QString,SPics> list , QString s )
    {
      style = s;
      html.clear();
      html.append(xhtml);
			resource.clear();
			if (list.size() > 0) {
      resource = list;
			}
    }
    QString Hxtml()
		{
		return QString(html.constData());
		}
		/*
		QString WebSource()
		{
			QString base = Hxtml();
			QdocXhtml *handler = new QdocXhtml(todoc());
			base = handler->Docxml();
			base.prepend(QString("<div class=\"qtpage\" xmlns=\"http://www.w3.org/1999/xhtml\" style=\"%1\">\n").arg(style));
			base.append(QString("\n</div>\n"));
			return base;
		}
		*/
    QTextDocument *todoc()
    {
			  QTextCursor helper;
        QTextDocument *d = new QTextDocument();
        d->setHtml(QString(html));
			  helper = QTextCursor(d);
			  ////////QMapIterator<QString,SPics> i(resource);
		     ////////while (i.hasNext()) {
           /////////i.next();
				   ////////////SPics record = i.value();
				   //////////////d->addResource( QTextDocument::ImageResource,QUrl(record.name),record.pix());
         ////////}
				 QTextFrame  *Tframe = d->rootFrame();
				 QTextFrame::iterator it;
         for (it = Tframe->begin(); !(it.atEnd()); ++it) {
					 QTextBlock para = it.currentBlock();
					 if (para.isValid()) {
						QTextBlockFormat Parformat = para.blockFormat();
						if (Parformat.bottomMargin() == 12 && Parformat.topMargin() == 12) {
							////////Parformat.setBottomMargin(0); 
              //////////Parformat.setTopMargin(0);
              /////////Parformat.setRightMargin(0);
              ///////////Parformat.setLeftMargin(0);
							////////////helper.setPosition(para.position(),QTextCursor::MoveAnchor);
							//////////////helper.setBlockFormat(Parformat);
						}
						 
						 
							QTextBlock::iterator de;
										for (de = para.begin(); !(de.atEnd()); ++de) {
												QTextFragment fr = de.fragment();
											  if (fr.isValid()) {
                           QTextCharFormat base = fr.charFormat();
                           QTextImageFormat pico = base.toImageFormat();
													      if (pico.isValid()) {
																		const QString hrefadress = pico.name();
																		SPics spico = resource[hrefadress];
																		/////////qDebug() << "### from doc hrefadress " << hrefadress;
																		/////////////qDebug() << "### from doc hrefadress " << spico.info;
																		pico.setToolTip(spico.info);
																		pico.setProperty(100,spico);
																	  ///////base.setProperty(100,spico);
																	  ///////base.setToolTip(spico.info);
																	  ////////////helper = QTextCursor(d);
																		helper.setPosition(fr.position());
                                    helper.setPosition(fr.position() + fr.length(),QTextCursor::KeepAnchor);
                                    helper.setCharFormat(pico);
																	  d->addResource( QTextDocument::ImageResource,QUrl(spico.name),spico.pix());
																}
												}
																 
										}
						 
					 }
			
		     }
				 

        return d;
    }
    QString style;
    QMap<QString,SPics> resource;
    QByteArray html;  /* qCompress */
    
};


Q_DECLARE_METATYPE(RichDoc);


inline QString SaveRichDoc( RichDoc e )
{
	///////////qDebug() << "#### SaveRichDoc " << e.todoc()->toPlainText();
	
	QByteArray bytes;
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::WriteOnly)) {
		/////////qDebug() << "#### SaveRichDoc  no buffer fatal error!";
		return QString();
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds << (quint32)RichDoc::MAGICNUMBER;
	ds << (quint32)RichDoc::VERSION;
	/* place header */
  ds << e.html;
  ds << e.style;
             
             QMapIterator<QString,SPics> i(e.resource);
             while (i.hasNext()) {
                 i.next();
				         SPics record = i.value();
                 ds << record;
             } 
	buffer.close();
	return bytes.toBase64();
}

/* decoded base64 stream to put on mysql row , file or network streams */
inline RichDoc OpenRichDoc( const QString datastream_base64   )
{
	
	//////////qDebug() << "### OpenRichDoc size " << datastream_base64.size();
	
	
  RichDoc li;
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
	if ( (quint32)RichDoc::MAGICNUMBER != magic ) {
  qDebug() << "######## RichDoc::MAGICNUMBER not ok  " << magic;
	buffer.close();
	return li;
	}
	ds >> version;
	if ( (quint32)RichDoc::VERSION != version ) {
	qDebug() << "######## RichDoc::VERSION not ok  " << version;
	buffer.close();
	return li;
	}
  
  QString s;
  QByteArray h;
  ds >> h;
  ds >> s;
  li.style = s;
  li.html = h;
  SPics appoint;
    while (!ds.atEnd()) {
         ds >> appoint;
         li.resource.insert(appoint.name,appoint);
    }
	buffer.close();
	return li;
}






#endif // RICHDOC_STRUCTURE_H

