#include "PDocument.h"



PDocument::PDocument()
 :QTextDocument(0),p_format(PPortrait)
{
   init(); 
   style();
}

void PDocument::init()
{
    QTextOption opt;
	opt.setUseDesignMetrics(true);
	opt.setTabStop(8);
	opt.setWrapMode ( QTextOption::WrapAtWordBoundaryOrAnywhere );
	setDefaultTextOption(opt);
}

QSizeF PDocument::size()
{
    return QTextDocument::pageSize();
}

QRectF PDocument::boundingRect() const
{
    return documentLayout()->frameBoundingRect(rootFrame());
}

void PDocument::style()
{
    /* osdiffkey */
    QFont valFont("Arial",DefaultFonzSize, QFont::Normal);
    setDefaultFont ( valFont );
    /*  "A4 (210 x 297 mm)"; */
    QDateTime timer1( QDateTime::currentDateTime() ); 
    setPageSize(QSizeF(MM_TO_POINT(210),MM_TO_POINT(297)));
    setMetaInformation(QTextDocument::DocumentTitle,QString("Document from %1").arg(timer1.toString()));
}

void PDocument::setHtml ( const QString & html )
{
    init();
    style();
    QTextDocument::setHtml(html);
    ///////setPageSize(QSizeF(MM_TO_POINT(210),MM_TO_POINT(297)));
    setPageSize(QSizeF(MM_TO_POINT(300),MM_TO_POINT(300)));
}

void PDocument::openFile( const QString file )
{
    QFileInfo fi(file);
    StreamDoc *buf = new StreamDoc();
    buf->LoadFile( fi.absoluteFilePath () );
    if (buf->stream().size() < 1) {
    delete buf;
    return;
    }
    setMetaInformation(QTextDocument::DocumentUrl,QString(fi.absolutePath()));
    const QString html = QString(buf->stream().constData()); 
    setHtml(html);
    delete buf;
    QTextFrameFormat foframe = rootFrame()->frameFormat();
    if (foframe.leftMargin() < 10) { 
    foframe.setLeftMargin(MM_TO_POINT(15));
    foframe.setRightMargin(MM_TO_POINT(15));
    foframe.setBottomMargin(MM_TO_POINT(25));
    foframe.setTopMargin(MM_TO_POINT(25));
    rootFrame()->setFrameFormat(foframe); 
    }
}

void PDocument::paintPage( const int index  , QPainter * painter  , const QRectF viewarea  )
{
      const QPointF topleft = pageIndexTopLeft(index);
      QAbstractTextDocumentLayout::PaintContext CTX; 
	  CTX.palette.setColor(QPalette::Text, Qt::black);
	  const QRectF body = QRectF(viewarea.topLeft().x(), viewarea.topLeft().y() ,pageSize().width(),pageSize().height()); /* on view */
      QRectF view(0, index * body.height(), body.width(), body.height() );   /* on doc */
      painter->save();
      painter->translate(body.topLeft());
      painter->setClipRect(view);
      CTX.clip = view;
      documentLayout()->draw(painter,CTX);
      painter->restore();
}


QPointF PDocument::pageIndexTopLeft( const int index  )
{
    Q_ASSERT(pageSize().isValid());
    Q_ASSERT(index < pageCount());
	const qreal fromTopY = index * pageSize().height();
	return QPointF(0,fromTopY);
}


void PDocument::setFormat( PageFormat e )
{
    Q_ASSERT(pageSize().isValid());
    QSizeF pageU = pageSize();
    qreal lmin = qMin(pageU.width(),pageU.height());
    p_format = e;
    if (e == PPortrait) {
    setPageSize(QSizeF(lmin,lmin * pageFaktor()));
    } else {
    setPageSize(QSizeF(lmin * pageFaktor() , lmin ));
    }
    emit formatChance();
}

PDocument::PageFormat PDocument::pageformat()
{
    return p_format;
}


qreal PDocument::pageFaktor() const
{
    Q_ASSERT(pageSize().isValid());
    QSizeF pageU = pageSize();
    qreal lmax = qMax(pageU.width(),pageU.height());
    qreal lmin = qMin(pageU.width(),pageU.height());
    return lmax / lmin;
}















