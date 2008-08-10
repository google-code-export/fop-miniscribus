#include "_LayerAbsolute_Floating.h"

AbsoluteLayer::~AbsoluteLayer()
{
  qDebug() << "### destroy obj ...";
}



AbsoluteLayer::AbsoluteLayer(QGraphicsItem *parent , LAYERTYPE layermodus )
    : QGraphicsRectItem(parent),dev(new AbsText),Rotate(0),OnMoveRects(false),id(10)
{
    dev->q = this;
    layermods = layermodus;
    qDebug() << "### init....";
    ApiSession *sx = ApiSession::instance();
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
    setZValue (10.5);
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


void AbsoluteLayer::UpdateDots()
{
    Angle_1->setPos(boundingRect().topLeft());
    Angle_4->setPos(boundingRect().bottomRight());
    Angle_2->setPos(boundingRect().topRight());
}


void AbsoluteLayer::updatearea( const QRect areas )
{
    lastUpdateRequest = areas; 
    qDebug() << "### updatearea " << areas;
    const qreal anspace = FooterHeaderPadding * 2;
    const QRectF txtrect = dev->txtControl()->boundingRect();
    if (rect().height() < txtrect.height()) {
    setRect(QRectF(0,0,rect().width(),txtrect.height()));
    }
    if (layermods == DIV_HEADER | layermods == DIV_FOOTER ) {
    
      if (txtrect.height() != rect().height()) {
         setRect(QRectF(0,0,rect().width(),txtrect.height()));
         ApiSession *sx = ApiSession::instance();
         const QRectF mold = sx->CurrentPageFormat().P_margin;
         //////QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
         if (layermods == DIV_HEADER ) {
         sx->current_Page_Format.SetMargin(QRectF(txtrect.height() + anspace ,mold.y(),mold.width(),mold.height()));
         }
         if (layermods == DIV_FOOTER ) {
         sx->current_Page_Format.SetMargin(QRectF(mold.x(),mold.y(),txtrect.height() + anspace ,mold.height()));
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
    qDebug() << "### Rotate " << Rotate;
    
    
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
         painter.setBrush(Qt::white);
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
    painter->setBrush(Qt::white);
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
                
  

    painter->setPen(Qt::NoPen);
    QColor Visiblerecord(Qt::red);
    Visiblerecord.setAlpha(22);
    painter->setBrush(Visiblerecord);
    painter->drawRect(lastUpdateRequest);
  
    QMatrix matrix;
    matrix.translate ( boundingRect().center().x() , boundingRect().center().y() );
    matrix.rotate(Rotate);
    matrix.translate ( - boundingRect().center().x() , - boundingRect().center().y() );

    setTransform(QTransform(matrix),false);
}

void AbsoluteLayer::focusInEvent ( QFocusEvent * event ) 
{
    qDebug() << "### AbsoluteLayer focusInEvent ..." << flags();
    QGraphicsItem::setSelected(true);
    scene()->setFocusItem(this,Qt::ShortcutFocusReason);
    dev->txtControl()->setBlinkingCursorEnabled(true);
    emit close_main_cursor();
    return QGraphicsItem::focusInEvent(event);
}

void AbsoluteLayer::focusOutEvent ( QFocusEvent * event ) 
{
    qDebug() << "### AbsoluteLayer focusOutEvent ...";
    QGraphicsItem::setSelected(false);
    dev->txtControl()->setBlinkingCursorEnabled(false);
    return QGraphicsItem::focusOutEvent(event);
}

void AbsoluteLayer::inputMethodEvent ( QInputMethodEvent * event )  
{
    qDebug() << "### inputMethodEvent ...";
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
   
     qDebug() << "### mouseDoubleClickEvent...";
     return QGraphicsItem::mouseDoubleClickEvent(event);
}


void AbsoluteLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{


     if (dev->txtControl()->procesevent(event)) {
      return;
      }
    
    qDebug() << "### mouseMoveEvent.. ";
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


   qDebug() << "### mouseReleaseEvent...";
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
     
     
    qDebug() << "### mousePressEvent...";
    return QGraphicsItem::mousePressEvent(event);
}

void AbsoluteLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "### hoverEnterEvent...";
    return QGraphicsItem::hoverEnterEvent(event);
}

void AbsoluteLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "### hoverLeaveEvent...";
    dev->txtControl()->setBlinkingCursorEnabled(false);
    return QGraphicsItem::hoverLeaveEvent(event);
}

void AbsoluteLayer::keyPressEvent( QKeyEvent * event ) 
{
   return dev->txtControl()->Controller_keyPressEvent(event);
   qDebug() << "### keyPressEvent...";
    return QGraphicsItem::keyPressEvent(event);
}

void AbsoluteLayer::keyReleaseEvent ( QKeyEvent * event )
{
    return dev->txtControl()->Controller_keyReleaseEvent(event);
    qDebug() << "### keyReleaseEvent...";
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


    ContextOpen = true;
    QTextFrame  *RootFrame = document()->rootFrame();
    FrameStyler *stylerwi = 0;
    QTextCursor c = textCursor();
    bool inlineFrameUnderCursor = false;
    if (c.currentFrame() && c.currentFrame() != RootFrame) {
        inlineFrameUnderCursor = true;
    }

   AbsCommandID BasicActions[] = { FTXTM_UNDO , FTXTM_REDO , FTXTM_SELECTALL , F_SEPARATOR, FTXTM_COPY , FTXTM_CUT , FTXTM_PASTE , F_SUBMENUS , FTXT_BOLD , FTXT_UNDERLINE , FTXT_STRIKOUT , FTXT_OVERLINE , F_SEPARATOR ,  FTXT_FONTS , FTXT_BG_COLOR , FBLOCK_BGCOLOR ,  FTXT_COLOR  ,  F_NONE };
 
 AbsCommandID TablesAction[] = { FTABLE_FORMATS ,  FTABLE_BGCOLOR ,  FTABLE_CELLBGCOLOR , FTABLE_APPENDCOOL , FTABLE_APPENDROW , F_SEPARATOR , FTABLE_REMCOOL , FTABLE_REMROW ,  F_SEPARATOR , FTABLE_MERGECELL , FTABLE_COOLWIDHT  ,  F_NONE };

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



 
 
    for (int j = 0; BasicActions[j] != F_NONE; j++) {
        AbsCommandID id = BasicActions[j];
                 if ( id == F_SEPARATOR) {
                     rootmenu->addSeparator();
                 }
                 if ( id == D_SUBMENUS ) {
                     rootmenu->addSeparator();
                     rootmenu->addAction(MenuTables->menuAction()); 
                     //////rootmenu->addAction(MenuFrame->menuAction()); 
                     rootmenu->addSeparator();
                 }
      QAction* a_1 = CommandStorage::instance()->actF(id);
      if (a_1) {
      rootmenu->addAction(a_1);
      }
    }
   qDebug() << "### contextMenuEvent....";
   rootmenu->exec(QCursor::pos());
   if (inlineFrameUnderCursor) {
    stylerwi->deleteLater();
   }     
   rootmenu->deleteLater();     
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




