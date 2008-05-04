#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QGraphicsView>
#include "GraphicsItemEdit.h"
#include "mounttextprivate.h"
#include "GraphicsScene.h"
#include <math.h>

class GraphicsView : public QGraphicsView
{
     Q_OBJECT
//
public:
  GraphicsView( QWidget * parent  = 0 );
protected:
    void resizeEvent(QResizeEvent *event);
    void wheelEvent (QWheelEvent * event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void scaleView (qreal scaleFactor);
  GraphicsScene *scene;
  qreal width;
  qreal height;
  uint layercount;
  QWidget *BigParent;
  QPixmap chessgrid;
  TextLayer *CurrentActive;
private:
  void fillNullItem();
signals:
public slots:
    void AppendDemo();
    void WorksOn(QGraphicsItem * item , qreal zindex );
    void DisplayTop();

};
//
#endif // GRAPHICSVIEW_H

