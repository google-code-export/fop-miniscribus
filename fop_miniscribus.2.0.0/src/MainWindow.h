#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Config.h"
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include "ui_gmain.h"
#include "BookTree.h"
#include "BasicFoConfig.h"
#define _MAINICONSIZE_  18
#include "Text_Api.h"
#include "Layer_Auto_Extended.h"
#include "Scribe_Parser.h"
#include "ziphandle.h"   ////// open file gzip 



class PaperEditor;
class GraphicsView;
class MainWindow : public QMainWindow, public Ui::Gmain
{
     Q_OBJECT
//
public:
    MainWindow( QWidget *parent  = 0 );
protected:
    uint barevents;
    QToolBar *docbar;
    QToolBar *tb_0;
    QToolBar *tb_1;
    QToolBar *tb_2;
    QToolBar *tb_3;
    QToolBar *tb_4;
    QToolBar *tb_5;
    QList<QToolBar*> toolse;
    PaperEditor *edit;  /* edit = new  PaperEditor(this) */
    QAction  *actioBooks;
    QSettings setter;
QToolButton *tbooks;
QDockWidget *pdfBookMarkManager;
BookTree *treeBooks;
bool eventFilter(QObject *obj, QEvent *event);
void resizeEvent ( QResizeEvent * e );
void prepareDocks();
private:
    void CheckFileApps();
    void menuAbsoluteLayer();
signals:

public slots:
    void on_pagereformat_triggered();
    void on_actionGenralSave_triggered();
    void setFile( const QString file , const QString option = QString() );
    void on_actionSaveAs_triggered();
    
private slots:
    /* remake action to cursor */
void menuUpdate( bool modus );
void showBooks(bool e );

};


class GraphicsView : public QGraphicsView
{
     Q_OBJECT
//
public:
    GraphicsView( QWidget *parent = 0 );
    inline TextLayer *page() { return pageFull; }
    GraphicsScene *scene;
    bool tryToAutoSave();
    void forceResizeExtern();
protected:
    QRectF rectToScene();
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent *event);
    TextLayer *pageFull;
    QSettings setter;
    /* current file setting */
    QString currentopenfilerunning;
    QString currentopenfilealternate;
    bool gzippedfile;
    QTextCodec *currentfilecodec;
    

private:
    
signals:
  void sceneSwap(bool);
  void NewPageFormatin();
  void inBookmark(QStringList);
  void inBookmark(QStringList,QStandardItemModel*);
  void fileBaseOpen(QString);
  void bookMarkActive(bool);
public slots:
    void viewDisplay( const QRectF area );
     void DisplayTop();
    void openFile();
    void openFile( const QString file );
    void saveAsFile();
    void saveOnFile();
    bool saveFopFile( const  QString file , bool memo = true );
    bool saveHtmlFile( const  QString file ,  bool memo = true  );
    void zoomChain();
    void zoomChainStop();
    void apacheFopConvert();
    void saveOnPageBinFile();
    void pageclear();


private slots:
void forceResizeIntern();
void cursorChange(bool);
void sendinBookmark(QStringList);
void openMasterMainFile();
};


static const int DefaultStartZoom = 220;


class PaperEditor : public QFrame
{
    Q_OBJECT
public:
    PaperEditor( QWidget *parent = 0 );
    GraphicsView *view() const;
public slots:

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void DisplayTop();
    void zoomIn();
    void zoomOut();
    void SceneChange();
    void toggleOpenGL();
private:
    GraphicsView *graphicsView;
    bool NotPaperUpdate;
    uint tievents;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QToolButton *printButton;
    QToolButton *openGlButton;
};















//
#endif // MAINWINDOW_H

