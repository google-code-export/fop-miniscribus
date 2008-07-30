#include "_LayerExtended.h"


TextMount::TextMount()
  : device(0)
{ 

}
/* only one connect */
ScribePage *TextMount::txtControl() const
{
    if (!device) {
       M_PageSize A4;
       TextLayer *that = const_cast<TextLayer *>(q); 
       device = new ScribePage(A4);
        //////////qDebug() << "### init  api txt ...........................................................................";
      connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursor_wake_up()));
      connect(device, SIGNAL(q_update(QRect) ),q, SLOT(updatearea(QRect)));
      connect(device, SIGNAL(q_visible(QRectF) ),q, SLOT(ensureVisible(QRectF)));
      connect(device, SIGNAL(q_update_scene()),q, SLOT(SceneReload()));
    }
    return device;
}


TextLayer::~TextLayer()
{
  qDebug() << "### destroy obj ...";
}



TextLayer::TextLayer( QGraphicsItem *parent  )
    : QGraphicsRectItem(QRectF(0,0,100,100),parent),dev(new TextMount)
{
    qDebug() << "### init....";
    dev->q = this;
	  setAcceptsHoverEvents(true);
    setAcceptDrops(true);
    QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setFlag(QGraphicsItem::ItemIsMovable,false);
    setZValue (3.555555);
    LastRect = dev->txtControl()->boundingRect();
    QGraphicsRectItem::setRect(LastRect);
    
    ////////qDebug() << "### init...." << LastRect;
    //////////////qDebug() << "### init...." << PageName();
    LastUpdateRequest = LastRect;
    
}

QTextCursor TextLayer::textCursor() 
{
  return dev->txtControl()->textCursor();
}


QTextDocument *TextLayer::document() const
{
  return dev->txtControl()->document();
}

void TextLayer::updatearea( const QRect areas )
{
    LastRect = dev->txtControl()->boundingRect();
    QGraphicsRectItem::setRect(LastRect);
    ////////////qDebug() << "### updatearea " << areas;
    LastUpdateRequest = areas;
    update(areas);
}

void TextLayer::cursor_wake_up()
{
    const QRectF rectblock = dev->txtControl()->CurrentBlockRect();
    updatearea(rectblock.toRect());
    ///////////////qDebug() << "### cursor....................................................................... ";
}


void TextLayer::SceneReload()
{
     /////////qDebug() << "### SceneReload page count " << document()->pageCount();
     GraphicsScene *sc;
     if (sc = qobject_cast<GraphicsScene *>(scene())) {
     sc->clearSelection();
     }
}

void TextLayer::ensureVisible( const QRectF areas )
{
     LastVisibleRequest = areas;
     GraphicsScene *sc;
     if (sc = qobject_cast<GraphicsScene *>(scene())) {
     sc->SetVisibleArea(areas);
     }
}


QString TextLayer::PageName()
{
    return dev->txtControl()->Model().HName();
}


void TextLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    LastRect = dev->txtControl()->boundingRect();
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen( Qt::NoPen );
    painter->setBrush(Qt::lightGray);
		painter->drawRect(LastRect);
    
    const int PageSumm = qBound (1,document()->pageCount(),MaximumPages);
    dev->txtControl()->paint(painter,option,widget);
    painter->setPen( Qt::NoPen );
    
    for (int i = 0; i < PageSumm; ++i)  {
        const QRectF pagen =  dev->txtControl()->Model().PageExternal(i);
        QRectF rightShadow(pagen.right(), pagen.top() + BorderShadow, BorderShadow, pagen.height());
        QRectF bottomShadow(pagen.left() + BorderShadow, pagen.bottom(), pagen.width(), BorderShadow);
        
        painter->fillRect(rightShadow, Qt::darkGray);
        painter->fillRect(bottomShadow, Qt::darkGray);
        
        
        painter->setBrush(Qt::NoBrush);
        painter->setPen( QPen(Qt::black,0.3) );
        painter->drawRect(pagen);
        painter->setPen( Qt::NoPen );
        
	  }
    
    
    QColor Visiblerecord(Qt::red);
		Visiblerecord.setAlpha(22);
    painter->setBrush(Visiblerecord);
    painter->drawRect(LastUpdateRequest);
    
    
    
   /*
    QColor BackHightlight("#a6ffc7");
		BackHightlight.setAlpha(80);
    
    painter->setPen( Qt::NoPen );
    painter->setBrush(BackHightlight);
		painter->drawRect(LastUpdateRequest);
    
    
     
    QColor Visiblerecord(Qt::red);
		Visiblerecord.setAlpha(45);
    
    if (!LastVisibleRequest.isNull()) {
    painter->setBrush(Visiblerecord);
		painter->drawRect(LastVisibleRequest);
    }
    */
    
}

void TextLayer::focusInEvent ( QFocusEvent * event ) 
{
    /////////////qDebug() << "### focusInEvent ..." << flags();
    QGraphicsItem::setSelected(true);
    scene()->setFocusItem(this,Qt::ShortcutFocusReason);
    return QGraphicsItem::focusInEvent(event);
}

void TextLayer::focusOutEvent ( QFocusEvent * event ) 
{
   //////////// qDebug() << "### focusOutEvent ...";
    QGraphicsItem::setSelected(false);
    return QGraphicsItem::focusOutEvent(event);
}

void TextLayer::inputMethodEvent ( QInputMethodEvent * event )  
{
    ///////////qDebug() << "### inputMethodEvent ...";
     return QGraphicsItem::inputMethodEvent(event);
}


QRectF TextLayer::boundingRect() const
{
    return dev->txtControl()->GroupboundingRect();  /* shadow */
}

void TextLayer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //////////// qDebug() << "### mouseDoubleClickEvent...";
     return QGraphicsItem::mouseDoubleClickEvent(event);
}


void TextLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ///////////qDebug() << "### mouseMoveEvent.. ";
    
    if (dev->txtControl()->AllowedPosition(event->pos())) {
    dev->txtControl()->procesevent(event);
    }
    
    ////////return QGraphicsItem::mouseMoveEvent(event);
    
}

void TextLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  ///////////qDebug() << "### mouseReleaseEvent...";
    
    if (dev->txtControl()->AllowedPosition(event->pos())) {
    dev->txtControl()->procesevent(event);
    }
    
    /////return QGraphicsItem::mouseReleaseEvent(event);
}

void TextLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ////qDebug() << "### mousePressEvent...";
    if (dev->txtControl()->AllowedPosition(event->pos())) {
    dev->txtControl()->procesevent(event);
    }
    return QGraphicsItem::mousePressEvent(event);
}

void TextLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    ///////qDebug() << "### hoverEnterEvent...";
    return QGraphicsItem::hoverEnterEvent(event);
}

void TextLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    ///////////qDebug() << "### hoverLeaveEvent...";
    return QGraphicsItem::hoverLeaveEvent(event);
}

void TextLayer::keyPressEvent( QKeyEvent * event ) 
{
   //////////qDebug() << "### keyPressEvent...";
   dev->txtControl()->Controller_keyPressEvent(event);
   /////return QGraphicsItem::keyPressEvent(event);
}

void TextLayer::keyReleaseEvent ( QKeyEvent * event )
{
    ///////qDebug() << "### keyReleaseEvent...";
    dev->txtControl()->Controller_keyReleaseEvent(event);
    //////////return QGraphicsItem::keyReleaseEvent(event);
}


bool TextLayer::sceneEvent(QEvent *event)
{
    /*    buggi qt version 4.4. 4.5 
    if (event->type() == QEvent::UngrabMouse || 
        event->type() == QEvent::GrabMouse ) {
        dev->txtControl()->procesevent(event);
     return true;   
    }
    */
    
    if (event->type() == QEvent::GraphicsSceneDragEnter || 
        event->type() == QEvent::GraphicsSceneDrop || 
        event->type() == QEvent::GraphicsSceneDragLeave || 
        event->type() == QEvent::GraphicsSceneDragMove) { 
        dev->txtControl()->procesevent(event);
       return true;
     }
    
  const int sxa = event->type();
  if (   sxa != 161 ) {  
  ///////////qDebug() << "### sceneEvent.... + type   " << event->type();
  }
  return QGraphicsItem::sceneEvent(event);
}

void TextLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   ///////////qDebug() << "### contextMenuEvent....";
}

