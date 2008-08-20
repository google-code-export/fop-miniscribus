#include "PageFormatGroup.h"
#include "BasicFoConfig.h"
#include "config.h"

/*

Element:  	simple-page-master
Attributes: 	| margin-top | margin-bottom | margin-left | margin-right | 
space-before | space-after | start-indent | end-indent | master-name |
page-height | page-width | reference-orientation | writing-mode | 

| region-body | region-before | region-after | region-start | region-end | 
*/



/* ###############################################################################################################*/

FoRegion::FoRegion() 
{
    /* default param region */
    margin_top = CM_TO_POINT(2.5);
    margin_bottom = CM_TO_POINT(2.5);
    margin_left = CM_TO_POINT(1.5);
    margin_right = CM_TO_POINT(1.5);
    bg = QColor(Qt::white);
    bog = QColor(Qt::white);
    edom = QByteArray("<scribe/>");
    name = 1;  /* 1 body 2 header  3 footer  4 start 5 end */
    border = 0.;
    enable = true;
}

FoRegion& FoRegion::operator=( const FoRegion& d )
{
    name = d.name;
    bg = d.bg;
    bog = d.bog;
    margin_top = d.margin_top;
    margin_bottom = d.margin_bottom;
    margin_left = d.margin_left;
    margin_right = d.margin_right;
    edom = d.edom;
    border = d.border;
    return *this;
}



void FoRegion::toAll( const qreal unique )
{
    margin_top = unique;
    margin_bottom = unique;
    margin_left = unique;
    margin_right = unique;
}


/* ###############################################################################################################*/


bool M_PageSize::operator!= ( const M_PageSize& d )
{
    return hashmodel() != d.hashmodel();
    
}


QByteArray M_PageSize::hashmodel() const
{
        QByteArray unique("Hash-PageFormat:");
        const QString header = QString("%1|%2|%3|%4|").arg(name).arg(G_regt.width())
                               .arg(G_regt.height()).arg(modus);
        const QString margin = QString("%1|%2|%3|%4").arg(body.margin_top).arg(body.margin_bottom)
                               .arg(body.margin_right).arg(body.margin_left);
         QString  position = "Portrait";
         if (landscape) {
             position = "Landscape";
         }
         unique.append(position);
         unique.append(header);
         unique.append(margin);
         QCryptographicHash enmd5( QCryptographicHash::Sha1 );
         enmd5.addData ( unique );
         const QByteArray chunkha = enmd5.result();
         return chunkha.toHex();
}



M_PageSize::~M_PageSize()
{
   
}



M_PageSize::M_PageSize()
{
    landscape = false;
    body = FoRegion();
    coolspace = 0;
    modus = 0;
    name = "A4 (210 x 297 mm)";
    P_rect = QPrinter::A4;
    G_regt = QRectF(0,0,MM_TO_POINT(210),MM_TO_POINT(297));
    RealSize = G_regt.size();
    AllowHeaderFooter = true;
}

M_PageSize& M_PageSize::operator=( const M_PageSize& d )
{
    name = d.name;
    landscape = d.landscape;
    G_regt = d.G_regt;
    modus = d.modus;
    RealSize = d.RealSize;
    coolspace = d.coolspace;
    P_rect = d.P_rect;
    body = d.body;
    return *this;
}



QPointF M_PageSize::HeaderInitPoints( const int index )
{
        const qreal fromTopY = index * G_regt.height();
        const qreal spacepage = index * InterSpace;
        return QPointF(body.margin_left,fromTopY + spacepage + FooterHeaderPadding);
}

M_PageSize::operator QVariant() const
{
   return QVariant::fromValue(*this);
}

QRectF M_PageSize::HeaderBoundingrect()
{
    return  QRectF(0,0,width(),body.margin_top - ( FooterHeaderPadding * 2 ));
}



QPointF M_PageSize::FooterInitPoints( const int index  )
{
        const qreal fromTopY = index * G_regt.height();
        const qreal Ytop = G_regt.height() - body.margin_bottom;
        if (index ==  0)
        {
            return QPointF(body.margin_left,Ytop + FooterHeaderPadding);
        }
        const qreal spacepage = index * InterSpace;
        return QPointF(body.margin_left,fromTopY + spacepage + Ytop + FooterHeaderPadding);
}




QRectF M_PageSize::FooterBoundingrect()
{
    return  QRectF(0,0,width(),body.margin_bottom - ( FooterHeaderPadding * 2 ));
}


QString M_PageSize::HName()
{
    QString sep(" /");
    QString reg = name;
    if (landscape)
    {
        reg.append(sep+QT_TR_NOOP("Landscape"));
    }
    else
    {
        reg.append(sep+QT_TR_NOOP("Portrait"));
    }
    return name;
}


qreal M_PageSize::faktor()
{
    return qMax(G_regt.width(),G_regt.height()) / qMin(G_regt.width(),G_regt.height());
}


void M_PageSize::Register( QString n , QPrinter::PageSize pp , bool La )
{
    G_regt = M_PagesizeMake(P_rect,La);
    RealSize = G_regt.size();
    name = n;
    landscape = La;
    P_rect = pp;
}


qreal M_PageSize::height() const
{
    return pageBoundingRect().height() - body.margin_bottom - body.margin_top + 0.5;  ////  -bottom - top
}

qreal M_PageSize::width() const
{
    return pageBoundingRect().width() - body.margin_left - body.margin_right + 0.5; /////////  -lef , - right
}

QSizeF M_PageSize::internal_size()
{
  return QSizeF(width(),height());
}
    
void M_PageSize::SetMargin( FoRegion e )
{
    body = e;
}

QRectF M_PageSize::BoundingPageSumme( const int summe )
{
    int summetotal = qBound (1,summe,MaximumPages);
    const qreal alto = summetotal * G_regt.height();
    return QRectF(0,0,G_regt.width(),alto);
}


QRectF M_PageSize::pageBoundingRect() const
{
       if (landscape) {
           return QRectF(0,0,qMax(G_regt.height(),G_regt.width()),qMin(G_regt.height(),G_regt.width()));
       } else {
           return QRectF(0,0,qMin(G_regt.height(),G_regt.width()),qMax(G_regt.height(),G_regt.width()));
       }
}


    /* internal rect whitout margin */
QRectF M_PageSize::PageInternal( const int index )
{
        const qreal fromTopY = index * G_regt.height();
        const qreal spacepage = index * InterSpace;
        return QRectF(QPointF(body.margin_left,fromTopY + spacepage + body.margin_top),internal_size());
}
    /* external rect document */
QRectF M_PageSize::PageExternal( const int index )
{
        const qreal fromTopY = index * G_regt.height();
        const qreal spacepage = index * InterSpace;
        return QRectF(QPointF(0,fromTopY + spacepage),G_regt.size());
}
    
QRectF M_PageSize::PrintArea()
{
        return QRectF(QPointF(body.margin_left,body.margin_top),QSizeF(width(),G_regt.height() * MaximumPages));
}



QPointF M_PageSize::MarginTranslate()
{
    return QPointF(body.margin_left,body.margin_top);
}

void M_PageSize::ReportPage( QDomElement e )
{
		const qreal TopMargin = Pointo(body.margin_top,"mm");
		const qreal RightMargin = Pointo(body.margin_right,"mm");
		const qreal BottomMargin = Pointo(body.margin_bottom,"mm");
		const qreal LeftMargin = Pointo(body.margin_left,"mm");
    
    e.setAttribute ("margin-top",QString("%1mm").arg(TopMargin));
    e.setAttribute ("margin-bottom",QString("%1mm").arg(BottomMargin));
    e.setAttribute ("margin-left",QString("%1mm").arg(LeftMargin));
    e.setAttribute ("margin-right",QString("%1mm").arg(RightMargin));
    e.setAttribute ("page-width",QString("%1mm").arg(Pointo(G_regt.width(),"mm")));
    e.setAttribute ("page-height",QString("%1mm").arg(Pointo(G_regt.height(),"mm")));
}

QTextDocument *M_PageSize::nulldoc( QString htm )
{
    QTextDocument *doc = new QTextDocument;
    if (htm.size() > 2)
    {
        doc->setHtml(htm);
    }
    HandlePrint(doc);
    return doc->clone();
}




/* form qtexdocument to this margin an papersize */
void M_PageSize::HandlePrint( QTextDocument *doc )
{
    const qreal RightMargin = body.margin_right;
    const qreal LeftMargin = body.margin_left;
    const qreal LargeDoc = G_regt.width() - RightMargin  - LeftMargin;
    doc->setPageSize ( G_regt.size() );
    QTextFrame  *Tframe = doc->rootFrame();
    QTextFrameFormat Ftf = Tframe->frameFormat();
    Ftf.setLeftMargin(body.margin_left);
    Ftf.setBottomMargin(body.margin_bottom);
    Ftf.setTopMargin(body.margin_top);
    Ftf.setBackground(QBrush(Qt::transparent));
    Ftf.setRightMargin(body.margin_right);
    Ftf.setPadding ( 0);
    Tframe->setFrameFormat(Ftf);
    doc->setPageSize ( G_regt.size() );
}



/* ###############################################################################################################*/




QRectF M_PagesizeMake( QPrinter::PageSize psize , bool landscape)
{
    QPrinter *print = new QPrinter(QPrinter::HighResolution);
    print->setFullPage(true);
    print->setPageSize(psize);
    if (landscape)
    {
        print->setOrientation(QPrinter::Landscape);
    }
    else
    {
        print->setOrientation(QPrinter::Portrait);
    }

    const QRectF pa = print->pageRect(QPrinter::Point);
    const qreal faktor = qMax(pa.width(),pa.height()) / qMin(pa.width(),pa.height());

    const qreal smaller = qMin(pa.width(),pa.height());
    const qreal biger = smaller * faktor;
    delete print;

    if ( landscape )
    {
        return QRectF(0,0,biger,smaller);
    }
    else
    {
        return QRectF(0,0,smaller,biger);
    }
}


































































