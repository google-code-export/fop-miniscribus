#include "qtextpanelimage.h"

#include "qtextpanelmime.h"


SPics::~SPics()
{
  ////////delete &data;
}



SPics::SPics()
{
	name = "error";
	info = "Image Description to blind people";
	extension = QByteArray("PNG");
}

SPics& SPics::operator=(const SPics& d)
{
	name = d.name;
	info = d.info;
	extension = d.extension;
	data = d.data;
	return *this;
}
/* md5 hash from data byte ? */
bool SPics::operator!=(const SPics& d)
{
	if (name != d.name)
	{
		return false;
	}
	else
	{
		return true;
	}
}

QUrl SPics::indoc()
{
	const int grep = name.size() -  name.indexOf(".");
	QString webname = name.left(grep);
	return QUrl(QString("./%1.%2").arg(name).arg(QString(extension.data()).toLower()));
}

void SPics::set_pics(QPixmap barcode)
{
	if (barcode.isNull())
	{
		return;
	}
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	barcode.save(&buffer,extension.data());
	///////data = qCompress(bytes,9);
    data = bytes;
}

void SPics::set_pics(const QPixmap * barcode)
{
	if (barcode->isNull())
	{
		return;
	}
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	barcode->save(&buffer,extension.data());
	///data = qCompress(bytes,9);
    data = bytes;
}

void SPics::set_pics( QImage barcode )
{
	if (barcode.isNull())
	{
		return;
	}
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	barcode.save(&buffer,extension.data());
    data = bytes;
	////data = qCompress(bytes,9);
    ////////SavePixThread("./");
}



void SPics::SavePixThread(QString dir)
{
	QString fullpath = dir + FileName();
	QFile f(fullpath);
	if (f.open(QIODevice::WriteOnly))
	{
		f.write(data);
		f.close();
	}

}

QString SPics::FileName()
{
	return imageName(name) + "." + QString(extension.data()).toLower();
}

QPixmap SPics::pix()
{
	if (data.size() < 1)
	{
		return erno_pix();
	}
	QPixmap resultimage;
	resultimage.loadFromData(data);
	if (resultimage.isNull())
	{
		return erno_pix();
	}

	return resultimage;
}

QPixmap SPics::erno_pix()
{

	QPixmap pError = QPixmap(20, 20);
	pError.fill(Qt::red);
	QPainter pter(&pError);
	pter.setFont(QFont("Helvetica", 8));
	pter.setBrush(Qt::green);
	pter.drawText(5, 12 , "0");
	return pError;
}

QString SPics::web()
{
	QString flusches;
	if (data.size() < 1)
	{
		return flusches;
	}
	///////QByteArray daunq = qUncompress(data);
	return data.toBase64();
}





QString SaveImageGroup(QList<SPics> li)
{
	if (li.size() < 1)
	{
		return QString();
	}
	QByteArray bytes;
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::WriteOnly))
	{
		return QString();
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds << (quint32)SPics::MAGICNUMBER;
	ds << (quint32)SPics::VERSION;
	/* place header */
	///////QApplication::setOverrideCursor(Qt::WaitCursor);
	for (int i=0; i<li.size(); i++)
	{
		SPics conni = li[i];
		ds << conni;
	}
	//////QApplication::restoreOverrideCursor();
	buffer.close();
	return bytes.toBase64();
}



/* decoded base64 stream to put on mysql row , file or network streams */
QList<SPics> OpenImageGroup(const QString datastream_base64)
{


	QList<SPics> li;
	QByteArray xcode("");
	xcode.append(datastream_base64);
	quint32 magic, version;
	QByteArray bytes(QByteArray::fromBase64(xcode));   /* decoded base64 string to QByteArray */
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::ReadOnly))
	{
		return li;
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds >> magic;
	if ((quint32)SPics::MAGICNUMBER != magic)
	{
		qDebug() << "######## SPics::MAGICNUMBER not ok  ";
		buffer.close();
		return li;
	}
	ds >> version;
	if ((quint32)SPics::VERSION != version)
	{
		qDebug() << "######## SPics::VERSION not ok  ";
		buffer.close();
		return li;
	}
	SPics appoint;
	/* place header */
	while (!ds.atEnd())
	{
		ds >> appoint;
		li.append(appoint);
	}
	buffer.close();
	return li;
}












QString SaveRichDoc(RichDoc e)
{
	QByteArray bytes;
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::WriteOnly))
	{
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
	while (i.hasNext())
	{
		i.next();
		SPics record = i.value();
		ds << record;
	}
	buffer.close();
	return bytes.toBase64();
}


/* decoded base64 stream to put on mysql row , file or network streams */
RichDoc OpenRichDoc(const QString datastream_base64)
{
	RichDoc li;
	QByteArray xcode("");
	xcode.append(datastream_base64);
	quint32 magic, version;
	QByteArray bytes(QByteArray::fromBase64(xcode));   /* decoded base64 string to QByteArray */
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::ReadOnly))
	{
		return li;
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds >> magic;
	if ((quint32)RichDoc::MAGICNUMBER != magic)
	{
		qWarning() << "######## RichDoc::MAGICNUMBER not ok  " << magic;
		buffer.close();
		return li;
	}
	ds >> version;
	if ((quint32)RichDoc::VERSION != version)
	{
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
	while (!ds.atEnd())
	{
		ds >> appoint;
		li.resource.insert(appoint.name,appoint);
	}
	buffer.close();
	return li;
}





QByteArray HeSaveLayer(RichDoc e , const int position)
{
	QByteArray bytes;
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::WriteOnly))
	{
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

QPair<int,RichDoc> HeOpenLayer(const QByteArray stream)
{
	RichDoc li;
	quint32 magic, version;
	QByteArray bytes(QByteArray::fromHex(stream));
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::ReadOnly))
	{
		return qMakePair(0,li);
	}
	QDataStream ds(&buffer);
	/* place header */
	ds.setVersion(QDataStream::Qt_4_2);
	ds >> magic;
	if ((quint32)RichDoc::MAGICNUMBER != magic)
	{
		qWarning() << "######## RichDoc::MAGICNUMBER not ok  " << magic;
		buffer.close();
		return qMakePair(0,li);
	}
	ds >> version;
	if ((quint32)RichDoc::VERSION != version)
	{
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








































