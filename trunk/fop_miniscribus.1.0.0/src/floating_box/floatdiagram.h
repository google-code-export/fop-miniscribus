#ifndef FLOATDIAGRAM_H
#define FLOATDIAGRAM_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <math.h>
#include <QGraphicsScene>
#include <QtGui>
#include <QApplication>

#include "graphicsscene.h"

#include <QWidget>
#include <QBrush>
#include <QPixmap>
#include <QColor>
#include <QPalette> 
#include <QGraphicsTextItem>
#include <QMenu>
#include "wymsingtxt.h"
#include <QMessageBox>
#include "main.h"


class FloatDiagram :  public QGraphicsTextItem
{ 
Q_OBJECT
    
    public:
        
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
  M_LOCK,  /* 61 */
  M_UNLOCK,  /* 62 */
  M_UNKNOW  /* 63 */
} T_MODE;


FloatDiagram( int idx , QMenu *Diss , QGraphicsItem *parent, QGraphicsScene *scene );
FloatDiagram *clone() const;
inline int Getid() { return id; }
inline void SetDimension( qreal w , qreal h ) { 
    wi = w;
    hi = h;
    LayerHightChecks();
    setTextWidth ( wi - 2 );
    ensureVisible(boundingRect(),2, 2);
}
inline void SetINTarget( QStringList t ) 
{ 
TargetInternalLink = t;
}

inline void PaintStausLayer( bool statut ) { 
    PaintLockStaus = statut;
    update();
}

void SetDomeWrite( QDomDocument docwritteln ) {
    doc = docwritteln;
}
inline void SetModus( T_MODE e ) { 
    modus = e;
    update();
}


inline T_MODE GetModus() { return modus; }

inline void SetupRoq( int degres ) { 
    BoxRotate = degres;
}
inline int GetDegree() { return BoxRotate; }
inline void SetBGColor( QColor qc ) { 
   BGColor = qc; 
   AlphaColor = qc.alpha();
   update();
}
inline QColor GetColor() { return BGColor; }
inline void SetBorder( QColor qc , qreal spess ) { 
   MarginColor = qc; 
   BorderDicks = spess;
}
inline void SetResource( TypImageContainer block ) { 
   ImageContainer.clear();
   ImageContainer = block;
}
inline TypImageContainer GetResource() {  return ImageContainer; }
inline QRectF Prect() const {
    QRectF orealrect( pos().x(), pos().y(), wi, hi ); 
    return orealrect;
}
inline void setSelected ( bool selected ) 
{ 
    IsSelectActive = selected;
    QGraphicsItem::setSelected(selected);
    QGraphicsTextItem::setSelected(selected);
}
inline bool isSelected() { return IsSelectActive; }
QRectF boundingRect() const;
QDomElement GetBaseLayerElement( QDomDocument docwritteln );   /* prepare to save layer element writteln attribute */
qreal zValue() const ;
void setZValue( qreal zindex );
int DocumentHighgtActual();
QString toDivHtml( QFileInfo f );
protected:
    void ImageFindertoHtml( QDomElement e , const QString imagepath  );   /* export html */
    QString SaveHtmlImage( const QString urlimage , const QString path  );   /* export html */
    QVimedit *d;
    bool IsSelectActive;
    QTransform ActualMatrixe( int r );
    void RestoreMoveAction();
    void LayerHightChecks();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void keyPressEvent ( QKeyEvent * event );
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event); 
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    int wi;
    int hi;
    qreal ZIndex;
    int BoxRotate;
    int OLD_BoxRotate;
    T_MODE modus;
    QColor ground;
    int id;
    bool PaintLockStaus;
    QMenu *PMenu;
    QPointF dragPosition;
    QTextDocument *tdoc;
    QTextCursor tcur;
    QAction *Aundo,
            *Aredo,
            *Aselall,
            *Asetmyfont,
            *Abgcolor,
            *AMacolor,
            *Apaste,
            *Acopy;
    QClipboard  *baseram;
    QColor BGColor;
    QColor MarginColor;
    int AlphaColor;
    qreal BorderDicks;
    WymsingTxt *editTXT;
    TypImageContainer ImageContainer;
    QDomDocument doc;
    QString  DefaultMessage;
    QGraphicsItem *itpararent;
    QGraphicsScene *scenes;
    QStringList TargetInternalLink;
    
    
signals:
   void Rebuild();
public slots:
    void Tcopy();
    void Tpaste();
    void TselectAll();
    void SetNewBGColor();
    void FlyFonts();
    void SetMarginColor();
    void EditAdvanceModus();
    void SPTransForm( int rot );
    void PaintAllFonts( QFont f );
    void InValues( QRectF mbox ,qreal margin ,QColor bg , QColor margincolor , QTextDocument * d  , TypImageContainer block );
};









#endif // FLOATDIAGRAM_H

