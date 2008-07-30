#ifndef LAYEREXTENDED_H
#define LAYEREXTENDED_H


#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "scribemime.h"
#include "SessionManager.h"
#include "_LayerApi.h"


class TextMount;
class TextLayer : public QObject, public QGraphicsRectItem
{
   Q_OBJECT 

public:
    
    TextLayer( QGraphicsItem *parent = 0 );
    ~TextLayer();
    QRectF boundingRect() const;
    QString PageName();
    QTextDocument *document() const;
    QTextCursor textCursor();
    void SwapPageModel( M_PageSize e );
    
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool sceneEvent(QEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event); 
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void keyPressEvent( QKeyEvent * event );
    void keyReleaseEvent ( QKeyEvent * event );
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );
    void inputMethodEvent ( QInputMethodEvent * event );

private:
  QRectF LastRect;
  TextMount *dev;
  QRectF LastUpdateRequest;
  QRectF LastVisibleRequest;

signals:

public slots:
    void SceneReload();
    void ensureVisible( const QRectF areas );
    void updatearea( const QRect areas );
    void cursor_wake_up();
    

};

Q_DECLARE_METATYPE(TextLayer *)


class TextMount: public QObject
{
    Q_OBJECT 
    
public:
   TextMount();
   ScribePage *txtControl() const;
   TextLayer *q;
   private:
   mutable ScribePage *device;
   
};

Q_DECLARE_METATYPE(TextMount *)
















//
#endif // LAYEREXTENDED_H

