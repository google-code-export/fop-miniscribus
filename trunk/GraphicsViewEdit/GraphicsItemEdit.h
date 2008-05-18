/*******************************************************************************
 * class TextLayer
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/

#ifndef TEXTLAYER_H
#define TEXTLAYER_H

#include "mounttextprivate.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QSharedData>
#include "GraphicsScene.h"
#define _SET_SELECTION_BY_SCENE_ 0

#define _DEBUGRANGE_WI_ 1


class Rotater : public QWidget
{
    Q_OBJECT
//
public:
 Rotater( int rotaten , QWidget *parent  )
{
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    label = new QLabel(tr("Rotate°:"),this);
    label->setObjectName(QString::fromUtf8("label"));
    label->setMaximumSize(QSize(16777215, 25));
    hboxLayout->addWidget(label);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem);
    lcdNumber = new QLCDNumber(this);
    lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
    lcdNumber->setMaximumSize(QSize(16777215, 25));
    lcdNumber->setAutoFillBackground(false);
    lcdNumber->setFrameShadow(QFrame::Sunken);
    lcdNumber->display(rotaten);
    hboxLayout->addWidget(lcdNumber);
    gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);
  
    dial = new QDial(this);
    dial->setObjectName(QString::fromUtf8("dial"));
    dial->setFocusPolicy(Qt::NoFocus);
    dial->setContextMenuPolicy(Qt::NoContextMenu);
    dial->setNotchesVisible(true);
    dial->setMaximum(360);
    dial->setValue(rotaten);
    gridLayout->addWidget(dial, 1, 0, 1, 1);
    connect(dial, SIGNAL(dialMoved(int)),this, SLOT(NewValue(int)));
  
}

~Rotater()
{
 dial->disconnect(this);
}


    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QSpacerItem *spacerItem;
    QLCDNumber *lcdNumber;
    QDial *dial;
private:
signals:
    void rotater(int);
public slots:
  
void NewValue( const int x )
{
  lcdNumber->display(x);
  emit rotater(x);
}
  


};





class TextController;
class LogData;

class TextLayer : public QObject, public QGraphicsItem
{
   Q_OBJECT 

public:
    enum CurrentModus{ Show, Edit, Move , MoveAll ,  Lock };
    enum LAYERTYPE{DIV_ABSOLUTE = 50,DIV_AUTO,/* 51 */DIV_FLOAT,/* 52 */DIV_HEADER,DIV_FOOTER};
    TextLayer(const int layer_id , QGraphicsItem *parent = 0 , QGraphicsScene *scene = 0);
    ~TextLayer();
    QList<QAction *> MainActions();
    bool editable();
    QRectF viewport_need();
    void setSelected( bool selected );
    void setModus( CurrentModus  e);
    qreal pointnext();
    QRectF boundingRect() const;
    QTextDocument *document();
    void setDocument ( QTextDocument * document );
    QTextCursor textCursor();
    void LayerHightChecks();
    RichDoc ReadActualItem();
    void insert( RichDoc Rdoc , bool clone = false );
    void setZValue(qreal index );
    bool contains(const QPointF &point) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    enum { Type = 22 };
    int type() const;
    int Ltype() const;
    void setStyle( QStringList syle , bool fromclone );
    QTextDocument *_doc;
    bool currentprintrender;
    inline void SetDimension( qreal w , qreal h ) { 
    wi = w;
    hi = h;
    document()->setPageSize(QSizeF(wi,hi));
    LayerHightChecks();
    }
protected:
    int getXcursor();
    bool AlertSize;
    QPointF lastclick;
    uint check_view_area_time;
    RichDoc guiwait;
    QSettings setter;
    QAction *actionSwapEdit,
             *actionSwapLock;
    void init();
    void read();
    void RestoreMoveAction();
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void timerEvent(QTimerEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event); 
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    bool ActionHover;
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
    qreal wisub_border;
    qreal wi;
    qreal hi;
    int Rotate;
    qreal text_hight;
    qreal text_width;
    QRectF CurrentCursorboundingRect;  /* repaint cursor area */
    QRectF TextboundingRect;
    QTransform ActualMatrixe( int r );
signals:
    void recalcarea();
    void clonehere();
    void remid(int);
public slots:
    void updatearea( const QRectF areas );
    void cursor_wake_up();
    void cursorPositionChanged( const QTextCursor cur);
    void CommitLayer();
    void InsertRevision();
    void EditModus();
    void SetNewBGColor();
    void SetNewBorderColor();
    void SaveFilelayer();
    void OpenFilelayer();
    void SwapEdit();
    void Borderwidht();
    void SwapLockmodus();
    void ShowInfos();
    void E_Reload();
    void RotateLayer( const int ro );
    void Copy_Html_Plain();
    void Copy_Text_Plain();
    void copyLayer();
    void CloneLayer();
    void Removehere();
    void seTBack();
    void seTFront();
    void cursor_area( const QRectF areas , const qreal ip );
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






//
#endif // TEXTLAYER_H

