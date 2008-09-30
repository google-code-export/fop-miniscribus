#include "DocEdit.h"



/**/
EditArea::EditArea( QWidget *parent )
  : QAbstractScrollArea(0),page(0,0,0,0),lineTimer(0),
    workArea(0,0),scaleFaktor(1.3),portrait_mode(true),mesure(11.2),_doc(new PDocument)
{
  mcurrent  = QTransform(scaleFaktor,0.,0.,scaleFaktor,0.,0.);
  _doc->openFile("index.html");
  adjustScrollbars();
  connect(verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(verticalValue(int)));
  resize(700,400);
}

void EditArea::paintEvent( QPaintEvent *Event )
{
  lineTimer++;
  QTextFrameFormat docrootformat = _doc->rootFrame()->frameFormat();
  QPainter *p = new QPainter( viewport() );
  p->setRenderHint(QPainter::Antialiasing, true);
  p->setPen(Qt::NoPen);
  p->setBrush(_EDITORBGCOLOR_);
  p->drawRect(Event->rect());
    
  if (lineTimer != 1) {
  const QRectF viewAreaRect(0,0,viewport()->width(),viewport()->height());
  const int y = yOffset();
  const int x = xOffset();
  p->translate(-x,-y);
  p->setWorldTransform(mcurrent,true);
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
    
  
  QTransform matrix;
  _doc->paintPage(0,p,page);
  paintShadow(p,page);
  /* reset matrix */
  p->setWorldTransform(matrix);
  paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,viewAreaRect.height()),mcurrent); 
  paintWidged(p,QRectF(0,0,viewAreaRect.width(),SLIDERMARGIN_TICK_TOTAL),mcurrent); 
  /* horrizzontal*/
  p->translate(-x,0);
  slider_Horrizzontal_Top = QRectF(mcurrent.m31(),SLIDERSPACER,workArea.width(),SLIDERMARGIN_TICK);  /* click top area */
  paintScale(p,slider_Horrizzontal_Top,qMakePair(docrootformat.leftMargin(),docrootformat.rightMargin()),mcurrent);
  top_matrix = p->worldTransform();
  /* horrizzontal*/
  p->setWorldTransform(matrix); /* reset */
  p->translate(0,-y);
  /* vertical */
  slider_Vertical_Left = QRectF(SLIDERSPACER,mcurrent.m32(),SLIDERMARGIN_TICK,workArea.height());  /* click left area */
  paintScale(p,slider_Vertical_Left,qMakePair(docrootformat.topMargin(),docrootformat.bottomMargin()),mcurrent);
  /* vertical */
  left_matrix = p->worldTransform();
  p->setWorldTransform(matrix);
  paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,SLIDERMARGIN_TICK_TOTAL),mcurrent);
  }  else  {
     qDebug() << "### maybe first run " << lineTimer;
     adjustScrollbars();
     fitToLarge();
     QAbstractScrollArea::paintEvent(Event);
  }
  cursorRectSlider(docrootformat,p);
  p->end();
}
void EditArea::cursorRectSlider( const QTextFrameFormat docrootformat  , QPainter *p )
{
    sl_cursor[0] = slider_Horrizzontal_Top;
    sl_cursor[1] = divideRect(slider_Horrizzontal_Top,0);
    sl_cursor[2] = divideRect(slider_Horrizzontal_Top,2);
    sl_cursor[3] = divideRect(slider_Vertical_Left,0);
    sl_cursor[4] = divideRect(slider_Vertical_Left,2);
    sl_cursor[5] = slider_Vertical_Left;
    /* debug to display if transform correct */
    /*
    p->save();
    p->setBrush(QColor(Qt::green));
    p->setPen(Qt::NoPen);
    p->drawRect(sl_cursor[3]);
    p->drawRect(cursor_2);
    p->restore();
    */
}

void EditArea::HandleMoveSlider(  QPointF point , bool top )
{
    setZoom(1);
    const int diffy = 0;
    
    QTextFrameFormat foframe = _doc->rootFrame()->frameFormat();
    bool moved = false;
    for (int i = 0; i < 6; ++i)  {
        if (sl_cursor[i].contains(point)) {
            qDebug() << "### wake  " << i;
            if (i == 1) {
                qreal goto_1 = point.x() - sl_cursor[0].topLeft().x();
                foframe.setLeftMargin(goto_1);
                moved = true;
            } 
            if (i == 2) {
                qreal goto_2 = sl_cursor[0].topRight().x() - point.x();
                foframe.setRightMargin(goto_2);
                moved = true;
            }
            if (i == 3) {
                qreal goto_3 =  (point.y() + diffy) - sl_cursor[5].topRight().y();
                foframe.setTopMargin(goto_3);
                moved = true;
            }
            if (i == 4) {
                qreal goto_4 =  sl_cursor[5].bottomRight().y() - (point.y() + diffy);
                qDebug() << "### move to  cm." << POINT_TO_CM(goto_4) << " p."<< point.y();
                foframe.setBottomMargin(goto_4);
                moved = true;
            }
        }
    }
    
    if (moved) {
    _doc->rootFrame()->setFrameFormat(foframe);
    viewport()->update();
    }
    ///////foframe.setLeftMargin(MM_TO_POINT(15));
    //////foframe.setRightMargin(MM_TO_POINT(15));
    ///////foframe.setBottomMargin(MM_TO_POINT(25));
    /////////foframe.setTopMargin(MM_TO_POINT(25));
    
    
}


void EditArea::mousePressEvent ( QMouseEvent *e )
{
    QPointF page_point = maps(e->pos()); 
    if ( e->button() == Qt::LeftButton) {
        if (page.contains(page_point)) {
            dotChain <<  page_point;
        } else {
            
            QPointF rack( QPointF(xOffset(),yOffset()) + e->pos());
            
            if (slider_Horrizzontal_Top.contains(rack)) {
            HandleMoveSlider(rack,true);
            } else if (slider_Vertical_Left.contains(rack)) {
            HandleMoveSlider(rack,false);  
            }
        } 
   }
    viewport()->update();
}
void EditArea::mouseDoubleClickEvent ( QMouseEvent *e )   
{
    QPointF page_point = maps(e->pos()); 
    if (page.contains(page_point)) {
        dotChain.clear();
        setZoom(1.0);
    } else {
        HandleMoveSlider(e->pos());
    }
    viewport()->update();
}
void EditArea::resizeEvent(QResizeEvent *e)
{
    adjustScrollbars();
}

void EditArea::keyPressEvent ( QKeyEvent * e )  
{
    const qreal docksscale = scaleFaktor;
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
    if (value < 0.45 || value > 10) {
     return;
    } else {
      //////qDebug() << "### setZoom " << value; 
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

void EditArea::contextMenuEvent(QContextMenuEvent *event)
{
     QMenu *menu = new QMenu(this);
     menu->addAction(tr("Swap page format"),this, SLOT(triggerFormat()));
     menu->addAction(tr("Fit to Window"),this, SLOT(fitToLarge()));
     menu->addAction(tr("Zoom in CTRL++"),this, SLOT(zoomIn()));
     menu->addAction(tr("Zoom out CTRL+-"),this, SLOT(zoomOut()));
     menu->exec(event->globalPos());
     delete menu;
}

void EditArea::triggerFormat()
{
    portrait_mode = portrait_mode == true ? false : true;
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
     _doc->setFormat(PDocument::PPortrait);
     } else {
     page = QRectF(0,0,lmax,lmin); 
     _doc->setFormat(PDocument::PLandscape);
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
     mcurrent  = QTransform(scaleFaktor,0.,0.,scaleFaktor,border_wi + PAPERSPACE,SLIDERMARGIN_TICK_TOTAL + PAPERSPACE); ////// ultimo da sopra penultimo da sinistra 
     QSize viewPanelSize = viewport()->size();
     setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
     setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
     verticalScrollBar()->setPageStep(workArea.height());  /* zoom state */
     horizontalScrollBar()->setPageStep(workArea.width()); /* zoom state */
     verticalScrollBar()->setRange(0, ( page.height() * scaleFaktor) - viewPanelSize.height() + SLIDERMARGIN_TICK_TOTAL * 3);
     horizontalScrollBar()->setRange(0, ( page.width() * scaleFaktor) - viewPanelSize.width()  + SLIDERMARGIN_TICK_TOTAL * 3);
     //////verticalScrollBar()->setRange(0,  page.width() * 10);
}

void EditArea::verticalValue( const int index ) 
{
    /////////qDebug() << "### verticalValue " << POINT_TO_CM(index * scaleFaktor) <<  " r." << index;
    qDebug() << "### scroll top_matrix  " << top_matrix;
    qDebug() << "### scroll left_matrix  " << left_matrix;
    qDebug() << "### scroll slider_Horrizzontal_Top  " << slider_Horrizzontal_Top;
}

QPointF EditArea::maps( QPointF p ) 
{
    QTransform transmouse = mcurrent.inverted();
    transmouse.translate (xOffset(),yOffset());  /* scroll bars x,y */
    return transmouse.map(p);
}
















