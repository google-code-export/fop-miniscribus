#include "MainWindow.h"
#include "PageFormatDlg.h"
#include "Fo_Format.h"
#include "Fo_Reader.h"
#include "Fo_Writter.h"
#include "oo_handler.h"
#include "ziphandle.h"   ////// open file gzip 

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif

using namespace ApacheFop;
using namespace OpenOffice;
using namespace OOO;


#include <QCloseEvent>


MainWindow::MainWindow( QWidget* parent )
	: QMainWindow( parent ),barevents(0)
{
	setupUi( this );
    setWindowTitle(_APPLICATIONS_NAME_);
    CheckFileApps();
    setMinimumSize(900,480);
    edit = new  PaperEditor(this);
    setCentralWidget(edit);
    prepareDocks();
    
    resize( setter.value( "Main/size", QSize( 900,480 )).toSize() );
    move( setter.value( "Main/pos", QPoint( 10, 10 )).toPoint( ) );   
}

void MainWindow::resizeEvent ( QResizeEvent * e )
{
   setter.setValue( "Main/size", size() );
   setter.setValue( "Main/pos", pos() );
   QMainWindow::resizeEvent(e);
}

void MainWindow::CheckFileApps()
{
    Cache(_APPSCACHE_);
    QFileInfo fi(_OOOV1FILE_);
    if (!fi.exists()) {
        /* user can edit style and save on apps cache */
        StreamFop *buf = new StreamFop();
        buf->LoadFile(":/file/recfile/sxw2xhtml.xsl");
        buf->PutOnFile(_OOOV1FILE_);
        delete buf;
    }
    QFileInfo fi2(_OOOV2FILE_);
    if (!fi2.exists()) {
        /* user can edit style and save on apps cache */
        StreamFop *buf = new StreamFop();
        buf->LoadFile(":/file/recfile/odt2html.xsl");
        buf->PutOnFile(_OOOV1FILE_);
        delete buf;
    } 
    
}

void MainWindow::linkExternal()
{
    QAction *ali = qobject_cast<QAction *>(sender());
    if (!ali) {
    return;
    }
    const int a = ali->data().toInt();
    if (a == 9000) {
    OpenDeskBrowser(QUrl("http://code.google.com/p/fop-miniscribus/"));  
    } else if ( a == 9001) {
    OpenDeskBrowser(QUrl("http://ciz.ch/"));  
    } else if ( a == 9002) {
    OpenDeskBrowser(QUrl("http://www.zvon.org/xxl/xslfoReference/Output/index.html"));  
    } else if ( a == 9003) {
    OpenDeskBrowser(QUrl("http://trolltech.com/"));  
    } else if ( a == 9004) {
    OpenDeskBrowser(QUrl("http://www.qtcentre.org/"));  
    } else if ( a == 9005) {
    OpenDeskBrowser(QUrl("http://qtforum.de/forum/"));  
    } else if ( a == 9006) {
    OpenDeskBrowser(QUrl("http://xmlgraphics.apache.org/fop/"));  
    }  else if ( a == 9007) {
    OpenDeskBrowser(QUrl("http://www.java.com/"));  
    }  else if ( a == 9008) {
    OpenDeskBrowser(QUrl("http://pages.cs.wisc.edu/~ghost/"));  
    }  else if ( a == 9009) {
    OpenDeskBrowser(QUrl("http://code.google.com/p/fop-miniscribus/wiki/KeyBoard"));  
    }  else if ( a == 9010) {
    OpenDeskBrowser(QUrl("http://sourceforge.net/project/project_donations.php?group_id=202496")); 
    }  else if ( a == 9011) {
    OpenDeskBrowser(QUrl("http://code.google.com/p/fop-miniscribus/issues/list")); 
    } 
}

/*
7777
LINK_GOOGLEBASE = 9000,
LINK_AUTHOR = 9001,
LINK_XSLDOC = 9002,
LINK_TROLLTECH = 9003,
LINK_QTFORUMEN = 9004,
LINK_QTFORUMDE = 9005,
LINK_APACHEFOP = 9006,
LINK_JAVA = 9007,
LINK_GHOSTPS = 9008,  
LINK_KEYBOARDOC= 9008,
  */

void MainWindow::prepareDocks()
{

    
/* bookmark manager */
pdfBookMarkManager = new QDockWidget(this);
treeBooks = new BookTree(pdfBookMarkManager);
pdfBookMarkManager->setWidget(treeBooks);
pdfBookMarkManager->setAllowedAreas( Qt::LeftDockWidgetArea |  Qt::RightDockWidgetArea);
addDockWidget(Qt::LeftDockWidgetArea,pdfBookMarkManager); 
pdfBookMarkManager->setWindowTitle(tr("PDF BookMark"));
pdfBookMarkManager->hide();
    
    
actioBooks = pdfBookMarkManager->toggleViewAction();
actioBooks->setIcon ( QIcon(":/img/document.png") );
actioBooks->setText(tr("BookMark on document"));

tbooks = new QToolButton;
tbooks->setDefaultAction (actioBooks);
tbooks->setCheckable(true);
    
toolBar->addWidget(tbooks);
/* bookmark manager */
    toolBar->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));   /* ui action */
    
    docbar = new QToolBar(this);
    docbar->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));
    
    tb_0 = new QToolBar(this);
    tb_0->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));   /* undo redo action from  auto  */
    tb_1 = new QToolBar(this);
    tb_1->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));   /* paragraph block edita auto */
    tb_2 = new QToolBar(this);
    tb_2->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));    /* table action auto */
    tb_3 = new QToolBar(this);
    tb_3->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));    /* static action never change */
    tb_4 = new QToolBar(this);
    tb_4->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));
    tb_5 = new QToolBar(this);
    tb_5->setIconSize(QSize(_MAINICONSIZE_,_MAINICONSIZE_));
    /* wake up on toolbar hover !!!!! */
    toolse << tb_0 << tb_1 << tb_2 << tb_3 << tb_4 << tb_5 << docbar;
    
    addToolBar(Qt::TopToolBarArea,docbar);
    ///////docbar->addAction(dync->actM(OPEN_PAGE_CHUNK));
    addToolBar(Qt::TopToolBarArea,tb_0);
    addToolBar(Qt::TopToolBarArea,tb_1);
    addToolBarBreak( Qt::TopToolBarArea );
    
    addToolBar(Qt::TopToolBarArea,tb_5);   /* absolute layer  */
    addToolBar(Qt::LeftToolBarArea,tb_3);
    addToolBar(Qt::LeftToolBarArea,tb_2);
    addToolBar(Qt::LeftToolBarArea,tb_4);
    
    
    CommandStorage *snc = CommandStorage::instance();
    snc->registerCommand_S(StaticCmd(LINK_GOOGLEBASE,tr("Check update"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_AUTHOR,tr("Author Home Page"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_XSLDOC,tr("XSL-FO Reference"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_TROLLTECH,tr("QT Trolltech web"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_QTFORUMEN,tr("QT Forum en qtcentre.org"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_QTFORUMDE,tr("QT Forum de"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_JAVA,tr("Java Home Page"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_APACHEFOP,tr("Apache fop Page"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_GHOSTPS,tr("Ghostscript Page"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_KEYBOARDOC,tr("Application KeyBoard shortcut"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_SUPPORT,tr("Support and Help to MiniScribus"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));
    snc->registerCommand_S(StaticCmd(LINK_BUG_ISSEUE,tr("Report a bug (require Gmail user)"),QIcon(":/img/web-48x48.png"),QKeySequence(),this,SLOT(linkExternal())));




    StaticCommandID LinkActionHelp[] = {  LINK_GOOGLEBASE , LINK_BUG_ISSEUE , LINK_SUPPORT , LINK_KEYBOARDOC , LINK_AUTHOR , LINK_XSLDOC , LINK_TROLLTECH , LINK_QTFORUMEN , LINK_QTFORUMDE , LINK_APACHEFOP , LINK_JAVA , LINK_GHOSTPS , S_NONE };
         
    for (int x = 0; LinkActionHelp[x] != S_NONE; x++) {
                 StaticCommandID id = LinkActionHelp[x];
                 QAction* a_2 = CommandStorage::instance()->actS(id);
                 if ( id == S_SEPARATOR) {
                 menuExternal_doc_link->addSeparator();
                 }
                 if (a_2) {
                 menuExternal_doc_link->addAction(a_2);
                }
           }

    
    
    
/*
LINK_GOOGLEBASE = 9000,
LINK_AUTHOR = 9001,
LINK_XSLDOC = 9002,
LINK_TROLLTECH = 9003,
LINK_QTFORUMEN = 9004,
LINK_QTFORUMDE = 9005,
LINK_APACHEFOP = 9006,
LINK_JAVA = 9007,
LINK_GHOSTPS = 9008,
  */







    docbar->addAction(snc->actM(OPEN_PAGE_CHUNK));
    tb_5->addAction(snc->actM(OPEN_PAGE_CHUNK));
    
      
    
    menuUpdate(true); /* insert first dinamic action */
           
           
    connect(edit->view(), SIGNAL(inBookmark(QStringList)),treeBooks, 
            SLOT(incommingBookmark(QStringList)));
    connect(edit->view(), SIGNAL(inBookmark(QStringList,QStandardItemModel*)),treeBooks, 
           SLOT(incommingBookmark(QStringList,QStandardItemModel*)));
    connect(edit->view(), SIGNAL(sceneSwap(bool)),this, SLOT(menuUpdate(bool)));
    connect(convertFop, SIGNAL(triggered()),edit->view(), SLOT(apacheFopConvert()));
    connect(savescribePage, SIGNAL(triggered()),edit->view(), SLOT(saveOnPageBinFile()));
    connect(openScribe, SIGNAL(triggered()),edit->view(), SLOT(openFile()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(edit->view(), SIGNAL(fileBaseOpen(QString)),this, SLOT(setWindowTitle(QString)));
    connect(actionNewDoc, SIGNAL(triggered()),edit->view(), SLOT(pageclear()));
    connect(edit->view(), SIGNAL(bookMarkActive(bool)),this, SLOT(showBooks(bool)));
    
    connect(savertf, SIGNAL(triggered()),edit->view(), SLOT(saveRtfDoc()));
    connect(savetiff, SIGNAL(triggered()),edit->view(), SLOT(saveTiffDoc()));
    connect(printfile, SIGNAL(triggered()),edit->view(), SLOT(printPreview()));
    
    
     for (int i = 0; i < toolse.size(); ++i) {
         toolse[i]->installEventFilter(this);  
     }
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    barevents++;
    
      if (event->type() == QEvent::ContextMenu) {
        return true;
      } 
      if (event->type() == QEvent::HoverMove) {
             QToolBar *nowlive = qobject_cast<QToolBar*>(obj);
             if (nowlive) {
                  /* create unixtime hash */
                 if (nowlive->isEnabled()) {
                     /////////qDebug() << "### menuUpdate  enable -> " << barevents;
                 } else {
                     ///////qDebug() << "### menuUpdate  disable -> " << barevents;
                 }
             }
      }
    return false;
}

/* true on auto - false on absolute */
void MainWindow::menuUpdate( bool modus )
{
    ///////////  tb_3  never remake is static command 
    ////////qDebug() << "### menuUpdate   ";
    if (modus) {
    tb_0->setDisabled ( false );
    tb_1->setDisabled ( false );
    tb_2->setDisabled ( false );
    tb_3->setDisabled ( false );
    tb_5->setDisabled ( true );
    tb_0->clear();
    tb_1->clear();
    tb_2->clear();
    tb_3->clear();
    
    CommandStorage *dync = CommandStorage::instance();
    
     StaticCommandID DocumentActions[] = {  NEW_LAYER_ABS , INSERT_IMAGE , LINK_TEXT , MARGIN_CURRENT_ELEMENT  , SHOW_SOURCE_HTML , SHOW_SOURCE_SCRIBE , SHOW_SOURCE_FOP , PARA_BREACK_PAGE_POLICY , S_NONE };
     ///////StaticCommandID DocumentActions[] = {  NEW_LAYER_ABS , INSERT_IMAGE , LINK_TEXT , MARGIN_CURRENT_ELEMENT , NEW_LAYER_ABS , SHOW_SOURCE_HTML , SHOW_SOURCE_SCRIBE , SHOW_SOURCE_FOP , PARA_BREACK_PAGE_POLICY ,  S_NONE };
     
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
    
    
    
    
    
    DynamicCommandID BasicActions[] = { TXTM_UNDO , TXTM_REDO , TXTM_SELECTALL , D_SEPARATOR, TXTM_COPY , TXTM_CUT , TXTM_PASTE , D_SUBMENUS , TXT_BOLD , TXT_UNDERLINE , TXT_ITALIC ,
TXT_STRIKOUT , TXT_OVERLINE , FONT_LETTER_SPACING ,TXT_NOBREAKLINE , TXT_SPAN_FONTS , TXT_BG_COLOR , BLOCK_BGCOLOR , TXT_COLOR  ,  D_NONE };
    DynamicCommandID TablesAction[] = { TABLE_FORMATS ,  TABLE_BGCOLOR ,  TABLE_CELLBGCOLOR , TABLE_APPENDCOOL , TABLE_APPENDROW , D_SEPARATOR , TABLE_REMCOOL , TABLE_REMROW ,  D_SEPARATOR , TABLE_MERGECELL , TABLE_COOLWIDHT  ,  D_NONE };
    DynamicCommandID BlockActionPara[] = { BLOCK_MARGINS , BLOCK_BGCOLOR , D_SEPARATOR , BLOCK_ALIGN_LEFT , BLOCK_ALIGN_CENTER ,  BLOCK_ALIGN_RIGHT , BLOCK_ALIGN_JUSTIFY , INTERNAL_BOOKMARK ,  D_NONE };
    
    
    
    tb_0->addWidget ( new QLabel(tr("Free-floating txt:")) );
    
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
    
    
   } else {
    tb_5->setDisabled ( false );
    tb_0->setDisabled ( true );
    tb_1->setDisabled ( true );
    tb_2->setDisabled ( true );
    tb_3->setDisabled ( true );  
   }
   
   menuAbsoluteLayer();
   
}


void MainWindow::menuAbsoluteLayer()
{
    
    tb_5->clear();
    tb_5->addWidget ( new QLabel(tr("Absolute txt Layer:")) );
   
   AbsCommandID BasicActions[] = { FTXTM_UNDO , FTXTM_REDO , FTXTM_SELECTALL , F_SEPARATOR, FTXTM_COPY , FTXTM_CUT , FTXTM_PASTE , F_SUBMENUS , FTXT_BOLD , FTXT_UNDERLINE , FTXT_ITALIC , FTXT_STRIKOUT , FTXT_OVERLINE , FLINK_TEXT , FTXT_NOBREAKLINE , FFONT_LETTER_SPACING , F_SEPARATOR ,  FTXT_FONTS , FTXT_BG_COLOR , FBLOCK_BGCOLOR , FLAYER_BG , FTXT_COLOR , ZINDEX_MIN , ZINDEX_MAX , F_REMLAYER , PLAY_SOURCE_LAYER , F_SEPARATOR  , F_NONE };


 for (int j = 0; BasicActions[j] != F_NONE; j++) {
        AbsCommandID id = BasicActions[j];
            if ( id == F_SEPARATOR) {
                tb_5->addSeparator();
            }
            
      QAction* a_1 = CommandStorage::instance()->actF(id);
      if (a_1) {
      tb_5->addAction(a_1);
      }
    }
    
}


void MainWindow::showBooks( bool e )
{
    tbooks-> setChecked ( e );
    if (e) {
     if (!pdfBookMarkManager->isFloating ()) {
         pdfBookMarkManager->setFloating (true);
         pdfBookMarkManager->show();
         pdfBookMarkManager->setFocus ( Qt::PopupFocusReason );
     }
    } else {
        pdfBookMarkManager->hide();
    }
}

void MainWindow::setFile( const QString file , const QString option )
{
    edit->view()->openFile(file);
    /* option print or other here timer */
    
}



void MainWindow::on_actionGenralSave_triggered()
{
    edit->view()->saveOnFile(); 
}

void MainWindow::on_actionSaveAs_triggered()
{
    edit->view()->saveAsFile(); 
}




void MainWindow::on_pagereformat_triggered()
{
    ApiSession *sx = ApiSession::instance();
    M_PageSize beforeFF = sx->CurrentPageFormat();
    PageFormatDlg::self( 0 )->fillFormat( sx->CurrentPageFormat()  );
    int rec = PageFormatDlg::self( 0 )->exec();
    if (rec == 1) {
    /* new page format wake up */
    ////////qDebug() << "### responder   " << rec;
    sx->SetPageFormat( PageFormatDlg::self( 0 )->format() );
    edit->view()->forceResizeExtern();  
    } else {
    sx->SetPageFormat( beforeFF );
    }
    
    delete PageFormatDlg::self( 0 );
}


GraphicsView::GraphicsView( QWidget* parent )
	:QGraphicsView( parent ),pageFull(new TextLayer(0))
{
    
    #if defined Q_WS_WIN
    foptipe = "Bat file";
    #endif
    #if defined Q_WS_X11
     foptipe = "sh script";
    #endif
    #if defined Q_WS_MAC
    foptipe = "sh script";
    #endif
    
   QPalette p = palette();
   p.setColor(QPalette::Window,Qt::lightGray);
   p.setColor(QPalette::Base,Qt::lightGray);
   setPalette(p);
   scene = new GraphicsScene(rectToScene(),this);
   scene->setObjectName(QString::fromUtf8("playground"));
   setCacheMode(CacheBackground);
   setScene(scene);
   scene->setSceneRect( rectToScene());
   scene->addItem(pageFull);
    
    CommandStorage *snc = CommandStorage::instance();
    snc->recordmainaction(StaticCmd(OPEN_PAGE_CHUNK,tr("Open file"),QIcon(":/img/open-48x48.png"),QKeySequence("CTRL+O"),this,SLOT(openFile())));
    
    scene->installEventFilter(this);
    
	connect(scene, SIGNAL(MakeVisible(QRectF)), this, SLOT(viewDisplay(QRectF)));
    connect(pageFull, SIGNAL(pageCountChange() ), this, SLOT(forceResizeIntern()));
    connect(pageFull, SIGNAL(autocursorchange(bool) ), this, SLOT(cursorChange(bool)));
    connect(pageFull, SIGNAL(inBookmark(QStringList)),this, SLOT(sendinBookmark(QStringList)));
    
    QTimer::singleShot(5, this, SLOT(DisplayTop()));
    QTimer::singleShot(50, this, SLOT(openMasterMainFile()));
}

bool GraphicsView::eventFilter(QObject *obj, QEvent *e)
 {
     if (obj->objectName() != QString::fromUtf8("playground")) {
         return false;
     }
     
     
     
             /* find file to open not gz file */
            if (e->type() == QEvent::GraphicsSceneDrop) {
                QStringList indexf = KnowMimeFile();
                QGraphicsSceneDragDropEvent *ev = static_cast<QGraphicsSceneDragDropEvent *>(e);
                   /////////qDebug("Tipe event  %d", e->type()); 
                    bool filefound = false;
                   const QMimeData *md = ev->mimeData();
                       if ( md->hasUrls() )  {
                          QList<QUrl> urls = md->urls();
                          for ( int i = 0; i < urls.size(); ++i ) { 
                               QUrl gettyurl(urls.at(i));
                              if (gettyurl.scheme() == "file") {
                                  QFileInfo fi(gettyurl.toLocalFile());
                                  const QString extension = fi.completeSuffix().toLower();
                                  const int havingmime = indexf.indexOf(extension);
                                  ////////////qDebug() << "### event drag ............ " << fi.absoluteFilePath();
                                  //////////qDebug() << "### event extension ............ " << havingmime;
                                  ///////////qDebug() << "### event extension ............ " << extension;
                                  
                                  if (havingmime) {
                                      openFile( fi.absoluteFilePath() );
                                      //////////qDebug() << "### event dd yes";
                                      filefound = true;
                                  } else {
                                      
                                        if (extension == "fo" || extension == "fop" ) {
                                        openFile( fi.absoluteFilePath() );
                                        filefound = true;
                                        }
                                      
                                      
                                     ////////////// qDebug() << "### event dd no";
                                  }
                                  
                              }
                          }
                          
                        }
                        
                  return filefound;
           }  else {
                 // standard event processing
                 return QObject::eventFilter(obj,e);
          }
}

void GraphicsView::pageclear()
{
    scene->clear();  /*  remove all item */
    pageFull =new TextLayer(0);
    scene->setSceneRect( rectToScene());
    scene->addItem(pageFull);
    connect(scene, SIGNAL(MakeVisible(QRectF)), this, SLOT(viewDisplay(QRectF)));
    connect(pageFull, SIGNAL(pageCountChange() ), this, SLOT(forceResizeIntern()));
    connect(pageFull, SIGNAL(autocursorchange(bool) ), this, SLOT(cursorChange(bool)));
    connect(pageFull, SIGNAL(inBookmark(QStringList)),this, SLOT(sendinBookmark(QStringList)));
    QTimer::singleShot(5, this, SLOT(DisplayTop()));
    emit fileBaseOpen(_APPLICATIONS_NAME_ + " / Untitled Document");
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

void GraphicsView::openMasterMainFile()
{
    openFile(":/file/recfile/main_doc.page");
}

void GraphicsView::openFile( const QString file )
{
    
    QFileInfo fi(file);
    ApiSession *session = ApiSession::instance();
    const QString dir_ = QDir::currentPath();
    QDir::setCurrent(fi.absolutePath());
    StreamFop *buf = new StreamFop();
    currentopenfilerunning = "";
    gzippedfile = false;
    pageclear();  /* clean old item if  having */
    ////////////ApiSession *sx = ApiSession::instance();
    const QString ext = fi.completeSuffix().toLower();
    emit fileBaseOpen(_APPLICATIONS_NAME_ + " / " + fi.fileName ());
    
    qDebug() << "### openFile  " << fi.absoluteFilePath() << " - " << ext;
    
    
    currentopenfilealternate = fi.absolutePath() + QString("/") + fi.baseName() + QString(".tmpfox");
    qt_unlink(currentopenfilealternate);
    currentopenfilerunning =  fi.absoluteFilePath();
    
    if (ext == "fo" || ext == "fop" || ext == "fop.gz" || ext == "fo.gz" || ext == "xml") {
        if (ext == "fop.gz" || ext == "fo.gz") {
        const QByteArray  stream = OpenGzipOneFileByte( fi.absoluteFilePath() );
        buf->device()->write(stream);
        buf->PutOnFile( currentopenfilealternate );
        delete buf;
        gzippedfile = true;
        currentopenfilerunning = currentopenfilealternate;
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
        
        if (gzippedfile) {
            /* remove open chunk gz if is */
            qt_unlink(currentopenfilealternate);
        }
        
        Fo_Reader * fops = new Fo_Reader(currentopenfilealternate);
        QMap<int,RichDoc> absoluteItem = fops->layers();
        
        
        
        
        
        const QTextDocument *fopdoc = fops->document()->clone();
        pageFull->setDocument(fopdoc);
        pageFull->appendLayer(absoluteItem);
        M_PageSize formatFF = fops->PSize();
        session->current_Page_Format = formatFF;
        session->AppendPaper(formatFF);
        pageFull->SwapPageModel( formatFF );
        
        /* session having format by Fo_Reader */
        
        /* bookmark open its */
        StreamFop *buf = new StreamFop();
        buf->LoadFile( currentopenfilealternate );
        const QByteArray chunkfop = buf->stream();
        delete buf;
        BookMarkModelRead  *booki = new BookMarkModelRead();
        booki->openStream( chunkfop );
        QStandardItemModel *dat = booki->bookModel();
        QStringList  inlinks =  booki->linkList();
      
        
        qDebug() << "### dat->rowCount()..."  << dat->rowCount();
        
        fops->deleteLater(); 
        QTimer::singleShot(1, this, SLOT(zoomChain())); 
        emit inBookmark(inlinks,dat);
        QDir::setCurrent(dir_);
        
        if (gzippedfile) {
            saveAsFile();
        }
        
        if (dat->rowCount() > 0) {
        emit bookMarkActive(true);
        } else {
        emit bookMarkActive(false);  
        }
        
        return;
    } else if ( ext ==  "sxw" || ext ==  "stw"   ) {
        M_PageSize defaultA4Page;
        /* :-)  html not know is format !!!!!!! */
        currentopenfilerunning = "";   /* only read modus save as fop or html */
        
        OO_Xslt *oohandler = new OO_Xslt(fi.absoluteFilePath(),DOC_1_VERSION);
        currentopenfilerunning = "";   /* only read modus save as fop or html */
        QTextDocument  *oodoc = oohandler->Document();
        defaultA4Page.HandlePrint( oodoc );
        session->current_Page_Format = defaultA4Page;
        session->AppendPaper(defaultA4Page);
        pageFull->setDocument(new QTextDocument(oodoc->toPlainText()) );
        
        BookMarkModelRead  *booki = new BookMarkModelRead();
        QStandardItemModel *dat = booki->bookModel();
        emit inBookmark(QStringList(),dat);
        QTimer::singleShot(1, this, SLOT(zoomChain())); 
        QDir::setCurrent(dir_);
        return;
    }  else if ( ext ==  "odt" ||  ext ==  "ott" ) {
        M_PageSize defaultA4Page;
        /* :-)  html not know is format !!!!!!! */
        currentopenfilerunning = "";   /* only read modus save as fop or html */
        
        /* crasher  beta 
        OO_Handler *ooswapper = new OO_Handler( fi.absoluteFilePath() );
        QTextDocument  *oodoc = ooswapper->document();
        defaultA4Page.HandlePrint( oodoc );
        session->current_Page_Format = defaultA4Page;
        session->AppendPaper(defaultA4Page);
        pageFull->setDocument(oodoc);
        */
        
        /* take xslt-method beta ....   */
        
        OO_Xslt *oohandler = new OO_Xslt(fi.absoluteFilePath(),DOC_1_VERSION);
        currentopenfilerunning = "";   /* only read modus save as fop or html */
        QTextDocument  *oodoc = oohandler->Document();
        defaultA4Page.HandlePrint( oodoc );
        session->current_Page_Format = defaultA4Page;
        session->AppendPaper(defaultA4Page);
        pageFull->setDocument(new QTextDocument(oodoc->toPlainText()) );
        
        BookMarkModelRead  *booki = new BookMarkModelRead();
        QStandardItemModel *dat = booki->bookModel();
        emit inBookmark(QStringList(),dat);
        QTimer::singleShot(1, this, SLOT(zoomChain())); 
        QDir::setCurrent(dir_);
        return;
    }  else if ( ext == "html" || ext == "htm" ) {
         M_PageSize defaultA4Page;
        /* :-)  html not know is format !!!!!!! */
        buf->LoadFile( currentopenfilerunning );
        const QByteArray chunkhtml = buf->stream();
        delete buf;
        QTextDocument *htmldoc = new QTextDocument();
        htmldoc->setHtml ( QString ( chunkhtml) );
        ///////htmldoc->setTextWidth ( defaultA4Page.G_regt.width() );
        defaultA4Page.HandlePrint( htmldoc );
        session->current_Page_Format = defaultA4Page;
        session->AppendPaper(defaultA4Page);
        pageFull->setDocument(htmldoc);
        
        BookMarkModelRead  *booki = new BookMarkModelRead();
        QStandardItemModel *dat = booki->bookModel();
        emit inBookmark(QStringList(),dat);
        QDir::setCurrent(dir_);
        QTimer::singleShot(1, this, SLOT(zoomChain())); 
        return;
    }  else if ( ext == "page" ) {
        M_PageSize defaultA4Page;
        qDebug() << "### pageload .......... ";
        buf->LoadFile( fi.absoluteFilePath() );
        const QByteArray chunkhtml = buf->stream();
        delete buf;
        PageDoc binDoc;
                binDoc.open( chunkhtml );
        QTextDocument *autodoc = binDoc.scripePrepare();
        defaultA4Page.HandlePrint( autodoc );
        session->current_Page_Format = defaultA4Page;
        session->AppendPaper(defaultA4Page);
        pageFull->setDocument(autodoc->clone());
        pageFull->appendLayer(binDoc.absolute);
        BookMarkModelRead  *booki = new BookMarkModelRead();
        QStandardItemModel *dat = booki->bookModel();
        emit inBookmark(QStringList(),dat);
        QDir::setCurrent(dir_);
        QTimer::singleShot(1, this, SLOT(zoomChainStop())); 
        return;
    }
    
    
    QDir::setCurrent(dir_);
}

void GraphicsView::zoomChain()
{
    /////////qDebug() << "### zoomChain ";
    zoomChainStop();
    return;
    QMatrix matrix;
    matrix.scale(0.00009,0.00009);
    setMatrix(matrix);
    QTimer::singleShot(100, this, SLOT(zoomChainStop())); 
}

void GraphicsView::zoomChainStop()
{
   //////////// qDebug() << "### zoomChainStop ";
    QMatrix matrix;
    matrix.scale(1.4,1.4);
    setMatrix(matrix);
    show();
    QTimer::singleShot(5, this, SLOT(DisplayTop())); 
}


void GraphicsView::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save FOP file",QString(setter.value("LastDir").toString()) , "*.fop *.fo *.xml");
    
    if (fileName.size() > 1) {
        QFileInfo fi(fileName);
        const QString ext = fi.completeSuffix().toLower();
        if (ext == "fo" || ext == "fop" || ext == "xml") {
          /* ok */
        } else {
        fileName = fileName+".fop"; 
        }
        QFileInfo fix(fileName);
        setter.setValue("LastDir",fix.absolutePath() + "/");
        if (!saveFopFile(fix.absoluteFilePath(),false)) {
                QMessageBox infoset;
                infoset.setWindowTitle(QObject::tr("Error found on document!") );
                infoset.setText ( tr("Unable to save document, check permission!") );
                infoset.exec();
                return;
        }
        openFile( fileName );
    }
    
}

void GraphicsView::saveOnFile()
{
    if (tryToAutoSave()) {
     return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, "Save FOP file",QString(setter.value("LastDir").toString()) , "*.fop *.fo *.xml");
    
    if (fileName.size() > 1) {
        QFileInfo fi(fileName);
        const QString ext = fi.completeSuffix().toLower();
        if (ext == "fo" || ext == "fop" || ext == "xml") {
          /* ok */
        } else {
        fileName = fileName+".fop"; 
        }
        QFileInfo fix(fileName);
        setter.setValue("LastDir",fix.absolutePath() + "/");
        if (!saveFopFile(fix.absoluteFilePath(),true)) {
                QMessageBox infoset;
                infoset.setWindowTitle(QObject::tr("Error found on document!") );
                infoset.setText ( tr("Unable to save document, check permission!") );
                infoset.exec();
        }
    }
}

void GraphicsView::saveOnPageBinFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Page file format",QString(setter.value("LastDir").toString()) , "*.page");
     
    if (fileName.isEmpty()) {
        return;
    }
    
    QFileInfo fi(fileName);
        const QString ext = fi.completeSuffix().toLower();
        if (ext == "page") {
          /* ok */
        } else {
        fileName = fileName+".page"; 
        }
        
        /* get chunk */
        PageDoc xpage = pageFull->binaryPageFormat();
        
        
    
         QFile f(fi.absoluteFilePath()); 
                if (f.open(QIODevice::WriteOnly)) {
                    f.write(xpage.save());
                    f.close();
                } 
    
}


bool GraphicsView::isFopInstall()
{
    QString foptipe,javadir,exefop;
    double jk_version = JavaVersion();
    
    #if defined Q_WS_WIN
    javadir = getenv("JAVA_HOME");
    #endif
    #if defined Q_WS_X11
    javadir = getenv("JAVA_HOME");
    #endif
    #if defined Q_WS_MAC
    javadir = "existforever..";   /* default install all version */
    #endif
    
    exefop = QString(setter.value("FopApplicationfi").toString());
    QFileInfo executefop(exefop);
    
    if (jk_version >= MINIMUMJAVAVERSION && executefop.exists() ) {
        /* java looks fine */
        return true;
    } else {
         fopInstaller();
        return false;
    }
    
}

bool GraphicsView::fopInstaller()
{
    QString exefop,javadir,impdf,bakfops;
 
    
    double jk_version = JavaVersion();
    if (jk_version >= MINIMUMJAVAVERSION) {
    QMessageBox::warning(this, tr("Java versio check."), tr("Java version is not ok installed %1 need %2.").arg(jk_version).arg(MINIMUMJAVAVERSION),QMessageBox::Cancel,QMessageBox::Cancel);
    } else {
    /* java envoirment or version faliled */
    return false;
    }
    exefop = QString(setter.value("FopApplicationfi").toString());
    QFileInfo fi(exefop);
    if (exefop.isEmpty()  || !fi.exists()) {
         exefop = QFileDialog::getOpenFileName(this, tr("Place of Apache Fop \"%1\" application ").arg(foptipe),"",tr("fop (*)"));
         if (exefop.isEmpty()) {
          return false;
         }
         setter.setValue("FopApplicationfi",exefop);
         if (!isFopInstall()) {
          /* if file not exist !!! */
         setter.setValue("FopApplicationfi",""); 
         return false;
         } else {
         return true;
         }
    } else {
    return true;
    }
    
}


void GraphicsView::saveTiffDoc()
{
    QFileInfo fi(currentopenfilerunning);
    if (!fi.exists()) {
    saveAsFile();
    return;
    }
    const QString ext = fi.completeSuffix().toLower();
    if (ext == "fo" || ext == "fop" || ext == "xml") {
        /* ok */
    } else {
    saveAsFile();
    }
    if (!isFopInstall()) {
    return;
    }
    
    QString rtffile = QFileDialog::getSaveFileName(this, "Save as",QString(setter.value("LastDirPDFSave").toString())+fi.baseName()+".tif", "Tif Fax page Format  (*.tif)");
    if (rtffile.isEmpty()) {
    return;
    }
    QFileInfo fipdf(rtffile);
    setter.setValue("LastDirPDFSave",fipdf.absolutePath() + "/");
    fopExcec( QStringList() << "-fo" << currentopenfilerunning << "-tiff" << rtffile , rtffile );
}


void GraphicsView::saveRtfDoc()
{
    QFileInfo fi(currentopenfilerunning);
    if (!fi.exists()) {
    saveAsFile();
    return;
    }
    const QString ext = fi.completeSuffix().toLower();
    if (ext == "fo" || ext == "fop" || ext == "xml") {
        /* ok */
    } else {
    saveAsFile();
    }
    if (!isFopInstall()) {
    return;
    }
    
    QString rtffile = QFileDialog::getSaveFileName(this, "Save as",QString(setter.value("LastDirPDFSave").toString())+fi.baseName()+".rtf", "Richt Text  (*.rtf)");
    if (rtffile.isEmpty()) {
    return;
    }
    QFileInfo fipdf(rtffile);
    setter.setValue("LastDirPDFSave",fipdf.absolutePath() + "/");
    fopExcec( QStringList() << "-fo" << currentopenfilerunning << "-rtf" << rtffile , rtffile );
}

void GraphicsView::printPreview()
{
    QFileInfo fi(currentopenfilerunning);
    if (!fi.exists()) {
    saveAsFile();
    return;
    }
    const QString ext = fi.completeSuffix().toLower();
    if (ext == "fo" || ext == "fop" || ext == "xml") {
        /* ok */
    } else {
    saveAsFile();
    }
    if (!isFopInstall()) {
    return;
    }
    QApplication::restoreOverrideCursor();
    QProcess process;
    process.startDetached( setter.value("FopApplicationfi").toString()  , QStringList()  << currentopenfilerunning << "-awt"  );
     if (!process.waitForFinished()) {
         
         /* no fop install !!! */
     }
}

void GraphicsView::apacheFopConvert()
{
    QString exefop,impdf;
    QFileInfo fi(currentopenfilerunning);
    if (!fi.exists()) {
    saveAsFile();
    return;
    }
    const QString ext = fi.completeSuffix().toLower();
    if (ext == "fo" || ext == "fop" || ext == "xml") {
        /* ok */
    } else {
    saveAsFile();
    }
    if (!isFopInstall()) {
    return;
    }

    exefop = QString(setter.value("FopApplicationfi").toString());
    if (exefop.isEmpty()) {
         exefop = QFileDialog::getOpenFileName(this, tr("Place of Fop \"%1\" application ").arg(foptipe),"",tr("fop (*)"));
         if (exefop.isEmpty()) {
          return;
         }
         setter.setValue("FopApplicationfi",exefop);
        
    }
    if (!isFopInstall()) {
    return;
    }  
    /* save location */
    impdf = QFileDialog::getSaveFileName(this, "Save as",QString(setter.value("LastDirPDFSave").toString())+fi.baseName()+".pdf", "Pdf  (*.pdf)");
        if (impdf.isEmpty()) {
          return;
         }
          


            if (impdf.endsWith(".pdf")) {
             /* ok */
            } else {
            impdf = impdf+".pdf";  
            }
            QFileInfo fipdf(impdf);
            setter.setValue("LastDirPDFSave",fipdf.absolutePath() + "/");
            fopExcec( QStringList() << "-fo" << currentopenfilerunning << "-pdf" << impdf , impdf );
}


void GraphicsView::fopExcec( QStringList commandlist , const QString file )
{
    if (!isFopInstall()) {
    return;
    }  
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QProcess process;
    process.setReadChannelMode(QProcess::MergedChannels);
    process.start(setter.value("FopApplicationfi").toString()  , commandlist);
                         
                  if (!process.waitForFinished()) {
                          QApplication::restoreOverrideCursor();
                             QMessageBox::critical(0, tr("Error by XSLT-FO apache"),tr("Unable to convert Your file!\nError %1").arg(QString(process.errorString())));
                             return;
                         } else {
                             QApplication::restoreOverrideCursor();
                                     if (file.size() > 3) {
                                     OpenDeskBrowser(QUrl(file));
                                     }
                           return;  
                         }          
                         
                         
    QApplication::restoreOverrideCursor();           
                         
}




bool GraphicsView::saveFopFile( const  QString file ,  bool memo )
{
    ApiSession *sx = ApiSession::instance();
    StreamFop *buf = new StreamFop();
    QFileInfo pi(file);
    /* layer put */
    /* save image if exist  */
    QDomDocument *fopdoc = pageFull->fopPagedoc();
    if (sx->saveImageFile.size() > 0 ) {
          const QString imgagedirgo = pi.absolutePath() + "/" + FOPIMAGEDIR;
             ///// Cache( imgagedirgo );
          QMapIterator<QString,QByteArray> i(sx->saveImageFile);
          while (i.hasNext()) {
             i.next();   // /// i.value();
              QFileInfo imgfi(imgagedirgo + i.key());
              if (!imgfi.exists())  {
                /* check if dir exist and make */
                Cache( imgagedirgo );
                  
                  
                  
                QFile f(imgfi.absoluteFilePath()); 
                if (f.open(QIODevice::WriteOnly)) {
                    f.write(i.value());
                    f.close();
                } 
              }
             
         }
        
        
    }
    
    buf->device()->write(fopdoc->toByteArray(1));
    buf->PutOnFile( file );
    delete buf;
    if (memo) {
    currentopenfilerunning = file;
    QFileInfo fi(currentopenfilerunning);
    return fi.exists(); 
    } else {
    QFileInfo fi(file);
    return fi.exists();
    }
}

bool GraphicsView::saveHtmlFile( const  QString file ,  bool memo )
{
    QString chunk = page()->document()->toHtml("utf-8");
    
    return false;
    
}

bool GraphicsView::tryToAutoSave()
{
    QFileInfo fi(currentopenfilerunning);
    const QString ext = fi.completeSuffix().toLower();
    if (fi.exists()) {
        if (ext == "fo" || ext == "fop" ) {
            if (isvalidXmlencoding( fi.absoluteFilePath() )) {
              /* save to fop silince */
                return saveFopFile(fi.absoluteFilePath(),true);
            }
        }
        
    }
  return false;
}


void GraphicsView::sendinBookmark( QStringList e )
{
    emit inBookmark(e);
}

QRectF GraphicsView::rectToScene()
{
    /////////ApiSession *sx = ApiSession::instance();
    //////M_PageSize PAGE_MODEL = sx->CurrentPageFormat();
    const QRectF fpage = pageFull->boundingRect();
    return QRectF(0,0,fpage.width(),fpage.height() + _BOTTOM_VIEW_SPACE_RESERVE_);
}

void GraphicsView::forceResizeExtern()
{
    ApiSession *sx = ApiSession::instance();
    pageFull->SwapPageModel( sx->CurrentPageFormat() );
    scene->setSceneRect( rectToScene());
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    forceResizeIntern();
    QGraphicsView::resizeEvent(event);
}

void GraphicsView::viewDisplay( const QRectF area )
{
    
    /* if scale to big return */
    QMatrix matx = matrix();
    qreal HHscaled = matx.m11();
    if (HHscaled > 5.1) {
    ///////qDebug() << "### GraphicsView::ViewDisplay -------- " << area;
    /////////qDebug() << "### GraphiHHscaled  -------- " << HHscaled;
    return;
    }
    QGraphicsView::ensureVisible(area);
}

void GraphicsView::DisplayTop()
{
    forceResizeExtern();
	horizontalScrollBar()->setValue(0); 
	verticalScrollBar()->setValue(0);
}


void GraphicsView::cursorChange( bool e )
{
    emit sceneSwap(e);
}

void GraphicsView::forceResizeIntern()
{
    scene->setSceneRect( rectToScene());
    scene->clearSelection();
    emit NewPageFormatin();
}


PaperEditor::PaperEditor( QWidget *parent)
    : QFrame(parent),tievents(0),NotPaperUpdate(true)
{
    //////setFrameStyle(Sunken | StyledPaperEditor);
    graphicsView = new GraphicsView(this);
    graphicsView->setObjectName(QString("graphicsView"));
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

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
		
	openGlButton = new QToolButton;
	openGlButton->setText(tr("OpenGL"));
	openGlButton->setCheckable(true);

    
		// Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);
    zoomSliderLayout->addWidget(resetButton);
    zoomSliderLayout->addWidget(openGlButton);
		
    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(graphicsView, 0, 0);
    topLayout->addLayout(zoomSliderLayout, 0, 1);
    setLayout(topLayout);
    
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    ////////connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
    ////////connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
    
    
    resetView();
    QTimer::singleShot(1, this, SLOT(DisplayTop()));  
   
    #if defined Q_WS_WIN
    /* dont start default openGlButton */
    #endif
    #if defined Q_WS_X11
    /* dont start default openGlButton */
    #endif
    #if defined Q_WS_MAC
    openGlButton->setChecked ( true );
    toggleOpenGL();
    #endif 
}




void PaperEditor::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
    QTimer::singleShot(1, this, SLOT(DisplayTop()));
}


void PaperEditor::SceneChange()
{
	  tievents++;
}

void PaperEditor::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void PaperEditor::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void PaperEditor::resetView()
{
    zoomSlider->setValue(DefaultStartZoom);
    QMatrix matrix;
    matrix.scale(1.4,1.4);
    graphicsView->setMatrix(matrix);
    resetButton->setEnabled(false);
    QTimer::singleShot(1, this, SLOT(DisplayTop()));
}



void PaperEditor::setupMatrix()
{
    qreal scale = ::pow(2.0, (zoomSlider->value() - DefaultStartZoom) / 50.0);
    /////////qDebug() << "### scale " << scale;
    //////////qDebug() << "### scale rr  " << zoomSlider->value();
    QMatrix matrix;
    matrix.scale(scale, scale);
    ///////matrix.rotate(10);
    graphicsView->setMatrix(matrix);
    setResetButtonEnabled();
    ///////graphicsView->forceResize();
}

void PaperEditor::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}


GraphicsView *PaperEditor::view() const
{
	return graphicsView;
}


void PaperEditor::DisplayTop()
{
	//////graphicsView->DisplayTop();
}


























