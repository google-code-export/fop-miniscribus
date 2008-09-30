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
     inline int xOffset() const { return horizontalScrollBar()->value(); }
     inline int yOffset() const { return verticalScrollBar()->value(); }
     
protected:
    
     void paintEvent( QPaintEvent *Event );
     void mousePressEvent ( QMouseEvent *e );
     void mouseDoubleClickEvent ( QMouseEvent *e );
     void adjustScrollbars();
     void wheelEvent (QWheelEvent * event);
     void resizeEvent(QResizeEvent *e);
     void contextMenuEvent(QContextMenuEvent *event);
     void keyPressEvent ( QKeyEvent * e );

private:
    
    QPointF maps( QPointF p );
    QRectF page;
    qreal mesure; /* fix faktor cm*/
    QTransform mcurrent;
    QList<QPointF> dotChain;
    QSize workArea;  /* mobile size from zoom */
    qreal scaleFaktor;
    bool portrait_mode;
    qint64 lineTimer;
    QRectF slider_Horrizzontal_Top;
    QRectF slider_Vertical_Left;
    PDocument *_doc;  ///  

signals:


public slots:
    void triggerFormat();
    void fitToLarge();
    void setZoom( const qreal value );
    void zoomIn();
    void zoomOut();

private slots:
    
void verticalValue( const int index );


};











#endif // DOCEDIT_H

