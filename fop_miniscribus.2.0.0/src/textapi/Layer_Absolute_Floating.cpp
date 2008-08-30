#include "Layer_Absolute_Floating.h"
#include "FoColorName.h"
#include "Fo_Format.h"
#include "Scribe_Parser.h"
#include "XML_Editor.h"
#include "Fo_Writter.h"

using namespace ApacheFop;

AbsoluteLayer::~AbsoluteLayer()
{
  //~ qDebug() << "### destroy obj ...";
}



AbsoluteLayer::AbsoluteLayer(QGraphicsItem *parent , LAYERTYPE layermodus , bool createdFromApps )
    : QGraphicsRectItem(parent),dev(new AbsText),Rotate(0),OnMoveRects(false),id(10),editorRunning(false)
{
    dev->q = this;
    
    ApiSession *sx = ApiSession::instance();
    PageFormat = sx->CurrentPageFormat();
    
    layermods = layermodus;
    if (createdFromApps) {
    Background_Color = NEWLAYERCOLORSET;
    } else {
    Background_Color = QColor(Qt::transparent); 
    }
    Border_Color = QColor(Qt::white);
    Border_Color_t = QColor(Qt::white);
    Border_Color_b = QColor(Qt::white);
    Border_Color_l = QColor(Qt::white);
    Border_Color_r = QColor(Qt::white);

    _border_top = 0.;
    _border_bottom = 0.;
    _border_left = 0.;
    _border_right = 0.;

    QRectF Prect = PageFormat.pageBoundingRect();
    ////const qreal widhtinit = sx->CurrentPageFormat().G_regt.width() - ( FooterHeaderPadding * 2 );
    //////const qreal hightinit = sx->CurrentPageFormat().P_margin.x() - ( FooterHeaderPadding * 2 );
    setAcceptsHoverEvents(true);
    setAcceptDrops(true);
    QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setFlag(QGraphicsItem::ItemIsMovable,false);
    setZValue(NEWLAYERZINDEXFLOATING);


    if (layermodus == DIV_HEADER | layermodus == DIV_FOOTER ) {
               if ( layermodus == DIV_HEADER ) {
               setPos(PageFormat.HeaderInitPoints(0));
               setRect(PageFormat.HeaderBoundingrect());
               }
               if ( layermodus == DIV_FOOTER ) {
               setPos(PageFormat.FooterInitPoints(0));
               setRect(PageFormat.FooterBoundingrect());
               }
    
    
    } else {
    setRect(QRectF(0,0,NEWLAYERWI,NEWLAYERHI));
    }
    
    if (layermodus == DIV_START) {
        Rotate = 270.;
        setPos(Prect.bottomLeft());
        Background_Color = QColor("azure");
        setRect(QRectF(0,0,Prect.height() - 2 ,PageFormat.body.margin_left - 2));
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
    UpdatePageFormat();
    setStyle( QString() );
    
}

RichDoc AbsoluteLayer::mimeDocumentLayer()
{
    ScribeParser *parsen = new ScribeParser(document(),ScribeParser::Psave);
    RichDoc volatiledoc = parsen->mimeDocument();
    QStringList css;
    QMapIterator<QString,QVariant> i(layerStyle());
         while (i.hasNext()) {
             i.next();
             const QString valuecss = i.value().toString();
             if ( !valuecss.isEmpty() ) {
             css.append(QString("%1:%2").arg(i.key()).arg(valuecss));
             } 
         }
         
   volatiledoc.style = css.join(";");
   return volatiledoc;
}

QDomDocument AbsoluteLayer::fopDomdoc()
{
    FopDom *fopdoc = new FopDom( new QTextDocument(),PageFormat);  
    return fopdoc->absoluteLayer(this);
}


QMap<QString,QVariant> AbsoluteLayer::layerStyle()
{
   QMap<QString,QVariant> out;
   out.clear();
   FopColor hc = FopColor();
   FoRegion x;
            x.styleReadDocument(document());
     
       QMapIterator<QString,QVariant> i(incss);
         while (i.hasNext()) {
             i.next();
             const QString valuecss = i.value().toString();
             if ( !valuecss.isEmpty() ) {
             out.insert(i.key(),i.value());
             }
         }
		const QString StringColor = QString("rgb(%1,%2,%3,%4)").arg(Background_Color.red())
                                      .arg(Background_Color.green())
                                      .arg(Background_Color.blue())
                                      .arg(Background_Color.alpha());
         
         out.insert("position","absolute");
         out.insert("top",metrics(pos().y()));
         out.insert("left",metrics(pos().x()));
         out.insert("width",metrics(rect().width()));
         out.insert("height",metrics(rect().height()));
         out.insert("background-color",StringColor);
         out.insert("reference-orientation",(int)Rotate);
         out.insert("z-index",zValue());
         out.insert("margin-right",metrics(x.margin_right));
         out.insert("margin-top",metrics(x.margin_top));
         out.insert("margin-left",metrics(x.margin_left));
         out.insert("margin-bottom",metrics(x.margin_bottom));
         out.insert("padding",metrics(x.padding));
             
             if (x.rpen != QPen(Qt::NoPen)) {
                  Qt::PenStyle ds = x.rpen.style();
                  if (ds == Qt::DotLine) {
                  out.insert("border-style","dotted");
                  } else if (ds == Qt::DashLine) { 
                  out.insert("border-style","dashed");
                  } else {
                  out.insert("border-style","solid");
                  }
               out.insert("border-width",metrics(x.rpen.width()));
               out.insert("border-color",hc.foName(x.bog));
             }
         
         
         /* append on top here the new one .. old not parsed attributes stay inside ... */
         
         
         
    return out;
}

void AbsoluteLayer::setStyle( QString stylelist )
{
    if (layermods != DIV_ABSOLUTE ) {
     return;
    }
    
    qreal maxi = MAXIMUMZINDEXFLOATING;  
    qreal minimums = MINIMUMZINDEXFLOATING;
    setZValue (minimums);
    
    FopColor fhc(QString("red"),FopColor::Transparent);
    QStringList syle = stylelist.split(";");
    qreal hi,wi;
    incss.clear();
    
    
    QStringList css2list;
    css2list.clear();
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
    
    
    for (int o = 0; o < css2list.size(); ++o)  {
         incss.insert(css2list.at(o),QString());
    }
    for (int i = 0; i < syle.size(); ++i)  {
        QString values = syle.at(i).trimmed();
        QStringList css = values.split(":");
        if (css.size() == 2) {
        incss.insert(css.at(0),css.at(1));
        }
    }
    
    const qreal docvervion = incss.value("docversion").toDouble();
    
    
    
    if (FopInt(incss.value("min-height").toString()) > 10) {
        hi = FopInt(incss.value("min-height").toString());
    }
    
   ////// 
    
    /////////qDebug() << "### left " << FopInt(incss.value("left").toString());
    
    if (!incss.value("left").toString().isEmpty()) {
        setPos(QPointF(FopInt(incss.value("left").toString()),FopInt(incss.value("top").toString())));
    }
    if ( FopInt(incss.value("width").toString()) > 5 ) {
        wi = FopInt(incss.value("width").toString());
            QTextFrame  *Tframe = document()->rootFrame();
            QTextFrameFormat rootformats = Tframe->frameFormat();
            rootformats.setWidth(wi); 
            Tframe->setFrameFormat(rootformats);
    }
    if (FopInt(incss.value("height").toString()) > 5) {
        hi = FopInt(incss.value("height").toString());
    }

    if (incss.value("z-index").toDouble() > minimums) {
        const qreal Ziindex = qBound(minimums,incss.value("z-index").toDouble(),maxi);
        setZValue(Ziindex);   
    }

    if (hi > 0 && wi > 0) {
    setRect(QRectF(0,0,wi,hi));
    }


    
    if ( !incss.value("background-color").toString().isEmpty() ) {
            Background_Color = fhc.foColor(incss.value("background-color").toString(),FopColor::Transparent);
        
          if (incss.value("opacity").toInt() > 0) {
              int alFaCol = qBound(0,incss.value("opacity").toInt(),255);
              Background_Color.setAlpha(alFaCol);
          }
          
          if (incss.value("background-color").toString() == "transparent") {
            Background_Color.setAlpha(0);
          }
          
          
    }
     
    if (incss.value("degree-rotation").toInt() !=0 || 
         incss.value("rotate").toInt() !=0  ||
         incss.value("reference-orientation").toInt() !=0 ) {
       int Rotx = 0;
       Rotx = qMax(incss.value("degree-rotation").toInt(),incss.value("rotate").toInt());
       Rotx = qMax(incss.value("reference-orientation").toInt(),Rotx);
       Rotate = Rotx;
             
        
    }
    
    
    const int lockstatus = incss.value("l-lock").toInt();
    /* sure to remove */
    incss.take("degree-rotation");
    incss.take("rotate");
    incss.take("reference-orientation");
    incss.take("docversion");
    incss.take("l-lock");
    incss.take("min-height");
    incss.take("opacity");
    incss.take("-moz-opacity");  /* from html */
    
    
     
    /* sure to remove */
    dev->txtControl()->setBlinkingCursorEnabled(false);
    UpdatePageFormat();
    
    QStringList css;
    QMapIterator<QString,QVariant> x(layerStyle());
         while (x.hasNext()) {
             x.next();
             const QString valuecss = x.value().toString();
             if ( !valuecss.isEmpty() ) {
             css.append(QString("%1:%2").arg(x.key()).arg(valuecss));
             } 
         }
   ////////////qDebug() << "### css stay inside  " << css.join(";");
}


void AbsoluteLayer::setDocument( const QTextDocument * doc , FileHandlerType Type )
{
    dev->txtControl()->setDocument(doc,Type);
    dev->txtControl()->SetRect ( rect() );
    if (layermods == DIV_HEADER | layermods == DIV_FOOTER ) {
    document()->setMaximumBlockCount(FooterHeaderMaxBlocks);
    }
    
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
    ///////////////////qDebug() << "### UpdatePageFormat...";
    
    ApiSession *sx = ApiSession::instance();
    PageFormat = sx->CurrentPageFormat();
    QRectF Prect = PageFormat.pageBoundingRect();
    
   if (layermods == DIV_HEADER | layermods == DIV_FOOTER ) {
               setZValue (HEADERFOOTER_ZINDEX);
               if ( layermods == DIV_HEADER ) {
               setPos(PageFormat.HeaderInitPoints(0));
               setRect(PageFormat.HeaderBoundingrect());
               }
               if (layermods == DIV_FOOTER ) {
               setPos(PageFormat.FooterInitPoints(0));
               setRect(PageFormat.FooterBoundingrect());
               }
   
   }
   if (layermods == DIV_START) {
        setZValue (STARTEND_ZINDEX);
        Rotate = 270.;
        setPos(Prect.bottomLeft());
        Background_Color = PageFormat.area[2].bg;
        setRect(QRectF(0,0,Prect.height() ,PageFormat.body.margin_left));
        PageFormat.area[2].styleSmallDocument( document() );
    }
   
   
   updatearea(boundingRect().toRect());
   UpdateDots();
   MakeAllCommand();
   emit scene_wakeup();
}

void AbsoluteLayer::UpdateDots()
{
    Angle_1->setPos(boundingRect().topLeft());
    Angle_4->setPos(boundingRect().bottomRight());
    Angle_2->setPos(boundingRect().topRight());
    
}


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
         ////////const QRectF mold = sx->CurrentPageFormat().P_margin;
         //////QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
         if (layermods == DIV_HEADER ) {
         ///////////sx->current_Page_Format.SetMargin(QRectF(txtrect.height() + anspace ,mold.y(),mold.width(),mold.height()));
         }
         if (layermods == DIV_FOOTER ) {
         /////////////////sx->current_Page_Format.SetMargin(QRectF(mold.x(),mold.y(),txtrect.height() + anspace ,mold.height()));
         //////////setPos(sx->CurrentPageFormat().FooterInitPoints(0));
         }
         emit pagesize_swap();
      }
    
    }
    UpdateDots();
    update(shape().controlPointRect().toRect());
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
    bool dirpaint = true;
    
    if (layermods != DIV_ABSOLUTE) {
            if (layermods == DIV_START) {
                dirpaint = PageFormat.area[2].enable;
            } else if (layermods == DIV_END) {
                dirpaint = PageFormat.area[4].enable;
            } else if (layermods == DIV_HEADER) {
                dirpaint = PageFormat.area[0].enable; 
            } else if (layermods == DIV_FOOTER) {
                dirpaint = PageFormat.area[1].enable; 
            }
    }
    
    if (!dirpaint) {
     /* not enable but must make to strech format page if need on margin
     to calc the area body on paper format */
     return;
    }
    
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setBrush(Background_Color);
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect());
    const qreal hi = rect().height();
    const qreal wi = rect().width();
    QRectF pagen = rect();

    dev->txtControl()->SetRect ( rect() );
    dev->txtControl()->paint(painter,option,widget);
    //////////  shape().controlPointRect()
    const qreal border = 0.5;
    const QRectF  BoRect(-border,-border,wi + (border * 2) , hi + (border * 2) );
   
    // if (option->state & QStyle::State_HasFocus  || option->state & QStyle::State_HasEditFocus ) {
    
    if (layermods == DIV_ABSOLUTE) {
          if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus)  | OnMoveRects ) {
          painter->setPen( QPen(Qt::black ,border));
          painter->setBrush(Qt::NoBrush);
          painter->drawRect(rect());
          }
    } else {
          if ( editorRunning ) {
          painter->setPen( QPen(Qt::red ,border));
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
    
    /*   shape debug 
    const QRectF externrect = shape().controlPointRect();        
    painter->setPen(Qt::NoPen);
    QColor Visiblerecord(Qt::red);
    Visiblerecord.setAlpha(22);
    painter->setBrush(Visiblerecord);
    painter->drawRect(externrect);
    */
   
  
    QMatrix matrix;
    if (layermods == DIV_START) {
    matrix.rotate(Rotate); 
    } else {
    matrix.translate ( boundingRect().center().x() , boundingRect().center().y() );
    matrix.rotate(Rotate);
    matrix.translate ( - boundingRect().center().x() , - boundingRect().center().y() );
    }

    setTransform(QTransform(matrix),false);
}

void AbsoluteLayer::focusInEvent ( QFocusEvent * event ) 
{
    /////////qDebug() << "### AbsoluteLayer focusInEvent ..." << flags();
    editorRunning = true;
    QGraphicsItem::setSelected(true);
    scene()->setFocusItem(this,Qt::ShortcutFocusReason);
    dev->txtControl()->setBlinkingCursorEnabled(editorRunning);
    emit close_main_cursor();
    MakeAllCommand();
    return QGraphicsItem::focusInEvent(event);
}


 QPainterPath AbsoluteLayer::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect().adjusted (-10,-10,20,20));
    return path;
}





void AbsoluteLayer::focusOutEvent ( QFocusEvent * event ) 
{
    editorRunning = false;
    QGraphicsItem::setSelected(false);
    dev->txtControl()->setBlinkingCursorEnabled(editorRunning);
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

  emit scene_wakeup();
    
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

   AbsCommandID BasicActions[] = { FTXTM_UNDO , FTXTM_REDO , FTXTM_SELECTALL , F_SEPARATOR, FTXTM_COPY , FTXTM_CUT , FTXTM_PASTE , F_SUBMENUS , FTXT_BOLD , FTXT_UNDERLINE , FTXT_ITALIC 
, FTXT_STRIKOUT , FTXT_OVERLINE , FLINK_TEXT , FTXT_NOBREAKLINE , FFONT_LETTER_SPACING , F_SEPARATOR ,  FTXT_FONTS , FTXT_BG_COLOR , FBLOCK_BGCOLOR , FLAYER_BG , 
FTXT_COLOR , 
ZINDEX_MIN , ZINDEX_MAX , F_REMLAYER , PLAY_SOURCE_LAYER , F_SEPARATOR  , F_NONE };
 
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
    //////////bool canpaste = sx->canmime();
    QTextFrame  *RootFrame = document()->rootFrame();
    bool canpaste = true;
    
    QTextCursor c = textCursor();
    QTextCharFormat fo = c.charFormat();
    QFont f = fo.font();
    bool isbold = textCursor().charFormat().font().bold() == true ? true : false;
    bool isitalic = textCursor().charFormat().fontItalic() == true ? true : false;
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
    
    dync->registerCommand_F(AbsoluteCmd(FTXT_ITALIC,true,isitalic,tr("Text Italic"),QIcon(":/img/textitalic.png"),QKeySequence("Ctrl+I"),dev->txtControl(),SLOT(ItalicText()),true));
    
    
    
    
    
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

dync->registerCommand_F(AbsoluteCmd(F_REMLAYER,true,unbreak,tr("Remove Layer"),QIcon(":/img/delete.png"),QKeySequence(),this,SLOT(removeCurrent()),true));








    
    
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

dync->registerCommand_F(AbsoluteCmd(PLAY_SOURCE_LAYER,false,false,tr("Fop source "),QIcon(":/img/textpointer.png"),QKeySequence(),this,SLOT(showSource()),true));


}








void AbsoluteLayer::showSource()
{
    XMLTextEdit *sHtml = new XMLTextEdit(0);
    sHtml->setWindowTitle(tr("Source display on debug...."));
	sHtml->setWindowFlags ( Qt::Window );
	sHtml->setMinimumSize ( 450 , 500 );
    sHtml->setPlainText( fopDomdoc().toString(1) );
    sHtml->show();
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
    qreal top = qBound(minimums,sc->zmax() + 2.1,maxi);
    top++;
    setZValue(top); 
  
    //~ qDebug() << "### seTFront " << top;
  
    update();
    emit pagesize_swap();
}

void AbsoluteLayer::removeCurrent()
{
     if (layermods == DIV_ABSOLUTE ) {
    dev->txtControl()->setBlinkingCursorEnabled(false);
    setData(AbsoluteLayerRemoveHashID,document()->toHtml("utf-8"));
    emit pagesize_swap();
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    sc->removeIdItem(AbsoluteLayerRemoveHashID);
     }
}



void AbsoluteLayer::RotateLayer( const int ro ) 
{ 
   Rotate = ro;
    ////////////qDebug() << "### RotateLayer " << ro;
   update();
}

void AbsoluteLayer::cursor_wake_up()
{ 
   //////qDebug() << "### cursor_wake_up ";
   ///////update();
    emit close_main_cursor();
}

FoRegion AbsoluteLayer::foRegion() const
{

    FoRegion regio;
    regio.toAll(0);
        if (blockType() != DIV_ABSOLUTE) {
            if (blockType() == DIV_START) {
                regio = PageFormat.area[2];
            } else if (blockType() == DIV_END) {
                regio = PageFormat.area[4];
            } else if (blockType() == DIV_HEADER) {
                regio = PageFormat.area[0];
            } else if (blockType() == DIV_FOOTER) {
                regio = PageFormat.area[1];
            }
         }
         
    return regio;
}


void AbsoluteLayer::ensureVisible( const QRectF areas )
{
     LastVisibleRequest = areas;
     
     /* translate to scene map and rotate !!!! */
    
     GraphicsScene *sc;
     if (sc = qobject_cast<GraphicsScene *>(scene())) {
     sc->SetVisibleArea(areas);
     }
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

/*



if (option->state & QStyle::State_HasFocus  || option->state & QStyle::State_HasEditFocus ) {


}

*/



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
       device = new LayerText();  /* text-api all event */
       connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursor_wake_up()));
       connect(device, SIGNAL(q_update(QRect) ),q, SLOT(updatearea(QRect)));
       connect(device, SIGNAL(q_visible(QRectF) ),q, SLOT(ensureVisible(QRectF)));
       ///////connect(device, SIGNAL(q_update_scene()),q, SLOT(SceneReload()));
    }
    return device;
}




