#ifndef TEXTLAYER_H
#define TEXTLAYER_H

#include "mounttextprivate.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class TextLayer : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    TextLayer(const int layer_id , QGraphicsItem *parent = 0 , QGraphicsScene *scene = 0);
    ~TextLayer();
    QTextDocument *document();
    inline void setDocument( QTextDocument * d ) { 
    mount->setDocument(d);
    }
    inline void setSelected ( bool selected ) 
    {
    if (!selected) {
      modus = M_VIEW;
    }
    IsSelectActive = selected;
    QGraphicsItem::setSelected(selected);
    update();
    }
    QTextCursor textcursor();
    ///////////static TextLayer* self( QGraphicsItem *parent = 0 , QGraphicsScene *scene = 0 );
    QRectF boundingRect() const;
    bool contains(const QPointF &point) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    enum { Type = 22 };
    typedef enum
    {  
      DIV_FLOAT = 50,
      DIV_ABSOLUTE,    /* 51 */
      DIV_AUTO,   /* 52 */
      DIV_UNKNOW  /* 53 */
    } DIV_TYPE;
    typedef enum
    {  
      M_SHOW = 50,
      M_VIEW,    /* 51 */
      M_HOVER,   /* 52 */
      M_EDITOR,   /* 53 */
      M_ADV_EDITOR,   /* 54 */
      M_WRITTELN,  /* 55 */
      M_MOVE_NORD,  /* 56 */
      M_MOVE_SUD,  /* 57 */
      M_MOVE_WEST,  /* 58 */
      M_MOVE_OWEST,  /* 59 */
      M_MOVE_ALL,  /* 60 */
      M_MOVE_CTRL,  /* 61 */
      M_LOCK,  /* 62 */
      M_UNLOCK,  /* 63 */
      M_UNKNOW  /* 64 */
    } T_MODE;
    int type() const;
protected:
    void RestoreMoveAction();
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void inputMethodEvent(QInputMethodEvent *event);
    void keyPressEvent( QKeyEvent * event );
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    uint evesum;
    T_MODE modus;
    QMenu *textedit;
    QMenu *layeredit;

    QAction *actionLayerBackColor,
            *actionInsertImage,
            *actionLayerMargin;
    bool IsSelectActive;
    int id;
    bool sceneEvent(QEvent *event);
private:
    DIV_TYPE e_current;
    QColor bgcolor;
    QColor bordercolor;
    qreal wi;
    qreal hi;
    qreal text_hight;
    qreal text_width;
    QRectF TextboundingRect;
    int pageNumber;
    bool moving;
    bool cursortime;
    bool edit_enable;
    MountTextPrivate *mount;
    friend class MountTextPrivate;
    
public slots:
    void cursorPositionChanged( const QTextCursor curs );
    void appendtext( const QString txt );
    void updatearea( const QRectF areas );
    void LayerBGcolor();
    void cursor_wake_up();
};

QT_END_NAMESPACE
Q_DECLARE_METATYPE(TextLayer *)

QT_END_HEADER



//
#endif // TEXTLAYER_H

