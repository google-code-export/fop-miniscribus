#include "graphicsscene.h"
//
/*  Save file as graphicsscene.cpp  */
/*  incomming class name GraphicsScene */
//
GraphicsScene::GraphicsScene( QObject * parent )
	: QGraphicsScene( parent )
{
	
}
GraphicsScene::GraphicsScene(  const QRectF & sceneRect, QObject * parent )
	: QGraphicsScene( sceneRect,  parent )
{
	setSceneRect( sceneRect );
}
GraphicsScene::GraphicsScene( qreal x, qreal y, qreal width, qreal height, QObject * parent )
	: QGraphicsScene(x,y,width,height,parent )
{
	setSceneRect(x,y,width,height);
}

/* filter only item a top Zindex /  zValue */
bool GraphicsScene::WakeUp( const QPointF incomming ) 
{
       QList<QGraphicsItem *>  listing = QGraphicsScene::items(incomming);
       qreal thebest = 0;  /* zindex on front */
       for (int o=0;o<listing.size();o++) {
           listing[o]->setSelected(false); 
           listing[o]->setFlag(QGraphicsItem::ItemIsSelectable,false);
           thebest = qMax(listing[o]->zValue(),thebest);
       }
       /* activate item at top z-index zValue  /  not the down not visible!!!! */
       for (int e=0;e<listing.size();e++) {
           if (listing[e]->zValue() == thebest) {
             listing[e]->setFlag(QGraphicsItem::ItemIsSelectable,true);
             emit SelectOn(listing[e],thebest);
             return true;
           }
       }
return false;
}
void GraphicsScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    }
    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}



void GraphicsScene::clearSelection()
{
    QGraphicsScene::clearSelection();
    
}





void GraphicsScene::setSceneRect( qreal x, qreal y, qreal w, qreal h ) 
{
    Area = QRectF(x,y,w,h);
    QGraphicsScene::setSceneRect(x,y,w,h);
}

void GraphicsScene::setSceneRect(  const QRectF & rect ) 
{
    Area = rect;
    QGraphicsScene::setSceneRect(rect);
}











