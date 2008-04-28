#ifndef PICS_STRUCTURE_H
#define PICS_STRUCTURE_H

#include <QtGui>
#include <QtCore>






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
        QPixmap curr = pix();
        curr.save(fullpath,extension.data());
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
             QApplication::setOverrideCursor(Qt::WaitCursor);
             for (int i=0; i<li.size(); i++) {
                 SPics conni = li[i];
                 ds << conni;
             }   
             QApplication::restoreOverrideCursor();
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
  QApplication::setOverrideCursor(Qt::WaitCursor);
    while (!ds.atEnd()) {
         ds >> appoint;
         li.append(appoint);
    }
   QApplication::restoreOverrideCursor();
	buffer.close();
	return li;
}










#endif // PICS_STRUCTURE_H

