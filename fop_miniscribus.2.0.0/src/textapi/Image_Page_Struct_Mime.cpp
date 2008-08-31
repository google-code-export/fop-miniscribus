#include "Image_Page_Struct_Mime.h"

#include "SessionManager.h"

SPics::SPics()
{
    name = "error";
    info = "Image Description to blind people";
    extension = QByteArray("PNG");
}

bool SPics::FopSaveImage( QDomElement e )
{
        const QString dirref = QString("./%1%2").arg(FOPIMAGEDIR).arg(FileName());
        e.setAttribute ("src",dirref); /* current path from export now! */
			///////////QFileInfo gosave(dirref);
			////////Cache( gosave.absolutePath() );
        ApiSession *sx = ApiSession::instance(); 
        sx->saveImageFile.insert(FileName(),qUncompress( data ));
        return true;				
}

QTextDocument *SPics::todoc()
{
        /* record image on session if a page format */
        ApiSession *sx = ApiSession::instance();
        QTextCursor helper;
        QTextDocument *d = new QTextDocument();
        d->setHtml(QString(html));
        helper = QTextCursor(d);
        QMapIterator<QString,SPics> i(resource);
        while (i.hasNext())
        {
            i.next();
            SPics record = i.value();
            sx->ImagePageList.insert(record.name,record);
            d->addResource( QTextDocument::ImageResource,QUrl(record.name),record.pix());
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
                if (Parformat.bottomMargin() == 12 && Parformat.topMargin() == 12 || nullmargin )
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


        return d;
}

SPics& SPics::operator=( const SPics& d )
{
    name = d.name;
    info = d.info;
    extension = d.extension;
    data = d.data;
    return *this;
}
/* md5 hash from data byte ? */
bool SPics::operator!=( const SPics& d )
{
    if (name != d.name )
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

void SPics::set_pics( QPixmap barcode )
{
    if (barcode.isNull())
    {
        return;
    }
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    barcode.save(&buffer,extension.data());
    data = qCompress(bytes,9);
}

void SPics::set_pics( const QPixmap * barcode )
{
    if (barcode->isNull())
    {
        return;
    }
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    barcode->save(&buffer,extension.data());
    data = qCompress(bytes,9);
}

void SPics::SavePixThread( QString dir )
{
    QString fullpath = dir + FileName();
    QFile f( fullpath );
    if ( f.open( QIODevice::WriteOnly ) )
    {
        f.write ( qUncompress( data ) );
        f.close();
    }

}

QString SPics::FileName()
{
    return Imagename(name) + "." + QString(extension.data()).toLower();
}

QPixmap SPics::pix()
{
    if (data.size() < 1)
    {
        return erno_pix();
    }
    QPixmap resultimage;
    QByteArray daunq = qUncompress( data );
    resultimage.loadFromData( daunq );
    if (resultimage.isNull())
    {
        return erno_pix();
    }

    return resultimage;
}

QPixmap SPics::erno_pix()
{

    QPixmap pError = QPixmap(20, 20 );
    pError.fill( Qt::red  );
    QPainter pter( &pError );
    pter.setFont( QFont( "Helvetica", 8 ) );
    pter.setBrush( Qt::green );
    pter.drawText( 5, 12 , "0" );
    return pError;
}

QString SPics::web()
{
    QString flusches;
    if (data.size() < 1)
    {
        return flusches;
    }
    QByteArray daunq = qUncompress( data );
    return daunq.toBase64();
}





QString SaveImageGroup( QList<SPics> li  )
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
QList<SPics> OpenImageGroup( const QString datastream_base64   )
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
    if ( (quint32)SPics::MAGICNUMBER != magic )
    {
        qDebug() << "######## SPics::MAGICNUMBER not ok  ";
        buffer.close();
        return li;
    }
    ds >> version;
    if ( (quint32)SPics::VERSION != version )
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












QString SaveRichDoc( RichDoc e )
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
RichDoc OpenRichDoc( const QString datastream_base64   )
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
    if ( (quint32)RichDoc::MAGICNUMBER != magic )
    {
        qWarning() << "######## RichDoc::MAGICNUMBER not ok  " << magic;
        buffer.close();
        return li;
    }
    ds >> version;
    if ( (quint32)RichDoc::VERSION != version )
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





QByteArray HeSaveLayer( RichDoc e , const int position )
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

QPair<int,RichDoc> HeOpenLayer( const QByteArray stream )
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
    if ( (quint32)RichDoc::MAGICNUMBER != magic )
    {
        qWarning() << "######## RichDoc::MAGICNUMBER not ok  " << magic;
        buffer.close();
        return qMakePair(0,li);
    }
    ds >> version;
    if ( (quint32)RichDoc::VERSION != version )
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






QPixmap ImagefromMime( const QMimeData *mime )
{
    QPixmap one;
    QStringList dli = mime->formats();

    if (dli.contains("application/x-picslists"))
    {
        QByteArray dd = mime->data("application/x-picslists");
        QList<SPics> li = OpenImageGroup(QString(dd.data()));
        if (li.size() > 0)
        {
            SPics primoi = li.first();
            return primoi.pix();
        }

    }


    if (dli.contains("text/html"))
    {
        QByteArray dd = mime->data("text/html");
        QTextDocument *picdoc = new QTextDocument();
        picdoc->setHtml ( QString(dd.data()) );
        /*
                   QMapIterator<QString,SPics> i(imglist());
                 while (i.hasNext()) {
                     i.next();
        							    SPics e  = i.value();
        				         picdoc->addResource(QTextDocument::ImageResource,QUrl(e.name),e.pix());
        						 }
        */

        QTextFrame  *Tframe = picdoc->rootFrame();
        QTextFrameFormat rootformats = Tframe->frameFormat();
        rootformats.setBottomMargin (0);
        rootformats.setTopMargin(0);
        rootformats.setRightMargin (0);
        rootformats.setLeftMargin (0);
        rootformats.setPadding (10);
        rootformats.setBorder(1);
        rootformats.setBorderBrush(Qt::black);
        rootformats.setBorderStyle ( QTextFrameFormat::BorderStyle_Solid );
        Tframe->setFrameFormat ( rootformats );
        picdoc->adjustSize();
        /* paint doc */
        QAbstractTextDocumentLayout *Layout = picdoc->documentLayout();
        QRectF docirec = Layout->frameBoundingRect(picdoc->rootFrame());
        QPixmap PicsDocument(docirec.size().toSize());
        PicsDocument.fill(Qt::white);
        QRectF clip(0, 0,PicsDocument.width(),PicsDocument.height());
        QPainter *p = new QPainter(&PicsDocument);
        p->setRenderHint(QPainter::Antialiasing, true);
        picdoc->drawContents(p,clip);
        p->end();
        return PicsDocument;
    }

    return one;
}

































