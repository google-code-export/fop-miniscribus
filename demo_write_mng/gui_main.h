#ifndef GUI_MAIN_H
#define GUI_MAIN_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QCloseEvent>
////////////#include "animationwriter.h"
#include "ui_gmain.h"
#include <QPainter>
#include "animationwriter.h"
#include <QDesktopWidget>
#include <QTimerEvent>
#include <QFileDialog>
#include <QMovie>
#include "movieplayer.h"



class Foto_DD : public QWidget
{
    Q_OBJECT
//
public:

    Foto_DD( QWidget* = 0 )
{
   rotatari = 0;
}
void paint( QPixmap e )
{
    base = e;
    repaint();
}

void paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawPixmap(0,0,base);
    painter.end();
}
protected:
    void mousePressEvent(QMouseEvent *e) 
{
    emit PrMouse(e);
}
void mouseMoveEvent(QMouseEvent *e)
{
    emit MoMouse(e);
}
private:
    QPixmap base;
    int rotatari;
signals:
     void PrMouse(QMouseEvent *e);
     void MoMouse(QMouseEvent *e);
public slots:

};





class Gui_Main : public QMainWindow, public Ui::MainWindow
{
     Q_OBJECT
//
public:
   Gui_Main( QWidget * parent = 0 );
protected:
void timerEvent(QTimerEvent *event);
AnimationWriter *animation;
QBasicTimer FrameTimeLine;
QImage CatScreen();
private:
   int framerate;
   MoviePlayer *Movie_Label;
   Foto_DD *devimg;
   uint timeline;
   void setCaptureEnabled(bool enable);
   bool RRunning;
signals:

public slots:
   void StopRead();
   void StartRead();
   void on_grabstart_clicked();
   void on_grabstop_clicked();
   void movie_end();
   void on_actionOpen_mng_activated();

};
//
#endif // GUI_MAIN_H

