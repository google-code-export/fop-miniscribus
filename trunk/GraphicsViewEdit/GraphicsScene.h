/*******************************************************************************
 * class GraphicsScene
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


#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

static const int ObjectNameEditor = 400;   /* normal layer div */
static const int InitTopPosition = 5;   /* scene initfrom top Y*/
static const int RectoSceneBottomFooterSpace = 70;   /* border bottom footer */
static const int InterSpacingFromAutoFloatLayerElements = 4; 


static const int ObjectNameController = 300;   /* element button on scene  */

static const int HeaderAndFooterHightSpace = RectoSceneBottomFooterSpace + InitTopPosition;

static const int ObyektSortingTimeline = 500;   /* display page order... */

#define _DEBUGRANGE_WI_ 1   /* begin draw on scene X position */

static const int DefaultPrintBorder = 1;   /* default printer border noprint 1% */

#include <QRectF>
#include "mounttextprivate.h"
#include <QGraphicsScene>
#include <QLabel>
#include <QPixmap>
#include <QSize>

static inline QRectF Reduce( QRectF rect , const int percentual )
{
    if (percentual < 1) {
    return rect;
    }
    const int leave_wi = rect.width() * percentual / 100;
    const int leave_hi = rect.height() * percentual / 100;
    return QRectF(0,0,rect.width() - leave_wi, rect.height() - leave_hi);
    /* use CenterRectSlaveFromMaster  to fill on rect target */
}

static inline QRectF CenterRectSlaveFromMaster( const QRectF Master ,
                                               QRectF Slave  )
{
  QRectF SlaveOnline = Slave.translated(Master.center());
  const qreal wi = Slave.width() / 2;
  const qreal hi = Slave.height() / 2;
  SlaveOnline.translate( 0 - wi , 0 - hi ); 
  return SlaveOnline;
}



static inline QPixmap CacheCursorSlider()
{
    QPixmap pixmap(10,26);
    pixmap.fill(Qt::transparent);  ///Qt::transparent
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.setPen(QPen(Qt::red,0.5));
    painter.drawLine(5,0,5, 26);
    
    painter.setBrush(Qt::red);
    painter.setPen(QPen(Qt::darkRed,1));
    QRect rectangle_0(0,-8,10,15);   //////// QRect ( int x, int y, int width, int height )
    painter.drawEllipse(rectangle_0);
    QRect rectangle_1(0,18,10,15); 
    painter.drawEllipse(rectangle_1);
    return pixmap;
}



#if QT_VERSION >= 0x040400
#include <QGraphicsProxyWidget>
#endif

class QResizeEvent;

class PreviewLabel : public QWidget
{
    Q_OBJECT

public:
    PreviewLabel(QWidget *parent = 0);
    void setPixmap(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap panello;
};



class GraphicsScene : public QGraphicsScene
{
     Q_OBJECT
    
public:
GraphicsScene( QObject * parent = 0 );
void clear();
void storno();
QList<QGraphicsItem *> l_items();
GraphicsScene( const QRectF & sceneRect, QObject * parent = 0 );
GraphicsScene( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 );
void setSceneRect( qreal x, qreal y, qreal w, qreal h );
void setSceneRect(  const QRectF & rect );
QGraphicsItem *ItemTop( const QPointF incomming );
void clearSelection();
void allclear();
qreal zmax();
qreal zmin();
#if QT_VERSION >= 0x040400
QGraphicsProxyWidget *addWidget(QWidget * widget, Qt::WindowFlags wFlags = Qt::Popup );
#endif
bool WakeUp( const QPointF incomming );

protected:
    qreal minimumH;
    QRectF Area;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * e );
    void mousePressEvent( QGraphicsSceneMouseEvent * e );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    
private:
signals:
   void nullitem();  /* QPointF */
   void SelectOn( QGraphicsItem * item , qreal zindex );  /* QPointF */
public slots:
    void reload();
    void TopFocus();
    void remid( const int id );
    void printPage(int index, int percentual , QPainter &painter, QPrinter * printer );   /* Preview and printer */

};



class GMarginScene : public QObject, public QGraphicsItem
{
   Q_OBJECT 

public:
    
    GMarginScene(qreal left , qreal right , QGraphicsScene *scene );
    ~GMarginScene();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget);
    QPainterPath shape() const;

   qreal Cursor_1_X;
   qreal Cursor_2_X;
   qreal wi;
   qreal hi;
   int MaximumCollisionAllowed;

signals:
    void CursorMove(qreal,qreal);

protected:
    QPixmap cursorimg;
    bool sceneEvent(QEvent *e);
    void HandleMove(  QPointF point );
    void HandleMouse(  QPointF point );
    int lastMove;
    QString units;
    QRectF AreaCursor_1;
    QRectF AreaCursor_2;
    QColor ColText;
    int dimfontsize;
    qreal actual_x;
};






#include "ui_previewdialogbase.h"



class QTreeWidgetItem;
typedef QList<QTreeWidgetItem *> StyleItems;

Q_DECLARE_METATYPE(StyleItems);

class PreviewDialog : public QDialog, private Ui::PreviewDialogBase
{
    Q_OBJECT

public:
    PreviewDialog( GraphicsScene *sceneprint );
    enum { SmallPreviewLength = 200, LargePreviewLength = 400 };
    bool isSelected(int index);
signals:
    void pageRequested(int,int,QPainter &,QPrinter*);

protected:
    void resizeEvent(QResizeEvent *);
    void RenderFaktor();
    void setupComboBoxes();
    void paintItem(QTreeWidgetItem *item, int index);
  
public slots:
    void on_paperSizeCombo_activated(int index);
    void on_paperOrientationCombo_activated(int index);
    void on_scales_valueChanged(int index);
    void on_pageList_currentItemChanged();
    void addPage();
    void setNumberOfPages(int count);
    void reject();
    void accept();
    void pdfsetter();
private:
    ///////void paintItem(QTreeWidgetItem *item, int index);
    void paintPreview(QPixmap &pixmap, int index );
    int ActualMaxPreview();
    QRectF Rscene;
    QSettings setter;
    int DefaultPrintBorderPercentual;
    bool printonpdf;
    QRectF Paper_Rect;
    QRectF rectScenePiece;
    QPushButton *pdfButton;
    PreviewLabel *previewLabel;
    GraphicsScene *scene;
    QPrinter *print;
    qreal faktor_print;
    qreal faktor_scene;
    bool canceled;
    int currentPage;  /* loop count */
    int pageCount;   /* summ pages */
};




















//
#endif // GRAPHICSSCENE_H

