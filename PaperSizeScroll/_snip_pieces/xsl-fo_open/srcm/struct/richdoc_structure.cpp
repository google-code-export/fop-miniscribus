#include "richdoc_structure.h"


QString SaveRichDoc( RichDoc e )
{
	QByteArray bytes;
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::WriteOnly)) {
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
RichDoc OpenRichDoc( const QString datastream_base64   )
{
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
  qWarning() << "######## RichDoc::MAGICNUMBER not ok  " << magic;
	buffer.close();
	return li;
	}
	ds >> version;
	if ( (quint32)RichDoc::VERSION != version ) {
	qWarning() << "######## RichDoc::VERSION not ok  " << version;
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





QByteArray HeSaveLayer( RichDoc e , const int position )
{
	QByteArray bytes;
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::WriteOnly)) {
		return QByteArray();
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds << (quint32)RichDoc::MAGICNUMBER;
	ds << (quint32)RichDoc::VERSION;
	/* place header */
  ds << position;
  ds << e;
	buffer.close();
	return bytes.toHex();
}

QPair<int,RichDoc> HeOpenLayer( const QByteArray stream )
{
  RichDoc li;
	quint32 magic, version;
	QByteArray bytes(QByteArray::fromHex(stream));
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::ReadOnly)) {
		return qMakePair(0,li);
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds >> magic;
	if ( (quint32)RichDoc::MAGICNUMBER != magic ) {
  qWarning() << "######## RichDoc::MAGICNUMBER not ok  " << magic;
	buffer.close();
	return qMakePair(0,li);
	}
	ds >> version;
	if ( (quint32)RichDoc::VERSION != version ) {
	qWarning() << "######## RichDoc::VERSION not ok  " << version;
	return qMakePair(0,li);
	}
  int pos;
  RichDoc e;
  ds >> pos;
  ds >> e;
  buffer.close();
  return qMakePair(pos,e);
}

















