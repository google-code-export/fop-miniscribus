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

    /*  text api */
    void setBlinkingCursorEnabled( bool enable );
    bool editEnable();
    void cursorMovetoPosition( const QPointF &pos );
    QTextCursor textCursor();
    void repaintCursor( bool allrect = false );
    QRectF CurrentBlockRect();
    QTransform pageMatrix();
    QTextLine currentTextLine(const QTextCursor &cursor);
    inline bool getoverwriteMode() {
        return overwriteMode;    /* insert modus */
    }

    /*   text api   */

protected:

    void paintEvent( QPaintEvent *Event );
    void mousePressEvent ( QMouseEvent *e );
    void mouseDoubleClickEvent ( QMouseEvent *e );
    void mouseMoveEvent ( QMouseEvent *e );
    void mouseReleaseEvent ( QMouseEvent *e );
    void timerEvent(QTimerEvent *event);
    void adjustScrollbars();
    void cursorCheck();
    void wheelEvent (QWheelEvent * event);
    void resizeEvent(QResizeEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent ( QKeyEvent * e );
    void cursorRectSlider( const QTextFrameFormat docrootformat  , QPainter *p );
    bool clickSlider( const QPointF p );
    inline int xOffset() const {
        return horizontalScrollBar()->value();
    }
    inline int yOffset() const {
        return verticalScrollBar()->value();
    }

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

    /*   text api   */
    QBasicTimer cursorTimeLine;
    bool cursortime;
    bool overwriteMode;
    QTextCursor C_cursor;
    QTextCharFormat LastCharFormat;
    QColor HightlightColor() const;
    int position_selection_start;

    void paintEditPage( const int index  , QPainter * painter  , const QRectF viewarea );
    bool cursorMoveKeyEvent(QKeyEvent *e);
    void swapOverwriteMode();
    void gotoNextTableCell();
    void gotoPreviousTableCell();
    QTextTableCell cellOnPosition( const int posi );


    void Controller_keyPressEvent ( QKeyEvent * e );



    /*   text api   */
signals:


public slots:
    void triggerFormat();
    void fitToLarge();
    void setZoom( const qreal value );
    void zoomIn();
    void zoomOut();
    void update( const  QRectF rect );
    void update();
    void selectAll();
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    void EnsureVisibleCursor();




private slots:
    void verticalValue( const int index );


};











#endif // DOCEDIT_H

