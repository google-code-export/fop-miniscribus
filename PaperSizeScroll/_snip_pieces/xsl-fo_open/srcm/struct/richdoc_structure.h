/*******************************************************************************
 * class RichDoc
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


#ifndef RICHDOC_STRUCTURE_H
#define RICHDOC_STRUCTURE_H


#include <QtGui>
#include <QtCore>
#include "pics_structure.h"
#include <QMetaType>
#define _IMAGE_PICS_ITEM_ 100





class RichDoc
{
    public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };
    RichDoc() {
    html = QByteArray("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Your text</p>");
		style = QString("min-height:60px;margin-left:0px;margin-right:0px;margin-top:0px;margin-bottom:0px;padding:0px;background-color:transparent;");
		nullmargin = false;
    css2value << "position" << "margin-left" << "margin-right" << "margin-top" << "margin-bottom" << "padding" << "top" << "left" << "width" << "min-height" << "degree-rotation" << "opacity" << "height" << "background-color" << "z-index" << "id" << "border-width" << "border-color" << "border-style" << "l-lock";
    }
		
		void margin( bool e )
		{
		nullmargin = e;
		}
    RichDoc& operator=( const RichDoc& d )
    {
      resource.clear();
      resource = d.resource;
      html = d.html;
      style = d.style;
			return *this;
    }
    operator QVariant() const {
    return QVariant::fromValue(*this);
    }
    
    void Register( QTextDocument *doc , QMap<QString,SPics> list , QString s = QString("min-height:300px;padding:0px 0px 0px 0px;background-color:#ffffff;")  )
    {
      style = s;
			//////////QTextDocument *d = RelinkFormat(doc);
      html.clear();
      html.append(doc->toHtml("utf-8"));
      resource.clear();
			if (list.size() > 0) {
      resource = list;
			}
    }
		
		void Register( const QString xhtml , QMap<QString,SPics> list , QString s = QString("min-height:300px;padding:0px 0px 0px 0px;background-color:#ffffff;")  )
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
		
		bool isAbsolute()
		{
			return style.contains("absolute");
		}
		QString styler()
		{
			if (isAbsolute()) {
				return style;
			} else {
				return QString();
			}
			
		}
		
		QTextDocument *easydoc()
    {
			QTextDocument *d = new QTextDocument();
			d->setHtml(QString(html));
			return d;
		}
		
		QTextDocument *todoc()
    {
			QTextDocument *d = new QTextDocument();
			d->setHtml(QString(html));
			writtelnDoc(d);
			return d;
		}
		void writtelnDoc( QTextDocument * d )
		{
			  QTextCursor helper = QTextCursor(d);
				//////FlashItem One;
		    //////FlashReference* flahandler = new FlashReference(One,d);
		    /////////d->documentLayout()->registerHandler(QTextCharFormat::UserObject + 1,flahandler);
			  QMapIterator<QString,SPics> i(resource);
		     while (i.hasNext()) {
           i.next();
				   SPics record = i.value();
				   d->addResource( QTextDocument::ImageResource,QUrl(record.name),record.pix());
         }
				 QTextFrame  *Tframe = d->rootFrame();
				 if (nullmargin) {
					 
					       QTextFrameFormat Ftf = Tframe->frameFormat();
					       Ftf.setLeftMargin(0);
                 Ftf.setBottomMargin(0);
                 Ftf.setTopMargin(0);
                 Ftf.setRightMargin(0);
					       Tframe->setFrameFormat(Ftf);
				 }
				 
				 
				 QTextFrame::iterator it;
         for (it = Tframe->begin(); !(it.atEnd()); ++it) {
					 QTextBlock para = it.currentBlock();
					 if (para.isValid()) {
						QTextBlockFormat Parformat = para.blockFormat();
						if (Parformat.bottomMargin() == 12 && Parformat.topMargin() == 12 || nullmargin ) {
							Parformat.setBottomMargin(0); 
              Parformat.setTopMargin(0);
              Parformat.setRightMargin(0);
              Parformat.setLeftMargin(0);
							helper.setPosition(para.position(),QTextCursor::MoveAnchor);
							helper.setBlockFormat(Parformat);
						}
						 
						 
							QTextBlock::iterator de;
										for (de = para.begin(); !(de.atEnd()); ++de) {
												QTextFragment fr = de.fragment();
											  if (fr.isValid()) {
                           QTextCharFormat base = fr.charFormat();
                           QTextImageFormat pico = base.toImageFormat();
																	if (base.isAnchor()) {
																    QString href = base.anchorHref();
																		QStringList namelist = base.anchorNames();
																		///////qDebug() << "### loading link found ..... " << href;
																	  
																		
																	  /* default color from link and insert fla flash item to edit */
																		if (namelist.size() > 0 || !base.anchorName().isEmpty() ) {
																			
																			/////////////qDebug() << "### loading namelist found ..... " << namelist;
																			
																			QString first;
																			if (!base.anchorName().isEmpty()) {
																				first = base.anchorName();
																			} else {
																			  first = namelist.first();
																			}
																			
																			if (first.startsWith("mediaf")) {
																				
																				
																			}
																		} else {
																			/* normal link */
																	    base.setForeground(QBrush(_LINK_COLOR_));
                                      base.setUnderlineStyle(QTextCharFormat::SingleUnderline);
																		  helper.setPosition(fr.position());
                                      helper.setPosition(fr.position() + fr.length(),QTextCursor::KeepAnchor);
																	    helper.setCharFormat(base);
																		}
																 }
																
													      if (pico.isValid()) {
																		const QString hrefadress = pico.name();
																		SPics spico = resource[hrefadress];
																		////////////qDebug() << "### from RichDoc add resource " << hrefadress;
																		////////////////qDebug() << "### from RichDoc info " << spico.info;
																		pico.setToolTip(spico.info);
																		pico.setProperty(_IMAGE_PICS_ITEM_,spico);
																		helper.setPosition(fr.position());
                                    helper.setPosition(fr.position() + fr.length(),QTextCursor::KeepAnchor);
                                    helper.setCharFormat(pico);
																}
												}
																 
										}
						 
					 }
			
		     }
    }
		QStringList css2value;  /* not save */
		bool nullmargin;
    QString style;
    QMap<QString,SPics> resource;
    QByteArray html;  /* qCompress */
    
};

inline QDataStream& operator<<(QDataStream& out, RichDoc& e) 
{
			
			out << e.html;
			out << e.style;
			
								 QMapIterator<QString,SPics> i(e.resource);
								 while (i.hasNext()) {
										 i.next();
										 SPics record = i.value();
										 out << record;
								 } 
								 
			return out;
}

inline QDataStream& operator>>(QDataStream& in, RichDoc& e) 
{
			
				in >> e.html;
				in >> e.style;
				while (!in.atEnd()) {
						 SPics appoint;
						 in >> appoint;
						 e.resource.insert(appoint.name,appoint);
				}
				return in;
}

Q_DECLARE_METATYPE(RichDoc);
typedef QMap<int,RichDoc> LayerList;
typedef QList<RichDoc> QLayerList;
Q_DECLARE_METATYPE(LayerList)
Q_DECLARE_METATYPE(QLayerList)


QString SaveRichDoc( RichDoc e );
RichDoc OpenRichDoc( const QString datastream_base64 );

QByteArray HeSaveLayer( RichDoc e , const int position );
QPair<int,RichDoc> HeOpenLayer( const QByteArray stream );




class PageDoc
{
    public:
    enum { MAGICNUMBER = 0xFFFAFFAA, VERSION = 1 };
    PageDoc() {
		Listening.clear();
		binformat.clear();
		modus = 1;
		}
		
		operator QVariant() const {
    return QVariant::fromValue(*this);
    }
		int size()
		{
		return Listening.size();
		}
		
		QByteArray save()
		{
				 QString line("\n\n");
			   QByteArray bytes;
			   QByteArray signature("PAGE\n");
			   binformat.clear();
				 QMapIterator<int,RichDoc> o(Listening);
					while (o.hasNext()) {
                 o.next();
				         RichDoc record = o.value();
							   QByteArray dat = HeSaveLayer(record,o.key());
							   binformat.append(dat);
					}
					
         	QBuffer buffer(&bytes);
					if (!buffer.open(QIODevice::WriteOnly)) {
						return QByteArray();
					}
					QDataStream ds(&buffer);
					/* place header */
					ds.setVersion(QDataStream::Qt_4_2);
					ds << signature;
					ds << (quint32)MAGICNUMBER;
					ds << (quint32)VERSION;
					ds << rect;
					ds << line;
					for (int i = 0; i < binformat.size(); ++i) {
						 ds << binformat.at(i);
				   }
					buffer.close();
					QCryptographicHash formats( QCryptographicHash::Md5 );
          formats.addData(bytes);
			    smd5 = formats.result();
	        return bytes;
		}
		
		void open( QByteArray stream )
		{
			  QCryptographicHash formats( QCryptographicHash::Md5 );
        formats.addData(stream);
			  smd5 = formats.result();
			  QBuffer buffer(&stream);
				if (!buffer.open(QIODevice::ReadOnly)) {
					return;
				}
				QString infostream;
				quint32 magic, version;
				QByteArray signer;
				QDataStream ds(&buffer);
				ds.setVersion(QDataStream::Qt_4_2);
				ds >> signer;
				if ( !signer.contains(QByteArray("PAGE")) ) {
				buffer.close();
				return;
				}
				ds >> magic;
				if ( (quint32)MAGICNUMBER != magic ) {
				qWarning() << "######## PageDoc::MAGICNUMBER not ok  " << magic;
				buffer.close();
				return;
				}
				
				ds >> version;
				if ( (quint32)VERSION != version ) {
				qWarning() << "######## PageDoc::VERSION not ok  " << version;
				buffer.close();
				return;
				}
				ds >> rect;
				ds >> infostream;
				QByteArray daten;
				Listening.clear();
				 while (!ds.atEnd()) {
              ds >> daten;
							QPair<int,RichDoc> Layers = HeOpenLayer(daten);
              Listening.insert(Layers.first,Layers.second);
          }
				 buffer.close();
		}
		QRectF rect;
		QByteArray smd5; 
		int modus;
		QMap<int,RichDoc> Listening;  /* QMap<int,RichDoc> */
		QList<QByteArray> binformat;
};



Q_DECLARE_METATYPE(PageDoc);














#endif // RICHDOC_STRUCTURE_H

