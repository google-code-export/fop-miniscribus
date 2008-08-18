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
#include "FoColorName.h"

///////qDebug() << "###  width" << boundingRect().height();
/////qDebug() << "###  largo" << alto;

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
    DIV_FOOTER = 3000
  };



class AbsText;

class AbsoluteLayer : public QObject, public QGraphicsRectItem
{
   Q_OBJECT 

public:
    /*
    die erste Angabe bedeutet den Abstand für oben und unten, 
    die zweite den Abstand für rechts und links.
    css 2 margin declaration 
    */
    enum WMARGIN {
    A_before = 0,
    A_after = 1, 
    A_right = 2,
    A_left = 3
  };


    
    AbsoluteLayer( QGraphicsItem *parent , LAYERTYPE layermodus = DIV_ABSOLUTE );
    ~AbsoluteLayer();
    QRectF boundingRect() const;
    inline int Type() const { return layermods; }
    QRectF absoluteRect();
    QLineF LineTops();
    QLineF Diagonal();
    QPicture LayerImage( const int pagenr = 0 );  /* draw header or footer */
    void setStyle( QString stylelist );
    QTextDocument *document();
    void setDocument( const QTextDocument * doc , FileHandlerType Type = FOP );
    QTextCursor textCursor();
    qreal borderWidth( WMARGIN e );  /* value from border area X */
    QColor borderColor( WMARGIN e );  /* qcolor from area X */
    
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintBorderLayer( QPainter *painter );
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
    FopColor xsl_fo_color;  /* reader from colors */
    /* layer absolute params */
    QRectF printrectarea;  /* allowed pos() */
    QColor Background_Color;
    QColor Border_Color;
    qreal  internPadding;
    QTextFrameFormat::BorderStyle penborder;  /* style from pen border */
    QMap<QString,QVariant> incss;  /* style css from original item */
    QStringList css2list; /* model css name variable */
    QColor Border_Color_t;
    QColor Border_Color_b;
    QColor Border_Color_l;
    QColor Border_Color_r;
    qreal _border_top;
    qreal _border_bottom;
    qreal _border_left;
    qreal _border_right;
    /*  _border_top + _border_bottom */
    /*  _border_left + _border_right */

    LAYERTYPE layermods;
    int id;
    bool ContextOpen;
    FWButton  *Angle_1;
    FWButton  *Angle_2;
    FWButton  *Angle_4;
    bool OnMoveRects;
    qreal Rotate;
    QRectF lastUpdateRequest;
    AbsText *dev;
signals:
void close_main_cursor();
void pagesize_swap();

private slots:
void slotModpos_1( const QPointF posi );
void slotResize_1( const QPointF posi );
void slotRotate_1( const QPointF posi );
void MoveActions( bool e );
void updatearea( const QRect areas );
void UpdateDots();
void ShowInfos();
void MakeAllCommand();

public slots:
  
void seTBack();
void seTFront();
void UpdatePageFormat();
void RotateLayer( const int ro ) ;
void BackGroundColor();

};


Q_DECLARE_METATYPE(AbsoluteLayer *)




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

