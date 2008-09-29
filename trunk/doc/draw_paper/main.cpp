
#include <QtGui>
#include <QPair>

static const qreal PAGEFAKTOR = 1.4;
static const qreal SCALING_STEEP = 0.055555555;
static const qreal SLIDERMARGIN_TICK = 20.0;
static const qreal SLIDERSPACER = 2.5;

static const qreal PAPERSPACE = 18.0;

static const qreal SLIDERMARGIN_TICK_LEFT = SLIDERMARGIN_TICK + SLIDERSPACER;
static const qreal SLIDERMARGIN_TICK_TOTAL = SLIDERMARGIN_TICK + SLIDERSPACER + SLIDERSPACER;
typedef QPair<qreal,qreal> bodyMargins;

#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)

////////////workArea.width() < viewport()->width() ? true : false;
/////////workArea.height() < viewport()->height() ? true : false; qMakePair(10,44)

static void paintWidged( QPainter *p , const QRectF rect , const QTransform trax )
{
    p->save();
    p->setPen(QPen(Qt::black,0.1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p->setBrush(QColor(Qt::lightGray));
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->restore();
}

static void paintScale( QPainter *p , const QRectF rect , const bodyMargins , const QTransform trax )
{
    bool horrizontal = qMax(rect.height(),rect.width()) == rect.width() ? true : false;
    const qreal one_unit = CM_TO_POINT(1);
    const qreal one_unit_dec = MM_TO_POINT(1) * trax.m11();
    const QPointF init = rect.topLeft();
    qreal linemid = qMin(rect.height(),rect.width())  / 5;
    qreal wi = qMax(rect.height(),rect.width());
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
        
        
        
        
        
        
    }
    p->restore();
    
}



class EditArea : public QAbstractScrollArea
{
    Q_OBJECT
//
public:
    
     EditArea( QWidget *parent = 0 , qreal dimension = 11.5 );
     inline int xOffset() const { return horizontalScrollBar()->value(); }
     inline int yOffset() const { return verticalScrollBar()->value(); }
     
protected:
    
     void paintEvent( QPaintEvent *Event );
     void mousePressEvent ( QMouseEvent *e );
     void mouseDoubleClickEvent ( QMouseEvent *e );
     void adjustScrollbars();
     void wheelEvent (QWheelEvent * event);
     void resizeEvent(QResizeEvent *e);
     void contextMenuEvent(QContextMenuEvent *event);

private:
    
    QPointF maps( QPointF p );
    QRectF page;
    qreal mesure; /* fix faktor cm*/
    QTransform mcurrent;
    QList<QPointF> dotChain;
    QSize workArea;  /* mobile size from zoom */
    qreal scaleFaktor;
    bool portrait_mode;
    qint64 lineTimer;
    QRectF slider_Horrizzontal_Top;
    QRectF slider_Vertical_Left;

signals:
public slots:
    void triggerFormat();
    void fitToLarge();
    
};
 
/**/
EditArea::EditArea( QWidget *parent  , qreal dimension )
  : QAbstractScrollArea(0),page(0,0,0,0),lineTimer(0),workArea(dimension,dimension * PAGEFAKTOR),scaleFaktor(1.8),mesure(dimension),portrait_mode(true)
{
  mcurrent  = QTransform(scaleFaktor,0.,0.,scaleFaktor,0.,0.);
  dotChain << maps(QPointF(33,33)) << maps(QPointF(444,444));
  adjustScrollbars();
  adjustScrollbars();
}

void EditArea::paintEvent( QPaintEvent *Event )
{
  lineTimer++;
  if (lineTimer != 1) {
  const QRectF viewAreaRect(0,0,viewport()->width(),viewport()->height());
  const int y = yOffset();
  const int x = xOffset();
  QPainter *p = new QPainter( viewport() );
  p->setRenderHint(QPainter::Antialiasing, true);
  p->setPen(Qt::NoPen);
  p->setBrush(QColor("#e7e7e7"));
  p->drawRect(Event->rect());
  p->translate(-x,-y);
  
  /////////paintScale(p,QRectF(SLIDERMARGIN_TICK_TOTAL,SLIDERSPACER,page.width(),SLIDERMARGIN_TICK),qMakePair(10.,44.),mcurrent);
      
  ////////////   
      
  ///////paintScale(p,QRectF(0,0,SLIDERMARGIN_TICK,page.height()),qMakePair(10.,44.));
  p->setWorldTransform(mcurrent,true);
  
  QFont fontText("Verdana",12,QFont::StyleItalic);
  fontText.setStyleStrategy(QFont::PreferBitmap);
  p->setBrush(Qt::white);
  p->drawRect(page);
  
  
    
    for (int i = 0; i < dotChain.size(); ++i) {
        const QPointF nowPoint = dotChain.at(i);
        if (viewAreaRect.contains(nowPoint)) {
        p->setPen(Qt::NoPen);
        QRectF fire(nowPoint  - QPointF(8,8),QSizeF(16,16));
                p->setBrush(Qt::red);
                p->setPen(Qt::white);
                p->setOpacity(0.8);
                p->drawEllipse(fire);
                p->setOpacity(1.0);
        } 
    }
  ///////}
    
  QTransform matrix;
  /* reset matrix */
  p->setWorldTransform(matrix);
  paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,viewAreaRect.height()),mcurrent); 
  paintWidged(p,QRectF(0,0,viewAreaRect.width(),SLIDERMARGIN_TICK_TOTAL),mcurrent); 
  /* horrizzontal*/
  slider_Horrizzontal_Top = QRectF(mcurrent.m31(),SLIDERSPACER,workArea.width(),SLIDERMARGIN_TICK);
  paintScale(p,slider_Horrizzontal_Top,qMakePair(10.,44.),mcurrent);
  /* horrizzontal*/

  p->translate(0,-y);
    
  /* vertical */
  slider_Vertical_Left = QRectF(SLIDERSPACER,mcurrent.m32(),SLIDERMARGIN_TICK,workArea.height());
  paintScale(p,slider_Vertical_Left,qMakePair(10.,44.),mcurrent);
  /* vertical */
    
    
  p->setWorldTransform(matrix);
  paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,SLIDERMARGIN_TICK_TOTAL),mcurrent);
  p->end();
    
    
    
      //////////qDebug() << "### lineTimer " << lineTimer;
  }  else  {
     qDebug() << "### maybe first run " << lineTimer;
     adjustScrollbars();
     resize(700,400);
     fitToLarge();
     QAbstractScrollArea::paintEvent(Event);
  }
  
}

void EditArea::mousePressEvent ( QMouseEvent *e )
{
    dotChain <<  maps(e->pos()); 
    viewport()->update();
}
void EditArea::mouseDoubleClickEvent ( QMouseEvent *e )   
{
    dotChain.clear();
    scaleFaktor = 1;
    adjustScrollbars();
    viewport()->update();
}
void EditArea::resizeEvent(QResizeEvent *e)
{
    adjustScrollbars();
}

void EditArea::contextMenuEvent(QContextMenuEvent *event)
{
     QMenu *menu = new QMenu(this);
     menu->addAction(tr("Swap page format"),this, SLOT(triggerFormat()));
     menu->addAction(tr("Fit to Window"),this, SLOT(fitToLarge()));
     menu->exec(event->globalPos());
     delete menu;
}

void EditArea::triggerFormat()
{
    portrait_mode = portrait_mode == true ? false : true;
    adjustScrollbars();
    viewport()->update();
}

void EditArea::fitToLarge()
{
    adjustScrollbars();
    const QRectF viewAreaRect(0,0,viewport()->width(),viewport()->height());
    const qreal maxavaiable = viewAreaRect.width() - SLIDERMARGIN_TICK_TOTAL;
    const qreal gozoom =  maxavaiable / page.width();
    qDebug() << "### fitToLarge " << gozoom;
    if (gozoom < 0.45 || gozoom > 10) {
     return;
    } else {
      scaleFaktor = gozoom;
      adjustScrollbars();
      viewport()->update();
    }
    
    
}

void EditArea::adjustScrollbars()
{
     /* portrait - landscape */
     qreal lmax = qMax(CM_TO_POINT(mesure),CM_TO_POINT(mesure) * PAGEFAKTOR);
     qreal lmin = qMin(CM_TO_POINT(mesure),CM_TO_POINT(mesure) * PAGEFAKTOR);
     if (portrait_mode) {
     page = QRectF(0,0,lmin,lmax);
     } else {
     page = QRectF(0,0,lmax,lmin); 
     }
     const qreal left_slide = SLIDERMARGIN_TICK_TOTAL;
     workArea = QSize(page.width() * scaleFaktor , page.height() * scaleFaktor);
     bool stayinwi =  workArea.width() < viewport()->width() ? true : false;
     bool stayinhi =  workArea.height() < viewport()->height() ? true : false;
     qreal border_wi = left_slide;
     qreal border_hi = 0.1;
      if ( stayinwi ) {
          border_wi = qMax( left_slide ,qAbs((qreal)(workArea.width()  / 2) - (viewport()->width() / 2)));
      } else {
          border_hi = left_slide;
      }
      /*
      if ( stayinhi ) {
          border_hi = qAbs((workArea.height()  / 2) - (viewport()->height() / 2));
      }
      */
     if (border_wi > 0) {
     qDebug() << "### mezza " << (double)border_wi;
     }
     mcurrent  = QTransform(scaleFaktor,0.,0.,scaleFaktor,border_wi + PAPERSPACE,SLIDERMARGIN_TICK_TOTAL + PAPERSPACE); ////// ultimo da sopra penultimo da sinistra 
     QSize viewPanelSize = viewport()->size();
     setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
     setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
     verticalScrollBar()->setPageStep(workArea.height());  /* zoom state */
     horizontalScrollBar()->setPageStep(workArea.width()); /* zoom state */
     verticalScrollBar()->setRange(0, ( page.height() * scaleFaktor) - viewPanelSize.height());
     horizontalScrollBar()->setRange(0, ( page.width() * scaleFaktor) - viewPanelSize.width());
     //////verticalScrollBar()->setRange(0,  page.width() * 10);
}



void EditArea::wheelEvent (QWheelEvent * event)
{
  qreal docksscale = scaleFaktor;
  if (event->delta() > 1) {
      docksscale = docksscale + SCALING_STEEP;
  } else {
      docksscale = docksscale - SCALING_STEEP;
  }
  if (docksscale < 0.45 || docksscale > 10) {
   return;
  } else {
      scaleFaktor = docksscale;
      adjustScrollbars();
      viewport()->update();
  }
 
}

QPointF EditArea::maps( QPointF p ) 
{
    QTransform transmouse = mcurrent.inverted();
    return transmouse.map(p);
}
  


int main(int argc, char **argv)
{
   QApplication a(argc, argv);
   EditArea w;
   w.show();
   return a.exec();
}

#include "main.moc" 






