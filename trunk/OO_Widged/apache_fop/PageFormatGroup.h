#ifndef PAGEFORMATGROUP_H
#define PAGEFORMATGROUP_H
#include "config.h"
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "BasicFoConfig.h"
#include <QDomElement>

QRectF M_PagesizeMake( QPrinter::PageSize psize , bool landscape = false );
QString metrics( const qreal screenpoint );

/*
Page formatting info enum qtprinter compatible
Margin params on QRectF
css like QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
http://de.selfhtml.org/css/eigenschaften/randabstand.htm#margin
*/


class FoRegion
{
public:
  FoRegion();
  FoRegion& operator=( const FoRegion& d );
  operator QVariant() const
  {
    return QVariant::fromValue(*this);
  }
  QString hash() const;
  void styleContainer( QDomElement e , const qreal height = 0 );
  void styleSmallDocument( QTextDocument *doc );
  void styleReadDocument( const QTextDocument *doc );
  void fomax(  FoRegion& m ,  FoRegion& d );
  /* same margin to all */
  void toAll( const qreal unique );
  void marginNoBody();
  QMap<QString,QPen> penstyle();  /* return avaiable pen */
  QColor bg; /* background color */
  QColor bog; /* border color */
  qreal margin_top;
  qreal margin_bottom;
  qreal margin_left;
  qreal margin_right;
  qreal border;
  qreal padding;
  QPen rpen;
  bool enable;
  QByteArray edom;   /* dome element*/
};

/*

QPen::QPen ( const QBrush & brush, qreal width, Qt::PenStyle style = Qt::SolidLine, Qt::PenCapStyle cap = Qt::SquareCap, Qt::PenJoinStyle join = Qt::BevelJoin )


*/


Q_DECLARE_METATYPE(FoRegion);


inline QDebug operator<<(QDebug debug, FoRegion& udoc)
{
    debug.nospace() << "FoRegion(Enable."
    << udoc.enable << ",top(),"
    << udoc.margin_top << ",bottom(),"
    << udoc.margin_bottom << ",right(),"
    << udoc.margin_right << ",left(),"
    << udoc.margin_left << ",PenWi(),"
    << udoc.rpen.widthF() << ",PenStyle(),"
    << udoc.rpen.style() << ")";
    return debug.space();
}




class M_PageSize
{
public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };
    M_PageSize();
    ~M_PageSize();
    /* to use as operator !=  == */
    QByteArray hashmodel() const;
    QDomDocument fopMeta();  /* dom meta to save fops */
    
    bool operator!= ( const M_PageSize& d );
    
    M_PageSize& operator=( const M_PageSize& d );
    
    operator QVariant() const;
    
    QRectF HeaderBoundingrect();
    
    QPointF HeaderInitPoints( const int index = 0);
    
    QPointF FooterInitPoints( const int index = 0 );

    QRectF FooterBoundingrect();

    QString HName();
    
    qreal faktor();
    
    void Register( QString n , QPrinter::PageSize pp , bool La = false );
    
    qreal height() const;
    
    qreal width() const;

    QSizeF internal_size();

    void SetMargin( FoRegion e );

    QRectF pageBoundingRect() const;

    QRectF BoundingPageSumme( const int summe );

    QRectF MarginArea()   /* page 0 */
    {
        const qreal LargeDoc = G_regt.width() - body.margin_right  - body.margin_left;
        const qreal InnHightDoc = G_regt.height() - body.margin_top  - body.margin_bottom;
        return QRectF(body.margin_left,body.margin_top,LargeDoc,InnHightDoc);
    }
    
    QRectF headerRect()   /* page 0 move to topLeft()  point */
    {
        return QRectF(0,0,width() + body.margin_right + body.margin_left ,body.margin_top);
    }
    
    QRectF footerRect()   /* page 0 move to bottomLeft()  point */
    {
        return QRectF(0,0,width() + body.margin_right + body.margin_left ,body.margin_bottom);
    }
    
    QRectF startRect()   /* page 0 move to topLeft()  point */
    {
        return QRectF(0,0,body.margin_left ,height() + body.margin_top  + body.margin_bottom );
    }
    
    QRectF endRect()   /* page 0 move to rightLeft()  point */
    {
        return QRectF(0,0,body.margin_right,height() + body.margin_top  + body.margin_bottom );
    }

    /* internal rect whitout margin */
    QRectF PageInternal( const int index = 0 );
    
    /* external rect document */
    QRectF PageExternal( const int index = 0 );
    
    QRectF PrintArea();
    
    QPointF MarginTranslate();
    
    /* write dom fo:region-body fop body */
    
    void ReportPage( QDomElement e );
    
    enum { region = 4 };  
   //////// FoRegion area[region];
    
    //////inline int header() { return 0; }
    //////inline int footer() { return 1; }
    //////inline int start() { return 2; }
    /////////inline int end() { return 3; }
    
    FoRegion area[region];
    FoRegion region_before() const { return area[0]; }
    FoRegion region_after() const { return area[1]; }
    FoRegion region_start() const  { return area[2]; }
    FoRegion region_end()  const { return area[3]; }
    
    /* open or set a qtexdocument from this */
    QTextDocument *nulldoc( QString htm );
    /* form qtexdocument to this margin an papersize */
    void HandlePrint( QTextDocument *doc );
    
     
    /*  | region-before = header 0 |  region-after = footer 1 | region-start = left  2 |> region-end = right  3 >*/
    
    QPrinter::PageSize P_rect;
    QRectF G_regt;
    FoRegion body;
    QSizeF RealSize;
    QString name;
    QString doctitle;
    QString autor;
    bool landscape;
    bool AllowHeaderFooter;
    int modus;  /* 0 = normal , 1 = book , 2 = 2column */
    int coolspace;
};

Q_DECLARE_METATYPE(M_PageSize);












//
#endif // PAGEFORMATGROUP_H

