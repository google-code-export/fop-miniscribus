
#include <QtGui>
#include <QPair>
#include <QTextDocument>

#if QT_VERSION >= 0x040500
#include <QXmlQuery>
#endif


static const qreal PAGEFAKTOR = 1.4;
static const qreal SCALING_STEEP = 0.05555555555;
static const qreal SLIDERMARGIN_TICK = 20.0;
static const qreal SLIDERSPACER = 2.5;
static const qreal SLIDERCURSORWI = 6.0;
static const qreal PAPERSPACE = 5;
static const int MaximumPages = 99;
static const qreal BorderShadow = 5.2;
static const int DefaultFonzSize = 10;
static const qreal SLIDERMARGIN_TICK_LEFT = SLIDERMARGIN_TICK + SLIDERSPACER;
static const qreal SLIDERMARGIN_TICK_TOTAL = SLIDERMARGIN_TICK + SLIDERSPACER + SLIDERSPACER;
typedef QPair<qreal,qreal> bodyMargins;

typedef QPair<qreal,qreal> bodyMargins;
#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)
#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)


#define _SLIDERBGCOLOR_ \
             QColor("#ebe9ed")
#define _EDITORBGCOLOR_ \
             QColor("#999999")
#define _SHADOWCOLOR_ \
             QColor("#7b6868")
      
class StreamBuf
{
  public:
  StreamBuf() 
  :d(new QBuffer())
  {
  d->open(QIODevice::ReadWrite);
  }
  ~StreamBuf() { d->close(); }
  bool clear() {
    d->write(QByteArray()); 
    return d->bytesAvailable() == 0 ? true : false;
  }
  
  bool PutOnFile( const QString file ) { 
      QFile f(file);
      d->seek(0);
      if (f.open(QFile::WriteOnly)) {
        uint bi = f.write(stream());
        f.close();
        return bi > 0 ? true : false;
      }
   return false; 
  }

  
  QIODevice *device() { return d; }
  QByteArray stream() { return d->data(); }
  /* <?xml version="1.0" encoding="utf-8"?> */
  QString data() { return QString::fromUtf8(stream()); }
  QBuffer *d;
}; 







static void paintWidged( QPainter *p , const QRectF rect , const QTransform trax )
{
    p->save();
    p->setPen(QPen(Qt::black,0.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p->setBrush(_SLIDERBGCOLOR_);
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->restore();
}

static void paintArea( QPainter *p , const QRectF rect , QColor col )
{
    p->save();
    p->setBrush(col);
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->restore();
}

static void paintCursor( QPainter *p , const QRectF rect )
{
    bool horrizontal = rect.width() == SLIDERCURSORWI ? true : false;
    qreal midiline = SLIDERCURSORWI / 2;
    QPointF soco(QPointF(midiline,0) + rect.topLeft());
    QPointF socoend(QPointF(midiline,0) + rect.bottomLeft());

    if (!horrizontal) {
        soco =  rect.bottomLeft() - QPointF(0,midiline);
        socoend = soco + QPointF(rect.width(),0);
    }

    p->save();

    p->setOpacity(0.4);
    p->setBrush(QColor(Qt::green));
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->setOpacity(1.0);

    p->setBrush(Qt::red);

    p->setPen(QPen(Qt::red,1.5));
    if (horrizontal) {
        p->drawLine(soco,socoend);
    } else {
        p->drawLine(soco,socoend);
    }
    p->restore();
}


static void paintScale( QPainter *p , const QRectF rect , QPair<qreal,qreal> bodyMargins , const QTransform trax )
{
    bool horrizontal = qMax(rect.height(),rect.width()) == rect.width() ? true : false;
    const qreal one_unit = CM_TO_POINT(1);
    const qreal one_unit_dec = MM_TO_POINT(1) * trax.m11();
    const QPointF init = rect.topLeft();
    qreal linemid = qMin(rect.height(),rect.width())  / 5;
    qreal wi = qMax(rect.height(),rect.width());

    const qreal marginLeftX = init.x() + ( bodyMargins.first * trax.m11() );
    const qreal marginRightX = init.x() +  rect.width()  - ( bodyMargins.second * trax.m11() );
    const qreal marginTopY = init.y() + ( bodyMargins.first * trax.m11() );
    const qreal marginToptX = init.y() +  rect.height()  - ( bodyMargins.second * trax.m11() );

    int fontsizetxt = 6;
    int sline = wi / one_unit;
    int slines = wi / one_unit_dec;

    p->save();
    p->setBrush(QColor(Qt::white));
    p->setPen(Qt::NoPen);
    p->drawRect(rect);

    QFont valFont("Arial",fontsizetxt, QFont::Normal);
    p->setFont(valFont);

    p->setPen(QPen(Qt::black,0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if (horrizontal) {
        p->drawLine(init.x(),linemid + init.y(), init.x() + wi,linemid + init.y());
        p->setPen(QPen(Qt::black,0.25, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));




        int PressUnit = -1;
        int numsUnit = 0;
        for (int x = 0; x<=slines; x++)  {
            PressUnit++;
            const int LeftPointer2 = (x * one_unit_dec) + init.x();
            if (PressUnit == 0) {
                p->drawLine(LeftPointer2,linemid + init.y(),LeftPointer2,linemid * 3.5 + init.y());
            } else if (PressUnit == 5) {
                p->drawLine(LeftPointer2,linemid + init.y(),LeftPointer2,linemid * 3 + init.y());
            } else {
                p->drawLine(LeftPointer2,linemid + init.y() ,LeftPointer2,linemid * 2 + init.y());
            }

            if (PressUnit == 0 && numsUnit > 0) {
                const QString strnum = QString("%1").arg(numsUnit);
                QRectF valR(LeftPointer2 - fontsizetxt ,linemid * 3 + init.y() ,fontsizetxt * 2,fontsizetxt + 2);
                p->drawText(valR, Qt::AlignCenter , strnum);

            }


            if (PressUnit == 9) {
                PressUnit = -1;
                numsUnit++;
            }
        }
        const QRectF cursor_1(marginLeftX - (SLIDERCURSORWI / 2),init.y(),SLIDERCURSORWI,rect.height());
        const QRectF cursor_2(marginRightX - (SLIDERCURSORWI / 2),init.y(),SLIDERCURSORWI,rect.height());
        paintCursor(p,cursor_1);
        paintCursor(p,cursor_2);


    } else {
        p->setPen(QPen(Qt::black,0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p->drawLine(init.x() + linemid,init.y(),init.x() + linemid, init.y() + wi);
        p->setPen(QPen(Qt::black,0.25, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        int PressUnit = -1;
        int numsUnit = 0;

        for (int x = 0; x<=slines; x++)  {
            PressUnit++;

            const int TopPointer = (x * one_unit_dec) + init.y();
            if (PressUnit == 0) {
                p->drawLine(init.x() + linemid,TopPointer,init.x() + (linemid * 3.5),TopPointer);
            } else if (PressUnit == 5) {
                p->drawLine(init.x() + linemid,TopPointer,init.x() + (linemid * 3),TopPointer);
            } else {
                p->drawLine(init.x() + linemid,TopPointer,init.x() + (linemid * 2),TopPointer);
            }


            if (PressUnit == 0 && numsUnit > 0) {
                const QString strnum = QString("%1").arg(numsUnit);
                QRectF valR(init.x() + (linemid * 2),TopPointer - fontsizetxt ,fontsizetxt * 2,fontsizetxt + 2);
                //////////////p->rotate ( 90 );
                p->drawText(valR, Qt::AlignCenter , strnum);
            }

            if (PressUnit == 9) {
                PressUnit = -1;
                numsUnit++;
            }



        }

        //////const QRectF cursor_1(marginLeftX - (SLIDERCURSORWI / 2),init.y(),SLIDERCURSORWI,rect.height());
        const QRectF cursor_3(init.x() ,marginTopY - (SLIDERCURSORWI / 2) ,rect.width(),SLIDERCURSORWI);
        const QRectF cursor_4(init.x() ,marginToptX - (SLIDERCURSORWI / 2),rect.width(),SLIDERCURSORWI);
        paintCursor(p,cursor_3);
        paintCursor(p,cursor_4);



    }

    p->setBrush(Qt::NoBrush);
    p->setPen(QPen(Qt::black,0.8));
    p->drawRect(rect);
    p->restore();

}



class EditArea : public QAbstractScrollArea
{
    Q_OBJECT
//
public:
    
     EditArea( QWidget *parent = 0 );
     inline int xOffset() const { return horizontalScrollBar()->value(); }
     inline int yOffset() const { return verticalScrollBar()->value(); }
     
protected:
    
     void paintEvent( QPaintEvent *Event );
     void paintEditPage( const int index  , QPainter * painter );
     void mousePressEvent ( QMouseEvent *e );
     void mouseDoubleClickEvent ( QMouseEvent *e );
     void adjustScrollbars();
     void wheelEvent (QWheelEvent * event);
     void resizeEvent(QResizeEvent *e);
     void contextMenuEvent(QContextMenuEvent *event);
     void keyPressEvent ( QKeyEvent * e );

private:
    
    QRectF page;
    qreal mesure; /* fix faktor cm*/
    QTransform mcurrent;
    QSize workArea;  /* mobile size from zoom */
    qreal scaleFaktor;
    bool portrait_mode;
    qint64 lineTimer;
    QTextDocument *_doc;
    QRectF slider_Horrizzontal_Top;
    QRectF slider_Vertical_Left;

signals:
public slots:
    void triggerFormat();
    void fitToLarge();
    void fitToNormal();
    void setZoom( const qreal value );
    void zoomIn();
    void zoomOut();
    
};
 
/**/
EditArea::EditArea( QWidget *parent)
  : QAbstractScrollArea(0),lineTimer(0),scaleFaktor(1.111),_doc(new QTextDocument("Hello World")),portrait_mode(true)
{
    
    #if QT_VERSION >= 0x040500
    QDateTime timer1( QDateTime::currentDateTime() );
    StreamBuf *buf = new StreamBuf();
    QXmlQuery xquery(QXmlQuery::XSLT20);
    xquery.setFocus(QUrl("http://www.qtcentre.org/forum/external.php?type=RSS2"));
    xquery.bindVariable("unixtime", QVariant(timer1.toTime_t()));
    xquery.bindVariable("ddate", QVariant(timer1.toString()));
    xquery.bindVariable("ddformat", QVariant(QDir::homePath () ));
    xquery.setQuery(QUrl("http://fop-miniscribus.googlecode.com/svn/trunk/doc/draw_paper/browser_rss.xsl"));
    xquery.evaluateTo(buf->device());
    _doc->setHtml ( buf->data() );
    ////////////buf->PutOnFile( "ddebughtml.html" );
    #endif


    
    
  _doc->setPageSize(QSizeF(MM_TO_POINT(210),MM_TO_POINT(297)));
    
    QTextFrameFormat foframe = _doc->rootFrame()->frameFormat();
    if (foframe.leftMargin() < 10) {
        foframe.setLeftMargin(MM_TO_POINT(15));
        foframe.setRightMargin(MM_TO_POINT(15));
        foframe.setBottomMargin(MM_TO_POINT(25));
        foframe.setTopMargin(MM_TO_POINT(25));
        _doc->rootFrame()->setFrameFormat(foframe);
    }
    
    
  adjustScrollbars();
  setAcceptDrops ( true );
  resize(900,499);
}

void EditArea::paintEvent( QPaintEvent *Event )
{
    lineTimer++;
    
    const QRectF viewAreaRect(0,0,viewport()->width(),viewport()->height());
    QTextFrameFormat docrootformat = _doc->rootFrame()->frameFormat();
    
    QTransform matrix;  /* zero matrix */
    QPainter *p = new QPainter( viewport() );
    p->setRenderHint(QPainter::Antialiasing, true);
    paintArea(p,Event->rect(),_EDITORBGCOLOR_);
    const int y = yOffset();
    const int x = xOffset();
    p->translate(-x,-y);
    p->setWorldTransform(mcurrent,true);  /*  zoom && center on adjustScrollbars() */
        const int PageSumm = qBound (1,_doc->pageCount(),MaximumPages);
         for (int o = 0; o < PageSumm; ++o)  {
            paintEditPage(o,p);
        }
        
     p->setWorldTransform(matrix);
     /* top left bars static to not move */
     paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,viewAreaRect.height()),mcurrent);
     paintWidged(p,QRectF(0,0,viewAreaRect.width(),SLIDERMARGIN_TICK_TOTAL),mcurrent);
     /* top left bars static to not move */
     /* horrizzontal  slider */
     p->translate(-x,0);
        
    /* horrizzontal  slider */
    p->translate(-x,0);
    QRectF slider_Horrizzontal_Top = QRectF(mcurrent.m31(),SLIDERSPACER,page.width() * scaleFaktor,SLIDERMARGIN_TICK);  /* click top area */
    paintScale(p,slider_Horrizzontal_Top,qMakePair(docrootformat.leftMargin(),docrootformat.rightMargin()),mcurrent);
        /* horrizzontal*/
    p->setWorldTransform(matrix); /* reset */
    p->translate(0,-y);
        /* vertical */
    QRectF slider_Vertical_Left = QRectF(SLIDERSPACER,mcurrent.m32(),SLIDERMARGIN_TICK,page.height() * scaleFaktor);  /* click left area */
    paintScale(p,slider_Vertical_Left,qMakePair(docrootformat.topMargin(),docrootformat.bottomMargin()),mcurrent);
    /* vertical */
        
        
        
        
     p->setWorldTransform(matrix);
     paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,SLIDERMARGIN_TICK_TOTAL),mcurrent);
     p->end();
}

void EditArea::paintEditPage( const int index  , QPainter * painter )
{
    QRectF viewarea(QPointF(0,index * _doc->pageSize().height()),_doc->pageSize());
    painter->save();
    painter->setBrush(Qt::white);
    painter->drawRect(viewarea);
    painter->restore();
    
    QAbstractTextDocumentLayout::PaintContext CTX;
    CTX.palette.setColor(QPalette::Text, Qt::black);
    const QRectF body = QRectF(viewarea.topLeft(),viewarea.size()); /* on view */
    QRectF view(viewarea.topLeft(),viewarea.size());   /* on doc */
    painter->save();
    painter->setClipRect(view);
    CTX.clip = view;
    CTX.cursorPosition = -1;
    _doc->documentLayout()->draw(painter,CTX);
    painter->restore();
}

void EditArea::mousePressEvent ( QMouseEvent *e )
{
    
}

void EditArea::mouseDoubleClickEvent ( QMouseEvent *e )   
{
   
}

void EditArea::resizeEvent(QResizeEvent *e)
{
    adjustScrollbars();
}

void EditArea::keyPressEvent ( QKeyEvent * e )  
{
    if ( e == QKeySequence::ZoomIn) {
        zoomIn();
    }
    if ( e == QKeySequence::ZoomOut) {
        zoomOut();
    }
}

void EditArea::zoomIn()
{
    setZoom(mcurrent.m11() + SCALING_STEEP);
}

void EditArea::zoomOut()
{
    setZoom(mcurrent.m11() - SCALING_STEEP);
}

void EditArea::setZoom( const qreal value )
{
    if (value < 0.22 || value > 10) {
     return;
    } else {
      qDebug() << "### setZoom " << value; 
      scaleFaktor = value;
      adjustScrollbars();
      viewport()->update();
    }
}

void EditArea::wheelEvent (QWheelEvent * event)
{
  qreal docksscale = scaleFaktor;
  if (event->delta() > 1) {
      zoomOut();
  } else {
      zoomIn();
  }
}


void EditArea::fitToLarge()
{
    adjustScrollbars();
    const QRectF viewAreaRect(0,0,viewport()->width(),viewport()->height());
    const qreal maxavaiable = viewAreaRect.width() - SLIDERMARGIN_TICK_TOTAL;
    setZoom(maxavaiable / page.width());
}

void EditArea::fitToNormal()
{
    setZoom(1.0);
}

void EditArea::contextMenuEvent(QContextMenuEvent *event)
{
        QMenu *menu = new QMenu(this);
        menu->addAction(tr("Swap page format"),this, SLOT(triggerFormat()));
        menu->addAction(tr("Fit to Window"),this, SLOT(fitToLarge()));
        menu->addAction(tr("Reset zoom to 1:1"),this, SLOT(fitToNormal()));
        menu->addAction(tr("Zoom in CTRL++"),this, SLOT(zoomIn()));
        menu->addAction(tr("Zoom out CTRL+-"),this, SLOT(zoomOut()));
        menu->exec(event->globalPos());
        delete menu;
}

void EditArea::triggerFormat()
{
    portrait_mode = portrait_mode == true ? false : true;
    if (portrait_mode) {
    _doc->setPageSize(QSizeF(MM_TO_POINT(210),MM_TO_POINT(297)));
    } else {
    _doc->setPageSize(QSizeF(MM_TO_POINT(297),MM_TO_POINT(210)));
    }
    adjustScrollbars();
    viewport()->update();
}



void EditArea::adjustScrollbars()
{
    /* portrait - landscape */
    qreal lmax = qMax(_doc->pageSize().width(),_doc->pageSize().height());
    qreal lmin = qMin(_doc->pageSize().width(),_doc->pageSize().height());
    if (portrait_mode) {
    page = QRectF(0,0,lmin,lmax);
    } else {
    page = QRectF(0,0,lmax,lmin);
    }
    const qreal left_slide = SLIDERMARGIN_TICK_TOTAL;
    
    const int PageSumm = qBound (1,_doc->pageCount(),MaximumPages);
    
    workArea = QSize(page.width() * scaleFaktor , (page.height() * scaleFaktor) *  PageSumm);
    bool stayinwi =  workArea.width() < viewport()->width() ? true : false;
    bool stayinhi =  workArea.height() < viewport()->height() ? true : false;
    qreal border_wi = left_slide;
    qreal border_hi = 0.1;
    if ( stayinwi ) {
        border_wi = qMax( left_slide ,qAbs((qreal)(workArea.width()  / 2) - (viewport()->width() / 2)));
    } else {
        border_hi = left_slide;
    }
    mcurrent  = QTransform(scaleFaktor,0.,0.,scaleFaktor,border_wi + PAPERSPACE,SLIDERMARGIN_TICK_TOTAL + PAPERSPACE); ////// ultimo da sopra penultimo da sinistra
    QSize viewPanelSize = viewport()->size();
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    verticalScrollBar()->setPageStep(workArea.height());  /* zoom state */
    horizontalScrollBar()->setPageStep(workArea.width()); /* zoom state */
    verticalScrollBar()->setRange(0, workArea.height()  - viewPanelSize.height() + SLIDERMARGIN_TICK_TOTAL * 3);
    horizontalScrollBar()->setRange(0, ( page.width() * scaleFaktor) - viewPanelSize.width()  + SLIDERMARGIN_TICK_TOTAL * 3);
}

  


int main(int argc, char **argv)
{
   QApplication a(argc, argv);
   EditArea w;
   w.show();
   return a.exec();
}

#include "main.moc" 






