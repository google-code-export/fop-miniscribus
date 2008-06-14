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


#ifndef PICS_STRUCTURE_H
#define PICS_STRUCTURE_H

#include <QtGui>
#include <QtCore>
#include <QPixmap>
#include <QSvgRenderer>


#define _LINK_COLOR_ \
             QColor("#dc0000")


static inline QPixmap RenderPixmapFromSvgByte(  QByteArray streams ) 
{
        QSvgRenderer  svgRenderer( streams );
        QPixmap pix(  svgRenderer.defaultSize() );
        pix.fill(Qt::transparent);
        QPainter paint(&pix);
        svgRenderer.render(&paint);
        return pix;
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




/* allowed char on file name image to save */
static inline QString Imagename( QString touri )
{
    touri = touri.replace(" ","_");
    QStringList piece = touri.split("");
    QString alloweduri = "abcdefghijklmnopqrstuvwxyz1234567890_-ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QStringList alist = alloweduri.split("");
    
     for (int i = 0; i < piece.size(); ++i) {
           QString pin = piece.at(i);
           if (!alist.contains(pin)) {
               touri.replace(pin,"");
           }
       }
    
    return touri.trimmed();
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



class SPics
{
    public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 1 };
    SPics() {
    name = "error";
    info = "Image Description to blind people";
    extension = QByteArray("PNG");
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
    
    void SavePix( QString dir = QString() )
    {
        QString fullpath = dir + FileName();
        
        
    }
    void SavePixThread( QString dir = QString() )
    {
        QString fullpath = dir + FileName();
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










#endif // PICS_STRUCTURE_H

