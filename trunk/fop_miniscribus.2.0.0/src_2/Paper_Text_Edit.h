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
   TextLayer *BASE_TEXT;
   
signals:
   void SceneSwap();
   void NewPageFormatin();
public slots:
   void DisplayTop();
   void  ViewDisplay( const QRectF area );
   void  SwapPaper();
private slots:
    void ForceResize();

protected:
    void pageclear();
    QPixmap chessgrid;
    void PrintSetup( bool printok );
    void keyPressEvent(QKeyEvent *e );
    void resizeEvent(QResizeEvent *event);
    QSettings setter;
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











#endif // SCRIBEAPI_H

