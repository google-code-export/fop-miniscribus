#include "DocEdit.h"

static void paintWidgedDebug( QPainter *p , const QRectF rect )
{
    p->save();
    p->setBrush(Qt::green);
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->restore();
}

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
  QTransform matrix;  
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
  p->setWorldTransform(mcurrent,true);  /*  zoom && center on adjustScrollbars() */
  p->setBrush(Qt::white);
  p->drawRect(page);
      
    for (int i = 0; i < dotChain.size(); ++i) {
        const QPointF nowPoint = dotChain.at(i);
        QRectF fire(nowPoint  - QPointF(8,8),QSizeF(16,16));
                p->setBrush(Qt::red);
                p->setPen(Qt::white);
                p->setOpacity(0.5);
                p->drawEllipse(fire);
                p->setOpacity(1.0);
    }
    
  
  
  _doc->paintPage(0,p,page);
  paintShadow(p,page);
  /* reset matrix as a null matrix */
  
  p->setWorldTransform(matrix);
  /* top left bars static to not move */
  paintWidged(p,QRectF(0,0,SLIDERMARGIN_TICK_TOTAL,viewAreaRect.height()),mcurrent); 
  paintWidged(p,QRectF(0,0,viewAreaRect.width(),SLIDERMARGIN_TICK_TOTAL),mcurrent); 
  /* top left bars static to not move */ 
  /* horrizzontal  slider */
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
  //////HandleTransform();
  //////paintWidgedDebug(p,debugRect);
  
  }  else  {
     qDebug() << "### maybe first run " << lineTimer;
     adjustScrollbars();
     fitToLarge();
     QAbstractScrollArea::paintEvent(Event);
  }
  p->setWorldTransform(matrix);
  cursorRectSlider(docrootformat,p);
  p->end();
}

void EditArea::cursorRectSlider( const QTextFrameFormat docrootformat  , QPainter *p )
{
    sl_cursor[0] = QRectF(0,0,_doc->size().width(),PAPERSPACE + SLIDERMARGIN_TICK_TOTAL);
    sl_cursor[5] = QRectF(0,0,PAPERSPACE + SLIDERMARGIN_TICK_TOTAL,_doc->size().height());
    
    sl_cursor[1] = divideRect(sl_cursor[0],0);
    sl_cursor[2] = divideRect(sl_cursor[0],2);
    sl_cursor[3] = divideRect(sl_cursor[5],0);
    sl_cursor[4] = divideRect(sl_cursor[5],2);
    
    ///////////QTransform aa;
    /////paintWidged(p,sl_cursor[5],aa);
}

bool EditArea::clickSlider( const QPointF p )
{
    for (int i = 0; i < 6; ++i)  {
        if (sl_cursor[i].contains(p)) {
          return true;  
        }
    }
    return false;
}

bool EditArea::HandleMoveSlider(  QPointF point , bool top )
{
    /*
    if (top) {
    qDebug() << "### HandleMoveSlider top  Xpos." << POINT_TO_CM(point.x()) << "  Ypos."<< point.y();
    } else {
    qDebug() << "### HandleMoveSlider left Ypos." << POINT_TO_CM(point.y()) << "  Ypos."<< point.x(); 
    }
    */
    
    QTextFrameFormat foframe = _doc->rootFrame()->frameFormat();
    bool moved = false;
    for (int i = 0; i < 6; ++i)  {
        if (sl_cursor[i].contains(point)) {
            if (i == 1 && top) {
               ///////qDebug() << "### wake top 1 " << i << point.y();
                foframe.setLeftMargin(point.x());
                moved = true;
            } 
            if (i == 2 && top) {
                /////////qDebug() << "### wake top 2 " << i << point.y();
                foframe.setRightMargin(_doc->size().width() - point.x());
                moved = true;
            }
            if (i == 3 && !top) {
                ///////////qDebug() << "### wake left " << i << point.y();
                foframe.setTopMargin(point.y());
                moved = true;
            }
            if (i == 4 && !top) {
                const qreal bottmm = _doc->size().height() - point.y();
                ////////////////qDebug() << "### wake left " << i << bottmm;
                foframe.setBottomMargin(bottmm);
                moved = true;
            }
        }
    }
    
    if (moved) {
    _doc->rootFrame()->setFrameFormat(foframe);
    update(boundingRect());
    return true;
    } else {
    update(boundingRect());
    return false;
    }
}

QRectF EditArea::boundingRect() const
{
    return QRectF(0,0,viewport()->width(),viewport()->height());
}

void EditArea::update( const  QRectF rect )
{
    viewport()->update(rect.toRect());
}


void EditArea::update()
{
    update(boundingRect());
}

bool EditArea::isOnSlider( const QPointF p )
{
    /* top area y0,x0 intersect slider top and slider left */
    QRectF totalForbitten(0,0,border_wi + PAPERSPACE,SLIDERMARGIN_TICK_TOTAL);
    QRectF sliderTop(totalForbitten.topRight(),QSizeF(boundingRect().width(),SLIDERMARGIN_TICK_TOTAL * 1.111));
    QRectF sliderLeft(totalForbitten.bottomLeft(),QSizeF(SLIDERMARGIN_TICK_TOTAL * 1.111,boundingRect().height()));
    if (totalForbitten.contains(p)) { 
    return false;
    }
    int cc = 0;
    bool isaccept = false;
    if (sliderTop.contains(p)) {
    cc = 1;
    isaccept = HandleMoveSlider(slider_maps(p,true),true);  
    } else if (sliderLeft.contains(p)) {
    cc = 2;
    isaccept = HandleMoveSlider(slider_maps(p,false),false);
    }
    
    if (isaccept) {
        if (cc == 1) {
        QApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
        } else if (cc == 2) {
        QApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));  
        }
    } else {
        cursorCheck();
    }
    
    
    return isaccept;
}

void EditArea::mouseReleaseEvent ( QMouseEvent *e )
{
    cursorCheck();
}

void EditArea::mousePressEvent ( QMouseEvent *e )
{
    if ( e->button() != Qt::LeftButton) {
     QApplication::restoreOverrideCursor();
     return;
    }
    bool isaccept = isOnSlider(e->pos());
    if (!isaccept) {
           if (isOnPage(_doc->boundingRect(),maps(e->pos()),scaleFaktor)) {
               /* text event to doc */
           }
    }
    //////
    e->setAccepted(isaccept);
}

void EditArea::mouseDoubleClickEvent( QMouseEvent *e )   
{
    if ( e->button() != Qt::LeftButton) {
     QApplication::restoreOverrideCursor();
     return;
    }
    bool isaccept = isOnSlider(e->pos());
    if (!isaccept) {
           if (isOnPage(_doc->boundingRect(),maps(e->pos()),scaleFaktor)) {
               /* text event to doc */
           }
    }
    //////QApplication::restoreOverrideCursor();
    e->setAccepted(isaccept);
}


void EditArea::mouseMoveEvent ( QMouseEvent *e )  
{
    if ( e->button() != Qt::LeftButton) {
     ////////return;
    }
    qDebug() << "------mouseMoveEvent--------------------------";
    bool isaccept = isOnSlider(e->pos());
    if (!isaccept) {
           if (isOnPage(_doc->boundingRect(),maps(e->pos()),scaleFaktor)) {
               /* text event to doc */
           }
    }
    //////QApplication::restoreOverrideCursor();
    e->setAccepted(isaccept);
    
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





void EditArea::wheelEvent (QWheelEvent * event)
{
  qreal docksscale = scaleFaktor;
  if (event->delta() > 1) {
      zoomOut();
  } else {
      zoomIn();
  }
}




void EditArea::contextMenuEvent(QContextMenuEvent *event)
{
    
    cursorCheck();
    
    if (isOnPage(_doc->boundingRect(),maps(event->pos()),scaleFaktor)) {
    
     QMenu *menu = new QMenu(this);
     menu->addAction(tr("Swap page format"),this, SLOT(triggerFormat()));
     menu->addAction(tr("Fit to Window"),this, SLOT(fitToLarge()));
     menu->addAction(tr("Zoom in CTRL++"),this, SLOT(zoomIn()));
     menu->addAction(tr("Zoom out CTRL+-"),this, SLOT(zoomOut()));
     menu->exec(event->globalPos());
     delete menu;
        
    }
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
     border_wi = left_slide;
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
     /////////////cursorCheck();
}

void EditArea::verticalValue( const int index ) 
{
    
}

/* ####################################### Public slot  init ###########################################*/

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
    setZoom(maxavaiable / page.width());
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
        
        qDebug() << "------setZoom--" << value << "------------------------";
        
        ////qDebug() << "-------------------------------------------------------------------";
        ////qDebug() << "### new setZoom " << value; 
        //////qDebug() << "-------------------------------------------------------------------";
      scaleFaktor = value;
      adjustScrollbars();
      viewport()->update();
    }
}


/* ####################################### Public slot  init ###########################################*/


/* ####################################### Translate mouse events init ###################################*/

void EditArea::cursorCheck()
{
    QCursor  *cur =  QApplication::overrideCursor();
    if (!cur) {
     return;
    }
    if (cur->shape() != Qt::ArrowCursor) {
     QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    }

}

QPointF EditArea::maps( QPointF p ) 
{
    QTransform transmouse = mcurrent.inverted();
    transmouse.translate (xOffset(),yOffset());  /* scroll bars x,y */
    return transmouse.map(p);
}


QPointF EditArea::slider_maps( QPointF p , bool top ) 
{
    bool hacks = false;
    QTransform transmouse = mcurrent.inverted();
    if (top) {
    transmouse.translate (xOffset(),SLIDERMARGIN_TICK_TOTAL + SLIDERSPACER);  /* scroll bars x,y */
    } else {
    if (xOffset() == 0) {
    hacks = true;
    transmouse.translate (0,yOffset());  /* scroll bars x,y */   
    } else {
    transmouse.translate(SLIDERMARGIN_TICK_TOTAL + SLIDERSPACER ,yOffset());  /* scroll bars x,y */  
    } 
    }
    QPointF np = transmouse.map(p);
    if (hacks) {
    np.setX ( SLIDERMARGIN_TICK_TOTAL / 2 );
    }
    return np;
}

/* ####################################### Translate mouse events end ###################################*/














