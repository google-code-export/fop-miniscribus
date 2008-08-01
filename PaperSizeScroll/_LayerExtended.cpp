#include "_LayerExtended.h"


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
  qDebug() << "### destroy obj ...";
}



TextLayer::TextLayer( QGraphicsItem *parent  )
    : QGraphicsRectItem(QRectF(0,0,100,100),parent),dev(new TextMount),ContextOpen(false)
{
    qDebug() << "### init....";
    dev->q = this;
	  setAcceptsHoverEvents(true);
    setAcceptDrops(true);
    
    QTextDocument *dummy = new QTextDocument();
	  dummy->setHtml(ReadFile("a.html")); /////  
	  setDocument(dummy,FOP);
    QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setFlag(QGraphicsItem::ItemIsMovable,false);
    setZValue (3.555555);
    
    
    LastRect = dev->txtControl()->boundingRect();
    QGraphicsRectItem::setRect(LastRect);
    LastUpdateRequest = LastRect;
    
}

QTextCursor TextLayer::textCursor() 
{
  return dev->txtControl()->textCursor();
}

void TextLayer::SwapPageModel( M_PageSize e )
{
    dev->txtControl()->SwapPageModel(e);
    SceneReload();
}


QTextDocument *TextLayer::document()
{
  return dev->txtControl()->document();
}
void TextLayer::setDocument( const QTextDocument * document , FileHandlerType Type )
{
    dev->txtControl()->setDocument(document,Type);
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

void TextLayer::cursor_wake_up()
{
    ////const QRectF rectblock = dev->txtControl()->CurrentBlockRect();
    /////updatearea(rectblock.toRect());
    /////////MakeDinamicCommand();
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
    dev->txtControl()->paint(painter,option,widget);
    painter->setPen( Qt::NoPen );
    
    for (int i = 0; i < PageSumm; ++i)  {
        const QRectF pagen =  dev->txtControl()->Model().PageExternal(i);
        QRectF rightShadow(pagen.right(), pagen.top() + BorderShadow, BorderShadow, pagen.height());
        QRectF bottomShadow(pagen.left() + BorderShadow, pagen.bottom(), pagen.width(), BorderShadow);
        
        painter->fillRect(rightShadow, Qt::darkGray);
        painter->fillRect(bottomShadow, Qt::darkGray);
        
        
        painter->setBrush(Qt::NoBrush);
        painter->setPen( QPen(Qt::black,0.3) );
        painter->drawRect(pagen);
        painter->setPen( Qt::NoPen );
        
	  }
    
    
    QColor Visiblerecord(Qt::red);
		Visiblerecord.setAlpha(22);
    painter->setBrush(Visiblerecord);
    painter->drawRect(LastUpdateRequest);
    
    
    
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
    /////////////qDebug() << "### focusInEvent ..." << flags();
    QGraphicsItem::setSelected(true);
    scene()->setFocusItem(this,Qt::ShortcutFocusReason);
    return QGraphicsItem::focusInEvent(event);
}

void TextLayer::focusOutEvent ( QFocusEvent * event ) 
{
   //////////// qDebug() << "### focusOutEvent ...";
    QGraphicsItem::setSelected(false);
    return QGraphicsItem::focusOutEvent(event);
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
      qDebug() << "###  mouseDoubleClickEvent... ";
      if (dev->txtControl()->procesevent(event)) {
      return;
      }
    }
    
    
     return QGraphicsItem::mouseDoubleClickEvent(event);
}


void TextLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "### layer mouseMoveEvent a.. " << event->button() ;  /* no button capture !!!! */
    
    if (dev->txtControl()->AllowedPosition(event->pos())) {
      qDebug() << "### layer mouseMoveEvent b .. ";
      if (dev->txtControl()->procesevent(event)) {
      return;
      }
    }
    return QGraphicsItem::mouseMoveEvent(event);
}

void TextLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  
    
    if (dev->txtControl()->AllowedPosition(event->pos()) && event->button() == Qt::LeftButton ) {
    qDebug() << "### layer mouseReleaseEvent.. left .";
      if (dev->txtControl()->procesevent(event)) {
      return;
      }
    }
    
    return QGraphicsItem::mouseReleaseEvent(event);
}

void TextLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    
    if (dev->txtControl()->AllowedPosition(event->pos()) && event->button() == Qt::LeftButton ) {
        
        qDebug() << "### layer mousePressEvent left ...";
        
       if (dev->txtControl()->procesevent(event)) {
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
    
    qDebug() << "### contextMenuEvent....";
    MakeDinamicCommand();
 
    
    CommandStorage *dync = CommandStorage::instance();
    StaticCommandID DocumentActions[] = { INSERT_IMAGE , SHOW_SOURCE_HTML , PARA_BREACK_PAGE_POLICY , S_NONE };
    DynamicCommandID BasicActions[] = { TXTM_UNDO , TXTM_REDO , TXTM_SELECTALL , D_SEPARATOR, TXTM_COPY , TXTM_CUT , TXTM_PASTE , D_SEPARATOR , TXT_BOLD , TXT_UNDERLINE , TXT_STRIKOUT , TXT_OVERLINE , D_SEPARATOR ,  TXT_FONTS , TXT_BG_COLOR , TXT_COLOR ,  D_NONE };
    
    
    QMenu *rootmenu = new QMenu(event->widget());  
    /* basic menu */
          for (int j = 0; BasicActions[j] != D_NONE; j++) {
			           DynamicCommandID id = BasicActions[j];
                 if ( id == D_SEPARATOR) {
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
	  rootmenu->deleteLater();
    ContextOpen = false;
    
}

void TextLayer::MakeActionHere()
{
    CommandStorage *snc = CommandStorage::instance();
    snc->clearS();
    snc->registerCommand_S(StaticCmd(SHOW_SOURCE_HTML,tr("Show source"),QIcon(":/img/view-source.png"),QKeySequence("Alt+S"),dev->txtControl(),SLOT(showhtml())));
    snc->registerCommand_S(StaticCmd(INSERT_IMAGE,tr("Insert image"),QIcon(":/img/pictures.png"),QKeySequence("Ctrl+J"),dev->txtControl(),SLOT(InsertImageonCursor())));
    
    
    
    snc->registerCommand_S(StaticCmd(PARA_BREACK_PAGE_POLICY,tr("Set Paragraph Page Breack Policy"),QIcon(":/img/wizard.png"),QKeySequence(),dev->txtControl(),SLOT(ParaBlockPageBreackPolicyInsert())));
    
   ////////// qDebug() << "### static count " << snc->countS();
    
    ///////////  StaticCommandID DocumentActions[] = { SHOW_SOURCE_HTML , S_NONE };
    
}

void TextLayer::MakeDinamicCommand()
{
    ApiSession *sx = ApiSession::instance();
    bool canpaste = sx->canmime();
    
    QTextCursor c = textCursor();
    QTextCharFormat fo = c.charFormat();
    QFont f = fo.font();
    bool isbold = textCursor().charFormat().font().bold() == true ? true : false;
    bool isunderline = c.charFormat().underlineStyle() == QTextCharFormat::NoUnderline ? false : true;
    
    
    const QIcon TXTcolorico = createColorToolButtonIcon(":/img/textpointer.png",textCursor().charFormat().foreground().color());
    const QIcon TXTBGcolorico = createColorToolButtonIcon(":/img/textpointer.png",textCursor().charFormat().background().color());
    
    
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
    dync->registerCommand_D(DinamicCmd(TXT_FONTS,false,false,tr("Text Fonts"),QIcon(":/img/textpointer.png"),QKeySequence(),dev->txtControl(),SLOT(FontText()),true));
    
    
    dync->registerCommand_D(DinamicCmd(TXT_BG_COLOR,false,false,tr("Text Fragment Background color"),TXTBGcolorico,QKeySequence(),dev->txtControl(),SLOT(BGcolor()),true));
    dync->registerCommand_D(DinamicCmd(TXT_COLOR,false,false,tr("Text color"),TXTcolorico,QKeySequence(),dev->txtControl(),SLOT(TXcolor()),true));
 
 
   
    
    
   /////////// qDebug() << "### dinamic count " << dync->countD();
    
    
   
    
    
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













