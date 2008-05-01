#ifndef MOUNTTEXTPRIVATE_H
#define MOUNTTEXTPRIVATE_H

#include <QtGui/qtextdocument.h>
#include <QtGui/qtextoption.h>
#include <QtGui/qtextcursor.h>
#include <QtGui/qtextformat.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qmenu.h>
#include <QtCore/qrect.h>
#include <QtGui/qabstracttextdocumentlayout.h>
#include <QtGui/qtextdocumentfragment.h>

#include "mimedataeditor.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QStyleSheet;
class QTextDocument;
class QMenu;
class QTextControlPrivate;
class QMimeData;
class QAbstractScrollArea;
class QEvent;
class QTimerEvent;

class TextWriter : public Layoutpainter
{
    Q_OBJECT
    //////Q_DECLARE_PRIVATE(TextWriter)
public:
    explicit TextWriter( QObject *parent );
    virtual ~TextWriter();
    void setBlinkingCursorEnabled(bool enable);
    QMenu *StandardMenu( QWidget * inparent );   /* copy paste select all qmenu */
    QTextLine currentTextLine(const QTextCursor &cursor);
    void setDocument ( QTextDocument * document , QObject *parent = 0 );
    inline bool editable() { return edit_enable; }
    void edit( bool e);
    QTextDocument *document();
    void paint_doc(  QPainter * painter ,
                         const QStyleOptionGraphicsItem *option , 
                         QBrush BGpage , 
                         const QRectF fulllayer ,  
                         bool alternate );
    QRectF boundingRect() const;
    inline QPointF controlOffset() { return QPointF(0., boundingRect().height()); }
    void procesevent( QEvent *e );
    void ClearSelections();
    void RegisterResource( QMap<QString,SPics> list );
    inline QMap<QString,SPics> imglist() { return imagemaps; }
private:
    QSettings setter;
    int hitTest(const QPointF &point, Qt::HitTestAccuracy accuracy) const;
    void selectionChanged(bool forceEmitSelectionChanged = false );
    QClipboard *clipboard;
    /* portanti */
    QRectF lastrect;
    int StartSelectionMouse;
    /////int cursor_position;
    int position_selection_start;
    QObject *bridge;
    //////QTextCursor C_cursor;
    //////QTextDocument *_d;
    QBasicTimer cursorTimeLine;
    QRectF TextHighlightSelect;

    void setContent(Qt::TextFormat format, QString text, QTextDocument *document);
    QMap<QString,SPics> imagemaps;
    
    Q_DISABLE_COPY(TextWriter)
    bool overwriteMode;  /* overwrite on write */
    bool edit_enable;
    bool cursorOn;
    bool cursortime;
    uint timeline;
    QAbstractTextDocumentLayout *_layout_1;
    bool cursorIsFocusIndicator;
    
QMimeData *createMimeDataFromSelection() const;
void insertFromMimeData(const QMimeData *source);
    
    /* events */
void setCursorPosition(const QPointF &pos);
void repaintCursor( bool allrect = false );  
void timerEvent(QTimerEvent *event);
bool cursorMoveKeyEvent(QKeyEvent *e);
void tmouseMoveEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos);
void tmousePressEvent(Qt::MouseButton button, const QPointF &pos, Qt::KeyboardModifiers modifiers,
                                          Qt::MouseButtons buttons, const QPoint &globalPos);
void tmouseReleaseEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos);
void tmouseDoubleClickEvent(QEvent *e, Qt::MouseButton button, const QPointF &pos);
void tkeyPressEvent(QKeyEvent *e);

    /* events */
signals:
    void docselectionChanged();
    void q_cursor_newPos();
    void q_update(QRectF);
public slots:
     void updateRequest( QRectF area );
     void selectAll();
     void cut();
     void paste();
     void copy();
     void int_clipboard_new();
     void deleteSelected();
     void SetLayerMargin();
     void  ImageonCursor( QString file );
     void  InsertImageonCursor();
     void  RegisterImage( SPics e , bool insert );
};

Q_DECLARE_METATYPE(TextWriter *)


QT_END_NAMESPACE

QT_END_HEADER

//
#endif // MOUNTTEXTPRIVATE_H

