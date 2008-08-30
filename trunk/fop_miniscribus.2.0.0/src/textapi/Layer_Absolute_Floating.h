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
#include "Config.h"
#include "SessionManager.h" 
#include "Text_Api.h"
#include "Layer_Absolute_Floating.h"


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
        void focusInEvent ( QFocusEvent * event );
        void focusOutEvent ( QFocusEvent * event );
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
        enum { Type = UserType + 3 };
        int type() const {return Type;}

    Q_SIGNALS:
        void dragging(const QPointF & point);
        void reset();
        void operate(bool);

    private:
        QGraphicsItem *    m_parent;
        QBrush      m_brush;
        QPointF     m_startPos;
        
        bool permission;
        QString txt;
};

Q_DECLARE_METATYPE(FWButton *)







  enum LAYERTYPE {
    DIV_ABSOLUTE = 1000,
    DIV_HEADER = 2000, 
    DIV_FOOTER = 3000,
    DIV_START = 4000,
    DIV_END = 5000
  };



class AbsText;
class AbsoluteLayer : public QObject, public QGraphicsRectItem
{
   Q_OBJECT 

public:
    
    AbsoluteLayer( QGraphicsItem *parent , LAYERTYPE layermodus = DIV_ABSOLUTE  , bool createdFromApps  = false );
    ~AbsoluteLayer();
    QRectF boundingRect() const;
     QPainterPath shape() const;
    enum { Layer = 1 };
    int type() const {return Layer;}
    LAYERTYPE blockType() const { return layermods; }
    QRectF absoluteRect();
    QLineF LineTops();
    QLineF Diagonal();
    RichDoc mimeDocumentLayer();   /* html format */
    QDomDocument fopDomdoc();   /* fop format */
    QPicture LayerImage( const int pagenr = 0 );  /* draw header or footer */
    void setStyle( QString stylelist );
    QTextDocument *document();
    void setDocument( const QTextDocument * doc , FileHandlerType Type = FOP );
    QTextCursor textCursor();
    M_PageSize PageFormat;  /* from parent item */
    QMap<QString,QVariant> layerStyle();
    FoRegion foRegion() const;
    
    
protected:
    QMap<QString,QVariant> incss;
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
    
    bool editorRunning;
    
    /* layer absolute params */
    QColor Background_Color;
    QColor Border_Color;

    QColor Border_Color_t;
    QColor Border_Color_b;
    QColor Border_Color_l;
    QColor Border_Color_r;
    qreal _border_top;
    qreal _border_bottom;
    qreal _border_left;
    qreal _border_right;
    

    LAYERTYPE layermods;
    int id;
    bool ContextOpen;
    FWButton  *Angle_1;
    FWButton  *Angle_2;
    FWButton  *Angle_4;
    bool OnMoveRects;
    qreal Rotate;
    QRectF lastUpdateRequest;
    QRectF LastVisibleRequest;
    AbsText *dev;
signals:
void close_main_cursor();
void pagesize_swap();
void scene_wakeup();

private slots:
void slotModpos_1( const QPointF posi );
void slotResize_1( const QPointF posi );
void slotRotate_1( const QPointF posi );
void MoveActions( bool e );
void updatearea( const QRect areas );
void UpdateDots();
void ShowInfos();
void MakeAllCommand();
void ensureVisible( const QRectF areas );

public slots:
void cursor_wake_up();
void seTBack();
void seTFront();
void UpdatePageFormat();
void RotateLayer( const int ro ) ;
void BackGroundColor();
void removeCurrent();
void showSource();


};


Q_DECLARE_METATYPE(AbsoluteLayer *)



template <class T> inline T layer_cast(QGraphicsItem *item)
{
    return int(static_cast<T>(0)->Layer) == int(AbsoluteLayer::Layer)
        || (item && int(static_cast<T>(0)->Type) == item->type()) ? static_cast<T>(item) : 0;
}

template <class T> inline T layer_cast(const QGraphicsItem *item)
{
    return int(static_cast<T>(0)->Layer) == int(AbsoluteLayer::Layer)
        || (item && int(static_cast<T>(0)->Type) == item->type()) ? static_cast<T>(item) : 0;
}


class AbsText: public QObject
{
    Q_OBJECT 
    
public:
   AbsText();
   LayerText *txtControl() const;
   AbsoluteLayer *q;
   private:
   mutable LayerText *device;
   
};

Q_DECLARE_METATYPE(AbsText *)


















//
#endif // LAYEREXTENDED_H

