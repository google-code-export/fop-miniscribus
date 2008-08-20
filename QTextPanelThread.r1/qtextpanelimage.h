#ifndef QTEXTPANELIMAGE_H
#define QTEXTPANELIMAGE_H



#include <QPair>
#include <QtGui>
#include <QtCore>
#include <QPixmap>

#include <QMetaType>
#define _IMAGE_PICS_ITEM_ 100

#define _LINK_COLOR_ \
             QColor("#dc0000")
             
#include "qtextpanelmime.h"


/*
Page formatting info enum QPrinter compatible
Margin params on QRectF
css like QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
http://de.selfhtml.org/css/eigenschaften/randabstand.htm#margin
*/
class PanelPageSize
{
	public:
		enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };

		/////////  MarginPage = QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
		QPrinter::PageSize P_rect;
		QRectF G_regt;
		QRectF P_margin;
		QSizeF RealSize;
		QString name;
		bool landscape;
		bool AllowHeaderFooter;
		int modus;  /* 0 = normal , 1 = book , 2 = 2column */
		int coolspace;

		PanelPageSize();
		PanelPageSize& operator=(const PanelPageSize& d);
		operator QVariant() const
		{
			return QVariant::fromValue(*this);
		}
		inline QRectF HeaderBoundingrect()
		{
			return  QRectF(0,0,width(),P_margin.x() - (FooterHeaderPadding * 2));
		}

		inline QPointF HeaderInitPoints(const int index = 0)
		{
			const qreal fromTopY = index * G_regt.height();
			const qreal spacepage = index * InterSpace;
			return QPointF(P_margin.height(),fromTopY + spacepage + FooterHeaderPadding);
		}

		inline QPointF FooterInitPoints(const int index = 0)
		{
			const qreal fromTopY = index * G_regt.height();
			const qreal Ytop = G_regt.height() - P_margin.width();
			if (index ==  0)
			{
				return QPointF(P_margin.height(),Ytop + FooterHeaderPadding);
			}
			const qreal spacepage = index * InterSpace;
			return QPointF(P_margin.height(),fromTopY + spacepage + Ytop + FooterHeaderPadding);
		}


		inline QRectF FooterBoundingrect()
		{
			return  QRectF(0,0,width(),P_margin.width() - (FooterHeaderPadding * 2));
		}

		QString HName();
		qreal faktor();
		void Register(QString n , QPrinter::PageSize pp , bool La = false);
		inline qreal hight()
		{
			return G_regt.height() - P_margin.width() - P_margin.x() + 0.5;  ////  -bottom - top
		}
		qreal width()
		{
			return G_regt.width() - P_margin.height() - P_margin.y() + 0.5; /////////  -lef , - right
		}
		QSizeF internal_size()
		{
			return QSizeF(width(),hight());
		}
		void SetMargin(QRectF rectp)
		{
			P_margin = rectp;
		}
		QRectF BoundingPageSumme(const int summe);

		QRectF MarginArea()   /* page 0 */
		{
			const qreal LargeDoc = G_regt.width() - P_margin.y()  - P_margin.height();
			const qreal InnHightDoc = G_regt.height() - P_margin.x()  - P_margin.width();
			return QRectF(P_margin.height(),P_margin.x(),LargeDoc,InnHightDoc);
		}
		/* swap right - left margin  */
		QRectF MarginSwapArea()   /* page /0\ */
		{
			const qreal LargeDoc = G_regt.width() - P_margin.y()  - P_margin.height();
			const qreal InnHightDoc = G_regt.height() - P_margin.x()  - P_margin.width();
			return QRectF(P_margin.y(),P_margin.x(),LargeDoc,InnHightDoc);
		}

		QRectF DocumentInternal(const int index)
		{
			const qreal fromTopY = internal_size().rheight() * index;
			return QRectF(QPointF(0,fromTopY),internal_size());
		}

		QRectF DocumentExternal(const int index)
		{
			const qreal fromTopY = G_regt.height() * index;
			return QRectF(QPointF(0,fromTopY),G_regt.size());
		}
		/* internal rect whitout margin */
		QRectF PageInternal(const int index)
		{
			const qreal fromTopY = index * G_regt.height();
			const qreal spacepage = index * InterSpace;
			return QRectF(QPointF(P_margin.height(),fromTopY + spacepage + P_margin.x()),internal_size());
		}
		/* external rect document */
		QRectF PageExternal(const int index)
		{
			const qreal fromTopY = index * G_regt.height();
			const qreal spacepage = index * InterSpace;
			return QRectF(QPointF(0,fromTopY + spacepage),G_regt.size());
		}
		QPointF MarginTranslate()
		{
			return QPointF(P_margin.height(),P_margin.x());
		}
        QPointF PageIndexTopLeft(const int index)
        {
            const qreal fromTopY = index * G_regt.height();
            const qreal spacepage = index * InterSpace;
            return QPointF(0,fromTopY + spacepage);
        }
		/*  edit modus */
		//////////////void HandleDocument( QTextDocument *doc );
		/* open or set a qtexdocument from this */
		QTextDocument *nulldoc(QString htm);
		/* write fop file margins */
		int FillFopAttributes(QDomElement e);
		/* form qtexdocument to this margin an papersize */
		void HandlePrint(QTextDocument *doc);

		QPixmap PicfromMime(QMimeData *mime);
};

Q_DECLARE_METATYPE(PanelPageSize);


             

/*

TPics image;
      image.name =
      image.set_pics( QImage barcode );


*/


class TPics
{
public:
	enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 5 };
	TPics();
    ~TPics();
	TPics& operator=(const TPics& d);
	bool operator!=(const TPics& d);
	operator QVariant() const
	{
		return QVariant::fromValue(*this);
	}
	QPixmap erno_pix();
	QString web();
	QPixmap pix();
	QString FileName();
	void SavePixThread(QString dir = QString());
	void set_pics(const QPixmap * barcode);
    void set_pics( QImage barcode );
	void set_pics(QPixmap barcode);
	inline int picskilo()
	{
		return data.size();
	}
	QUrl indoc();
	/* vars permanent to put on stream  */
	QString name;
	QString info;
	QByteArray extension;
	QByteArray data; 
};

Q_DECLARE_METATYPE(TPics);


inline QDebug operator<<(QDebug debug, TPics& udoc)
{
	debug.nospace() << "TPics(name."
	<< udoc.name << ","
	<< udoc.info << ",size()"
	<< udoc.data.size() << ",type()"
	<< udoc.extension << ")";
	return debug.space();
}

inline QDataStream& operator<<(QDataStream& out, const TPics& udoc)
{
	out << udoc.name;
	out << udoc.info;
	out << udoc.extension;
	out << udoc.data;
	return out;
}

inline QDataStream& operator>>(QDataStream& in, TPics& udoc)
{
	in >> udoc.name;
	in >> udoc.info;
	in >> udoc.extension;
	in >> udoc.data;
	return in;
}







class RichDoc
{
public:
	enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };
	RichDoc()
	{
		html = QByteArray("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Your text</p>");
		style = QString("min-height:300px;padding:0px 0px 0px 0px;background-color:#ffffff;");
		nullmargin = false;
	}
	void margin(bool e)
	{
		nullmargin = e;
	}
	RichDoc& operator=(const RichDoc& d)
	{
		resource.clear();
		resource = d.resource;
		html = d.html;
		style = d.style;
		return *this;
	}
	operator QVariant() const
	{
		return QVariant::fromValue(*this);
	}

	void Register(QTextDocument *doc , QMap<QString,TPics> list , QString s)
	{
		style = s;
		html.clear();
		html.append(doc->clone()->toHtml("utf-8"));
		resource.clear();
		if (list.size() > 0)
		{
			resource = list;
		}
	}

	void Register(const QString xhtml , QMap<QString,TPics> list , QString s)
	{
		style = s;
		html.clear();
		html.append(xhtml);
		resource.clear();
		if (list.size() > 0)
		{
			resource = list;
		}
	}
	QString Hxtml()
	{
		return QString(html.constData());
	}

	bool isAbsolute()
	{
		return style.contains("position:absolute;");
	}

	QTextDocument *todoc()
	{
		QTextCursor helper;
		QTextDocument *d = new QTextDocument();
		d->setHtml(QString(html));
		helper = QTextCursor(d);
		QMapIterator<QString,TPics> i(resource);
		while (i.hasNext())
		{
			i.next();
			TPics record = i.value();
			d->addResource(QTextDocument::ImageResource,QUrl(record.name),record.pix());
		}
		QTextFrame  *Tframe = d->rootFrame();
		if (nullmargin)
		{

			QTextFrameFormat Ftf = Tframe->frameFormat();
			Ftf.setLeftMargin(0);
			Ftf.setBottomMargin(0);
			Ftf.setTopMargin(0);
			Ftf.setRightMargin(0);
			Tframe->setFrameFormat(Ftf);
		}


		QTextFrame::iterator it;
		for (it = Tframe->begin(); !(it.atEnd()); ++it)
		{
			QTextBlock para = it.currentBlock();
			if (para.isValid())
			{
				QTextBlockFormat Parformat = para.blockFormat();
				if (Parformat.bottomMargin() == 12 && Parformat.topMargin() == 12 || nullmargin)
				{
					Parformat.setBottomMargin(0);
					Parformat.setTopMargin(0);
					Parformat.setRightMargin(0);
					Parformat.setLeftMargin(0);
					helper.setPosition(para.position(),QTextCursor::MoveAnchor);
					helper.setBlockFormat(Parformat);
				}


				QTextBlock::iterator de;
				for (de = para.begin(); !(de.atEnd()); ++de)
				{
					QTextFragment fr = de.fragment();
					if (fr.isValid())
					{
						QTextCharFormat base = fr.charFormat();
						QTextImageFormat pico = base.toImageFormat();
						if (base.isAnchor())
						{
							//////////////qDebug() << "### link found..... ";
							base.setForeground(QBrush(_LINK_COLOR_));
							base.setUnderlineStyle(QTextCharFormat::SingleUnderline);
							helper.setPosition(fr.position());
							helper.setPosition(fr.position() + fr.length(),QTextCursor::KeepAnchor);
							helper.setCharFormat(base);
						}

						if (pico.isValid())
						{
							const QString hrefadress = pico.name();
							TPics spico = resource[hrefadress];
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


		return d;
	}
	bool nullmargin;
	QString style;
	QMap<QString,TPics> resource;
	QByteArray html;  /* qCompress */

};


inline QDataStream& operator<<(QDataStream& out, RichDoc& e)
{

	out << e.html;
	out << e.style;

	QMapIterator<QString,TPics> i(e.resource);
	while (i.hasNext())
	{
		i.next();
		TPics record = i.value();
		out << record;
	}

	return out;
}

inline QDataStream& operator>>(QDataStream& in, RichDoc& e)
{

	in >> e.html;
	in >> e.style;
	while (!in.atEnd())
	{
		TPics appoint;
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



/* save pics  list stream */
QString SaveImageGroup(QList<TPics> li);
/* decoded base64 stream to put on mysql row , file or network streams */
QList<TPics> OpenImageGroup(const QString datastream_base64);





/* save a doc to stream */
QString SaveRichDoc(RichDoc e);
/* decoded base64 stream to put on mysql row , file or network streams */
RichDoc OpenRichDoc(const QString datastream_base64);



/* inline stream from page format */
QByteArray HeSaveLayer(RichDoc e , const int position);
QPair<int,RichDoc> HeOpenLayer(const QByteArray stream);


class PageDoc
{
public:
	enum { MAGICNUMBER = 0xFFFAFFAA, VERSION = 1 };
	PageDoc()
	{
		Listening.clear();
		binformat.clear();
		modus = 1;
	}

	operator QVariant() const
	{
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
		while (o.hasNext())
		{
			o.next();
			RichDoc record = o.value();
			QByteArray dat = HeSaveLayer(record,o.key());
			binformat.append(dat);
		}

		QBuffer buffer(&bytes);
		if (!buffer.open(QIODevice::WriteOnly))
		{
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
		for (int i = 0; i < binformat.size(); ++i)
		{
			ds << binformat.at(i);
		}
		buffer.close();
		QCryptographicHash formats(QCryptographicHash::Md5);
		formats.addData(bytes);
		smd5 = formats.result();
		return bytes;
	}

	void open(QByteArray stream)
	{
		QCryptographicHash formats(QCryptographicHash::Md5);
		formats.addData(stream);
		smd5 = formats.result();
		QBuffer buffer(&stream);
		if (!buffer.open(QIODevice::ReadOnly))
		{
			return;
		}
		QString infostream;
		quint32 magic, version;
		QByteArray signer;
		QDataStream ds(&buffer);
		ds.setVersion(QDataStream::Qt_4_2);
		ds >> signer;
		if (!signer.contains(QByteArray("PAGE")))
		{
			buffer.close();
			return;
		}
		ds >> magic;
		if ((quint32)MAGICNUMBER != magic)
		{
			qWarning() << "######## PageDoc::MAGICNUMBER not ok  " << magic;
			buffer.close();
			return;
		}

		ds >> version;
		if ((quint32)VERSION != version)
		{
			qWarning() << "######## PageDoc::VERSION not ok  " << version;
			buffer.close();
			return;
		}
		ds >> rect;
		ds >> infostream;
		QByteArray daten;
		Listening.clear();
		while (!ds.atEnd())
		{
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



/* Theard objekt flush */
class QRichPage
{
public:
	enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };
	QRichPage() {}
    ~QRichPage() {}
    void load(  const QTextDocument *doc , PanelPageSize format ) {
    html = doc->toHtml("utf-8").toUtf8();
    model = format;
    imagelist.clear();
        
         QMap<QString,TPics> imagelist;
         QVector<QTextFormat> iter = doc->allFormats();
         for (int i = 0; i < iter.size(); ++i) {
             QTextFormat pline = iter.at(i);
             if (pline.isImageFormat() ) {
                 QTextImageFormat ipic = pline.toImageFormat();
                 const QVariant xx = doc->resource(QTextDocument::ImageResource,QUrl(ipic.name()));
                 if (!xx.isNull()) {
                     TPics image;
                     image.name = ipic.name();
                     QImage qi = xx.value<QImage>();
                     QPixmap qp = xx.value<QPixmap>();
                     if (!qi.isNull()) {
                       image.set_pics( qi );
                       imagelist.insert(ipic.name(),image);
                     } else if (!qp.isNull()) {
                       image.set_pics( qp ); 
                       imagelist.insert(ipic.name(),image);                         
                     }
                 }                     
             }  
         }
        
    }
    operator QVariant() const
	{
		return QVariant::fromValue(*this);
	}
    /* refill a document */
    void imageToDoc( QTextDocument *d ) {
    QMapIterator<QString,TPics> i(imagelist);
		while (i.hasNext())
		{
			i.next();
			TPics record = i.value();
			d->addResource(QTextDocument::ImageResource,QUrl(record.name),record.pix());
		}
    }
    QMap<int,RichDoc> layers;  /* layer absolute */
    QMap<QString,TPics> imagelist;
    PanelPageSize model;
    QByteArray html;

};

Q_DECLARE_METATYPE(QRichPage);






#endif // IMAGE_PAGE_MIME_H

