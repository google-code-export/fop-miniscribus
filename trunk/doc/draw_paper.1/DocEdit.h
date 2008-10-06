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
    void insertImage( const  QImage image );
    void insertImage( const QPixmap image );
    void insertMimeDataOnCursor( const QMimeData *md );
    void setBlinkingCursorEnabled( bool enable );  /* edit on/off */
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
    inline QRectF textCursorRect() { return visibleRects; }

    /*   text api   */

protected:

    void paintEvent( QPaintEvent *Event );
    void mousePressEvent ( QMouseEvent *e );
    void mouseDoubleClickEvent ( QMouseEvent *e );
    void mouseMoveEvent ( QMouseEvent *e );
    void mouseReleaseEvent ( QMouseEvent *e );
    void dragMoveEvent(QDragMoveEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
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
    QRectF visibleRects;
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
    
    QScrollBar *vbar;
    QScrollBar *hbar;

    /*   text api   */
    QClipboard *clipboard;
    QBasicTimer cursorTimeLine;
    QBasicTimer trippleClickTimer;
    int preeditCursor;
    /////QBasicTimer dragClickTimer;
    bool cursortime;
    bool overwriteMode;
    bool cursorIsFocusIndicator;
    QTextCursor C_cursor;
    QTextCharFormat LastCharFormat;
    QColor HightlightColor() const;
    int position_selection_start;
    
    QPair<int,int> RangeSelection;

    void paintEditPage( const int index  , QPainter * painter  , const QRectF viewarea );
    bool cursorMoveKeyEvent(QKeyEvent *e);
    void swapOverwriteMode();
    void gotoNextTableCell();
    void gotoPreviousTableCell();
    QTextTableCell cellOnPosition( const int posi );

    QMimeData *createMimeDataFromSelection();

    void controller_keyPressEvent ( QKeyEvent * e );
    void textMoveEvent( const QPointF point  );
    void textDoubleClickEvent( const  QPointF point );
    void resetClickTimer();
    void startDragAction();
    
    bool isSelfPlacePaste();
    QRectF rectForPosition(int position) const;
    bool rtl;   /* is right to left writteln */

    /*   text api   */
signals:
    void txtcursorChanged(QTextCursor);

public slots:
    void triggerFormat();
    void fitToLarge();
    void fitToNormal();
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
    void ensureVisible( const QRectF rect );
    void AutoReload();




private slots:
    void verticalValue( const int index );
    void clipboard_new();
    void cursorPosition( const QTextCursor  cursor );


};











#endif // DOCEDIT_H

