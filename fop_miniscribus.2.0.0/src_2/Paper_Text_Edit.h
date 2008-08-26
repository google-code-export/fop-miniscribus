#ifndef SCRIBEAPI_H
#define SCRIBEAPI_H
#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>


#include "SessionManager.h"
#include <QGraphicsView>
#include "Layer_Auto_Extended.h"
#include "Text_Api.h"
#include "xslt_convert.h"
#include "Fo_Reader.h"
#include "Tools_Basic.h"


#define _MAINICONSIZE_  18

/*

file to display scene and view !!! 


*/

static const int DefaultStartZoom = 220;



class GraphicsView : public QGraphicsView
{
     Q_OBJECT
//
public:
   GraphicsView( QWidget * parent  = 0 );
   ~GraphicsView();
   QRectF boundingRect();
   GraphicsScene *scene;
   QRectF rectToScene();
   TextLayer *autopage() { return BASE_TEXT; }
   TextLayer *BASE_TEXT;
   
signals:
   void sceneSwap();
   void NewPageFormatin();
   void dinamic_autocursor_swap();
public slots:
   void DisplayTop();
   void ViewDisplay( const QRectF area );
   void SwapPaper();
   void forceResize();
   void openFile( const QString file );
   void openFile();
   void cursorChange();
private slots:
    

protected:
    QString currentopenfilerunning;
    QString currentopenfilealternate;
    bool gzippedfile;
    QTextCodec *currentfilecodec;
    void pageclear();
    QPixmap chessgrid;
    void PrintSetup( bool printok );
    void keyPressEvent(QKeyEvent *e );
    void resizeEvent(QResizeEvent *event);
    QSettings setter;
    void recordActionHere();
private:
    bool OnPrintRender;

};


class Panel : public QFrame
{
    Q_OBJECT
public:
    Panel( QWidget *parent = 0 );
    GraphicsView *view() const;
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
    void toggleOpenGL();
    void PaperSwap( const int index );
    void FillPaperSize();
private:
    ///////QToolBar *tb;
    bool NotPaperUpdate;
    uint tievents;
    QLabel *label;
    QComboBox *PortraitPaper;
    //////QComboBox *LandscapePaper;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
    QToolButton *printButton;
    QToolButton *openGlButton;
};




class PaperTextEdit : public QMainWindow

{
    Q_OBJECT
    
public:
    PaperTextEdit( QWidget *parent = 0 );
    ~PaperTextEdit();
private:
    QToolBar *docbar;
    QToolBar *tb_0;
    QToolBar *tb_1;
    QToolBar *tb_2;
    QToolBar *tb_3;
    QToolBar *tb_4;
    QToolBar *tb_5;
    Panel *panel;

public slots:

private slots:
    void auto_page_dinamic();
};









#endif // SCRIBEAPI_H

