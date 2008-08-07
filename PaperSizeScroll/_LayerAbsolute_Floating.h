#ifndef ABSOLUTELAYEREXTENDED_H
#define ABSOLUTELAYEREXTENDED_H


#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "Layer_CommandStorage.h"
#include "scribemime.h"
#include "SessionManager.h"
#include "_LayerApi.h"

    ///////qDebug() << "###  width" << boundingRect().height();
    /////qDebug() << "###  largo" << alto;

#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265



static const qreal MinimumHightLayer = 15.0;
static const qreal MinimumWhidhLayer = 15.0;

static const qreal MAXLargoTmp = 1000.0;


/* drag point to rotate and resize absolute layer */

class FWButton : public QObject, public QGraphicsItem
{
    Q_OBJECT
    public:
        FWButton( QGraphicsItem * parent, const QBrush & brush , const QString msg );
        QRectF boundingRect() const;
        void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
        void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
        void mousePressEvent( QGraphicsSceneMouseEvent * event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
        void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
        void allow( bool e );

    Q_SIGNALS:
        void dragging(const QPointF & point);
        void reset();

    private:
        QGraphicsItem *    m_parent;
        QBrush      m_brush;
        QPointF     m_startPos;
        bool permission;
        QString txt;
};



Q_DECLARE_METATYPE(FWButton *)






class AbsoluteLayer : public QObject, public QGraphicsRectItem
{
   Q_OBJECT 

public:
    
    AbsoluteLayer( QGraphicsItem *parent );
    ~AbsoluteLayer();
    QRectF boundingRect() const;
    
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
    FWButton  *Angle_1;
    FWButton  *Angle_2;
    FWButton  *Angle_4;
    qreal Rotate;
private slots:
void slotModpos_1( const QPointF posi );
void slotResize_1( const QPointF posi );
void slotRotate_1( const QPointF posi );

};


Q_DECLARE_METATYPE(AbsoluteLayer *)





//
#endif // LAYEREXTENDED_H

