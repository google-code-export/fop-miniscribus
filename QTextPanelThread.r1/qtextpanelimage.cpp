#include "qtextpanelimage.h"




TPics::~TPics()
{
  ////////delete &data;
}



TPics::TPics()
{
	name = "error";
	info = "Image Description to blind people";
	extension = QByteArray("PNG");
}

TPics& TPics::operator=(const TPics& d)
{
	name = d.name;
	info = d.info;
	extension = d.extension;
	data = d.data;
	return *this;
}
/* md5 hash from data byte ? */
bool TPics::operator!=(const TPics& d)
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

QUrl TPics::indoc()
{
	const int grep = name.size() -  name.indexOf(".");
	QString webname = name.left(grep);
	return QUrl(QString("./%1.%2").arg(name).arg(QString(extension.data()).toLower()));
}

void TPics::set_pics(QPixmap barcode)
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

void TPics::set_pics(const QPixmap * barcode)
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

void TPics::set_pics( QImage barcode )
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



void TPics::SavePixThread(QString dir)
{
	QString fullpath = dir + FileName();
	QFile f(fullpath);
	if (f.open(QIODevice::WriteOnly))
	{
		f.write(data);
		f.close();
	}

}

QString TPics::FileName()
{
	return imageName(name) + "." + QString(extension.data()).toLower();
}

QPixmap TPics::pix()
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

QPixmap TPics::erno_pix()
{

	QPixmap pError = QPixmap(20, 20);
	pError.fill(Qt::red);
	QPainter pter(&pError);
	pter.setFont(QFont("Helvetica", 8));
	pter.setBrush(Qt::green);
	pter.drawText(5, 12 , "0");
	return pError;
}

QString TPics::web()
{
	QString flusches;
	if (data.size() < 1)
	{
		return flusches;
	}
	///////QByteArray daunq = qUncompress(data);
	return data.toBase64();
}





QString SaveImageGroup(QList<TPics> li)
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
	ds << (quint32)TPics::MAGICNUMBER;
	ds << (quint32)TPics::VERSION;
	/* place header */
	///////QApplication::setOverrideCursor(Qt::WaitCursor);
	for (int i=0; i<li.size(); i++)
	{
		TPics conni = li[i];
		ds << conni;
	}
	//////QApplication::restoreOverrideCursor();
	buffer.close();
	return bytes.toBase64();
}



/* decoded base64 stream to put on mysql row , file or network streams */
QList<TPics> OpenImageGroup(const QString datastream_base64)
{


	QList<TPics> li;
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
	if ((quint32)TPics::MAGICNUMBER != magic)
	{
		qDebug() << "######## TPics::MAGICNUMBER not ok  ";
		buffer.close();
		return li;
	}
	ds >> version;
	if ((quint32)TPics::VERSION != version)
	{
		qDebug() << "######## TPics::VERSION not ok  ";
		buffer.close();
		return li;
	}
	TPics appoint;
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
	QMapIterator<QString,TPics> i(e.resource);
	while (i.hasNext())
	{
		i.next();
		TPics record = i.value();
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
	TPics appoint;
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








































