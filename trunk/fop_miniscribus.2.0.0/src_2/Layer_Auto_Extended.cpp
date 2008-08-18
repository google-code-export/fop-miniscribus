#include "Layer_Auto_Extended.h"
#include "Text_Api.h"

TextMount::TextMount()
  : device(0)
{ 

}
/* only one connect */
ScribePage *TextMount::txtControl() const
{
    if (!device) {
       M_PageSize A4;
       TextLayer *that = const_cast<TextLayer *>(q); 
       device = new ScribePage(A4);
        //////////qDebug() << "### init  api txt ...........................................................................";
      connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursor_wake_up()));
      connect(device, SIGNAL(q_update(QRect) ),q, SLOT(updatearea(QRect)));
      connect(device, SIGNAL(q_visible(QRectF) ),q, SLOT(ensureVisible(QRectF)));
      connect(device, SIGNAL(q_update_scene()),q, SLOT(SceneReload()));
    }
    return device;
}


TextLayer::~TextLayer()
{
  //~ qDebug() << "### destroy obj ...";
}



TextLayer::TextLayer( QGraphicsItem *parent  )
    : QGraphicsRectItem(QRectF(0,0,100,100),parent),dev(new TextMount),ContextOpen(false),PageRecords(1)
{
    qDebug() << "### init  auto layer ....";
    dev->q = this;
    Aheader = 0;
    Afooter = 0;
    setAcceptsHoverEvents(true);
    setAcceptDrops(true);
    /////////QTextDocument *dummy = new QTextDocument();
    ////////dummy->setHtml("<p>Hello world text </p>"); /////  
    ////////////////setDocument(dummy,FOP);
    
    ApiSession *sx = ApiSession::instance();
    FoRegion pbody = sx->CurrentPageFormat().body;
    qDebug() << "### init auto layer margin show " << pbody;
    
    
    
    
    QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setFlag(QGraphicsItem::ItemIsMovable,false);
    setZValue (0.555555);
    LastRect = dev->txtControl()->boundingRect();
    setRect(LastRect);
    LastUpdateRequest = LastRect;
    SetupHeaderFooter();
    QTimer::singleShot(0, this, SLOT(cursor_wake_up())); 
}

void TextLayer::appendLayer( QMap<int,RichDoc> floatingelement  )
{
               QMapIterator<int,RichDoc> o(floatingelement);
					while (o.hasNext()) {
                     o.next();
				             RichDoc record = o.value();
						     AbsoluteLayer *absolute = new AbsoluteLayer(this,DIV_ABSOLUTE);
                             absolute->setDocument(record.todoc(),FOP);
                             absolute->setPos( QPointF(10,10) );   //////
                             absolute->setStyle( record.style );
                             connect(absolute, SIGNAL(close_main_cursor() ),this, SLOT(cursor_stop_it()));
                             connect(absolute, SIGNAL(pagesize_swap() ),this, SLOT(PageSizeReload()));
					}
}











void TextLayer::SetupHeaderFooter()
{
    Aheader = new AbsoluteLayer(this,DIV_HEADER);
    Afooter = new AbsoluteLayer(this,DIV_FOOTER);
    QTextDocument *dummy = new QTextDocument();
    dummy->setHtml ( "<p>Header and Logo.</p>" );
    QTextDocument *dummy2 = new QTextDocument();
    dummy2->setHtml ( "<p>Footer Page "+_PAGE_NUMERATION_+"</p>" );
    Aheader->setDocument(dummy,FOP);
    Afooter->setDocument(dummy2,FOP);
    connect(Aheader, SIGNAL(close_main_cursor() ),this, SLOT(cursor_stop_it()));
    connect(Aheader, SIGNAL(pagesize_swap() ),this, SLOT(PageSizeReload()));
    connect(Afooter, SIGNAL(close_main_cursor() ),this, SLOT(cursor_stop_it()));
    connect(Afooter, SIGNAL(pagesize_swap() ),this, SLOT(PageSizeReload()));
}

void TextLayer::Append_Layer()
{

   QTextDocument *dummy = new QTextDocument();
    dummy->setHtml ( "<p>Your text.</p>" );

   QLineF cursorpos = dev->txtControl()->ViewBlinkedCursorLine();

   AbsoluteLayer *absolute = new AbsoluteLayer(this,DIV_ABSOLUTE);
   absolute->setPos( cursorpos.p1() );
   absolute->setDocument(dummy,FOP);

    connect(absolute, SIGNAL(close_main_cursor() ),this, SLOT(cursor_stop_it()));
    connect(absolute, SIGNAL(pagesize_swap() ),this, SLOT(PageSizeReload()));
}


QTextCursor TextLayer::textCursor() 
{
  return dev->txtControl()->textCursor();
}

void TextLayer::SwapPageModel( M_PageSize e )
{
    dev->txtControl()->SwapPageModel(e);
 
    if (Aheader && Afooter) {
      Aheader->UpdatePageFormat();
      Afooter->UpdatePageFormat();
    }

    SceneReload();
}


QTextDocument *TextLayer::document()
{
  return dev->txtControl()->document();
}



void TextLayer::setDocument( const QTextDocument * documentin , FileHandlerType Type )
{
    dev->txtControl()->setDocument(documentin,Type);
    ApiSession *session = ApiSession::instance();
    session->ensureImageDoc(document());
    MakeActionHere();
}

void TextLayer::updatearea( const QRect areas )
{
    LastRect = dev->txtControl()->boundingRect();
    QGraphicsRectItem::setRect(LastRect);
    ////////////qDebug() << "### updatearea " << areas;
    LastUpdateRequest = areas;
    update(areas);
}



void TextLayer::cursor_stop_it()
{
   dev->txtControl()->setBlinkingCursorEnabled(false);
   SceneReload();
}

void TextLayer::PageSizeReload()
{
    ApiSession *sx = ApiSession::instance();
    SwapPageModel( sx->CurrentPageFormat() );
}


void TextLayer::cursor_wake_up()
{
    const int PageSumm = qBound (1,document()->pageCount(),MaximumPages);
    if (PageRecords != PageSumm) {
    PageRecords = PageSumm;
    SceneReload();
    emit pageCountChange();
    }
    MakeDinamicCommand();  /* redraw action depending cursor */
    emit autocursorchange();
}


void TextLayer::SceneReload()
{
     /////////qDebug() << "### SceneReload page count " << document()->pageCount();
     GraphicsScene *sc;
     if (sc = qobject_cast<GraphicsScene *>(scene())) {
     sc->clearSelection();
     }
}

void TextLayer::ensureVisible( const QRectF areas )
{
     LastVisibleRequest = areas;
     GraphicsScene *sc;
     if (sc = qobject_cast<GraphicsScene *>(scene())) {
     sc->SetVisibleArea(areas);
     }
}


QString TextLayer::PageName()
{
    return dev->txtControl()->Model().HName();
}


void TextLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    LastRect = dev->txtControl()->boundingRect();
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen( Qt::NoPen );
    painter->setBrush(Qt::lightGray);
    painter->drawRect(LastRect);
    const int PageSumm = qBound (1,document()->pageCount(),MaximumPages);
    PageRecords = PageSumm;
    
    ApiSession *sx = ApiSession::instance();
    M_PageSize e = sx->CurrentPageFormat();
    QRectF printrectarea = e.PrintArea();
    
    

    /* draw white first background */
    for (int o = 0; o < PageSumm; ++o)  {
        painter->save();
        const QRectF pagen =  dev->txtControl()->Model().PageExternal(o);
        drawPageGround(painter,o,dev->txtControl()->Model());
        dev->txtControl()->DrawPage(o,painter,o);
        drawPageShadow(painter,o,dev->txtControl()->Model());
        /* small border */
        painter->setBrush(Qt::NoBrush);
        painter->setPen( QPen(Qt::black,0.3) );
        painter->drawRect(pagen);
        painter->setPen( Qt::NoPen );
    
        
       if (o !=0) {
       /* draw  header */
       QPicture headerpaint = Aheader->LayerImage(o);
       QPointF posheader = dev->txtControl()->Model().HeaderInitPoints(o);
       painter->drawPicture(posheader, headerpaint);
       /* draw footer */
       QPicture footerpaint = Afooter->LayerImage(o);
       QPointF posfooter = dev->txtControl()->Model().FooterInitPoints(o);
       painter->drawPicture(posfooter, footerpaint);
       }
      
       painter->restore();
    }
    
   
    QColor Visiblerecord(Qt::red);
		Visiblerecord.setAlpha(22);
    painter->setBrush(Visiblerecord);
    painter->drawRect(printrectarea);

    
    
    
   /*
    QColor BackHightlight("#a6ffc7");
		BackHightlight.setAlpha(80);
    
    painter->setPen( Qt::NoPen );
    painter->setBrush(BackHightlight);
		painter->drawRect(LastUpdateRequest);
    
    
     
    QColor Visiblerecord(Qt::red);
		Visiblerecord.setAlpha(45);
    
    if (!LastVisibleRequest.isNull()) {
    painter->setBrush(Visiblerecord);
		painter->drawRect(LastVisibleRequest);
    }
    */
    
}

void TextLayer::focusInEvent ( QFocusEvent * event ) 
{
    //~ qDebug() << "### TextLayer focusInEvent ..." << flags();
    const int PageSumm = qBound (1,document()->pageCount(),MaximumPages);
    if (PageRecords != PageSumm) {
    PageRecords = PageSumm;
    SceneReload();
    emit pageCountChange();
    return;
    }
    QGraphicsItem::setSelected(true);
    scene()->setFocusItem(this,Qt::ShortcutFocusReason);
    dev->txtControl()->setBlinkingCursorEnabled(true);
    cursor_wake_up();
    ///////return QGraphicsItem::focusInEvent(event);
}

void TextLayer::focusOutEvent ( QFocusEvent * event ) 
{
    //~ qDebug() << "### TextLayer focusOutEvent ...";
    QGraphicsItem::setSelected(false);
    dev->txtControl()->setBlinkingCursorEnabled(false);
    //////return QGraphicsItem::focusOutEvent(event);
}

void TextLayer::inputMethodEvent ( QInputMethodEvent * event )  
{
    ///////////qDebug() << "### inputMethodEvent ...";
     return QGraphicsItem::inputMethodEvent(event);
}


QRectF TextLayer::boundingRect() const
{
    return dev->txtControl()->GroupboundingRect();  /* shadow */
}

void TextLayer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
     
     if (dev->txtControl()->AllowedPosition(event->pos()) && event->button() == Qt::LeftButton ) {
     /////////// qDebug() << "###  mouseDoubleClickEvent... ";
      if (dev->txtControl()->procesevent(event)) {
        cursor_wake_up();
        return;
      }
    }
    
    
     return QGraphicsItem::mouseDoubleClickEvent(event);
}


void TextLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ///////////qDebug() << "### layer mouseMoveEvent a.. " << event->button() ;  /* no button capture !!!! */
    
    if (dev->txtControl()->AllowedPosition(event->pos())) {
     //////////// qDebug() << "### layer mouseMoveEvent b .. ";
      if (dev->txtControl()->procesevent(event)) {
      return;
      }
    }
    return QGraphicsItem::mouseMoveEvent(event);
}

void TextLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  
    
    if (dev->txtControl()->AllowedPosition(event->pos()) && event->button() == Qt::LeftButton ) {
    ////////////qDebug() << "### layer mouseReleaseEvent.. left .";
      if (dev->txtControl()->procesevent(event)) {
      return;
      }
    }
    
    return QGraphicsItem::mouseReleaseEvent(event);
}

void TextLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    
    
    if (dev->txtControl()->AllowedPosition(event->pos()) && event->button() == Qt::LeftButton ) {
        
        //////////////qDebug() << "### layer mousePressEvent left ...";
        
       if (dev->txtControl()->procesevent(event)) {
        cursor_wake_up();
        return;
       }
    }
    
    
    return QGraphicsItem::mousePressEvent(event);
}

void TextLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
   /////// qDebug() << "### hoverEnterEvent...";
    return QGraphicsItem::hoverEnterEvent(event);
}

void TextLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //////////qDebug() << "### hoverLeaveEvent...";
    return QGraphicsItem::hoverLeaveEvent(event);
}

void TextLayer::keyPressEvent( QKeyEvent * event ) 
{
   /////////qDebug() << "### keyPressEvent...";
    
  
    
   return dev->txtControl()->Controller_keyPressEvent(event);
   /////return QGraphicsItem::keyPressEvent(event);
}

void TextLayer::keyReleaseEvent ( QKeyEvent * event )
{
    /////////////qDebug() << "### keyReleaseEvent...";
    return dev->txtControl()->Controller_keyReleaseEvent(event);
    //////////return QGraphicsItem::keyReleaseEvent(event);
}


bool TextLayer::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneDragEnter || 
        event->type() == QEvent::GraphicsSceneDrop || 
        event->type() == QEvent::GraphicsSceneDragLeave || 
        event->type() == QEvent::GraphicsSceneDragMove) { 
        dev->txtControl()->procesevent(event);
       return true;
     }
     /* buggi events */
     ///////if (event->type() == QEvent::UngrabMouse ||
         //////event->type() == QEvent::GrabMouse) {
      //////return true;    
     //////}
  return QGraphicsItem::sceneEvent(event);
}

void TextLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextOpen = true;
    QTextFrame  *RootFrame = document()->rootFrame();
    FrameStyler *stylerwi = 0;
    QTextCursor c = textCursor();
    bool inlineFrameUnderCursor = false;
    if (c.currentFrame() && c.currentFrame() != RootFrame) {
        inlineFrameUnderCursor = true;
    }
    
    ////////////qDebug() << "### contextMenuEvent....";
    MakeDinamicCommand();
 
    
    CommandStorage *dync = CommandStorage::instance();
    StaticCommandID DocumentActions[] = {  INSERT_IMAGE , LINK_TEXT , MARGIN_CURRENT_ELEMENT , NEW_LAYER_ABS , SHOW_SOURCE_HTML , SHOW_SOURCE_SCRIBE , SHOW_SOURCE_FOP , PARA_BREACK_PAGE_POLICY , S_NONE };
    DynamicCommandID BasicActions[] = { TXTM_UNDO , TXTM_REDO , TXTM_SELECTALL , D_SEPARATOR, TXTM_COPY , TXTM_CUT , TXTM_PASTE , D_SUBMENUS , TXT_BOLD , TXT_UNDERLINE ,
TXT_STRIKOUT , TXT_OVERLINE , FONT_LETTER_SPACING ,TXT_NOBREAKLINE , TXT_SPAN_FONTS , TXT_BG_COLOR , BLOCK_BGCOLOR , TXT_COLOR  ,  D_NONE };
    DynamicCommandID TablesAction[] = { TABLE_FORMATS ,  TABLE_BGCOLOR ,  TABLE_CELLBGCOLOR , TABLE_APPENDCOOL , TABLE_APPENDROW , D_SEPARATOR , TABLE_REMCOOL , TABLE_REMROW ,  D_SEPARATOR , TABLE_MERGECELL , TABLE_COOLWIDHT  ,  D_NONE };
  
  DynamicCommandID BlockActionPara[] = { BLOCK_MARGINS , BLOCK_BGCOLOR , D_SEPARATOR , BLOCK_ALIGN_LEFT , BLOCK_ALIGN_CENTER ,  BLOCK_ALIGN_RIGHT , BLOCK_ALIGN_JUSTIFY ,  D_NONE };
  
  
    
    QMenu *rootmenu = new QMenu(event->widget());  
    /* basic menu */
    /* table / frame menu */
    
    QMenu *MenuFrame = new QMenu(tr("Frame handler"),rootmenu);
    MenuFrame->setIcon(QIcon(QString::fromUtf8(":/img/frame.png")));
    MenuFrame->addAction(CommandStorage::instance()->actS(INSERT_FRAME));
    MenuFrame->addAction(CommandStorage::instance()->actD(FRAME_BGCOLOR));
    if (inlineFrameUnderCursor) {
    stylerwi = new FrameStyler(c.currentFrame(),event->widget());
    QWidgetAction *widgetmenu = new QWidgetAction(event->widget());
    widgetmenu->setDefaultWidget(stylerwi);
    MenuFrame->addAction(widgetmenu); 
    MenuFrame->addAction(CommandStorage::instance()->actS(MARGIN_CURRENT_ELEMENT));
    }
    
    
    QMenu *MenuTables = new QMenu(tr("Table handler"),rootmenu);
    MenuTables->setIcon(QIcon(QString::fromUtf8(":/img/newtodo.png")));
    MenuTables->addAction(CommandStorage::instance()->actS(INSERT_TABLE));
    for (int w = 0; TablesAction[w] != D_NONE; w++) {
                 DynamicCommandID id = TablesAction[w];
                 if ( id == D_SEPARATOR) {
                    MenuTables->addSeparator();
                 }
                 QAction* t_1 = CommandStorage::instance()->actD(id);
                 if (t_1) {
                 MenuTables->addAction(t_1);
                 }
        
    }
  
  
  QMenu *MenuParagr = new QMenu(tr("TextBlock formats"),rootmenu);
  MenuParagr->setIcon(QIcon(QString::fromUtf8(":/img/document.png")));
  
  for (int o = 0; BlockActionPara[o] != D_NONE; o++) {
                 DynamicCommandID id = BlockActionPara[o];
                 if ( id == D_SEPARATOR) {
                    MenuParagr->addSeparator();
                 }
                 QAction* t_1 = CommandStorage::instance()->actD(id);
                 if (t_1) {
                 MenuParagr->addAction(t_1);
                 }
        
    }
  
  
    
    
    //////rootmenu->addAction(MenuFrame->menuAction()); 
    
    
          for (int j = 0; BasicActions[j] != D_NONE; j++) {
			           DynamicCommandID id = BasicActions[j];
                 if ( id == D_SEPARATOR) {
                     rootmenu->addSeparator();
                 }
                 
                 if ( id == D_SUBMENUS ) {
                     rootmenu->addSeparator();
                     rootmenu->addAction(MenuParagr->menuAction()); 
                     rootmenu->addAction(CommandStorage::instance()->actD(TXT_SPAN_FONTS));
                     rootmenu->addAction(MenuTables->menuAction()); 
                     rootmenu->addAction(MenuFrame->menuAction()); 
                     rootmenu->addSeparator();
                 }
              
              
                 QAction* a_1 = CommandStorage::instance()->actD(id);
						          if (a_1) {
											rootmenu->addAction(a_1);
											}
           }
           
           
           
           rootmenu->addSeparator();
           
           for (int x = 0; DocumentActions[x] != S_NONE; x++) {
			           StaticCommandID id = DocumentActions[x];
                 QAction* a_2 = CommandStorage::instance()->actS(id);
               
                 if ( id == S_SEPARATOR) {
                     rootmenu->addSeparator();
                 }
               
               
						          if (a_2) {
											rootmenu->addAction(a_2);
											}
           }
    
    
    rootmenu->exec(QCursor::pos());
    if (inlineFrameUnderCursor) {
    stylerwi->deleteLater();
    }     
    rootmenu->deleteLater();     
    ContextOpen = false;
    dev->txtControl()->ClearCurrentSelection();
    
}

void TextLayer::MakeActionHere()
{
    CommandStorage *snc = CommandStorage::instance();
    snc->clearS();
    snc->registerCommand_S(StaticCmd(SHOW_SOURCE_HTML,tr("Show source as XHTML"),QIcon(":/img/view-source.png"),QKeySequence("Alt+S"),dev->txtControl(),SLOT(showhtml())));

snc->registerCommand_S(StaticCmd(SHOW_SOURCE_SCRIBE,tr("Show source as Debug"),QIcon(":/img/view-source.png"),QKeySequence(),dev->txtControl(),SLOT(showhtml())));

snc->registerCommand_S(StaticCmd(SHOW_SOURCE_FOP,tr("Show source as XSL_FO"),QIcon(":/img/view-source.png"),QKeySequence(),dev->txtControl(),SLOT(showhtml())));






    snc->registerCommand_S(StaticCmd(INSERT_IMAGE,tr("Insert image"),QIcon(":/img/pictures.png"),QKeySequence("Ctrl+J"),dev->txtControl(),SLOT(InsertImageonCursor())));
    snc->registerCommand_S(StaticCmd(PARA_BREACK_PAGE_POLICY,tr("Set Paragraph/Inline Frame/Table Page Breack Policy"),QIcon(":/img/wizard.png"),QKeySequence(),dev->txtControl(),SLOT(ParaBlockPageBreackPolicyInsert())));
    snc->registerCommand_S(StaticCmd(INSERT_FRAME,tr("Insert inline Frame"),QIcon(":/img/frame.png"),QKeySequence(),dev->txtControl(),SLOT(FosInsertFrame())));
    snc->registerCommand_S(StaticCmd(MARGIN_CURRENT_ELEMENT,tr("Set Margin Paragraph/Table/Inline Frame"),QIcon(":/img/frame.png"),QKeySequence(),dev->txtControl(),SLOT(SetElementMargin())));
    snc->registerCommand_S(StaticCmd(INSERT_TABLE,tr("Insert Table"),QIcon(":/img/table.png"),QKeySequence(),dev->txtControl(),SLOT(CreateanewTable())));

snc->registerCommand_S(StaticCmd(NEW_LAYER_ABS,tr("Insert new Absolute Layer"),QIcon(":/img/frame.png"),QKeySequence(),this,SLOT(Append_Layer())));

snc->registerCommand_S(StaticCmd(LINK_TEXT,tr("Insert Link"),QIcon(":/img/web-48x48.png"),QKeySequence(),dev->txtControl(),SLOT(LinkText())));






    
    
   ////////// qDebug() << "### static count " << snc->countS();
    
    ///////////  StaticCommandID DocumentActions[] = { SHOW_SOURCE_HTML , S_NONE };
    
}

void TextLayer::MakeDinamicCommand()
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
    dync->clearD();

dync->registerCommand_D(DinamicCmd(TXTM_COPY,false,false,tr("Copy"),QIcon(":/img/copy.png"),QKeySequence("Ctrl+C"),dev->txtControl(),SLOT(copy()),textCursor().hasSelection()));
    dync->registerCommand_D(DinamicCmd(TXTM_PASTE,false,false,tr("Paste"),QIcon(":/img/paste.png"),QKeySequence("Ctrl+V"),dev->txtControl(),SLOT(paste()),canpaste));
    dync->registerCommand_D(DinamicCmd(TXTM_CUT,false,false,tr("Cut"),QIcon(":/img/cut.png"),QKeySequence("Ctrl+X"),dev->txtControl(),SLOT(cut()),textCursor().hasSelection()));
    dync->registerCommand_D(DinamicCmd(TXTM_REDO,false,false,tr("Redo"),QIcon(":/img/editredo.png"),QKeySequence("Ctrl+Z"),dev->txtControl(),SLOT(redo()),true)); ///////  document()->isRedoAvailable()
    dync->registerCommand_D(DinamicCmd(TXTM_UNDO,false,false,tr("Undo"),QIcon(":/img/editundo.png"),QKeySequence("Ctrl+Y"),dev->txtControl(),SLOT(undo()),true)); /////document()->isUndoAvailable()
    dync->registerCommand_D(DinamicCmd(TXTM_SELECTALL,false,false,tr("Select All"),QIcon(":/img/new.png"),QKeySequence("Ctrl+A"),dev->txtControl(),SLOT(selectAll()),true));
    
    dync->registerCommand_D(DinamicCmd(TXT_BOLD,true,isbold,tr("Text Bold"),QIcon(":/img/textbold.png"),QKeySequence("Ctrl+B"),dev->txtControl(),SLOT(BoldText()),true));
    dync->registerCommand_D(DinamicCmd(TXT_UNDERLINE,true,isunderline,tr("Text Underline"),QIcon(":/img/textunder.png"),QKeySequence("Ctrl+U"),dev->txtControl(),SLOT(UnderlineText()),true));
    dync->registerCommand_D(DinamicCmd(TXT_STRIKOUT,true,f.strikeOut(),tr("Text Strikeout "),QIcon(":/img/texstrickout.png"),QKeySequence(),dev->txtControl(),SLOT(StrickText()),true));
    dync->registerCommand_D(DinamicCmd(TXT_OVERLINE,true,f.overline(),tr("Text Overline"),QIcon(":/img/texoverline.png"),QKeySequence(),dev->txtControl(),SLOT(OverlineText()),true));
    dync->registerCommand_D(DinamicCmd(TXT_SPAN_FONTS,false,false,tr("Fonts"),QIcon(":/img/textpointer.png"),QKeySequence(),dev->txtControl(),SLOT(FontText()),true));
    
    
    dync->registerCommand_D(DinamicCmd(TXT_BG_COLOR,false,false,tr("Text Fragment Background color"),TXTBGcolorico,QKeySequence(),dev->txtControl(),SLOT(BGcolor()),true));
    
    
     dync->registerCommand_D(DinamicCmd(BLOCK_BGCOLOR,false,false,tr("Paragraph Background color"),BlockBGcolorico,QKeySequence(),dev->txtControl(),SLOT(ParaBGcolor()),true));
    
    
    
    
    
    dync->registerCommand_D(DinamicCmd(TXT_COLOR,false,false,tr("Text color"),TXTcolorico,QKeySequence(),dev->txtControl(),SLOT(TXcolor()),true));
 
     
     /* frame menu separat INSERT_FRAME  static */
    dync->registerCommand_D(DinamicCmd(FRAME_BGCOLOR,false,false,tr("Frame Background color"),createColorIcon(textCursor().currentFrame()->frameFormat().background().color()),QKeySequence(),dev->txtControl(),SLOT(SetFrameBGColor()),inlineFrameUnderCursor));
     
    QColor tableCC = textCursor().currentFrame()->frameFormat().background().color();
    ///////QColor cellCC = textCursor().currentFrame()->frameFormat().background().color();
    dync->registerCommand_D(DinamicCmd(TABLE_FORMATS,false,false,tr("Table Format"),QIcon(":/img/table.png"),QKeySequence(),dev->txtControl(),SLOT(UpdateTableFormat()),istable));
    dync->registerCommand_D(DinamicCmd(TABLE_BGCOLOR,false,false,tr("Table background color"),createColorIcon( tableCC == QColor( Qt::black ) ? QColor( Qt::white ) : tableCC  ),QKeySequence(),dev->txtControl(),SLOT(MaketableColorBG()),istable));
    dync->registerCommand_D(DinamicCmd(TABLE_CELLBGCOLOR,false,false,tr("Cell background color"),createColorIcon(tableCC == QColor( Qt::black ) ? QColor( Qt::white ) : tableCC ),QKeySequence(),dev->txtControl(),SLOT(SetTableCellColor()),istable));
    dync->registerCommand_D(DinamicCmd(TABLE_APPENDCOOL,false,false,tr("Append column"),QIcon(":/img/row_table.png"),QKeySequence(),dev->txtControl(),SLOT(AppendTableCools()),istable));
    dync->registerCommand_D(DinamicCmd(TABLE_APPENDROW,false,false,tr("Append row"),QIcon(":/img/row_table.png"),QKeySequence(),dev->txtControl(),SLOT(AppendTableRows()),istable));
    dync->registerCommand_D(DinamicCmd(TABLE_REMCOOL,false,false,tr("Remove column on cursor"),QIcon(":/img/stop.png"),QKeySequence(),dev->txtControl(),SLOT(RemoveCoolByCursorPosition()),istable));
    dync->registerCommand_D(DinamicCmd(TABLE_REMROW,false,false,tr("Remove row on cursor"),QIcon(":/img/stop.png"),QKeySequence(),dev->txtControl(),SLOT(RemoveRowByCursorPosition()),istable));
    dync->registerCommand_D(DinamicCmd(TABLE_MERGECELL,false,false,tr("Merge selected cell"),QIcon(":/img/reload.png"),QKeySequence(),dev->txtControl(),SLOT(MergeCellByCursorPosition()),istable));
    dync->registerCommand_D(DinamicCmd(TABLE_COOLWIDHT,false,false,tr("Table Column width"),QIcon(":/img/configure.png"),QKeySequence(),dev->txtControl(),SLOT(SetColumLarge()),istable));
    
    
    
    
dync->registerCommand_D(DinamicCmd(BLOCK_ALIGN_LEFT,true,dev->txtControl()->CheckedAlign(BLOCK_ALIGN_LEFT),tr("Text align left"),QIcon(":/img/textleft.png"),QKeySequence(),dev->txtControl(),SLOT(MaketextAlign()),true));

dync->registerCommand_D(DinamicCmd(BLOCK_ALIGN_RIGHT,true,dev->txtControl()->CheckedAlign(BLOCK_ALIGN_RIGHT),tr("Text align right"),QIcon(":/img/textright.png"),QKeySequence(),dev->txtControl(),SLOT(MaketextAlign()),true));


dync->registerCommand_D(DinamicCmd(BLOCK_ALIGN_CENTER,true,dev->txtControl()->CheckedAlign(BLOCK_ALIGN_CENTER),tr("Text align center"),QIcon(":/img/textcenter.png"),QKeySequence(),dev->txtControl(),SLOT(MaketextAlign()),true));

dync->registerCommand_D(DinamicCmd(BLOCK_ALIGN_JUSTIFY,true,dev->txtControl()->CheckedAlign(BLOCK_ALIGN_JUSTIFY),tr("Text align Justify"),QIcon(":/img/textjustify.png"),QKeySequence(),dev->txtControl(),SLOT(MaketextAlign()),true));


dync->registerCommand_D(DinamicCmd(BLOCK_MARGINS,false,false,tr("Paragraph Margin"),QIcon(":/img/document.png"),QKeySequence(),dev->txtControl(),SLOT(SetParaMargin()),true));
    
    

    
    
dync->registerCommand_D(DinamicCmd(FONT_LETTER_SPACING,false,false,tr("Font Letter Spacing"),QIcon(":/img/textpointer.png"),QKeySequence(),dev->txtControl(),SLOT(FontsLetterSpacing()) , true )); 
    
    
bool unbreak = c.blockFormat().nonBreakableLines();


dync->registerCommand_D(DinamicCmd(TXT_NOBREAKLINE,true,unbreak,tr("Set Unbrekable Block"),QIcon(":/img/document.png"),QKeySequence(),dev->txtControl(),SLOT(SwapUnbrekableBlock()),true));
    
    

   
   
 //////////////qDebug() << "### dinamic count " << dync->countD();
    
    
   
    
    
}


/*
void TextLayer::deleteSelected()
{
   dev->txtControl()->deleteSelected(); 
}

void TextLayer::cut()
{
   dev->txtControl()->cut(); 
}
void TextLayer::paste()
{
    dev->txtControl()->paste(); 
}
void TextLayer::copy()
{
    dev->txtControl()->copy(); 
}
void TextLayer::undo()
{
    dev->txtControl()->undo(); 
}
void TextLayer::showhtml()
{
    dev->txtControl()->showhtml(); 
}
void TextLayer::redo()
{
    dev->txtControl()->redo(); 
}
void TextLayer::selectAll()
{
   textCursor().select(QTextCursor::Document);
	 SceneReload();
}
void TextLayer::InsertImageonCursor()
{
    dev->txtControl()->InsertImageonCursor(); 
}

*/













