#include "PageFormatGroup.h"
#include "BasicFoConfig.h"
#include "Config.h"

#include "FoColorName.h"


QString metrics( const qreal screenpoint )
{
   return QString("%1mm").arg(Pointo(screenpoint,"mm"));
}








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
    margin_left = CM_TO_POINT(2.5);
    margin_right = CM_TO_POINT(1.5);
    bg = QColor(Qt::white);
    bog = QColor(Qt::transparent);
    edom = QByteArray("<scribe/>");
    border = 0.;  /* border > 0 draw */
    padding = CM_TO_POINT(0.2);
    rpen = QPen(Qt::NoPen);
    enable = false;
}
void FoRegion::marginNoBody()
{
   toAll(CM_TO_POINT(0.1));
   padding = CM_TO_POINT(0.1);
   border = 0.;
}


QMap<QString,QPen> FoRegion::penstyle()
{
    QMap<QString,QPen> plist;
         plist.insert( QString("0.0mm empty border") ,QPen(Qt::NoPen));
        plist.insert( QString("0.5mm solid") ,QPen(Qt::black,MM_TO_POINT(0.5),Qt::SolidLine));
        plist.insert( QString("0.5mm dotline") ,QPen(Qt::black,MM_TO_POINT(0.5),Qt::DotLine));
    for (int i = 1; i < 9; ++i)  {
        plist.insert( QString("%1mm solid").arg(i) ,QPen(Qt::black,MM_TO_POINT(i),Qt::SolidLine));
        plist.insert( QString("%1mm dotline").arg(i) ,QPen(Qt::black,MM_TO_POINT(i),Qt::DotLine));
        plist.insert( QString("%1mm dashline").arg(i) ,QPen(Qt::black,MM_TO_POINT(i),Qt::DashLine));
    }
   return plist;
}
/*  fo:block-container or other frame apache not on region */
void FoRegion::styleContainer( QDomElement e , const qreal height )
{
     FopColor hc = FopColor();
     e.setAttribute ("background-color",hc.foName(bg));
        if (rpen != QPen(Qt::NoPen)) {
           e.setAttribute ("border-width",metrics(rpen.width()));
           Qt::PenStyle ds = rpen.style();
           if (ds == Qt::DotLine) {
            e.setAttribute ("border-style","dotted"); 
           } else if (ds == Qt::DashLine) {
             e.setAttribute ("border-style","dashed"); 
           } else {
               /* solid */
             e.setAttribute ("border-style","solid"); 
           }
         e.setAttribute ("border-color",hc.foName(bog));
       }
       
    e.setAttribute ("margin-top",metrics(margin_top));
    e.setAttribute ("margin-bottom",metrics(margin_bottom));
    e.setAttribute ("margin-left",metrics(margin_left));
    e.setAttribute ("margin-right",metrics(margin_right));
    e.setAttribute ("padding",metrics(padding));
    e.setAttribute ("overflow","hidden");
       
       
       
    if (height !=0) {
     e.setAttribute ("height",metrics(qAbs(height - margin_top - padding- margin_bottom)));
    }
       
}

/* form qtexdocument to this margin an papersize */
void FoRegion::styleSmallDocument( QTextDocument *doc )
{
    QTextFrame  *Tframe = doc->rootFrame();
    QTextFrameFormat Ftf = Tframe->frameFormat();
    Ftf.setLeftMargin(margin_left);
    Ftf.setBottomMargin(margin_bottom);
    Ftf.setTopMargin(margin_top);
    Ftf.setBackground(QBrush(bg));
    Ftf.setRightMargin(margin_right);
    Ftf.setPadding(padding);
    Tframe->setFrameFormat(Ftf);
}

void FoRegion::styleReadDocument( const QTextDocument *doc )
{
    QTextFrame  *Tframe = doc->rootFrame();
    QTextFrameFormat formatibb = Tframe->frameFormat();
    margin_top = formatibb.topMargin();
    margin_bottom = formatibb.bottomMargin();
    margin_left = formatibb.leftMargin();
    margin_right = formatibb.rightMargin();
    padding = formatibb.padding();
}







QString FoRegion::hash() const
{
        QByteArray unique("Hash-Forerin:");
        const QString header = QString("%1|%2|%3|%4|").arg(margin_top).arg(margin_bottom)
                               .arg(margin_right).arg(margin_left);
        const QString margin = QString("%1|%2|%3").arg(bg.name()).arg(bog.name())
                               .arg(border);
         QString  position = "Disable";
         if (enable) {
             position = "Enable";
         }
         unique.append(position);
         unique.append(header);
         unique.append(margin);
         QCryptographicHash enmd5( QCryptographicHash::Sha1 );
         enmd5.addData ( unique );
         const QByteArray chunkha = enmd5.result();
         return QString(chunkha.toHex());
}


FoRegion& FoRegion::operator=( const FoRegion& d )
{
    bg = d.bg;
    bog = d.bog;
    margin_top = d.margin_top;
    margin_bottom = d.margin_bottom;
    margin_left = d.margin_left;
    margin_right = d.margin_right;
    edom = d.edom;
    border = d.border;
    enable = d.enable;
    padding = d.padding;
    rpen = d.rpen;
    return *this;
}

/*  1 init / 2 master */
void FoRegion::fomax(  FoRegion& m ,  FoRegion& d )
{
    bg = d.bg;
    bog = d.bog;
    margin_top = qMax(d.margin_top,m.margin_top);
    margin_bottom = qMax(d.margin_bottom,m.margin_bottom);
    margin_left = qMax(d.margin_left,m.margin_left);
    margin_right = qMax(d.margin_right,m.margin_right);
    edom = d.edom;
    border = d.border;
    enable = d.enable;
    padding = d.padding;
    rpen = d.rpen;
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
         const int totr = (int)region;
         for (int i = 0; i < totr; ++i) {
             unique.append(area[i].hash());
         }
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
    doctitle = _NOTITLEONDOC_;
    autor = _APPLICATIONS_NAME_;
    
    FoRegion topR;
    FoRegion bottomR;
    FoRegion leftR;
    FoRegion rightR;
    
    area[0] = topR;
    area[1] = bottomR;
    area[2] = leftR;
    area[3] = rightR;
    
    
      /*
      FoRegion region_before() const { return area[0]; }
    FoRegion region_after() const { return area[1]; }
    FoRegion region_start() const  { return area[2]; }
    FoRegion region_end()  const { return area[3]; }
    
    */
    
    area[0].marginNoBody();
    area[1].marginNoBody();
    area[2].marginNoBody();
    area[3].marginNoBody();
    
    area[0].enable = false;
    area[1].enable = false;
    area[2].enable = false;
    area[3].enable = false;
    
}

M_PageSize& M_PageSize::operator=( const M_PageSize& d )
{
    name = d.name;
    doctitle = d.doctitle;
    autor = d.autor;
    landscape = d.landscape;
    G_regt = d.G_regt;
    modus = d.modus;
    RealSize = d.RealSize;
    coolspace = d.coolspace;
    P_rect = d.P_rect;
    body = d.body;
    area[0] = d.area[0];
    area[1] = d.area[1];
    area[2] = d.area[2];
    area[3] = d.area[3];
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


/* fo:layout-master-set */

void M_PageSize::ReportPage( QDomElement e )
{
    const QString ReferenzeNames = Imagename(name);
    
    e.setAttribute ("master-name",ReferenzeNames);
    
    e.setAttribute ("page-width",QString("%1mm").arg(Pointo(G_regt.width(),"mm")));
    e.setAttribute ("page-height",QString("%1mm").arg(Pointo(G_regt.height(),"mm")));
    
    FopColor hc = FopColor();
    
    QDomElement rbody = e.ownerDocument().createElement("fo:region-body");
	rbody.setAttribute ("region-name","xsl-region-body");
    rbody.setAttribute ("background-color",hc.foName(body.bg));
    rbody.setAttribute ("margin-top",metrics(body.margin_top));
    rbody.setAttribute ("margin-bottom",metrics(body.margin_bottom));
    rbody.setAttribute ("margin-left",metrics(body.margin_left));
    rbody.setAttribute ("margin-right",metrics(body.margin_right));
    e.appendChild( rbody );
    
    if (area[0].enable) {
    QDomElement rbefore = e.ownerDocument().createElement("fo:region-before");
	rbefore.setAttribute ("region-name","xsl-region-before");
    rbefore.setAttribute ("background-color",hc.foName(area[0].bg));
    rbefore.setAttribute ("extent",metrics(body.margin_top));
    rbefore.setAttribute ("reference-orientation","0");
    e.appendChild( rbefore );
    }
    
    if (area[1].enable) {
    QDomElement rafter = e.ownerDocument().createElement("fo:region-after");
	rafter.setAttribute ("region-name","xsl-region-after");
    rafter.setAttribute ("background-color",hc.foName(area[1].bg));
    rafter.setAttribute ("extent",metrics(body.margin_bottom));
    rafter.setAttribute ("reference-orientation","0");
	e.appendChild( rafter );
    }
    
    if (area[2].enable) {
    QDomElement rstart = e.ownerDocument().createElement("fo:region-start");
	rstart.setAttribute ("region-name","xsl-region-start");
    rstart.setAttribute ("background-color",hc.foName(area[2].bg));
    rstart.setAttribute ("extent",metrics(body.margin_left));
    rstart.setAttribute ("reference-orientation","-270");
	e.appendChild( rstart );
    }
    
    
    if (area[3].enable) {
    QDomElement rend = e.ownerDocument().createElement("fo:region-end");
	rend.setAttribute ("region-name","xsl-region-end");
    rend.setAttribute ("background-color",hc.foName(area[3].bg));
    rend.setAttribute ("extent",metrics(body.margin_right));
    rend.setAttribute ("reference-orientation","270");
	e.appendChild( rend );
    }
    
    
    
    
    
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



QDomDocument  M_PageSize::fopMeta()
{
    QDomDocument dom;
    const QString ReferenzeNames = Imagename(name);
    FopColor hc = FopColor();   ////  hc.foName( const QColor e )
    QDomProcessingInstruction header = dom.createProcessingInstruction( "xml",QString("version=\"1.0\" encoding=\"utf-8\"" ));
	dom.appendChild( header );
	//////QDateTime timer1( QDateTime::currentDateTime() );  /* time root */
	QDomElement basexslforoot = dom.createElement("fo:root");
                basexslforoot.setAttribute ("xmlns:fo","http://www.w3.org/1999/XSL/Format");
                basexslforoot.setAttribute ("xmlns:svg","http://www.w3.org/2000/svg");
                basexslforoot.setAttribute ("xmlns:cms","http://www.pulitzer.ch/2007/CMSFormat");
                basexslforoot.setAttribute ("xmlns:fox","http://xmlgraphics.apache.org/fop/extensions");
    dom.appendChild( basexslforoot );
    
    const int blocks = 10;
    ////////////  fox:outline
    
    QDomElement layout = dom.createElement("fo:layout-master-set");
    layout.setAttribute ("writing-mode","lr-tb");
    layout.setAttribute ("master-name",ReferenzeNames);
	basexslforoot.appendChild( layout );

	QDomElement pmaster = dom.createElement("fo:simple-page-master");
    ReportPage(pmaster);
    layout.appendChild( pmaster );

    /* bookmark here ..........   */
    
    QDomElement bbtree = dom.createElement("fo:bookmark-tree");
    
    
    QDomElement root_bookmark = dom.createElement("fo:bookmark");
    bbtree.appendChild( root_bookmark);
    root_bookmark.setAttribute ("internal-destination","sec0");
    
    QDomElement tree_a = dom.createElement("fo:bookmark-title");
    tree_a .appendChild( dom.createTextNode ( "Internal dest.0" )  );
    root_bookmark.appendChild( tree_a );
    
       for (int i = 1; i < blocks; ++i) {
           
           QDomElement sub = dom.createElement("fo:bookmark");
           sub.setAttribute ("internal-destination",QString("sec%1").arg(i));
           
           QDomElement label= dom.createElement("fo:bookmark-title");
           label.appendChild( dom.createTextNode ( QString("Internal dest.%1" ).arg(i)  ));
           sub.appendChild( label );
           
           root_bookmark.appendChild( sub );
       }
    
    
    
    basexslforoot.appendChild( bbtree );
       
       
    
  	QDomElement pageseq1 = dom.createElement("fo:page-sequence");
	pageseq1.setAttribute ("master-reference",ReferenzeNames);
	basexslforoot.appendChild( pageseq1 );
       
    QDomElement pdftitle = dom.createElement("fo:title");
    pdftitle.appendChild( dom.createTextNode ( doctitle )  );
    pageseq1.appendChild( pdftitle );
    /* header*/
    if (area[0].enable) {
    QDomElement txt_1 = dom.createElement("fo:static-content");
    txt_1.setAttribute ("flow-name","xsl-region-before");
    QDomElement a_txt_1 = dom.createElement("fo:block-container");
    area[0].styleContainer(a_txt_1,body.margin_top);
    txt_1.appendChild( a_txt_1 );
    QDomElement b_txt_1 = dom.createElement("fo:block");
    b_txt_1.setAttribute ("color",hc.foName(area[0].bg));
    b_txt_1.appendChild( dom.createTextNode ( "." )  );
    a_txt_1.appendChild( b_txt_1 );
    b_txt_1.appendChild( dom.createComment(QString("Header region / xsl-region-before ")) );
    pageseq1.appendChild( txt_1 );
    }
    /* footer */
    if (area[1].enable) {
    QDomElement txt_2 = dom.createElement("fo:static-content");
    txt_2.setAttribute ("flow-name","xsl-region-after");
    QDomElement a_txt_2 = dom.createElement("fo:block-container");
    area[1].styleContainer(a_txt_2,body.margin_bottom);
    txt_2.appendChild( a_txt_2 );
    QDomElement b_txt_2 = dom.createElement("fo:block");
    ///////b_txt_2.setAttribute ("color",hc.foName(area[1].bg));
    b_txt_2.appendChild( dom.createTextNode ( "." )  );
    a_txt_2.appendChild( b_txt_2 );
    b_txt_2.appendChild( dom.createComment(QString("Footer region / xsl-region-after ")) );
    pageseq1.appendChild( txt_2 );
    }
    
    if (area[2].enable) {
    QDomElement txt_3 = dom.createElement("fo:static-content");
    txt_3.setAttribute ("flow-name","xsl-region-start");
    QDomElement a_txt_3 = dom.createElement("fo:block-container");
    area[2].styleContainer(a_txt_3,body.margin_left);
    txt_3.appendChild( a_txt_3 );
    QDomElement b_txt_3 = dom.createElement("fo:block");
    //////////b_txt_3.setAttribute ("color",hc.foName(area[2].bg));
    b_txt_3.appendChild( dom.createTextNode ( "." )  );
    a_txt_3.appendChild( b_txt_3 );
    b_txt_3.appendChild( dom.createComment(QString("Left  region / xsl-region-start ")) );
    pageseq1.appendChild( txt_3 );
    }
    
    if (area[3].enable) {
    QDomElement txt_4 = dom.createElement("fo:static-content");
    txt_4.setAttribute ("flow-name","xsl-region-end");
    QDomElement a_txt_4 = dom.createElement("fo:block-container");
    area[3].styleContainer(a_txt_4,body.margin_bottom);
    txt_4.appendChild( a_txt_4 );
    QDomElement b_txt_4 = dom.createElement("fo:block");
    /////////b_txt_4.setAttribute ("color",hc.foName(area[3].bg));
    b_txt_4.appendChild( dom.createTextNode ( "." )  );
    a_txt_4.appendChild( b_txt_4 );
    b_txt_4.appendChild( dom.createComment(QString("Right  region / xsl-region-end ")) );
    pageseq1.appendChild( txt_4 );
    }
    
    
    
    
 
    
    
    
    QDomElement body = dom.createElement("fo:flow");
	body.setAttribute ("flow-name","xsl-region-body");
    body.appendChild( dom.createComment (QString("body blocks")) );
    
    
    QDomElement sample = dom.createElement("fo:block");
    sample.setAttribute ("break-after","page");
    sample.setAttribute ("id",QString("sec0"));
    sample.setAttribute ("margin-top","2cm");
    sample.appendChild( dom.createTextNode ( "Hello World! .........." )  );
    body.appendChild( sample );
       
       
          for (int i = 1; i < blocks; ++i) {
           
           QDomElement bloc = dom.createElement("fo:block");
           bloc.setAttribute ("margin-top","2cm");
           bloc.setAttribute ("id",QString("sec%1").arg(i));
           bloc.setAttribute ("break-after","page");
           bloc.appendChild( dom.createTextNode ( QString("Hello World nr.%1" ).arg(i)  ));
           body.appendChild( bloc );
       }
    
    
    
    
    
    pageseq1.appendChild( body );
    
    
   //////////// body.appendChild( dom.createComment (QString("body blocks")) );
    
    
    
    return dom;
}



























































