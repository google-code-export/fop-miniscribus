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

#include <math.h>
#include <QPair>
#ifndef QT_NO_OPENGL
#include <QGLWidget>
#endif



#include "GraphicsScene.h"





class GraphicsView : public QGraphicsView
{
     Q_OBJECT
//
public:
   GraphicsView( QWidget * parent  = 0 );
signals:
   void SceneSwap();
public slots:
private slots:
protected:
private:




};




class Panel : public QFrame
{
    Q_OBJECT
public:
    Panel(const QString &name, QWidget *parent = 0);
    GraphicsView *view() const;
    inline QGraphicsItem *Active() { return graphicsView->scene->Active(); }

    ///////QSize minimumSizeHint() const;
    ///////////QSize sizeHint() const;

    GraphicsView *graphicsView;
public slots:

private slots:
 
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void DisplayTop();
    void zoomIn();
    void zoomOut();
    void SceneChange();

private:
    uint tievents;
    
    QLabel *label;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
    QToolButton *printButton;
 
};




#endif // GRAPHICSVIEW_H






















//


