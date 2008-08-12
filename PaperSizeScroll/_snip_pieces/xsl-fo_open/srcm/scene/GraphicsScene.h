#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H





static const int ObjectNameEditor = 400;   /* normal layer div */
static const int InitTopPosition = 22;   /* scene initfrom top Y*/
static const int RectoSceneBottomFooterSpace = 70;   /* border bottom footer */
static const int InterSpacingFromAutoFloatLayerElements = 5; 


static const int ObjectNameController = 300;   /* element button on scene  */

static const int HeaderAndFooterHightSpace = RectoSceneBottomFooterSpace + InitTopPosition;

static const int ObyektSortingTimeline = 500;   /* display page order... */

#define _DEBUGRANGE_WI_ 10   /* begin draw on scene X position */

static const int DefaultPrintBorder = 1;   /* default printer border noprint 1% */






#include <QRectF>
#include <QGraphicsScene>
#include <QLabel>
#include <QPixmap>
#include <QSize>
#include <QPrinter>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QPainter>
#include <QGraphicsItem>
#include "SessionManager.h"




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
    /* check if slave is to large for master? */
  QRectF SlaveOnline = Slave.translated(Master.center());
  const qreal wi = Slave.width() / 2;
  const qreal hi = Slave.height() / 2;
  SlaveOnline.translate( 0 - wi , 0 - hi ); 
  return SlaveOnline;
}











class GraphicsScene : public QGraphicsScene
{
     Q_OBJECT
    
public:
GraphicsScene( QObject * parent = 0 );
inline QGraphicsItem *Active() { return bridge; }
void clear();
int PrintPageSumm( QPrinter * printer );
void storno();
GraphicsScene( const QRectF & sceneRect, QObject * parent = 0 );
GraphicsScene( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 );
void setSceneRect( qreal x, qreal y, qreal w, qreal h );
void setSceneRect(  const QRectF & rect );
void clearSelection();
void paste( bool cloneornot = false );
qreal zmax();
qreal zmin();
void printPage(int index, int percentual , QPainter &painter, QPrinter * printer );   /* Preview and printer */


signals:
    void SelectOn(QGraphicsItem*);
    void LayerPaste(bool);
public slots:
    void reload();

protected:
    bool WakeUp( const QPointF incomming );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * e );
    void mousePressEvent( QGraphicsSceneMouseEvent * e );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    QGraphicsItem *bridge;

    

};











//
#endif // GRAPHICSSCENE_H

