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
  /* same margin to all */
  void toAll( const qreal unique );
  QColor bg; /* background color */
  QColor bog; /* border color */
  int name;
  qreal margin_top;
  qreal margin_bottom;
  qreal margin_left;
  qreal margin_right;
  qreal border;
  bool enable;
  QByteArray edom;   /* dome element*/
};


Q_DECLARE_METATYPE(FoRegion);


inline QDebug operator<<(QDebug debug, FoRegion& udoc)
{
    debug.nospace() << "FoRegion(name."
    << udoc.name << ",top()"
    << udoc.margin_top << ",bottom()"
    << udoc.margin_bottom << ",right()"
    << udoc.margin_right << ",left()"
    << udoc.margin_left << ")";
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

    /* internal rect whitout margin */
    QRectF PageInternal( const int index = 0 );
    
    /* external rect document */
    QRectF PageExternal( const int index = 0 );
    
    QRectF PrintArea();
    
    QPointF MarginTranslate();
    
    /* write dom fo:region-body fop body */
    
    void ReportPage( QDomElement e );
    
    /* open or set a qtexdocument from this */
    QTextDocument *nulldoc( QString htm );
    /* form qtexdocument to this margin an papersize */
    void HandlePrint( QTextDocument *doc );
    
    
    
    QPrinter::PageSize P_rect;
    QRectF G_regt;
    FoRegion body;
    QSizeF RealSize;
    QString name;
    bool landscape;
    bool AllowHeaderFooter;
    int modus;  /* 0 = normal , 1 = book , 2 = 2column */
    int coolspace;
};

Q_DECLARE_METATYPE(M_PageSize);












//
#endif // PAGEFORMATGROUP_H

