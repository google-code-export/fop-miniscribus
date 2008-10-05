#ifndef DOCEDIT_H
#define DOCEDIT_H

#include "PDocConfig.h"
#include "PDocument.h"


class EditArea : public QAbstractScrollArea
{
    Q_OBJECT
//
public:
    
     EditArea( QWidget *parent = 0 );
     QRectF boundingRect() const; /* viewport rect */
     
protected:
    
     void paintEvent( QPaintEvent *Event );
     void mousePressEvent ( QMouseEvent *e );
     void mouseDoubleClickEvent ( QMouseEvent *e );
     void mouseMoveEvent ( QMouseEvent *e );
     void mouseReleaseEvent ( QMouseEvent *e );
     void adjustScrollbars();
     void cursorCheck();
     void wheelEvent (QWheelEvent * event);
     void resizeEvent(QResizeEvent *e);
     void contextMenuEvent(QContextMenuEvent *event);
     void keyPressEvent ( QKeyEvent * e );
     void cursorRectSlider( const QTextFrameFormat docrootformat  , QPainter *p );
     bool clickSlider( const QPointF p );
     inline int xOffset() const { return horizontalScrollBar()->value(); }
     inline int yOffset() const { return verticalScrollBar()->value(); }

private:
    QRectF sl_cursor[6];
    QRectF debugRect;
    QPointF maps( QPointF p );
    QPointF slider_maps( QPointF p , bool top );
    QRectF page;
    qreal mesure; /* fix faktor cm*/
    qreal border_wi;  /*  border from left to first topslider center zoom */
    QTransform mcurrent;
    QTransform top_matrix;
    QTransform left_matrix;
    QList<QPointF> dotChain;
    QSize workArea;  /* mobile size from zoom */
    qreal scaleFaktor;
    bool portrait_mode;
    qint64 lineTimer;
    QRectF slider_Horrizzontal_Top;
    QRectF slider_Vertical_Left;
    PDocument *_doc;  ///  
    bool isOnSlider( const QPointF p );
    bool HandleMoveSlider(  QPointF point , bool top = true );

signals:


public slots:
    void triggerFormat();
    void fitToLarge();
    void setZoom( const qreal value );
    void zoomIn();
    void zoomOut();
    void update( const  QRectF rect );
    void update();

private slots:
void verticalValue( const int index );


};











#endif // DOCEDIT_H

