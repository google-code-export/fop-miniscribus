#include "main.h"
#include "ui_gmain.h"

#include "gui_about.h"
#include "getmargin.h"

#ifdef Q_WS_MAC
extern void qt_mac_set_dock_menu(QMenu *);
#include <Carbon/Carbon.h>
#endif

#if defined _COMPOSE_STATIC_
#include <QtPlugin>
#if defined _USE_qjpeg
Q_IMPORT_PLUGIN(qjpeg)
#endif
#if defined _USE_qmng
Q_IMPORT_PLUGIN(qmng)
#endif
#if defined _USE_qgif
Q_IMPORT_PLUGIN(qgif)
#endif
#if defined _USE_qtiff
Q_IMPORT_PLUGIN(qtiff)
#endif
#endif


/* main to enable move  event */
class Gui_Main : public QMainWindow, public Ui::GMain
{
    Q_OBJECT
//
public:
 Gui_Main( QWidget* = 0 )
{
 setupUi( this );
 setMinimumSize ( sizeHint() ) ;
 LoadBaseMenu();
 label_4->setText(tr("Page/s 1"));
    
    
    #if defined Q_WS_WIN
    QString exepath;
    exepath = QCoreApplication::applicationDirPath()+"/";
    QSettings softs("HKEY_LOCAL_MACHINE\\Software\\"+QString(_ORGANIZATION_NAME__)+"\\"+QString(_REGISTER_PROGRAM_NAME_),QSettings::NativeFormat);
    softs.setValue("path",exepath);   /* to regedit  to find from friend programm db label */
    softs.setValue("Path",exepath);   /* to regedit  to find from friend programm db label */
    softs.setValue("Bundlename",_BUNDLE_NAME_);
    softs.setValue("Lastversion",QString(_CURRENT_VERSION_));   /* to regedit  to find from friend programm db label */
    softs.setValue(QString(_CURRENT_VERSION_)+"/pdfdirect","1");
    if (setter.value("ghosthaving").toInt() == 1) {
    /* update to friend programm*/
    softs.setValue("gscmd",getGSDefaultExeName());
    }
    #endif
    
    
   
    
    
  QTimer::singleShot(800, this, SLOT(CheckGhostInstall()));
    
}



void LoadBaseMenu() 
{
    toolBarZoom = new QToolBar(this);
    enablepageselect = false;
    addToolBar(Qt::TopToolBarArea, toolBarZoom);
    QStringList scales;
    scales << tr("45%") << tr("55%") << tr("65%") << tr("75%") << tr("85%")<< tr("95%")  << tr("100%") << tr("110%") << tr("120%") << tr("125%") << tr("150%") << tr("200%") << tr("300%") << tr("400%");
    
    QStringList units;
    units << "mm" << "cm" <<  "inch" << "pt" << "em" << "dm" << "pi" << "dd" << "cc" << "in";
    unita->clear();
    unita->addItems(units);
    
    gridwihi->setValue(10);
    Gridcolor = QColor(230, 230, 230);
    unitas = setter.value("CurrentRunningUnit").toString();
    if (unitas.size() < 1) {
    unitas = "pt";
    }
    gridwihi->setSuffix (QString(" %1.").arg(unitas));
    qreal gridchess = setter.value("GRIDChessMesurePoint").toDouble();
    if (gridchess > 0 ) {
    gridwihi->setValue(Pointo(gridchess,unitas));
    }
    UpdateUnits();
    
    colorgrid->setIcon(createColorToolButtonIcon(":/img/background2.png",Gridcolor));
    connect(colorgrid, SIGNAL(clicked()),this, SLOT(SetGridColor()));
    
    connect(ruota, SIGNAL(valueChanged(int)),this, SLOT(RotateLayer(int)));
    
    
     
    
    QLabel *label = new QLabel(this);
    label->setText(tr(" Zoom: "));
    toolBarZoom->addWidget(label);
    intzoom = new QComboBox(this);  
    intzoom->addItems(scales);
    intzoom->setCurrentIndex(6); 
    QObject::connect(intzoom, SIGNAL(currentIndexChanged(const QString &)), view, SLOT(sceneScaleChanged(const QString &)));
    toolBarZoom->addWidget(intzoom);
  
    
    
    
        backgroundToolButton = new QToolButton(this); 
		backgroundToolButton->setToolTip(tr("Set BackgroundColor to one floating layer if selected."));
        backgroundToolButton->setPopupMode(QToolButton::MenuButtonPopup);
		backgroundToolButton->setMenu(createColorMenu(SLOT(backgroundColorTriggered()),Qt::white));
		backgroundToolButton->setIcon(createColorToolButtonIcon(":/img/html_div.png", Qt::white));
        toolBarZoom->addWidget(backgroundToolButton);
        QObject::connect(view, SIGNAL(PaintButton(FloatDiagram*)), this, SLOT(BgButton(FloatDiagram*))); 
        QObject::connect(view, SIGNAL(OnRotate(int)), this, SLOT(UpdateDegree(int))); 
        QObject::connect(view, SIGNAL(TotalPage(int)), this, SLOT(PageUpdate(int)));
        
    
     QObject::connect(gomargin, SIGNAL(clicked()), this, SLOT(PageMargin()));
     QObject::connect(unita, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(UpdateUnits()));
     QObject::connect(gridwihi, SIGNAL(valueChanged(double)), this, SLOT(UpdateUnits()));
     
     pagebox->clear();
}


void SetDB( PageDB *dbin ) {
db = dbin;
QObject::connect(db, SIGNAL(newdata()), this, SLOT(refillformats()));
}
QSettings setter;
bool enablepageselect;
QToolBar *toolBarZoom;
QComboBox *intzoom;
QComboBox *paperformats;
QColor Gridcolor;
QToolButton *backgroundToolButton;
QString unitas;
PageDB *db;
protected:
QMenu *createColorMenu(const char *slot, QColor defaultColor)
{
	  QStringList names = QColor::colorNames();
	
    QMenu *colorMenu = new QMenu;
    for (int i = 0; i < names.size(); ++i) {
			  QColor conow(names.at(i));
        QAction *action = new QAction(names.at(i), this);
        action->setData(conow);
        action->setIcon(createColorIcon(conow));
        connect(action, SIGNAL(triggered()),view, slot);
        colorMenu->addAction(action);
        if (conow == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}
QIcon createColorToolButtonIcon(const QString &imageFile,QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);
    return QIcon(pixmap);
}
QIcon createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);
    return QIcon(pixmap);
}
QSize sizeHint() const
{
     return QSize(1000, 710);
} 
void mousePressEvent(QMouseEvent *event)
 {
     /* || event->button() ==  Qt::RightButton */
     if (event->button() == Qt::LeftButton  ) {
         /////dragPosition = event->globalPos() - frameGeometry().topLeft();
         //////event->accept();
     }
 }
void mouseMoveEvent(QMouseEvent * event) {
    
    if (event->buttons() & Qt::LeftButton  ) {
        ////// move(event->globalPos() - dragPosition);
         ///////event->accept();
     }
}

private:
    QPoint dragPosition;
signals:
public slots:
void RotateLayer( int r ) 
{
    fdialdeegres->setText(tr("Current %1°").arg(r));
    view->RotateCurrentActive( r );
}
void PageUpdate( int p )
{
    label_4->setText(tr("Page/s %1").arg(p));
}



void UpdateDegree( int r ) 
{
    fdialdeegres->setText(tr("Current %1°").arg(r));
}


void GhostScriptLinks() 
{
 #if defined Q_WS_MAC
    view->OpenDesktop( QUrl("http://www.google.ch/search?q=espgs-7.07.1.ppc.dmg") );
 #endif
 #if defined Q_WS_WIN
    view->OpenDesktop( QUrl("http://www.google.ch/search?q=GhostScript") );
 #endif
 #if defined Q_WS_X11
    view->OpenDesktop( QUrl("http://www.google.ch/search?q=GhostScript") );
 #endif
}



void AboutApps() {
        Gui_About *ab = new Gui_About(this);  
        ab->setWindowTitle(tr("Info - %1").arg(_REGISTER_PROGRAM_NAME_));
        ab->exec();
}
void BgButton( FloatDiagram * item  )
{
    if (item->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    enablepageselect = false;
    fdialdeegres->setText(tr("Current %1°").arg(item->GetDegree()));
    backgroundToolButton->setIcon(createColorToolButtonIcon(":/img/html_div.png",item->GetColor()));
    ladisplay->setText(tr("Layer:X/%1 Y/%2\npt.%3X%4").arg(item->Prect().x()).arg(item->Prect().y()).arg(item->Prect().width()).arg(item->Prect().height()));
     pagebox->setCurrentIndex ( view->PageFormat() );
     enablepageselect = true;
    
    
}
void SetGridColor() {
     bool ok;
     QRgb col = QColorDialog::getRgba(Gridcolor.rgba(),&ok);
     if (!ok) {
        return; 
     }
     int AlphaColor =  qAlpha ( col );
     Gridcolor = QColor( col );
     Gridcolor.setAlpha(AlphaColor);
     colorgrid->setIcon(createColorToolButtonIcon(":/img/background2.png",Gridcolor));
     UpdateUnits();
}

void UpdateUnits() 
{
    gridwihi->setSuffix (QString(" %1.").arg(unita->currentText()));
    unitas = unita->currentText();
    setter.setValue("CurrentRunningUnit",unitas);
    qreal larges = ToPoint(gridwihi->value(),unitas);
    setter.setValue("GRIDChessMesurePoint",larges);
    QPixmap m_tile = QPixmap(larges * 2,larges * 2);
    m_tile.fill(Qt::white);
    QPainter pt(&m_tile);
    pt.fillRect(0, 0, larges,larges, Gridcolor);
    pt.fillRect(larges,larges,larges,larges, Gridcolor);
    pt.end();
    view->SetChessGrid( m_tile );
}

void PageMargin() 
{
    QRectF actual = view->GetmarginPage();
    GetMargin *marge = new GetMargin(this);
    marge->setWindowTitle(tr("Set Page Margin"));
    marge->Set_margin( actual );
    int faxme = marge->exec();
    if (faxme == 1) {
          view->SetmarginPage(marge->Get_margin());
    }
}

void refillformats() 
{
    enablepageselect = false;
    pagebox->clear();
    if (db->connect()) {
    QList<PageDimensionName*> pages =  db->Listing();
        for (int i=0;i<pages.size();i++) {
            pagebox->addItem(tr("Name:") + pages[i]->name() + " - " + pages[i]->display());
        }
    }
    enablepageselect = true;
    QObject::connect(pagebox, SIGNAL(currentIndexChanged(int)), this, SLOT(SwapPaperFormat(int)));
}

void SwapPaperFormat( int i )
{
    if (!enablepageselect) {
    return;
    }
    view->SwapPaperFormat(i);
}
 
void CheckGhostInstall() 
{
    if (setter.value("ghosthaving").toInt() == 2) {
    return;   /* not ask 2 time no! */
    }
    
    
     if (getGSVersion().size() < 5)  {
        QString msgDB =tr("GhostScript is not install on this system: you will need to grab a copy if you like to insert PS, EPS , PDF as image ....");
         
         
         QMessageBox::StandardButton reply;
         reply = QMessageBox::question(this, tr("GhostScript tools."),
                                    msgDB,
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
          
         if (reply == QMessageBox::Yes) {
             setter.setValue("ghosthaving",0);
             GhostScriptLinks();
         } else if (reply == QMessageBox::No) {
            setter.setValue("ghosthaving",2);
         } else {
            setter.setValue("ghosthaving",0);
         }
        
      } else {
         setter.setValue("ghosthaving",1);
         setter.setValue("gscmd",getGSDefaultExeName());
      }
    
}


};


#define osApplication \
    (static_cast<OS_application*>(QCoreApplication::instance()))

class OS_application : public QApplication
{
    Q_OBJECT
//
public:
 OS_application( int &argc, char **argv )
  : QApplication(argc, argv)
{
         
       QApplication::restoreOverrideCursor();  /* if other apps ? */
        QCoreApplication::setOrganizationName(_ORGANIZATION_NAME__);
        QCoreApplication::setOrganizationDomain("fop.ciz.ch");
        QCoreApplication::setApplicationName(_REGISTER_PROGRAM_NAME_);
        
        
        
        FileSend = false;
        getpdf = false;
        QString argument = argv[1];
        GuiMain = new Gui_Main();
        db = new PageDB(GuiMain);    /* load data base page dimension  static cast mainwindow */
        GuiMain->SetDB(db);
        GuiMain->view->SetDB(db);
        db->reload();
        #if defined Q_WS_WIN
        QString exepath;
        exepath = QCoreApplication::applicationDirPath()+"/";
        GuiMain->setter.setValue("path",exepath);   /* to regedit  to find from friend programm db label */
        #endif
        /* mac is forever /Application/ */
        
        
        const QString usersetfont = GuiMain->setter.value("DefaultFontsW").toString();
        if (usersetfont.size() > 0 ) {
            QFont settfont;
            settfont.fromString(usersetfont);
            setFont(settfont);
        }
        
        
        
        
    
        GuiMain->setWindowTitle( _PROGRAM_TITLE_ );
        QObject::connect(GuiMain->action_Exit, SIGNAL(triggered()),this, SLOT(CleanProcess()));
        QObject::connect(GuiMain->actionOpen_Fop_doc, SIGNAL(triggered()), this, SLOT(OpenFopFile()));
    
    	
    
        
        QObject::connect(GuiMain->actionGhostScript_link, SIGNAL(triggered()), GuiMain, SLOT(GhostScriptLinks()));
       QObject::connect(GuiMain->actionLock_current_item, SIGNAL(triggered()), GuiMain->view, SLOT(LockItem()));
       QObject::connect(GuiMain->actionUnlock_current_item, SIGNAL(triggered()), GuiMain->view, SLOT(UnLockItem()));
       QObject::connect(GuiMain->actionAdvanced_edit, SIGNAL(triggered()), GuiMain->view, SLOT(BigEdit()));
       QObject::connect(GuiMain->actionBring_current_to_front, SIGNAL(triggered()), GuiMain->view, SLOT(bringToFront()));
       QObject::connect(GuiMain->actionSend_current_back, SIGNAL(triggered()), GuiMain->view, SLOT(sendToBack()));
       QObject::connect(GuiMain->actionAppend_a_new_Layer, SIGNAL(triggered()), GuiMain->view, SLOT(AppendLayerInside()));
       QObject::connect(GuiMain->actionClone_current_Layer_to_new, SIGNAL(triggered()), GuiMain->view, SLOT(AppendCurrenttoClone()));
    
       QObject::connect(GuiMain->actionRemote_current, SIGNAL(triggered()), GuiMain->view, SLOT(RemoveItem()));
       
       QObject::connect(GuiMain->actionExtract_all_single_image, SIGNAL(triggered()), GuiMain->view, SLOT(ExtractImages()));
       
       QObject::connect(GuiMain->actionLoad_Demo_Page, SIGNAL(triggered()), this, SLOT(InsertDemo()));
       
       
       QObject::connect(GuiMain->actionAppend_a_page, SIGNAL(triggered()), GuiMain->view, SLOT(AppendOnePage()));
       QObject::connect(GuiMain->actionRemove_last_page, SIGNAL(triggered()), GuiMain->view, SLOT(RemoveOnePage()));
       
       
       QObject::connect(GuiMain->actionSet_Rotate_0, SIGNAL(triggered()), GuiMain->view, SLOT(RotateReset()));
       
       QObject::connect(GuiMain->actionExport_to_XHTML_Layer, SIGNAL(triggered()), GuiMain->view, SLOT(HtmlExport()));
       
       
       
       
       QObject::connect(GuiMain->actionFont_base_setting, SIGNAL(triggered()), GuiMain->view, SLOT(FontSetting()));
       QObject::connect(GuiMain->actionPage_universal_Font, SIGNAL(triggered()), GuiMain->view, SLOT(PageFontSetting()));
       
       
    
       /* print */
       QObject::connect(GuiMain->actionPrint_document, SIGNAL(triggered()), GuiMain->view, SLOT(NormalPrint()));
       QObject::connect(GuiMain->actionExport_to_Postcript, SIGNAL(triggered()), GuiMain->view, SLOT(PrintPostcript()));
       QObject::connect(GuiMain->actionExport_to_PDF, SIGNAL(triggered()), GuiMain->view, SLOT(MakefilePrintPdf()));
       QObject::connect(GuiMain->actionApplication_info, SIGNAL(triggered()), GuiMain, SLOT(AboutApps()));
       
       QObject::connect(GuiMain->actionSave_doc, SIGNAL(triggered()), this, SLOT(SaveFile()));
        QObject::connect(GuiMain->actionSave_doc_as, SIGNAL(triggered()), this, SLOT(SaveAsUserInputFile()));
        
        QObject::connect(GuiMain->action_New_FOP_doc, SIGNAL(triggered()), this, SLOT(NewFopDocument()));
        
        QObject::connect(GuiMain->actionCheck_Update, SIGNAL(triggered()), this, SLOT(ChecUpdateApps()));
        QObject::connect(GuiMain->actionAutor_Home_Page, SIGNAL(triggered()), this, SLOT(AutorHP()));
        
        
        QObject::connect(GuiMain->actionNew_Page_format, SIGNAL(triggered()), this, SLOT(NewPageFormatten()));
     
        
        
        
    
    
        QMenu *LMenu = new QMenu(GuiMain);
        LMenu->addMenu(GuiMain->menu_File);
        LMenu->addAction(GuiMain->actionBring_current_to_front);
        LMenu->addAction(GuiMain->actionSend_current_back);
        LMenu->addAction(GuiMain->actionSet_Rotate_0);
        
        #ifdef Q_WS_MAC
        CreateMacDockMenu();
        #endif
        
        LMenu->addSeparator ();
        LMenu->addAction(GuiMain->actionLock_current_item);
        LMenu->addAction(GuiMain->actionUnlock_current_item);
        LMenu->addSeparator();
        LMenu->addAction(GuiMain->actionAdvanced_edit);
        LMenu->addSeparator ();
        LMenu->addAction(GuiMain->actionAppend_a_new_Layer);
        LMenu->addAction(GuiMain->actionClone_current_Layer_to_new);
        LMenu->addSeparator ();
        LMenu->addAction(GuiMain->actionRemote_current);   /* remove */
        LMenu->addSeparator();
        LMenu->addAction(GuiMain->actionApplication_info);
        LMenu->addMenu(GuiMain->menuExport_format);
        LMenu->addMenu(GuiMain->menuPage);
        GuiMain->view->SetMenuExpandet(LMenu);
   
    
        GuiMain->show();
        
        
        const QStringList args = arguments();
        QStringList files;
        
        for (int i = 1; i < args.count(); ++i)  {
        const QString argument = args.at(i);
        std::cout << "argument nr. " << i << "[" << qPrintable(argument) << "]" << std::endl;
                if (argument == QLatin1String("-pdf")) {
                /* print to pdf  */
                    getpdf = true;
                } else if (!files.contains(argument)) {
                        QFileInfo pi(argument);
                        if (pi.completeSuffix() == "pdf") {
                        getpdf = true;
                        maketopdf = argument;
                        ////////////qDebug() << "### maketopdf  " << maketopdf;
                        } else {
                        files.append(argument);
                        }
                }
        }
        foreach (QString file, files) {
            //////////qDebug() << "### open args  " << file;
            OpenFile(file);
        }
        
        if (!getpdf) {
                    if (WORK_FILE_CURRENT.isEmpty())  {
                     GuiMain->action_New_FOP_doc->activate ( QAction::Trigger ); 
                    }
        }
}
    


protected:
    Gui_Main *GuiMain;
    QSettings setter;
    QString osfile;
    QString maketopdf;
    QString WORK_FILE_CURRENT;
    QString HUMANNAME_FILE;
    Fop_Handler *Fop_open;
    Fop_Handler *Fop_save;
    bool FileSend;
    PageDB *db;
    bool getpdf;
#ifdef Q_WS_MAC
void CreateMacDockMenu()
{
               QMenu *macdocks = new QMenu();
               macdocks->addAction(tr("&Quit"),this, SLOT(CleanProcess()));
               macdocks->addSeparator();
               macdocks->addAction(GuiMain->actionSave_doc);
               macdocks->addAction(GuiMain->actionExport_to_PDF);
               macdocks->addAction(GuiMain->actionExport_to_Postcript);
               macdocks->addAction(GuiMain->actionApplication_info);
               macdocks->addAction(GuiMain->actionLoad_Demo_Page);
               macdocks->addAction(GuiMain->actionCheck_Update);
               macdocks->addAction(GuiMain->actionAutor_Home_Page);
              qt_mac_set_dock_menu(macdocks);
}
#endif
bool event(QEvent *ev)
{
    if (ev->type() == QEvent::FileOpen) {
        osfile = static_cast<QFileOpenEvent *>(ev)->file();
        OpenFile(osfile);
        return true;
    }
    return QApplication::event(ev);
}
private:
public slots:  
void AutorHP() 
{
GuiMain->view->OpenDesktop( QUrl ("http://ciz.ch/") );     
}



        
void ChecUpdateApps() 
{
GuiMain->view->OpenDesktop( QUrl("http://code.google.com/p/fop-miniscribus/") );
}


void NewFopDocument()
{
    WORK_FILE_CURRENT = "";
    GuiMain->setWindowTitle( _PROGRAM_TITLE_ );
    GuiMain->view->NewDocumentSet();
    if (sender()) {
              QAction *textAction = qobject_cast<QAction *>(sender());
                qDebug() << "### NewFopDocument " << textAction->objectName();
                if (textAction->objectName() == "action_New_FOP_doc" && getGSVersion().size() > 3) {
                    int removeyes = QMessageBox::question(0, tr("Confirm please."),tr("Insert a new Text Layer?."),
                                                                        tr("&Yes"), tr("&No"),
                                                                         QString(),8888,9999);
                    
                        if (removeyes == 0) {
                            GuiMain->view->AppendLayerInside();
                            SaveFile();
                        }
                    
                }
    
    }
    
    
    
}
void SaveAsUserInputFile()
{
     QString fileName = QFileDialog::getSaveFileName(0, "Save FOP file",QString(GuiMain->setter.value("LastDir").toString()), "*.fop");
            if (fileName.size() > 1) {
                    if (!fileName.endsWith(".fop",Qt::CaseInsensitive)) {
                    fileName = fileName+".fop"; 
                    }
                 SaveOnFileFix(fileName);   
            }
}
void SaveOnFileFix( const QString file ) 
{
     QFileInfo fi(file);
     WORK_FILE_CURRENT = file;
     HUMANNAME_FILE = fi.fileName();
     GuiMain->setWindowTitle(QString("%1 - %2").arg( _PROGRAM_TITLE_ ).arg(HUMANNAME_FILE) );
     GuiMain->setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
     GuiMain->view->SaveOn(file);
}    
void SaveFile()
{
    QFileInfo fi(WORK_FILE_CURRENT);
    if (fi.exists ()) {
    SaveOnFileFix(WORK_FILE_CURRENT);
    } else {
    SaveAsUserInputFile();
    }
}


void OpenFopFile()
{
   ////////// qDebug() << "### setter " << GuiMain->setter.value("LastDir").toString();
    QString im = QFileDialog::getOpenFileName(0,tr("Open Fop File Layer typ block-container"),GuiMain->setter.value("LastDir").toString(),"Fop file (*.xml *.fo *.fop *.fop.gz)");
    if (im.size() > 0) {
        GuiMain->setter.setValue("LastDir",im.left(im.lastIndexOf("/"))+"/");
        if (!WORK_FILE_CURRENT.isEmpty())  {
            QFileInfo fi(WORK_FILE_CURRENT);
            int removeyes = QMessageBox::question(0, tr("Confirm please."),tr("Remove data from file %1.... Say no if you like merge/union document and layer.").arg(fi.fileName()),
                                                            tr("&Yes"), tr("&No"),
                                                             QString(),8888,9999);
            
            if (removeyes == 0) {
              NewFopDocument();
            }
            
        }
        
        OpenFile(im);
    }
}

void InsertDemo()
{
    NewFopDocument();
    Fop_open = new Fop_Handler(QString::fromUtf8(":/img/FirstDocument.fop"),true,db,GuiMain);
    QObject::connect(Fop_open, SIGNAL(ConnectList(bool)), this, SLOT(RenderPage(bool)));
}

void OpenFile( const QString file ) 
{ 
    QFileInfo fi(file);
    if (fi.completeSuffix() == "fop" || 
        fi.completeSuffix() == "fo" || 
        fi.completeSuffix() == "fop.gz" ||
        fi.completeSuffix() == "xml")  {
            if (fi.exists ()) {
                    if (!getpdf) {
                    WORK_FILE_CURRENT = file;
                    HUMANNAME_FILE = fi.fileName();
                    } else {
                    HUMANNAME_FILE = "tempfile.fop"; 
                    }
                    GuiMain->setWindowTitle(QString("%1 - %2").arg( _PROGRAM_TITLE_ ).arg(HUMANNAME_FILE) );
                if (!getpdf) {
                GuiMain->setter.setValue("LastDir",fi.absolutePath()+"/");
                }
                ////////////qDebug() << "### setter " << GuiMain->setter.value("LastDir").toString();
            Fop_open = new Fop_Handler(file,true,db,GuiMain);
            QObject::connect(Fop_open, SIGNAL(ConnectList(bool)), this, SLOT(RenderPage(bool)));
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            }                
    }
}
void RenderPage(bool e )  {
    /////////qDebug() << "### Main ErrorSize ->" << Fop_open->ErrorSize();
    /////qDebug() << "### Main RenderPage ->" << e;
    FileSend = e;
    QString errorMg;
    
    if (Fop_open->ErrorSize()) {
        
        TypErnoMap  errori =  Fop_open->GetError();
        QMapIterator<int,QString> i(errori);
         while (i.hasNext()) {
             i.next();
             QString line = QString("Nr. %1 ->").arg(i.key());
             line.append(i.value()+"\n");
             errorMg +=line;
         } 
         if (errorMg.contains("Unable to find a fo:block-container",Qt::CaseInsensitive)) {
             errorMg.prepend(QObject::tr("Your file not having fo:block-container. and now is render all inside a new block-container.\nAppend page to view all.\n"));
         }
         
                
    }
    
    TypnotexistFontMap notfoundfont = Fop_open->FontError();
    
    if (notfoundfont.size() > 0) {
        errorMg.append(QObject::tr("Unable to find the follow Font on your System:\n"));
    }
    
    QMapIterator<QString,QFont> fo(notfoundfont);
    
    while (fo.hasNext()) {
           fo.next();
        errorMg.append(QObject::tr("Font \"%1\" replace by system to %2...\n").arg(fo.key()).arg(fo.value().family()));
    }
    
    if (errorMg.size() > 0) {
        QApplication::restoreOverrideCursor();
        
    QMessageBox infoset;
                infoset.setWindowTitle(QObject::tr("Error found on document!") );
                infoset.setText ( errorMg );
                infoset.exec(); 
    }        
    
    if (!getpdf) {
    GuiMain->view->PaintPage(Fop_open);
        std::cout << "## ohne pdf " << std::endl;
        QApplication::restoreOverrideCursor();
    } else if (getpdf && maketopdf.size() > 0)  {
        std::cout << "## mit pdf " << std::endl;
    GuiMain->view->PaintPage(Fop_open,maketopdf);
        QApplication::restoreOverrideCursor();
    }
} 
void NewPageFormatten()
{
   
std::cout << "NewPageFormatten->[0]" << std::endl;
    
}
void CleanProcess() { 
std::cout << "CleanProcess->[1]" << std::endl;
GuiMain->close();
GuiMain->deleteLater();
quit();
}
    
    
};







int main(int argc, char *argv[]) {
     Q_INIT_RESOURCE(osApplication);
    OS_application a( argc, argv );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( CleanProcess() ) );
    return a.exec();
}
#include "main.moc"








