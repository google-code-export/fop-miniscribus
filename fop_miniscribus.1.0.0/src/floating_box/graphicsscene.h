#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class GraphicsScene : public QGraphicsScene
{
     Q_OBJECT
    
public:
GraphicsScene( QObject * parent = 0 );
GraphicsScene( const QRectF & sceneRect, QObject * parent = 0 );
GraphicsScene( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 );
void setSceneRect( qreal x, qreal y, qreal w, qreal h );
void setSceneRect(  const QRectF & rect );
void clearSelection();
protected:
    QRectF Area;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * e );
    void mousePressEvent( QGraphicsSceneMouseEvent * e );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    bool WakeUp( const QPointF incomming );
private:
signals:
   ////////void Activate(QPointF);  /* QPointF */
   void SelectOn( QGraphicsItem * item , qreal zindex );  /* QPointF */
public slots:

};
//
#endif // GRAPHICSSCENE_H

