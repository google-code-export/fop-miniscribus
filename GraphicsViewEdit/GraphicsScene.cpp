/*******************************************************************************
 * class GraphicsScene
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/

#include "GraphicsScene.h"
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

void GraphicsScene::reload()
{
  update();
}
QGraphicsItem *GraphicsScene::ItemTop( const QPointF incomming ) 
{
       QList<QGraphicsItem *>  listing = QGraphicsScene::items(incomming);
       qreal thebest = 0;  /* zindex on front */
       for (int o=0;o<listing.size();o++) {
           thebest = qMax(listing[o]->zValue(),thebest);
       }
       for (int e=0;e<listing.size();e++) {
           if (listing[e]->zValue() == thebest) {
             return listing[e];
           }
       }
       emit nullitem();
       
return NULL;
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
       ///////qDebug() << "### thebest->" << thebest;
       /* activate item at top z-index zValue  /  not the down not visible!!!! */
       for (int e=0;e<listing.size();e++) {
           if (listing[e]->zValue() == thebest) {
             listing[e]->setFlag(QGraphicsItem::ItemIsSelectable,true);
             listing[e]->setSelected(true); 
             emit SelectOn(listing[e],thebest);
             return true;
           }
       }
       emit nullitem();
return false;
}

void GraphicsScene::TopFocus()
{
   QGraphicsItem *xx = QGraphicsScene::itemAt(1,1);
   QGraphicsScene::setFocusItem(xx);
   
}

void GraphicsScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void GraphicsScene::storno()
{
    emit nullitem();
    
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
void GraphicsScene::remid( const int id )
{
    clearSelection();
    QList<QGraphicsItem *> listing = QGraphicsScene::items();
    for (int o=0;o<listing.size();o++) {
        
        if (listing[o]->data(ObjectNameEditor).toInt() == id) {
            QGraphicsScene::removeItem(listing[o]);  
            //////delete listing[o];
        }
    }
    reload();
    storno();
}

void GraphicsScene::clear()
{
    allclear();
}


void GraphicsScene::allclear()
{
    clearSelection();
    QList<QGraphicsItem *> listing = QGraphicsScene::items();
    for (int o=0;o<listing.size();o++) {
    QGraphicsScene::removeItem(listing[o]);  
    }
    storno();
}
qreal GraphicsScene::zmax()
{
 qreal mValue = 0.00;
 QList<QGraphicsItem *> items = QGraphicsScene::items();
     for (int o=0;o<items.size();o++) {
       mValue = qMax(items[o]->zValue(),mValue);
    }
 return mValue;
}

qreal GraphicsScene::zmin()
{
 qreal mValue = zmax();
 QList<QGraphicsItem *> items = QGraphicsScene::items();
     for (int o=0;o<items.size();o++) {
       mValue = qMin(items[o]->zValue(),mValue);
    }
 return mValue;
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
#if QT_VERSION >= 0x040400
QGraphicsProxyWidget *GraphicsScene::addWidget(QWidget * widget, Qt::WindowFlags wFlags )
{
    return QGraphicsScene::addWidget(widget,wFlags);
}
#endif








