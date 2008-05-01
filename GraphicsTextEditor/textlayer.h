#ifndef TEXTLAYER_H
#define TEXTLAYER_H

#include "mounttextprivate.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QSharedData>

QT_BEGIN_HEADER

typedef enum {DIV_ABSOLUTE = 50,DIV_AUTO,/* 51 */DIV_FLOAT,/* 52 */DIV_HEADER,DIV_FOOTER} LAYERTYPE;

QT_BEGIN_NAMESPACE

class TextController;
class LogData;

class TextLayer : public QObject, public QGraphicsItem
{
   Q_OBJECT 

public:
    TextLayer(const int layer_id , QGraphicsItem *parent = 0 , QGraphicsScene *scene = 0);
    ~TextLayer();
    void setSelected( bool selected );
    QRectF boundingRect() const;
    QTextDocument *document();
    void setDocument ( QTextDocument * document );
    QTextCursor textCursor();
    RichDoc ReadActualItem();
    void insert( RichDoc Rdoc );
    void setZValue(qreal index );
    bool contains(const QPointF &point) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    enum { Type = 22 };
    int type() const;
    LAYERTYPE Ltype() const;
    enum CurrentModus{ Show, Edit, Move , Lock };
    void setStyle( QStringList syle , bool fromclone );
    QTextDocument *_doc;
protected:
    QSettings setter;
    QAction *actionSwapEdit;
    void init();
    void read();
    void RestoreMoveAction();
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void timerEvent(QTimerEvent *event);
    ////void inputMethodEvent(QInputMethodEvent *event);
    void keyPressEvent( QKeyEvent * event );
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QMenu *RootMenu;
    QAbstractTextDocumentLayout *DLayout;
    LAYERTYPE format;
    CurrentModus modus;
    uint evesum;
    bool IsSelectActive;
    int id;
    bool sceneEvent(QEvent *event);
private:
    TextController *mount;
    QMap<uint,QString> history;
    QColor bgcolor;
    QColor bordercolor;
    qreal border;
    qreal wi;
    qreal hi;
    qreal text_hight;
    qreal text_width;
    QRectF TextboundingRect;
public slots:
    void updatearea( const QRectF areas );
    void cursor_wake_up();
    void cursorPositionChanged( const QTextCursor cur);
    void CommitLayer();
    void InsertRevision();
    void EditModus();
    void SetNewBGColor();
    void SaveFilelayer();
    void OpenFilelayer();
    void SwapEdit();
};

Q_DECLARE_METATYPE(TextLayer *)

class TextController: public QObject
{
    Q_OBJECT 
    
public:
   TextController();
   TextWriter *txtControl() const;
   mutable TextWriter *device;
   TextLayer *q;
   public slots:
};

Q_DECLARE_METATYPE(TextController *)





QT_END_NAMESPACE
QT_END_HEADER



//
#endif // TEXTLAYER_H

