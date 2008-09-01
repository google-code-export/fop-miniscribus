#include "Layer_Auto_Extended.h"
#include "Config.h"
#include "Text_Api.h"
#include "Fo_Format.h"
#include "XML_Editor.h"


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
      connect(device, SIGNAL(q_updatelink()),q, SLOT(bookmarkRecord()));
      connect(device, SIGNAL(q_showfopsource()),q, SLOT(showSource()));
        
        
        
    }
    return device;
}


TextLayer::~TextLayer()
{
  //~ qDebug() << "### destroy obj ...";
}



TextLayer::TextLayer( QGraphicsItem *parent  )
    : QGraphicsRectItem(QRectF(0,0,100,100),parent),dev(new TextMount),ContextOpen(false),PageRecords(1),lastEventCursorSend(0)
{
    //////qDebug() << "### init....";
    dev->q = this;
    
    Aheader = 0;
    Afooter = 0;
    Astartreg = 0;
    Aendreg = 0;
    
    setAcceptsHoverEvents(true);
    setAcceptDrops(true);
    
    QTextDocument *dummy = new QTextDocument();
    //////dummy->setHtml(ReadFile("a.html")); /////  
    setDocument(dummy,FOP);
    QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setFlag(QGraphicsItem::ItemIsMovable,false);
    setZValue (0.255555);
    LastRect = dev->txtControl()->boundingRect();
    QGraphicsRectItem::setRect(LastRect);
    LastUpdateRequest = LastRect;
    /////////////SetupHeaderFooter();
    QTimer::singleShot(1, this, SLOT(cursor_wake_up()));
    SetupHeaderFooter();
}


PageDoc TextLayer::binaryPageFormat()
{
    PageDoc o;
    ScribeParser *pari = new ScribeParser(document(),ScribeParser::Psave);
    RichDoc autop = pari->mimeDocument();
    o.Listening.insert(o.Listening.size() + 1 , autop);
    QList<QGraphicsItem *> subLevelItems = childItems();
     for (int i = 0; i < subLevelItems.size(); ++i) { 
               AbsoluteLayer *e = 0;
               e = layer_cast<AbsoluteLayer *>(subLevelItems[i]);
               if (e) {
                 if (e->blockType() == DIV_ABSOLUTE) {
                     RichDoc absoluti = e->mimeDocumentLayer();
                     o.Listening.insert(o.Listening.size() + 1 ,absoluti);
                 }
                 
             }
     }
  return o;
}


QDomDocument *TextLayer::fopPagedoc()
{
    
   ScribeParser *Fop_pars = new ScribeParser(document(),ScribeParser::PFopModus);
   QDomDocument *fop = new QDomDocument();
   fop->setContent(Fop_pars->dom()->toByteArray(1),false);
   QDomElement root = fop->documentElement();
    
    if (root.tagName() !="fo:root")
    {
        return new QDomDocument();
    }
    
    QDomElement master = root.firstChildElement("fo:page-sequence");
    QDomElement page = master.firstChildElement("fo:flow");
    /* append here only floating elements !!!! */
    if (page.isNull()) {
     return new QDomDocument();  
    }
    ///////QDomElement frameElement = fop->createElement("coniglio");
    ///////////page.appendChild(frameElement);
    
    QList<QGraphicsItem *> subLevelItems = childItems();
     for (int i = 0; i < subLevelItems.size(); ++i) { 
               AbsoluteLayer *e = 0;
               e = layer_cast<AbsoluteLayer *>(subLevelItems[i]);
               if (e) {
                 if (e->blockType() == DIV_ABSOLUTE) {
                     QDomDocument in = e->fopDomdoc(); 
                     QDomElement r = in.documentElement();
                     QDomElement frameElement = fop->createElement(r.tagName());
                     QDomNamedNodeMap alist = r.attributes(); 
                                   for (int x=0; x<alist.count(); x++){
                                        QDomNode nod = alist.item(x);
                                        frameElement.setAttribute(nod.nodeName().toLower(),nod.nodeValue());
                                    }  
                       QDomNode child = r.firstChild();
					    while ( !child.isNull() ) {
                            if ( child.isElement() ) {
                            frameElement.appendChild(fop->importNode(child,true).toElement());
                            }
				         child = child.nextSibling();            
				        }
                        
                    /////page.appendChild(frameElement);
                    page.insertBefore(frameElement,page.firstChildElement());
              
                }
              }
          }
    
    
    
    
   return fop;
}


void TextLayer::showSource()
{
    XMLTextEdit *sHtml = new XMLTextEdit(0);
    sHtml->setWindowTitle(tr("Source display on debug auto float...."));
	sHtml->setWindowFlags ( Qt::Window );
	sHtml->setMinimumSize ( 450 , 500 );
    sHtml->setPlainText( fopPagedoc()->toString(1) );
    sHtml->show();
}


void TextLayer::bookmarkRecord()
{
      ScribeParser *parsen = new ScribeParser(document(),ScribeParser::Plinker);
      QStringList internalinks = parsen->internals();
      QStringList cclinks = parsen->destinations();
      delete parsen;
      emit inBookmark(internalinks);
    
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
        childAreaCheck();
        GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
        update(boundingRect());
        sc->clearSelection();
        QTimer::singleShot(50, this, SLOT(SceneReload()));
        QTimer::singleShot(700, this, SLOT(SceneReload()));
}




void TextLayer::SetupHeaderFooter()
{
    
    Astartreg = new AbsoluteLayer(this,DIV_START);
    Astartreg->setDocument(new QTextDocument("Start page"),FOP);
    connect(Astartreg, SIGNAL(close_main_cursor() ),this, SLOT(cursor_stop_it()));
    connect(Astartreg, SIGNAL(pagesize_swap() ),this, SLOT(PageSizeReload()));
    connect(Astartreg, SIGNAL(scene_wakeup() ),this, SLOT(SceneReload()));
    
    
    
    
    
    
    
    
    
    AbsoluteLayer *dummyl = new AbsoluteLayer(0,DIV_ABSOLUTE);
    dummyl->setDocument(new QTextDocument("Dummy make menu....."),FOP);
    dummyl->setZValue (0.000005);
    
    childAreaCheck();
    return;
    
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

   AbsoluteLayer *absolute = new AbsoluteLayer(this,DIV_ABSOLUTE,true);
   absolute->setPos( cursorpos.p1() );
   absolute->setDocument(dummy,FOP);

    connect(absolute, SIGNAL(close_main_cursor() ),this, SLOT(cursor_stop_it()));
    connect(absolute, SIGNAL(pagesize_swap() ),this, SLOT(PageSizeReload()));
    connect(absolute, SIGNAL(scene_wakeup() ),this, SLOT(SceneReload()));
    
    childAreaCheck();
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
    MakeDinamicCommand();
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
   qDebug() << "### stop big area cursor ........ ";
   ///////SceneReload();
   ////////MakeDinamicCommand();  /* redraw action depending cursor reset .... */
   emit autocursorchange(false);  /* absolute works  */
    
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
    ///////SceneReload();
    childAreaCheck();
    emit pageCountChange();
    }
    const uint unixtime = QTime_Null();
    if (lastEventCursorSend == unixtime) {
     return;
    }
    qDebug() << "### send action   -------- " << unixtime;
    /* only each 1000 ms intervall dont stress it from keyboard ! */
    MakeDinamicCommand();  /* redraw action depending cursor */
    childAreaCheck();
    emit autocursorchange(true);
    lastEventCursorSend = unixtime;
}

/* check if absolute layer need a new page document  */
void TextLayer::childAreaCheck()
{
    /////// qDebug() << "### childArea ";
    AbsoluteLayer *itemabsolute = NULL;
    QList<QGraphicsItem *> subLevelItems = childItems();
    qreal lastdown = 400.0; /* A4:2 */
    
          for (int i = 0; i < subLevelItems.size(); ++i) { 
               itemabsolute = layer_cast<AbsoluteLayer *>(subLevelItems[i]);
              if (itemabsolute ) {
              itemabsolute->UpdatePageFormat();
              /* stop cursor */
              itemabsolute->editPermission(false);
              lastdown = qMax (lastdown,itemabsolute->pos().y());
              }
          }
          
          QPointF lastpoint(50.,lastdown);
          ////////qDebug() << "### childArea lastpoint " << lastpoint;
          const QRectF allpageRect = boundingRect();
          if (!allpageRect.contains(lastpoint)) {
            QTextCursor Tcursor = textCursor();
            Tcursor.movePosition(QTextCursor::End);
            Tcursor.beginEditBlock();
            Tcursor.insertText(" \t");
            Tcursor.setBlockFormat(PageBreackBlock());
            Tcursor.endEditBlock();
          }
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
     M_PageSize pax = dev->txtControl()->Model();
     QRectF needarea(areas);
     needarea.setWidth ( areas.width() - pax.body.margin_right - 40 );
     LastVisibleRequest = needarea;
     GraphicsScene *sc;
     if (sc = qobject_cast<GraphicsScene *>(scene())) {
     sc->SetVisibleArea(LastVisibleRequest);
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

    /* draw white first background */
    for (int o = 0; o < PageSumm; ++o)  {
        const QRectF pagen =  dev->txtControl()->Model().PageExternal(o);
        dev->txtControl()->paintStyle(painter,pagen,false);
        dev->txtControl()->paintPage(o,painter);
        painter->save();
        QRectF rightShadow(pagen.right(), pagen.top() + BorderShadow, BorderShadow, pagen.height());
        QRectF bottomShadow(pagen.left() + BorderShadow, pagen.bottom(), pagen.width(), BorderShadow);
        painter->fillRect(rightShadow, Qt::darkGray);
        painter->fillRect(bottomShadow, Qt::darkGray);
        /* small border */
        
        /*
        
       if (o !=0) {
       QPicture headerpaint = Aheader->LayerImage(o);
       QPointF posheader = dev->txtControl()->Model().HeaderInitPoints(o);
       painter->drawPicture(posheader, headerpaint);
       QPicture footerpaint = Afooter->LayerImage(o);
       QPointF posfooter = dev->txtControl()->Model().FooterInitPoints(o);
       painter->drawPicture(posfooter, footerpaint);
       }
       */
      
       painter->restore();
       dev->txtControl()->paintMarginCover(painter,pagen,false);
       
    }
    
    #if defined _LOGGERON_APPS_
    painter->save();
    painter->setPen(Qt::NoPen);
	QColor Visiblerecord(Qt::red);
	Visiblerecord.setAlpha(22);
	painter->setBrush(Visiblerecord);
	painter->drawRect(LastVisibleRequest);  ///////////  LastVisibleRequest  LastUpdateRequest
    painter->restore();
    #endif

    
    
    
    
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
    StaticCommandID DocumentActions[] = {  INSERT_IMAGE , LINK_TEXT , MARGIN_CURRENT_ELEMENT , NEW_LAYER_ABS , SHOW_SOURCE_HTML , SHOW_SOURCE_SCRIBE , SHOW_SOURCE_FOP , PARA_BREACK_PAGE_POLICY ,   S_NONE };
    DynamicCommandID BasicActions[] = { TXTM_UNDO , TXTM_REDO , TXTM_SELECTALL , D_SEPARATOR, TXTM_COPY , TXTM_CUT , TXTM_PASTE , D_SUBMENUS , TXT_BOLD , TXT_ITALIC , D_SEPARATOR  , TXT_UNDERLINE ,
TXT_STRIKOUT , TXT_OVERLINE , FONT_LETTER_SPACING ,TXT_NOBREAKLINE , TXT_SPAN_FONTS , TXT_BG_COLOR , BLOCK_BGCOLOR , TXT_COLOR  ,  D_NONE };
    DynamicCommandID TablesAction[] = { TABLE_FORMATS ,  TABLE_BGCOLOR ,  TABLE_CELLBGCOLOR , TABLE_APPENDCOOL , TABLE_APPENDROW , D_SEPARATOR , TABLE_REMCOOL , TABLE_REMROW ,  D_SEPARATOR , TABLE_MERGECELL , TABLE_COOLWIDHT  ,  D_NONE };
  
  DynamicCommandID BlockActionPara[] = { BLOCK_MARGINS , BLOCK_BGCOLOR , D_SEPARATOR , BLOCK_ALIGN_LEFT , BLOCK_ALIGN_CENTER ,  BLOCK_ALIGN_RIGHT , BLOCK_ALIGN_JUSTIFY , INTERNAL_BOOKMARK ,  D_NONE };
  
  
    
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




///////////snc->registerCommand_S(StaticCmd(NEW_LAYER_NEW,tr("Insert new Absolute Layer"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(Append_Layer())));










    
    
   ////////// qDebug() << "### static count " << snc->countS();
    
    ///////////  StaticCommandID DocumentActions[] = { SHOW_SOURCE_HTML , S_NONE };
    
}

void TextLayer::MakeDinamicCommand()
{
    ApiSession *sx = ApiSession::instance();
    ///////bool canpaste = sx->canmime();
    QTextFrame  *RootFrame = document()->rootFrame();
    bool canpaste = true;
    MakeActionHere();
    
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
  
    bool isitalic = textCursor().charFormat().fontItalic() == true ? true : false;
    
    CommandStorage *dync = CommandStorage::instance();

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
    
    
    dync->registerCommand_D(DinamicCmd(TXT_ITALIC,true,isitalic,tr("Text Italic"),QIcon(":/img/textitalic.png"),QKeySequence("Ctrl+I"),dev->txtControl(),SLOT(ItalicText()),true));
    
    //////dync->registerCommand_F(AbsoluteCmd(FTXT_ITALIC,true,isitalic,tr("Text Italic"),QIcon(":/img/textitalic.png"),QKeySequence("Ctrl+I"),dev->txtControl(),SLOT(ItalicText()),true));
    
    
    
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
    
    
dync->registerCommand_D(DinamicCmd(INTERNAL_BOOKMARK,true,true,tr("Insert iternal PDF BookMark FOP only"),QIcon(":/img/acrobat.png"),QKeySequence(),dev->txtControl(),SLOT(bookMarkIdonBlock()),true));
    
    

   
   
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













