#include "Layer_Absolute_Floating.h"


#include "Fo_Format.h"
#include "FoColorName.h"

AbsoluteLayer::~AbsoluteLayer()
{
  //~ qDebug() << "### destroy obj ...";
}



AbsoluteLayer::AbsoluteLayer(QGraphicsItem *parent , LAYERTYPE layermodus )
    : QGraphicsRectItem(parent),dev(new AbsText),Rotate(0),OnMoveRects(false),id(10),xsl_fo_color(FopColor())
{
    dev->q = this;
    
    ApiSession *sx = ApiSession::instance();
    M_PageSize e = sx->CurrentPageFormat();
    printrectarea = e.PrintArea();
    
    penborder = QTextFrameFormat::BorderStyle_None;
    layermods = layermodus;
    internPadding = 0;
    Background_Color = QColor(255,255,255,0);
    Border_Color = QColor(Qt::white);
    Border_Color_t = QColor(Qt::white);
    Border_Color_b = QColor(Qt::white);
    Border_Color_l = QColor(Qt::white);
    Border_Color_r = QColor(Qt::white);

    _border_top = 0.;
    _border_bottom = 0.;
    _border_left = 0.;
    _border_right = 0.;

    /////////qDebug() << "### init....";
    ////const qreal widhtinit = sx->CurrentPageFormat().G_regt.width() - ( FooterHeaderPadding * 2 );
    //////const qreal hightinit = sx->CurrentPageFormat().P_margin.x() - ( FooterHeaderPadding * 2 );
    setAcceptsHoverEvents(true);
    setAcceptDrops(true);
    QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setFlag(QGraphicsItem::ItemIsMovable,false);
 
    



    if (layermodus == DIV_HEADER | layermodus == DIV_FOOTER ) {
    setZValue (1.5);
               if ( layermodus == DIV_HEADER ) {
               setPos(sx->CurrentPageFormat().HeaderInitPoints(0));
               setRect(sx->CurrentPageFormat().HeaderBoundingrect());
               }
               if ( layermodus == DIV_FOOTER ) {
               setPos(sx->CurrentPageFormat().FooterInitPoints(0));
               setRect(sx->CurrentPageFormat().FooterBoundingrect());
               }
    
    
    } else {
    setZValue (2.5);
    setRect(QRectF(0,0,200,23));
    }
    
    
    
    Angle_1 = new FWButton(this,Qt::green,tr("Move Layer"));
    Angle_2 = new FWButton(this,Qt::green,tr("Rotate layer"));
    Angle_4 = new FWButton(this,Qt::green,tr("Resize layer"));

    if (layermodus == DIV_ABSOLUTE) {
    Angle_1->allow(true);
    Angle_4->allow(true);
    Angle_2->allow(true);
    connect( Angle_1, SIGNAL(dragging(const QPointF&)), this, SLOT(slotModpos_1(const QPointF&)));   
    connect( Angle_4, SIGNAL(dragging(const QPointF&)), this, SLOT(slotResize_1(const QPointF&)));
    connect( Angle_2, SIGNAL(dragging(const QPointF&)), this, SLOT(slotRotate_1(const QPointF&)));
    connect( Angle_1, SIGNAL(operate(bool)), this, SLOT(MoveActions(bool)));
    connect( Angle_2, SIGNAL(operate(bool)), this, SLOT(MoveActions(bool)));
    connect( Angle_4, SIGNAL(operate(bool)), this, SLOT(MoveActions(bool)));
    }
  
    lastUpdateRequest = rect();  
    QTextDocument *dummy = new QTextDocument();
    dummy->setHtml ( "<p></p>" );
    setDocument(dummy,FOP);
    dev->txtControl()->SetRect ( rect() );
    UpdateDots();
    
    
}

void AbsoluteLayer::setStyle( QString stylelist )
{
    if (layermods == DIV_HEADER | layermods == DIV_FOOTER ) {
     return;
    }
    ApiSession *sx = ApiSession::instance();
    internPadding = 0;
    qDebug() << "### syle " << stylelist;
    incss.clear();
    #define ALPHAHTML(alpha) ((alpha)*254.99999999)
    QStringList syle = stylelist.split(";");
    css2list.clear();
    qreal hi,wi;
    qreal maxi = 111.1;  
    qreal minimums = 10.1; 
    css2list << "position" << "top" << "left" << "width" << "min-height" << "degree-rotation" << "rotate" << "reference-orientation" << "opacity";
    css2list << "height" << "background-color" << "z-index" << "id" << "border-width" << "border-color";
    css2list << "absolute-position" << "top" << "right" << "bottom" << "left" << "background-attachment" << "background-image";
    css2list << "background-repeat" << "background-position-horizontal" << "background-position-vertical";
    css2list << "border-before-color" << "border-before-style" << "border-before-width" << "border-after-color";
    css2list << "border-after-style" << "border-after-width" << "border-start-color" << "border-start-style";
    css2list << "border-start-width" << "border-end-color" << "border-end-style" << "border-end-width";
    css2list << "border-top-color" << "border-top-style" << "border-top-width" << "border-bottom-color";
    css2list << "border-bottom-style" << "border-bottom-width" << "border-left-color" << "border-left-style";
    css2list << "border-left-width" << "border-right-color" << "border-right-style" << "border-right-width";
    css2list << "padding-before" << "padding-after" << "padding-start" << "padding-end" << "padding-top" << "padding-bottom";
    css2list << "padding-left" << "padding-right" << "margin-top" << "margin-bottom" << "margin-left" << "margin-right";
    css2list << "space-before" << "space-after" << "start-indent" << "end-indent" << "block-progression-dimension";
    css2list << "break-after" << "break-before" << "clip" << "display-align" << "height" << "id" << "inline-progression-dimension";
    css2list << "intrusion-displace" << "keep-together" << "keep-with-next" << "keep-with-previous" << "overflow";
    css2list << "border-style" << "docversion" << "l-lock";  //////  border-color:#FFFF00; border-width:2px; border-style:solid;
    
    
    /*
    A_before = 0,
    A_after = 1, 
    A_right = 2,
    A_left = 3
    */
    
 
    
    ///////qDebug() << "### find " << find;
    
    for (int o = 0; o < css2list.size(); ++o)  {
         incss.insert(css2list.at(o),QString());
    }
    for (int i = 0; i < syle.size(); ++i)  {
        QString values = syle.at(i).trimmed();
        QStringList css = values.split(":");
        if (css.size() == 2) {
        incss.insert(css.at(0),css.at(1));
            
            if (css.at(0).startsWith("padding-") ) {
            internPadding = qMax(FopInt(css.at(1)),internPadding);
            }
            
        }
    }
    
    const qreal docvervion = incss.value("docversion").toDouble();
    Q_ASSERT(docvervion > 1.1);
    
    
    if (FopInt(incss.value("min-height").toString()) > 10) {
        hi = FopInt(incss.value("min-height").toString());
    }
    
    
    
    
    
    if (!incss.value("left").toString().isEmpty()) {
        setPos(QPointF(FopInt(incss.value("left").toString()),FopInt(incss.value("top").toString())));
    }
    if ( FopInt(incss.value("width").toString()) > 10 ) {
        wi = FopInt(incss.value("width").toString());
           
       
    }
    if (FopInt(incss.value("height").toString()) > 10) {
        hi = FopInt(incss.value("height").toString());
    }

    if (incss.value("z-index").toInt() > 1) {
        const qreal Ziindex = qBound(minimums,incss.value("z-index").toDouble(),maxi);
        setZValue(Ziindex);   
    }

    if (hi > 0 && wi > 0) {
    setRect(QRectF(0,0,wi,hi));
    }
    
    
    if (incss.value("background-color").toString() !="0") {
        
            Background_Color = xsl_fo_color.foColor(incss.value("background-color").toString(),FopColor::Transparent);
            /* old format compatible new not having */
            if (incss.value("opacity").toInt() > 0) {
              const int alFaCol = qBound(0,incss.value("opacity").toInt(),255);
              qDebug() << "### alFaCol " << alFaCol;
              Background_Color.setAlpha(alFaCol);
            }
          
          if (incss.value("background-color").toString() == "transparent") {
            Background_Color.setAlpha(0);
          }
    }
    penborder = QTextFrameFormat::BorderStyle_None;
    QStringList bordspen;
	bordspen << "border-style" << "border-before-style" << "border-bottom-style" << "border-left-style" << "border-right-style" << "border-start-style" << "border-top-style";
    
    for (int i = 0; i < bordspen.size(); ++i)  {
              const QString borderstyler = bordspen.at(i);
              if (incss.value(borderstyler).toString() !="0") {
                penborder =  StyleBrushBorder(incss.value(borderstyler).toString());
              }
    }
    
    _border_top = borderWidth(A_before);
    _border_bottom = borderWidth(A_after);
    _border_left = borderWidth(A_left);
    _border_right = borderWidth(A_right);
    const qreal havingborder = qMax (qMax (_border_right,_border_left),
    qMax(_border_top,_border_bottom));
 
    if (havingborder > 0 || penborder != QTextFrameFormat::BorderStyle_None) {
         penborder = QTextFrameFormat::BorderStyle_Solid;
         /////////////qDebug() << "### pen style to border ........ -------- " << penborder;
        
        
        
                     for (int i = 0; i < bordspen.size(); ++i)  {
                      const QString borderstyler = bordspen.at(i);
                      if (incss.value(borderstyler).toString() !="0") {
                        penborder =  StyleBrushBorder(incss.value(borderstyler).toString());
                      }
                    }
                    
                
                /*  set all black if not found !! */
                Border_Color_t = borderColor( A_before);
                Border_Color_b = borderColor( A_after );
                Border_Color_l = borderColor( A_left);
                Border_Color_r = borderColor( A_right );
                    
                if (docvervion <= 1.9) {
                /* bug on old version minor of 2.0 */
                Border_Color_t = Border_Color_l;
                Border_Color_b = Border_Color_l;
                Border_Color_r = Border_Color_l; 
                }

               

          /* having border style and not tickness */                    
         if (havingborder < 0.2) {
             _border_top = 0.8;
             _border_bottom = 0.8;
             _border_left = 0.8;
             _border_right = 0.8;
         }
         
         
                if (docvervion <= 1.9) {
                    /* border external from wi x hi */
                    if (hi > 0 && wi > 0) {
                      setRect(QRectF(0,0,wi + _border_left + _border_right ,hi + _border_top +  _border_bottom));
                    }
                    
                }
                    
                
    }
    
    M_PageSize e = sx->CurrentPageFormat();
    qDebug() << "### docvervion  " << docvervion ;
    printrectarea = e.PrintArea();
    
    
    /*
    
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
    
    */
    
    if (docvervion > 1.3) {
     /* translate absolute position on print area !!!! */
		const qreal RightMargin = e.body.margin_right;
		const qreal BottomMargin = e.body.margin_bottom;
		const qreal LeftMargin = e.body.margin_left;
        const qreal TopMargin = e.body.margin_top;
        const qreal nowleft = pos().x();
        const qreal nowtop = pos().y();
        
        qDebug() << "### nowleft nowleft nowleft " << nowleft;
        qDebug() << "### nowleft LeftMargin " << LeftMargin;
        
        if (nowleft < printrectarea.left()) {
        /////////setPos(QPointF(printrectarea.left(),nowtop)); 
        } 
    } else {
        
    }

    ///////ApiSession *sx = ApiSession::instance();
    ////const qreal widhtinit = sx->CurrentPageFormat().G_regt.width() - ( FooterHeaderPadding * 2 );
    
    /* Layer orientation */
    ////////"degree-rotation" << "rotate" << "reference-orientation" 
    if (incss.value("rotate").toInt() !=0 || 
         incss.value("degree-rotation").toInt() !=0 ||
         incss.value("reference-orientation").toInt() !=0 ) {   
       if (incss.value("reference-orientation").toDouble() !=0) {
       Rotate =  incss.value("reference-orientation").toDouble();
       }
       if (incss.value("rotate").toDouble() !=0) {
       Rotate =  incss.value("rotate").toDouble();
       }
       if (incss.value("degree-rotation").toDouble() !=0) {
       Rotate =  incss.value("degree-rotation").toDouble();
       }
    }
    /* reset zero from fop reader auto correct error */
    if (incss.value("display-align").toString() == "auto") {
       Rotate = 0;
    }
    
    /* write lock enable 1 */
    const int lockstatus = incss.value("l-lock").toInt();

    
    /* border padding check */
    const qreal nwidht = rect().width();
    document()->setPageSize ( QSizeF(nwidht,nwidht * 10));

    dev->txtControl()->setBlinkingCursorEnabled(false);
    UpdatePageFormat();
    
}


QColor AbsoluteLayer::borderColor( WMARGIN e )
{
   QStringList marginli;
   marginli.clear();
   qreal result = 0.;
    if (e == A_before) {
     marginli << "border-color" << "border-before-color" << "border-nord-color";
    } else if (e == A_after) {
     marginli << "border-color" << "border-after-color" << "border-sud-color";
    } else if (e == A_right) {
     marginli << "border-color" << "border-end-color" << "border-ost-color";
    } else if (e == A_left) {
     marginli << "border-color" << "border-start-color" << "border-west-color";
    }
     for (int i = 0; i < marginli.size(); ++i)  {
              const QString param = marginli.at(i);
              const QString value = incss.value(param).toString();
              if (!value.isEmpty()) {
               return xsl_fo_color.foColor(incss.value(param).toString(),FopColor::DarkColor);
              }
          
            
    }
return xsl_fo_color.foColor("black",FopColor::DarkColor);
}

qreal AbsoluteLayer::borderWidth( WMARGIN e )
{
   QStringList marginli;
   marginli.clear();
   qreal result = 0.;
    if (e == A_before) {
     marginli << "border-top" << "border-before-width" << "border-nord" << "border-width";
    } else if (e == A_after) {
     marginli << "border-bottom" << "border-after-width" << "border-sud" << "border-width";
    } else if (e == A_right) {
     marginli << "border-right" << "border-end-width" << "border-ost" << "border-width";
    } else if (e == A_left) {
     marginli << "border-left" << "border-start-width" << "border-west" << "border-width";
    }
    for (int i = 0; i < marginli.size(); ++i)  {
              const QString param = marginli.at(i);
              const QString value = incss.value(param).toString();
              if (!value.isEmpty()) {
                  if (value == "thick") {
                     result = 5.2; 
                  }
                  if (value == "thin") {
                     result = 0.7; 
                  }
                  if (value == "small") {
                     result = 1; 
                  }
                  result = qMax(FopInt(value),result);
              }
          
            
    }
    
    qDebug() << "### border widht......... -------- " << result;
    
    
   return result;
}

void AbsoluteLayer::paintBorderLayer( QPainter *painter )
{
    const QRectF rec = boundingRect();
    
    if (_border_top > 0) {
    const qreal bbwi = _border_top;
    QPen tPen(QBrush(Border_Color_t),bbwi);
    painter->setPen(tPen);
    QLineF Line(rec.topLeft() + QPointF(bbwi / 2, bbwi /2 ),rec.topRight() + QPointF(bbwi / 2, bbwi /2 ));
    Line.setLength ( rec.width() -  bbwi );
    painter->drawLine(Line);
    }
    
    if (_border_bottom > 0) {
    const qreal bbwi = _border_bottom;
    QPen tPen(QBrush(Border_Color_b),bbwi);
    painter->setPen(tPen);
    QLineF Line(rec.bottomLeft() + QPointF(+(bbwi /2),-(bbwi /2)),rec.bottomRight() + QPointF(+(bbwi /2),-(bbwi /2)));
    Line.setLength ( rec.width() -  bbwi );
    painter->drawLine(Line);
    }
    
    if (_border_left > 0) {
    const qreal bbwi = _border_left;
    QPen tPen(QBrush(Border_Color_l),bbwi);
    painter->setPen(tPen);
    QLineF Line(rec.topLeft() + QPointF(bbwi / 2, bbwi /2 ), rec.bottomLeft() + QPointF(+(bbwi /2),-(bbwi /2)) );
    Line.setLength ( rec.height() -  bbwi );
    painter->drawLine(Line);
    }
    
    if (_border_right > 0) {
    const qreal bbwi = _border_right;
    QPen tPen(QBrush(Border_Color_r),bbwi);
    painter->setPen(tPen);
    QLineF Line(rec.topRight() + QPointF(bbwi / 2, bbwi /2 ), rec.bottomRight() + QPointF(+(bbwi /2),-(bbwi /2)) );
    Line.setLength ( rec.height() -  bbwi );
    painter->drawLine(Line);
    }
    
    
    
    
    
    
    /*
    
      Border_Color_t = xsl_fo_color.foColor(incss.value("border-start-color").toString(),FopColor::Transparent);
                Border_Color_b = xsl_fo_color.foColor(incss.value("border-start-color").toString(),FopColor::Transparent);
                Border_Color_l = xsl_fo_color.foColor(incss.value("border-start-color").toString(),FopColor::Transparent);
                Border_Color_r = xsl_fo_color.foColor(incss.value("border-start-color").toString(),FopColor::Transparent);
                _border_top = FopInt(incss.value("border-width").toString()); 
                _border_bottom = FopInt(incss.value("border-width").toString()); 
                _border_left = FopInt(incss.value("border-width").toString()); 
                _border_right = FopInt(incss.value("border-width").toString()); 
    
    */
    
    
    
    
    
    
    
    
    
}

void AbsoluteLayer::UpdateDots()
{
    Angle_1->setPos(boundingRect().topLeft());
    Angle_4->setPos(boundingRect().bottomRight());
    Angle_2->setPos(boundingRect().topRight());
    if (layermods == DIV_ABSOLUTE) {
    document()->setPageSize ( QSizeF(boundingRect().width(),boundingRect().height()));
    /* frame root */
                 QTextFrame  *RootFrame = document()->rootFrame();
                 QTextFrameFormat formatibb = RootFrame->frameFormat(); 
                 formatibb.setLeftMargin(_border_left);
                 formatibb.setBottomMargin(_border_bottom);
                 formatibb.setTopMargin(_border_top);
                 formatibb.setRightMargin(_border_right);
                 formatibb.setPadding ( internPadding);
                 RootFrame->setFrameFormat(formatibb);
     /*
     document()->setPageSize ( QSizeF(boundingRect().width() - (_border_left + _border_right ),
                  boundingRect().height() - (_border_top + _border_bottom )));
    */
        
        
    emit pagesize_swap();
    }
    
}

void AbsoluteLayer::setDocument( const QTextDocument * doc , FileHandlerType Type )
{
    dev->txtControl()->setDocument(doc,Type);
    dev->txtControl()->SetRect ( rect() );
    if (layermods == DIV_HEADER | layermods == DIV_FOOTER ) {
    document()->setMaximumBlockCount(FooterHeaderMaxBlocks);
    }
    ApiSession *session = ApiSession::instance();
    session->ensureImageDoc(document());
}

QTextCursor AbsoluteLayer::textCursor() 
{
  return dev->txtControl()->textCursor();
}

QTextDocument *AbsoluteLayer::document()
{
  return dev->txtControl()->document();
}


void AbsoluteLayer::UpdatePageFormat()
{
   if (layermods == DIV_HEADER | layermods == DIV_FOOTER ) {
               ApiSession *sx = ApiSession::instance();
               if ( layermods == DIV_HEADER ) {
               setPos(sx->CurrentPageFormat().HeaderInitPoints(0));
               setRect(sx->CurrentPageFormat().HeaderBoundingrect());
               }
               if (layermods == DIV_FOOTER ) {
               setPos(sx->CurrentPageFormat().FooterInitPoints(0));
               setRect(sx->CurrentPageFormat().FooterBoundingrect());
               }
   
   }
   updatearea(boundingRect().toRect());
   UpdateDots();
}


/*
const qreal RightMargin = e.body.margin_right;
		const qreal BottomMargin = e.body.margin_bottom;
		const qreal LeftMargin = e.body.margin_left;
        const qreal TopMargin = e.body.margin_top;

*/


void AbsoluteLayer::updatearea( const QRect areas )
{
    lastUpdateRequest = areas; 
    ////////qDebug() << "### updatearea " << areas;
    const qreal anspace = FooterHeaderPadding * 2;
    const QRectF txtrect = dev->txtControl()->boundingRect();
    if (rect().height() < txtrect.height()) {
    setRect(QRectF(0,0,rect().width(),txtrect.height()));
    }
    if (layermods == DIV_HEADER | layermods == DIV_FOOTER ) {
    
      if (txtrect.height() != rect().height()) {
         setRect(QRectF(0,0,rect().width(),txtrect.height()));
         ApiSession *sx = ApiSession::instance();
         /////const QRectF mold = sx->CurrentPageFormat().P_margin;
         //////QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
         if (layermods == DIV_HEADER ) {
         ///////sx->current_Page_Format.SetMargin(QRectF(txtrect.height() + anspace ,mold.y(),mold.width(),mold.height()));
         }
         if (layermods == DIV_FOOTER ) {
         //////sx->current_Page_Format.SetMargin(QRectF(mold.x(),mold.y(),txtrect.height() + anspace ,mold.height()));
         setPos(sx->CurrentPageFormat().FooterInitPoints(0));
         }
         emit pagesize_swap();
      }
    
    }
    UpdateDots();
    update(areas);
}

void AbsoluteLayer::slotRotate_1( const QPointF posi )
{
    QPointF newPos = mapFromScene(posi);
    QPointF refPos = Angle_2->pos();
    if (newPos == refPos) {
        return;
    }
    QLineF newangles(boundingRect().topLeft(),newPos);
    newangles.setLength ( boundingRect().width() );
    Rotate = 360 - newangles.angle();
    if (Rotate > 359.5 | Rotate < 0.9) {
    Rotate = 0;
    }
    //////////qDebug() << "### Rotate " << Rotate;
    
    
    dev->txtControl()->SetRect ( rect() );
    UpdateDots();
    update();
}


QLineF AbsoluteLayer::LineTops()
{
  return transform().map(QLineF(0,0,rect().width(),0));
}

QLineF AbsoluteLayer::Diagonal()
{
  return transform().map(QLineF(QPointF(0,0),boundingRect().bottomRight()));
}


////////////////QPointF ( qreal x, qreal y )

void AbsoluteLayer::slotModpos_1( const QPointF posi )
{
    QPointF newPos = mapFromScene(posi);
    QPointF refPos = Angle_1->pos();
    if (newPos == refPos) {
        return;
    }
    QTransform trap = transform();
    QRectF truebound = trap.mapRect ( absoluteRect() );
    QPointF buttonrelative = mapFromItem( Angle_1 ,  posi );
    setPos(buttonrelative - LineTops().p1());
    UpdateDots();
}

void AbsoluteLayer::slotResize_1( const QPointF posi )
{
    QPointF newPos = mapFromScene(posi);
    QPointF refPos = Angle_4->pos();
    if (newPos == refPos) {
        return;
    }
    qreal Altominimo = MinimumHightLayer;
    /////if (dev->txtControl()->boundingRect().height() < Altominimo) {
    Altominimo = dev->txtControl()->boundingRect().height();
    ///////}
    
  
    QPointF buttonrelative = mapFromItem( Angle_4 ,  posi ) ;
    QPointF tl = Diagonal().p1();
    const qreal largo = qAbs(pos().rx() +  boundingRect().width()  - buttonrelative.x()  - tl.x() );
    const qreal alto = qAbs(pos().ry() +  boundingRect().height()  - buttonrelative.y() - tl.y());
    setRect(QRectF(0,0,qBound(MinimumWhidhLayer,largo,MAXLargoTmp),qBound(Altominimo,alto,MAXLargoTmp)));
    UpdateDots();
    dev->txtControl()->SetRect ( rect() );

}

void AbsoluteLayer::MoveActions( bool e )
{
   OnMoveRects = e;
   //////qDebug() << "### MoveActions " << e;
   UpdateDots();
   if (!e)  {
   ///////dev->txtControl()->setBlinkingCursorEnabled(true);
   update(boundingRect());
   } else {
   dev->txtControl()->SetRect ( rect() );
   }
   ShowInfos();
}


void AbsoluteLayer::ShowInfos()
{
   
   QString infoPart = QString("Zindex %1  ID = %2 Rotate =%3 ").arg(zValue()).arg(id).arg(Rotate);
   infoPart.append(QString("\n[Document width = %1pt]\n").arg(document()->size().width()));
   setToolTip(QString("Layer %1pt x %2pt X=%4 pt Y=%3 pt %5")
                                .arg(ToUnit(rect().width(),"pt"))
                                .arg(ToUnit(rect().height(),"pt"))
                                .arg(ToUnit(pos().y(),"pt"))
                                .arg(ToUnit(pos().x(),"pt")).arg(infoPart)); 
    
}

QPicture AbsoluteLayer::LayerImage( const int pagenr )
{
        QPicture img;
        img.setBoundingRect(rect().toRect());
        int pagefollow = pagenr + 1;
        QTextDocument * doc = document()->clone();


          for (QTextBlock srcBlock = document()->firstBlock(), dstBlock = doc->firstBlock();
             srcBlock.isValid() && dstBlock.isValid();
             srcBlock = srcBlock.next(), dstBlock = dstBlock.next()) {
            dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
          }



        QTextCursor cu(doc);
        cu.setPosition(0,QTextCursor::MoveAnchor);

        /* search  _PAGE_NUMERATION_   #Page#  and replace nr. */
        QTextCursor bcu = doc->find(_PAGE_NUMERATION_,cu,QTextDocument::FindWholeWords);
        if (!bcu.isNull ()) {
          if (bcu.hasSelection()) {
           QTextCharFormat format = bcu.charFormat();
                 QString remtxt = bcu.selectedText();
                 for (int i = 0; i < remtxt.size(); ++i) {
                   bcu.deleteChar();
                 }
          bcu.insertText(QString("%1").arg(pagefollow),format);
          bcu.clearSelection();
          }
        }

        
        QTextFrame  *Tframe = doc->rootFrame();
        const QRectF stxt = doc->documentLayout()->frameBoundingRect(Tframe);


         QPainter painter;
         painter.begin(&img);  
         painter.setRenderHint(QPainter::TextAntialiasing);
         painter.setBrush(Background_Color);
         painter.setPen(Qt::NoPen);
         painter.drawRect(rect());

         QAbstractTextDocumentLayout::PaintContext CTX;
         painter.setClipRect(stxt);
         CTX.cursorPosition = -1;
         CTX.clip = stxt;
         doc->documentLayout()->draw(&painter,CTX);
         painter.end(); 
         
         
  return img;
}



void AbsoluteLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setBrush(Background_Color);
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect());
    
    const qreal hi = rect().height();
    const qreal wi = rect().width();
    QRectF pagen = rect();

    dev->txtControl()->SetRect ( rect() );
    dev->txtControl()->paint(painter,option,widget);

    const qreal border = 0.5;
    const QRectF  BoRect(-border,-border,wi + (border * 2) , hi + (border * 2) );
   
    if (layermods == DIV_ABSOLUTE) {
          if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus)  | OnMoveRects ) {
          painter->setPen( QPen(Qt::black ,0.5));
          painter->setBrush(Qt::NoBrush);
          painter->drawRect(BoRect);
          }
    } else {
          if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus)  | OnMoveRects ) {
          painter->setPen( QPen(Qt::lightGray ,0.5));  /////Qt::DotLine
          painter->setBrush(Qt::NoBrush);
          painter->drawRect(BoRect);
          }
    
    }


    if (OnMoveRects) {
    painter->setOpacity(0.8);
    const qreal fullliwid = 1.5;
    const qreal limid = fullliwid / 2;
    //////painter->setPen( QPen( Qt::green ,fullliwid));
    
        QRectF rightShadow(pagen.right(), pagen.top() + fullliwid + limid, fullliwid + limid , pagen.height());
        QRectF bottomShadow(pagen.left() + fullliwid + limid, pagen.bottom(), pagen.width(), fullliwid + limid);
        painter->fillRect(rightShadow, Qt::green);
        painter->fillRect(bottomShadow, Qt::green);
    
    
    painter->setOpacity(1.0);
    }
                
    paintBorderLayer(painter);
    /*
    painter->setPen(Qt::NoPen);
    QColor Visiblerecord(Qt::red);
    Visiblerecord.setAlpha(22);
    painter->setBrush(Visiblerecord);
    painter->drawRect(lastUpdateRequest);
   */
  
    QMatrix matrix;
    matrix.translate ( boundingRect().center().x() , boundingRect().center().y() );
    matrix.rotate(Rotate);
    matrix.translate ( - boundingRect().center().x() , - boundingRect().center().y() );

    setTransform(QTransform(matrix),false);
}

void AbsoluteLayer::focusInEvent ( QFocusEvent * event ) 
{
    /////////qDebug() << "### AbsoluteLayer focusInEvent ..." << flags();
    QGraphicsItem::setSelected(true);
    scene()->setFocusItem(this,Qt::ShortcutFocusReason);
    dev->txtControl()->setBlinkingCursorEnabled(true);
    emit close_main_cursor();
    return QGraphicsItem::focusInEvent(event);
}

void AbsoluteLayer::focusOutEvent ( QFocusEvent * event ) 
{
    ///////////qDebug() << "### AbsoluteLayer focusOutEvent ...";
    QGraphicsItem::setSelected(false);
    dev->txtControl()->setBlinkingCursorEnabled(false);
    return QGraphicsItem::focusOutEvent(event);
}

void AbsoluteLayer::inputMethodEvent ( QInputMethodEvent * event )  
{
    //~ qDebug() << "### inputMethodEvent ...";
     return QGraphicsItem::inputMethodEvent(event);
}


QRectF AbsoluteLayer::boundingRect() const
{
    return rect();
}

QRectF AbsoluteLayer::absoluteRect()
{
    return QRectF(pos(),boundingRect().size());
}





void AbsoluteLayer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

      if (event->button() != Qt::LeftButton) {
      return;
      }
     
     if (dev->txtControl()->procesevent(event)) {
      return;
     }
   
     //~ qDebug() << "### mouseDoubleClickEvent...";
     return QGraphicsItem::mouseDoubleClickEvent(event);
}


void AbsoluteLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{


     if (dev->txtControl()->procesevent(event)) {
      return;
      }
    
    //~ qDebug() << "### mouseMoveEvent.. ";
    return QGraphicsItem::mouseMoveEvent(event);
    
}

void AbsoluteLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
     
  if (event->button() != Qt::LeftButton) {
   return;
  }

  if (dev->txtControl()->procesevent(event)) {
  return;
  }


   //~ qDebug() << "### mouseReleaseEvent...";
  return QGraphicsItem::mouseReleaseEvent(event);
}

void AbsoluteLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
      if (!dev->txtControl()->Edit_On()) {
      dev->txtControl()->setBlinkingCursorEnabled(true);
      emit close_main_cursor();
      }
      
      if (event->button() != Qt::LeftButton) {
      return;
      }

       if (dev->txtControl()->procesevent(event)) {
        return;
       }
     
     
    //~ qDebug() << "### mousePressEvent...";
    return QGraphicsItem::mousePressEvent(event);
}

void AbsoluteLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //~ qDebug() << "### hoverEnterEvent...";
    return QGraphicsItem::hoverEnterEvent(event);
}

void AbsoluteLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //~ qDebug() << "### hoverLeaveEvent...";
    dev->txtControl()->setBlinkingCursorEnabled(false);
    return QGraphicsItem::hoverLeaveEvent(event);
}

void AbsoluteLayer::keyPressEvent( QKeyEvent * event ) 
{

   if (layermods == DIV_ABSOLUTE) {
     if ((event->modifiers() & Qt::AltModifier) && event->key() == Qt::Key_Up) {
     seTFront();
     }
     if ((event->modifiers() & Qt::AltModifier) && event->key() == Qt::Key_Down) {
     seTBack();
     }
   }

   return dev->txtControl()->Controller_keyPressEvent(event);
   //~ qDebug() << "### keyPressEvent...";
    return QGraphicsItem::keyPressEvent(event);
}

void AbsoluteLayer::keyReleaseEvent ( QKeyEvent * event )
{
    return dev->txtControl()->Controller_keyReleaseEvent(event);
    //~ qDebug() << "### keyReleaseEvent...";
    return QGraphicsItem::keyReleaseEvent(event);
}


bool AbsoluteLayer::sceneEvent(QEvent *event)
{
  /////////qDebug() << "### sceneEvent.... + type   " << event->type();
  return QGraphicsItem::sceneEvent(event);
}

void AbsoluteLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   MakeAllCommand();

   Rotater *slider;
   QWidgetAction *widgetslider = 0;


   if (dev->txtControl()->Edit_On() && layermods == DIV_ABSOLUTE ) {
        slider = new Rotater(Rotate,event->widget());
        widgetslider = new QWidgetAction(event->widget());
        widgetslider->setDefaultWidget(slider);
        connect(slider, SIGNAL(rotater(int)),this, SLOT(RotateLayer(int)));  
   }



    ContextOpen = true;
    QTextFrame  *RootFrame = document()->rootFrame();
    FrameStyler *stylerwi = 0;
    QTextCursor c = textCursor();
    bool inlineFrameUnderCursor = false;
    if (c.currentFrame() && c.currentFrame() != RootFrame) {
        inlineFrameUnderCursor = true;
    }

   AbsCommandID BasicActions[] = { FTXTM_UNDO , FTXTM_REDO , FTXTM_SELECTALL , F_SEPARATOR, FTXTM_COPY , FTXTM_CUT , FTXTM_PASTE , F_SUBMENUS , FTXT_BOLD , FTXT_UNDERLINE
, FTXT_STRIKOUT , FTXT_OVERLINE , FLINK_TEXT , FTXT_NOBREAKLINE , FFONT_LETTER_SPACING , F_SEPARATOR ,  FTXT_FONTS , FTXT_BG_COLOR , FBLOCK_BGCOLOR , FLAYER_BG , 
FTXT_COLOR , 
ZINDEX_MIN , ZINDEX_MAX , F_NONE };
 
 AbsCommandID TablesAction[] = { FTABLE_FORMATS ,  FTABLE_BGCOLOR ,  FTABLE_CELLBGCOLOR , FTABLE_APPENDCOOL , FTABLE_APPENDROW , F_SEPARATOR , FTABLE_REMCOOL , FTABLE_REMROW ,  F_SEPARATOR , FTABLE_MERGECELL , FTABLE_COOLWIDHT  ,  F_NONE };

AbsCommandID BlockActionPara[] = { FBLOCK_MARGINS , FBLOCK_BGCOLOR , F_SEPARATOR , FBLOCK_ALIGN_LEFT , FBLOCK_ALIGN_CENTER ,  FBLOCK_ALIGN_RIGHT , FBLOCK_ALIGN_JUSTIFY ,  F_NONE };


QMenu *rootmenu = new QMenu(event->widget());  




    QMenu *MenuTables = new QMenu(tr("Table handler"),rootmenu);
    MenuTables->setIcon(QIcon(QString::fromUtf8(":/img/newtodo.png")));
    MenuTables->addAction(CommandStorage::instance()->actF(FINSERT_TABLE));

    for (int w = 0; TablesAction[w] != F_NONE; w++) {
                 AbsCommandID id = TablesAction[w];
                 if ( id == F_SEPARATOR) {
                    MenuTables->addSeparator();
                 }
                 QAction* t_1 = CommandStorage::instance()->actF(id);
                 if (t_1) {
                 MenuTables->addAction(t_1);
                 }
        
    }
  
  QMenu *MenuParagr = new QMenu(tr("TextBlock formats"),rootmenu);
  MenuParagr->setIcon(QIcon(QString::fromUtf8(":/img/document.png")));
  
  
    for (int o = 0; BlockActionPara[o] != F_NONE; o++) {
                 AbsCommandID id = BlockActionPara[o];
                 if ( id == F_SEPARATOR) {
                    MenuParagr->addSeparator();
                 }
                 QAction* t_1 = CommandStorage::instance()->actF(id);
                 if (t_1) {
                 MenuParagr->addAction(t_1);
                 }
        
    }
  
  
   


 
 
    for (int j = 0; BasicActions[j] != F_NONE; j++) {
        AbsCommandID id = BasicActions[j];
                 if ( id == F_SEPARATOR) {
                     rootmenu->addSeparator();
                 }
                 if ( id == D_SUBMENUS ) {
                     rootmenu->addSeparator();
                     rootmenu->addAction(MenuParagr->menuAction()); 
                     rootmenu->addAction(MenuTables->menuAction()); 
                     //////rootmenu->addAction(MenuFrame->menuAction()); 
                     if (widgetslider) {
                     rootmenu->addAction(widgetslider); 
                     }
                 
                 
                     rootmenu->addSeparator();
                 }
      QAction* a_1 = CommandStorage::instance()->actF(id);
      if (a_1) {
      rootmenu->addAction(a_1);
      }
    }
   //~ qDebug() << "### contextMenuEvent....";
   rootmenu->exec(QCursor::pos());
   if (inlineFrameUnderCursor) {
    stylerwi->deleteLater();
   }     
   rootmenu->deleteLater(); 
   if (widgetslider) {
    widgetslider->deleteLater();
   }   
   ContextOpen = false;
   dev->txtControl()->ClearCurrentSelection();
}

void AbsoluteLayer::MakeAllCommand()
{
    ApiSession *sx = ApiSession::instance();
    bool canpaste = sx->canmime();
    QTextFrame  *RootFrame = document()->rootFrame();
    
    QTextCursor c = textCursor();
    QTextCharFormat fo = c.charFormat();
    QFont f = fo.font();
    bool isbold = textCursor().charFormat().font().bold() == true ? true : false;
    bool isunderline = c.charFormat().underlineStyle() == QTextCharFormat::NoUnderline ? false : true;
    bool inlineFrameUnderCursor = false;
    if (c.currentFrame() && c.currentFrame() != RootFrame) {
        inlineFrameUnderCursor = true;
    }
    bool istable = c.currentTable();
    
    const QIcon TXTcolorico = createColorToolButtonIcon(":/img/textpointer.png",textCursor().charFormat().foreground().color());
    const QIcon TXTBGcolorico = createColorToolButtonIcon(":/img/textpointer.png",textCursor().charFormat().background().color());
  
   const QIcon BlockBGcolorico = createColorToolButtonIcon(":/img/textpointer.png",textCursor().blockFormat().background().color());
    
    
    CommandStorage *dync = CommandStorage::instance();
    dync->clearF();
  
    dync->registerCommand_F(AbsoluteCmd(FTXTM_COPY,false,false,tr("Copy"),QIcon(":/img/copy.png"),QKeySequence("Ctrl+C"),dev->txtControl(),SLOT(copy()),textCursor().hasSelection()));
    dync->registerCommand_F(AbsoluteCmd(FTXTM_PASTE,false,false,tr("Paste"),QIcon(":/img/paste.png"),QKeySequence("Ctrl+V"),dev->txtControl(),SLOT(paste()),canpaste));
    dync->registerCommand_F(AbsoluteCmd(FTXTM_CUT,false,false,tr("Cut"),QIcon(":/img/cut.png"),QKeySequence("Ctrl+X"),dev->txtControl(),SLOT(cut()),textCursor().hasSelection()));
    dync->registerCommand_F(AbsoluteCmd(FTXTM_REDO,false,false,tr("Redo"),QIcon(":/img/editredo.png"),QKeySequence("Ctrl+Z"),dev->txtControl(),SLOT(redo()),true)); ///////  document()->isRedoAvailable()
    dync->registerCommand_F(AbsoluteCmd(FTXTM_UNDO,false,false,tr("Undo"),QIcon(":/img/editundo.png"),QKeySequence("Ctrl+Y"),dev->txtControl(),SLOT(undo()),true)); /////document()->isUndoAvailable()
    dync->registerCommand_F(AbsoluteCmd(FTXTM_SELECTALL,false,false,tr("Select All"),QIcon(":/img/new.png"),QKeySequence("Ctrl+A"),dev->txtControl(),SLOT(selectAll()),true));
    
    
    dync->registerCommand_F(AbsoluteCmd(FTXT_BOLD,true,isbold,tr("Text Bold"),QIcon(":/img/textbold.png"),QKeySequence("Ctrl+B"),dev->txtControl(),SLOT(BoldText()),true));
    dync->registerCommand_F(AbsoluteCmd(FTXT_UNDERLINE,true,isunderline,tr("Text Underline"),QIcon(":/img/textunder.png"),QKeySequence("Ctrl+U"),dev->txtControl(),SLOT(UnderlineText()),true));
    dync->registerCommand_F(AbsoluteCmd(FTXT_STRIKOUT,true,f.strikeOut(),tr("Text Strikeout "),QIcon(":/img/texstrickout.png"),QKeySequence(),dev->txtControl(),SLOT(StrickText()),true));
    dync->registerCommand_F(AbsoluteCmd(FTXT_OVERLINE,true,f.overline(),tr("Text Overline"),QIcon(":/img/texoverline.png"),QKeySequence(),dev->txtControl(),SLOT(OverlineText()),true));
    dync->registerCommand_F(AbsoluteCmd(FTXT_FONTS,false,false,tr("Text Fonts"),QIcon(":/img/textpointer.png"),QKeySequence(),dev->txtControl(),SLOT(FontText()),true));
    
    
    dync->registerCommand_F(AbsoluteCmd(FTXT_BG_COLOR,false,false,tr("Text Fragment Background color"),TXTBGcolorico,QKeySequence(),dev->txtControl(),SLOT(BGcolor()),true));
    
  dync->registerCommand_F(AbsoluteCmd(FBLOCK_BGCOLOR,false,false,tr("Paragraph Background color"),BlockBGcolorico,QKeySequence(),dev->txtControl(),SLOT(ParaBGcolor()),true));
    
    
dync->registerCommand_F(AbsoluteCmd(FBLOCK_ALIGN_LEFT,true,dev->txtControl()->CheckedAlign(FBLOCK_ALIGN_LEFT),tr("Text align left"),QIcon(":/img/textleft.png"),QKeySequence(),dev->txtControl(),SLOT(MaketextAlign()),true));

dync->registerCommand_F(AbsoluteCmd(FBLOCK_ALIGN_RIGHT,true,dev->txtControl()->CheckedAlign(FBLOCK_ALIGN_RIGHT),tr("Text align right"),QIcon(":/img/textright.png"),QKeySequence(),dev->txtControl(),SLOT(MaketextAlign()),true));


dync->registerCommand_F(AbsoluteCmd(FBLOCK_ALIGN_CENTER,true,dev->txtControl()->CheckedAlign(FBLOCK_ALIGN_CENTER),tr("Text align center"),QIcon(":/img/textcenter.png"),QKeySequence(),dev->txtControl(),SLOT(MaketextAlign()),true));

dync->registerCommand_F(AbsoluteCmd(FBLOCK_ALIGN_JUSTIFY,true,dev->txtControl()->CheckedAlign(FBLOCK_ALIGN_JUSTIFY),tr("Text align Justify"),QIcon(":/img/textjustify.png"),QKeySequence(),dev->txtControl(),SLOT(MaketextAlign()),true));


dync->registerCommand_F(AbsoluteCmd(FBLOCK_MARGINS,false,false,tr("Paragraph Margin"),QIcon(":/img/document.png"),QKeySequence(),dev->txtControl(),SLOT(SetParaMargin()),true));

bool unbreak = c.blockFormat().nonBreakableLines();

dync->registerCommand_F(AbsoluteCmd(FTXT_NOBREAKLINE,true,unbreak,tr("Set Unbrekable Block"),QIcon(":/img/document.png"),QKeySequence(),dev->txtControl(),SLOT(SwapUnbrekableBlock()),true));




dync->registerCommand_F(AbsoluteCmd(FLINK_TEXT,true,unbreak,tr("Insert Link"),QIcon(":/img/web-48x48.png"),QKeySequence(),dev->txtControl(),SLOT(LinkText()),true));
    
    
    /*
      BLOCK_ALIGN_LEFT = 1200,
        BLOCK_ALIGN_RIGHT = 1201,
        BLOCK_ALIGN_CENTER = 1202,
        BLOCK_ALIGN_JUSTIFY = 1203,
    */
    
    
    
    dync->registerCommand_F(AbsoluteCmd(FTXT_COLOR,false,false,tr("Text color"),TXTcolorico,QKeySequence(),dev->txtControl(),SLOT(TXcolor()),true));
    
    /* table */
    
    
    dync->registerCommand_F(AbsoluteCmd(FINSERT_TABLE,false,false,tr("Insert Table"),QIcon(":/img/table.png"),QKeySequence(),dev->txtControl(),SLOT(CreateanewTable()),true));
    

    dync->registerCommand_F(AbsoluteCmd(FTABLE_FORMATS,false,false,tr("Table Format"),QIcon(":/img/table.png"),QKeySequence(),dev->txtControl(),SLOT(UpdateTableFormat()),istable));
    dync->registerCommand_F(AbsoluteCmd(FTABLE_BGCOLOR,false,false,tr("Table background color"),createColorIcon(textCursor().currentFrame()->frameFormat().background().color()),QKeySequence(),dev->txtControl(),SLOT(MaketableColorBG()),istable));
    dync->registerCommand_F(AbsoluteCmd(FTABLE_CELLBGCOLOR,false,false,tr("Cell background color"),createColorIcon(textCursor().currentFrame()->frameFormat().background().color()),QKeySequence(),dev->txtControl(),SLOT(SetTableCellColor()),istable));
    dync->registerCommand_F(AbsoluteCmd(FTABLE_APPENDCOOL,false,false,tr("Append column"),QIcon(":/img/row_table.png"),QKeySequence(),dev->txtControl(),SLOT(AppendTableCools()),istable));
    dync->registerCommand_F(AbsoluteCmd(FTABLE_APPENDROW,false,false,tr("Append row"),QIcon(":/img/row_table.png"),QKeySequence(),dev->txtControl(),SLOT(AppendTableRows()),istable));
    dync->registerCommand_F(AbsoluteCmd(FTABLE_REMCOOL,false,false,tr("Remove column on cursor"),QIcon(":/img/stop.png"),QKeySequence(),dev->txtControl(),SLOT(RemoveCoolByCursorPosition()),istable));
    dync->registerCommand_F(AbsoluteCmd(FTABLE_REMROW,false,false,tr("Remove row on cursor"),QIcon(":/img/stop.png"),QKeySequence(),dev->txtControl(),SLOT(RemoveRowByCursorPosition()),istable));
    dync->registerCommand_F(AbsoluteCmd(FTABLE_MERGECELL,false,false,tr("Merge selected cell"),QIcon(":/img/reload.png"),QKeySequence(),dev->txtControl(),SLOT(MergeCellByCursorPosition()),istable));
    dync->registerCommand_F(AbsoluteCmd(FTABLE_COOLWIDHT,false,false,tr("Table Column width"),QIcon(":/img/configure.png"),QKeySequence(),dev->txtControl(),SLOT(SetColumLarge()),istable));
    
    dync->registerCommand_F(AbsoluteCmd(ZINDEX_MAX,false,false,tr("Send Back zindex"),QIcon(":/img/sendtoback.png"),QKeySequence("Alt+Down"),this,SLOT(seTBack()),true));
    
     dync->registerCommand_F(AbsoluteCmd(ZINDEX_MIN,false,false,tr("Send Front zindex"),QIcon(":/img/bringtofront.png"),QKeySequence("Alt+Up"),this,SLOT(seTFront()),true));
  
 dync->registerCommand_F(AbsoluteCmd(FFONT_LETTER_SPACING,false,false,tr("Font Letter Spacing"),QIcon(":/img/textpointer.png"),QKeySequence(),dev->txtControl(),SLOT(FontsLetterSpacing()) , true ));


dync->registerCommand_F(AbsoluteCmd(FLAYER_BG,false,false,tr("Background color"),createColorIcon( Background_Color ),QKeySequence(),this,SLOT(BackGroundColor()),true));



}



void AbsoluteLayer::BackGroundColor()
{
	QRgb col = QColorDialog::getRgba(Background_Color.rgba()); 
	QColor col2 = QColor(col);
	int trans = qAlpha(col); 
	col2.setAlpha(trans);
	if (!col2.isValid()) {
	return;
	}
	Background_Color = col2;

	QTextFrame  *Tframe = document()->rootFrame();
	QTextFrameFormat Ftf = Tframe->frameFormat();
	Ftf.setBackground(QBrush(Qt::transparent));
	Tframe->setFrameFormat(Ftf);
	update();
}




void AbsoluteLayer::seTBack()
{
    if (layermods != DIV_ABSOLUTE ) {
    return;
    }
    qreal maxi = 999.9;
    qreal minimums = 1.5;
    
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    qreal backs = qBound(minimums,sc->zmin() - 0.1,maxi);
    setZValue(backs);
  
    //~ qDebug() << "### seTBack" << backs;
  
    update();
    emit pagesize_swap();
    
}


void AbsoluteLayer::seTFront()
{
    if (layermods != DIV_ABSOLUTE ) {
    return;
    }
  
    qreal maxi = 999.9;
    qreal minimums = 1.5;
    
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    qreal top = qBound(minimums,sc->zmax() + 0.1,maxi);
    top++;
    setZValue(top); 
  
    //~ qDebug() << "### seTFront " << top;
  
    update();
    emit pagesize_swap();
}



void AbsoluteLayer::RotateLayer( const int ro ) 
{ 
   Rotate = ro;
    ////////////qDebug() << "### RotateLayer " << ro;
   update();
}















FWButton::FWButton(QGraphicsItem * parent, const QBrush & brush , const QString msg )
    : QGraphicsItem(parent)
    , m_brush(brush),permission(false)
{
    setAcceptsHoverEvents(true);
    txt = msg;
    QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setToolTip(txt);
}

void FWButton::allow( bool e )
{
    permission = e;
    if (e) {
        QGraphicsItem::setCursor(Qt::SizeAllCursor);
    } else {
        QGraphicsItem::setCursor(Qt::ForbiddenCursor);
    }
}
QRectF FWButton::boundingRect() const
{
    return QRectF(-8, -8, 16, 16);
}

void FWButton::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
    if (permission) {
            if (option->state & QStyle::State_MouseOver) {
                painter->setBrush(m_brush);
                painter->setPen(Qt::white);
                painter->setOpacity(0.8);
                painter->drawEllipse(boundingRect());
                painter->setOpacity(1.0);
            }
            setToolTip(txt);
    }
}

void FWButton::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    emit operate(true);
    m_startPos = event->scenePos();
}

void FWButton::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (m_startPos.isNull())
        return;
    event->accept();
    emit dragging(event->scenePos());
}

void FWButton::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    m_startPos = QPointF();
}

void FWButton::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    emit reset();
}


void FWButton::focusInEvent ( QFocusEvent * event ) 
{
    emit operate(true);
}

void FWButton::focusOutEvent ( QFocusEvent * event ) 
{
    emit operate(false);
}



void FWButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit operate(true);
}

void FWButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit operate(false);
}



AbsText::AbsText()
  : device(0)
{ 

}

/* only one connect */
LayerText *AbsText::txtControl() const
{
    if (!device) {
       AbsoluteLayer *that = const_cast<AbsoluteLayer *>(q); 
       device = new LayerText();
       ////////connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursor_wake_up()));
       connect(device, SIGNAL(q_update(QRect) ),q, SLOT(updatearea(QRect)));
       ///////connect(device, SIGNAL(q_visible(QRectF) ),q, SLOT(ensureVisible(QRectF)));
       ///////connect(device, SIGNAL(q_update_scene()),q, SLOT(SceneReload()));
    }
    return device;
}




