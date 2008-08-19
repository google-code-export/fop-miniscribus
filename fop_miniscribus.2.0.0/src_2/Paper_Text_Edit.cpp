#include "Paper_Text_Edit.h"
#include "Fo_Reader.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif


#include "Config.h"

using namespace ApacheFop;

GraphicsView::GraphicsView( M_PageSize format , QWidget * parent )
	: QGraphicsView( parent ),
    OnPrintRender(false),BASE_TEXT(new TextLayer(format,0)),
    gzippedfile(false)
{
   QApplication::restoreOverrideCursor();
   recordActionHere();
   qDebug() << "### init  gview .... a";
   QPalette p = palette();
   p.setColor(QPalette::Window,Qt::lightGray);
   p.setColor(QPalette::Base,Qt::lightGray);
   setPalette(p);
   scene = new GraphicsScene(rectToScene(),this);
   setCacheMode(CacheBackground);
   setScene(scene);
   qDebug() << "### init  gview .... b";
    
   scene->addItem(BASE_TEXT);
   connect(scene, SIGNAL(MakeVisible(QRectF)), this, SLOT(viewDisplay(QRectF)));
   connect(BASE_TEXT, SIGNAL(pageCountChange() ), this, SLOT(forceResize()));
   connect(BASE_TEXT, SIGNAL(autocursorchange() ), this, SLOT(cursorChange()));
   connect(BASE_TEXT, SIGNAL(absolutecursorchange() ), this, SLOT(cursorChange()));
   
    qDebug() << "### init  gview .... c";
   ///////////pageclear();
    ///////
    
    /* ony dev time */
  
    /* ony dev time */
    
    /*
      Fo_Reader * fops = new Fo_Reader("ademo.fop");
    const QTextDocument *fopdoc = fops->document()->clone();
    QMap<int,RichDoc> floatingelement = fops->layers();
    qDebug() << "### layers " << floatingelement.size();
    qDebug() << "### init  gview .... d";
    fops->deleteLater(); 
    BASE_TEXT->setDocument(fopdoc);
    BASE_TEXT->appendLayer( floatingelement );
    */
    
    
    qDebug() << "### init  gview .... e";
    /* load document and recalculate the first time */
    ////////////QTimer::singleShot(22, this, SLOT(forceResize()));
    qDebug() << "### init  gview .... z";
}



QRectF GraphicsView::rectToScene() const
{
    
    ApiSession *sx = ApiSession::instance();
    M_PageSize PAGE_MODEL = sx->CurrentPageFormat();
    const QTextDocument *bdoc = BASE_TEXT->document()->clone();
    const int PageSumm = qBound (1,bdoc->pageCount(),MaximumPages);
    const qreal fromTopY = PageSumm * PAGE_MODEL.pageBoundingRect().height();
    const qreal spacepage = (PageSumm - 1) * InterSpace;
    return QRectF(0,0,PAGE_MODEL.pageBoundingRect().width(),fromTopY + spacepage + _BOTTOM_VIEW_SPACE_RESERVE_);
}


void GraphicsView::pageclear()
{
    scene->clear();  /*  remove all item */
    ApiSession *sx = ApiSession::instance();
    M_PageSize format = sx->CurrentPageFormat();
    BASE_TEXT = new TextLayer(format,0);
    scene->addItem(BASE_TEXT);
	connect(scene, SIGNAL(MakeVisible(QRectF)), this, SLOT(viewDisplay(QRectF)));
    connect(BASE_TEXT, SIGNAL(pageCountChange() ), this, SLOT(forceResize()));
    connect(BASE_TEXT, SIGNAL(autocursorchange() ), this, SLOT(cursorChange()));
    connect(BASE_TEXT, SIGNAL(absolutecursorchange() ), this, SLOT(cursorChange()));
    
}

void GraphicsView::cursorChange()
{
    emit sceneSwap();
}


void GraphicsView::recordActionHere()
{
    CommandStorage *snc = CommandStorage::instance();
    snc->recordmainaction(StaticCmd(OPEN_PAGE_CHUNK,tr("Open file"),QIcon(":/img/open-48x48.png"),QKeySequence("CTRL+O"),this,SLOT(openFile())));
    
}

void GraphicsView::openFile()
{
    QString file = QFileDialog::getOpenFileName(this, tr( "Choose a file to open" ), QString(setter.value("LastDir").toString()) , FileFilterHaving() );
    if ( file.isEmpty() ) {
    return;
    }
    
    QFileInfo fi(file);
    setter.setValue("LastDir",fi.absolutePath() +"/");
    openFile( file );
}

void GraphicsView::openFile( const QString file )
{
    pageclear();
    
    QFileInfo fi(file);
    ApiSession *sx = ApiSession::instance();
    const QString dir_ = QDir::currentPath();
    QDir::setCurrent(fi.absolutePath());
    StreamFop *buf = new StreamFop();
    currentopenfilerunning = "";
    gzippedfile = false;
    const QString ext = fi.completeSuffix().toLower();
    currentopenfilealternate = fi.absolutePath() + QString("/") + fi.baseName() + QString(".tmpfox");
    qt_unlink(currentopenfilealternate);
    currentopenfilerunning =  fi.absoluteFilePath();
    
    if (ext == "fo" || ext == "fop" || ext == "fop.gz" || ext == "fo.gz") {
        if (ext == "fop.gz" || ext == "fo.gz") {
        const QByteArray  stream = OpenGzipOneFileByte( fi.absoluteFilePath() );
        buf->device()->write(stream);
        buf->PutOnFile( currentopenfilealternate );
        delete buf;
        gzippedfile = true;
        } else {
        currentopenfilealternate =  fi.absoluteFilePath();
        currentopenfilerunning =  fi.absoluteFilePath();
        }
        if (!isvalidXmlencoding( currentopenfilealternate )) {
        QMessageBox::warning(this, tr("Alert on %1").arg(_APPLICATIONS_NAME_),
                           tr("Sorry file %1 dont having a correct xml encoding on first line!").arg(fi.baseName()));  
        QDir::setCurrent(dir_);
        return;
        }
        currentfilecodec = GetcodecfromXml( currentopenfilealternate  );
        Fo_Reader * fops = new Fo_Reader(currentopenfilealternate);
        const QTextDocument *fopdoc = fops->document()->clone();
        
        QMap<int,RichDoc> floatingelement = fops->layers();
        BASE_TEXT->appendLayer( floatingelement );
        fops->deleteLater(); 
        BASE_TEXT->setDocument(fopdoc);
        forceResize();
        QTimer::singleShot(90, this, SLOT(forceResize())); 
        QDir::setCurrent(dir_);
        return;
    } else if ( ext == "html" || ext == "htm" ) {
        M_PageSize defaultA4Page;
        FoRegion  reghtml;
        reghtml.toAll( MM_TO_POINT(10) );
        defaultA4Page.SetMargin(reghtml);
        /* :-)  html not know is format !!!!!!! */
        buf->LoadFile( currentopenfilerunning );
        const QByteArray chunkhtml = buf->stream();
        delete buf;
        QTextDocument *htmldoc = new QTextDocument();
        htmldoc->setHtml ( QString ( chunkhtml) );
        htmldoc->setTextWidth ( defaultA4Page.pageBoundingRect().width() );
        defaultA4Page.HandlePrint( htmldoc );
        sx->current_Page_Format = defaultA4Page;
        sx->AppendPaper(defaultA4Page);
        BASE_TEXT->setDocument(htmldoc);
        forceResize();
        QTimer::singleShot(90, this, SLOT(forceResize())); 
        QDir::setCurrent(dir_);
    }
    
    QDir::setCurrent(dir_);
}



void GraphicsView::forceResize()
{
    scene->setSceneRect( rectToScene());
    emit NewPageFormatin();
}


void GraphicsView::resizeEvent(QResizeEvent *event)
{
    scene->setSceneRect( rectToScene());
    QGraphicsView::resizeEvent(event);
 
    ///////ApiSession *sx = ApiSession::instance();
    /////const qreal Page_Width = sx->CurrentPageFormat().G_regt.width();
    ////////ensureVisible(QRectF(0, 0, Page_Width, Page_Width / 2));
    ///////////fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
   
}


void GraphicsView::viewDisplay( const QRectF area )
{
    ////////qDebug() << "### GraphicsView::ViewDisplay -------- " << area;
    /* if scale to big return */
    QMatrix matx = matrix();
    qreal HHscaled = matx.m11();
    if (HHscaled > 1.4333333) {
    return;
    }
    QGraphicsView::ensureVisible(area);
}



void GraphicsView::PrintSetup( bool printok )
{
    OnPrintRender = printok;
    update();
}

void GraphicsView::swapPaper()
{
   const QRectF rectscene = rectToScene();
   scene->setSceneRect( rectscene );
   ApiSession *sx = ApiSession::instance();
   M_PageSize PAGE_MODEL = sx->CurrentPageFormat();
   BASE_TEXT->swapPageModel(PAGE_MODEL);
}



void GraphicsView::keyPressEvent(QKeyEvent *e )
{
  QGraphicsView::keyPressEvent(e);
}


QRectF GraphicsView::boundingRect()
{
	return scene->sceneRect();
}

void GraphicsView::DisplayTop()
{
	horizontalScrollBar()->setValue(0); 
	verticalScrollBar()->setValue(0);
}



GraphicsView::~GraphicsView()
{
	pageclear();   /* clear all item */
  /* delete session */
  ////SessionManager *sx = SessionManager::instance();
  /////sx->~SessionManager();
}



Panel::Panel( QWidget *parent)
    : QFrame(parent),tievents(0),NotPaperUpdate(true)
{
    setFrameStyle(Sunken | StyledPanel);
    
    ApiSession *sx = ApiSession::instance();
    M_PageSize format = sx->CurrentPageFormat();
    graphicsView = new GraphicsView(format,this);
    graphicsView->setObjectName(QString("graphicsView"));
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    /* first call graphicsView to become all action */
  
    
    
    
    
    setMinimumSize(750,508);
    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);
    
     // top menu 
    QHBoxLayout *labelLayout = new QHBoxLayout;
    openGlButton = new QToolButton;
    openGlButton->setText(tr("OpenGL"));
    openGlButton->setCheckable(true);
    
    
    labelLayout->addWidget(openGlButton);
    labelLayout->addStretch();
    
    PortraitPaper = new QComboBox;
    /////LandscapePaper = new QComboBox;
    labelLayout->addWidget(PortraitPaper);
    FillPaperSize();
    

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/img/viewzoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/img/viewzoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(400);
    zoomSlider->setValue(DefaultStartZoom);
    zoomSlider->setTickPosition(QSlider::TicksRight);
		
		resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);
		
		
		// Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    zoomSliderLayout->addWidget(resetButton);
		
		QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    setLayout(topLayout);
		connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
		connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
		connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
		connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
		///////connect(graphicsView, SIGNAL(SceneSwap()), this, SLOT(SceneChange()));
    connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
    connect(PortraitPaper, SIGNAL(currentIndexChanged(int)), this, SLOT(PaperSwap(int)));
    connect(graphicsView, SIGNAL(NewPageFormatin()),this, SLOT(FillPaperSize()));
    
    
    
   resetView();
   QTimer::singleShot(10, this, SLOT(DisplayTop()));  
   #ifndef OPENGLINITYES
   openGlButton->setChecked ( true );
   toggleOpenGL();
   #endif
    
    
}

void Panel::PaperSwap( const int index )
{
   if (NotPaperUpdate) {
    return;
   }
   const int PaperNr = PortraitPaper->itemData(index).toInt();
   ///////qDebug() << "### PaperSwap  " << index;
   ApiSession *sx = ApiSession::instance();
   QMap<int,M_PageSize> allpaper = sx->mpages();
   M_PageSize OtherFormat = allpaper[PaperNr];
   sx->SetPageFormat(OtherFormat);
   qDebug() << "### PaperSwap  " << OtherFormat.name;
   graphicsView->swapPaper();
}

void Panel::FillPaperSize()
{
   NotPaperUpdate = true;
   ApiSession *sx = ApiSession::instance();
  ///////////// QMap<int,M_PageSize> allpaper = ;
   PortraitPaper->clear();
   M_PageSize activeformat = sx->CurrentPageFormat();  /* user define or other */
   /* to index */
   const QString currentname = activeformat.HName();
   int posi = -1;
   int activeindex = 0;
 
     QMapIterator<int,M_PageSize> i(sx->mpages());
         while (i.hasNext()) {
             i.next();
             posi++;
             M_PageSize current = i.value();
             if (currentname == current.HName()) {
              activeindex = posi;
             }
             PortraitPaper->addItem(QIcon(":/img/page.png"),current.HName(),i.key());
         }
    PortraitPaper->setCurrentIndex ( activeindex );
    ///////PortraitPaper = new QComboBox;
   //////////LandscapePaper = new QComboBox;
   graphicsView->swapPaper();
   NotPaperUpdate = false;
   
}


void Panel::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}


void Panel::SceneChange()
{
	  tievents++;
}

void Panel::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void Panel::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void Panel::resetView()
{
 
    ApiSession *sx = ApiSession::instance();
	  const qreal Page_Width = sx->CurrentPageFormat().G_regt.width();
    zoomSlider->setValue(DefaultStartZoom);
    setupMatrix();
    ///////graphicsView->ensureVisible(QRectF(0, 0, Page_Width,10));
    resetButton->setEnabled(false);
    ////////////////graphicsView->fitInView(graphicsView->scene->sceneRect(), Qt::KeepAspectRatio);
}



void Panel::setupMatrix()
{
 
    ///////////////////qDebug() << "### setupMatrix " << zoomSlider->value();
    qreal scale = ::pow(2.0, (zoomSlider->value() - DefaultStartZoom) / 50.0);
    QMatrix matrix;
    matrix.scale(scale, scale);
    ///////matrix.rotate(10);
    graphicsView->setMatrix(matrix);
    setResetButtonEnabled();
    graphicsView->forceResize();
}

void Panel::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}


GraphicsView *Panel::view() const
{
	return graphicsView;
}


void Panel::DisplayTop()
{
	graphicsView->DisplayTop();
}







PaperTextEdit::PaperTextEdit( QWidget *parent )
  : QMainWindow(parent)
{
    panel = new Panel(this);
    setCentralWidget (panel);
    bool maketool = true;
    
    
    
    if (maketool) {
    docbar = new QToolBar(this);
    docbar->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));
    
    tb_0 = new QToolBar(this);
    tb_0->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));   /* undo redo action from  auto  */
    tb_1 = new QToolBar(this);
    tb_1->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));   /* paragraph block edita auto */
    tb_2 = new QToolBar(this);
    tb_2->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));    /* table action auto */
    tb_3 = new QToolBar(this);
    tb_3->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));
    tb_4 = new QToolBar(this);
    tb_4->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));
    tb_5 = new QToolBar(this);
    tb_5->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));
    
    CommandStorage *dync = CommandStorage::instance();
    
    addToolBar(Qt::TopToolBarArea,docbar);
    docbar->addAction(dync->actM(OPEN_PAGE_CHUNK));
    
      
    
    
    addToolBar(Qt::TopToolBarArea,tb_0);
    addToolBar(Qt::TopToolBarArea,tb_1);
    addToolBarBreak( Qt::TopToolBarArea );
    addToolBar(Qt::TopToolBarArea,tb_3);
    addToolBar(Qt::LeftToolBarArea,tb_2);
    addToolBar(Qt::LeftToolBarArea,tb_4);
    addToolBar(Qt::LeftToolBarArea,tb_5);
    
    
    StaticCommandID DocumentActions[] = {  INSERT_IMAGE , LINK_TEXT , MARGIN_CURRENT_ELEMENT , NEW_LAYER_ABS , SHOW_SOURCE_HTML , SHOW_SOURCE_SCRIBE , SHOW_SOURCE_FOP , PARA_BREACK_PAGE_POLICY , S_NONE };
         for (int x = 0; DocumentActions[x] != S_NONE; x++) {
                 StaticCommandID id = DocumentActions[x];
                 QAction* a_2 = CommandStorage::instance()->actS(id);
                 if ( id == S_SEPARATOR) {
                 tb_3->addSeparator();
                 }
                 if (a_2) {
                 tb_3->addAction(a_2);
                }
           }
           
           
     
    connect(panel->view()->autopage(), SIGNAL(autocursorchange()),this, SLOT(auto_page_dinamic()));
    }
}


void PaperTextEdit::auto_page_dinamic()
{
    ///////qDebug() << "### auto_page_dinamic cursor swap ....";
    CommandStorage *dync = CommandStorage::instance();
    DynamicCommandID BasicActions[] = { TXTM_UNDO , TXTM_REDO , TXTM_SELECTALL , D_SEPARATOR, TXTM_COPY , TXTM_CUT , TXTM_PASTE , D_SUBMENUS , TXT_BOLD , TXT_UNDERLINE ,
TXT_STRIKOUT , TXT_OVERLINE , FONT_LETTER_SPACING ,TXT_NOBREAKLINE , TXT_SPAN_FONTS , TXT_BG_COLOR , BLOCK_BGCOLOR , TXT_COLOR  ,  D_NONE };
    DynamicCommandID TablesAction[] = { TABLE_FORMATS ,  TABLE_BGCOLOR ,  TABLE_CELLBGCOLOR , TABLE_APPENDCOOL , TABLE_APPENDROW , D_SEPARATOR , TABLE_REMCOOL , TABLE_REMROW ,  D_SEPARATOR , TABLE_MERGECELL , TABLE_COOLWIDHT  ,  D_NONE };
    DynamicCommandID BlockActionPara[] = { BLOCK_MARGINS , BLOCK_BGCOLOR , D_SEPARATOR , BLOCK_ALIGN_LEFT , BLOCK_ALIGN_CENTER ,  BLOCK_ALIGN_RIGHT , BLOCK_ALIGN_JUSTIFY ,  D_NONE };
    
    tb_0->clear();
    tb_1->clear();
    tb_2->clear();
    
    
    tb_2->addAction(CommandStorage::instance()->actS(INSERT_TABLE));
    
     for (int j = 0; BasicActions[j] != D_NONE; j++) {
          DynamicCommandID id = BasicActions[j];     
          QAction* a_1 = CommandStorage::instance()->actD(id);
          if (a_1) {
          tb_0->addAction(a_1);
          }
     }
     for (int o = 0; BlockActionPara[o] != D_NONE; o++) {
                 DynamicCommandID id = BlockActionPara[o];
                 QAction* t_2 = CommandStorage::instance()->actD(id);
                 if (t_2) {
                 tb_1->addAction(t_2);
                 }
        
    }
    for (int w = 0; TablesAction[w] != D_NONE; w++) {
                 DynamicCommandID id = TablesAction[w];
                 QAction* t_3 = CommandStorage::instance()->actD(id);
                 if (t_3) {
                 tb_2->addAction(t_3);
                 }
        
    }
    
}








PaperTextEdit::~PaperTextEdit()
{
    
}







