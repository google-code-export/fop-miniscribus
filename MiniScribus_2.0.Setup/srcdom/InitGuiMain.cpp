#include "InitGuiMain.h"


Gui_Main::Gui_Main( QWidget* parent )
	: QMainWindow( parent ),modus(1)
{
	setter.setValue("wheel/zoom",0);
	setter.setValue("gview/wi",500);
	setter.setValue("gview/hi",800);
	setter.setValue("CurrentRunningUnit","mm");
	unitas = setter.value("CurrentRunningUnit").toString();
  const QString usersetfont = setter.value("DefaultFontsW").toString();
	
        if (usersetfont.size() > 0 ) {
            QFont settfont;
            settfont.fromString(usersetfont);
            QApplication::setFont(settfont);
        }
   
	MainUpdate();
  setMinimumSize ( avaiable );
	work = new WorkPanel(this);
	work->setScrollBarsEnabled( true );
  setCentralWidget(work); 
  OpenPanels();  /* panel to a null page to set */
  Load_Toolbar(modus);
}


void Gui_Main::OpenPanels() 
{
    Opage = new PageXmlHandler();
    Pattribut_tree = new PageFirstTree(Opage);
    pattributes = new QDockWidget(this);
    ///////AttLabel = new QLabel("   "+Opage->current_title());
    /////////pattributes->setTitleBarWidget ( AttLabel ); 
    pattributes->setAllowedAreas( Qt::LeftDockWidgetArea |  Qt::RightDockWidgetArea);
    pattributes->setMaximumWidth(450);
    pattributes->setMinimumWidth(210);
    pattributes->setWidget(Pattribut_tree);
		addDockWidget(Qt::LeftDockWidgetArea,pattributes); 
    actionAttributeSetter = pattributes->toggleViewAction();
    actionAttributeSetter->setIcon ( QIcon(":/img/view_choose.png") );
    connect(Opage, SIGNAL(updated() ), this , SLOT(update_attributes() ));
   
    xml_editor = new XMLTextEdit(this);
    xml_editor->setPlainText(Opage->doc_b.toString(1));
    connect(Opage, SIGNAL(datachanged(QString) ), xml_editor , SLOT(setPlainText(QString) ));
   
   
    xmlcontroll = new QDockWidget(this);
    xmlcontroll->setAllowedAreas( Qt::LeftDockWidgetArea |  Qt::RightDockWidgetArea);
    xmlcontroll->setWidget(xml_editor);
		addDockWidget(Qt::LeftDockWidgetArea,xmlcontroll); 
    
    actionXmlCheckers = xmlcontroll->toggleViewAction();
    actionXmlCheckers->setIcon ( QIcon(":/img/arts-environment-48x48.png") );
    
}

void Gui_Main::update_attributes()
{
   Msg( tr("Update page ...")+ " / " +Opage->current_title());
   Pattribut_tree->swapdevice(Opage);
   ////////AttLabel->setText ("   "+Pattribut_tree->windowTitle());
}

void Gui_Main::Msg( const QString txt ) 
{
    statuslabel->setText("  "+txt);
}

bool Gui_Main::event(QEvent *ev)
{
    bool confirm = false;
	
	  /////////////qDebug() << "### ev tipe  " << ev->type();
	
    if (ev->type() == 112) {
    const QString statustxt = static_cast<QStatusTipEvent *>(ev)->tip();
    Msg(statustxt);
        confirm = true;
    }
    if (!confirm) {
    return QMainWindow::event(ev);
    } else {
    ev->setAccepted(true); 
    return confirm; 
    }        
}

void Gui_Main::MainUpdate()
{
   QDesktopWidget *desk = QApplication::desktop();
   QRect winsize = desk->screenGeometry();
   avaiable = QSize(desk->screenGeometry().width() - 250,desk->screenGeometry().height() - 400);
}


void Gui_Main::Load_Toolbar( const int modus)
{
	 toolBar_0 = new QToolBar;
   toolBar_1 = new QToolBar;
   toolBar_2 = new QToolBar;   /* left */
   toolBar_3 = new QToolBar;
   toolBar_4 = new QToolBar;
	 toolBar_pages = new QToolBar;
	 addToolBar(Qt::TopToolBarArea, toolBar_pages);
   addToolBar(Qt::TopToolBarArea, toolBar_0);
   addToolBar(Qt::TopToolBarArea, toolBar_1);
   addToolBarBreak(Qt::TopToolBarArea); 
	 addToolBar(Qt::LeftToolBarArea, toolBar_2);
   addToolBar(Qt::TopToolBarArea, toolBar_3);
   addToolBar(Qt::TopToolBarArea, toolBar_4);
   
   
   toolBar_0->addAction ( actionAttributeSetter );
   toolBar_0->addAction ( actionXmlCheckers );
   
	
	
		qDebug() << "### Load_Toolbar  " << modus;
    swapedit = new QAction(tr("Edit current layer"),this);
    swapedit->setIcon(QIcon(QString::fromUtf8(":/img/icon.png")));
	  swapedit->setStatusTip ( tr("Edit current layer if active") );
	  swapedit->setCheckable(true);
    toolBar_0->addAction(swapedit); 
    scalesett = new QWidget;
	  scalesett->setContentsMargins (0,0,0,0);
    QGridLayout *grid_6 = new QGridLayout(scalesett);
    grid_6->addWidget(new QLabel(tr("Zoom:"),xysett),0,0);
    comboBox = new QComboBox(scalesett);
    QStringList scales;
    scales << QString("45%") << QString("55%") << QString("65%") << QString("75%") << QString("85%")<< QString("95%")  << QString("100%") << QString("110%") << QString("120%") << QString("125%") << QString("150%") << QString("200%") << QString("300%") << QString("400%");
		comboBox->addItems (scales); 
		comboBox->setCurrentIndex(6);
    grid_6->addWidget(comboBox,0,1);
    comboBox->setStatusTip ( tr("Zoom in or out wiever") );
    
    xysett = new QWidget;
		xysett->setContentsMargins (0,0,0,0);
    QGridLayout *grid_5 = new QGridLayout(xysett);
		grid_5->setContentsMargins (0,0,0,0);
		
    const int wi = setter.value("gview/wi").toInt();
	  const int hi = setter.value("gview/hi").toInt();
		
		
    xxpos = new QDoubleSpinBox(xysett);
    xxpos->setDecimals(2);
    xxpos->setMinimum(-wi);
    xxpos->setMaximum(hi);
    
    yypos = new QDoubleSpinBox(xysett);
    yypos->setDecimals(2);
    yypos->setMinimum(-wi);
    yypos->setMaximum(hi);
    
    grid_5->addWidget(new QLabel(tr("X pos:"),xysett),0,0);
    grid_5->addWidget(xxpos,0,1);
    grid_5->addWidget(new QLabel(tr("Y pos:"),xysett),0,2);
    grid_5->addWidget(yypos,0,3);
    
    
    laywi = new QDoubleSpinBox(xysett);
    laywi->setDecimals(1);
    laywi->setMinimum(20);
    laywi->setMaximum(wi);
    
    layhi = new QDoubleSpinBox(xysett);
    layhi->setDecimals(1);
    layhi->setMinimum(20);
    layhi->setMaximum(hi);
    
    grid_5->addWidget(new QLabel(tr("Width:"),xysett),0,4);
    grid_5->addWidget(laywi,0,5);
    grid_5->addWidget(new QLabel(tr("Height:"),xysett),0,6);
    grid_5->addWidget(layhi,0,7);
    
    
    
    
    
    TextLayer *dummys = new TextLayer(0,0,0);
    QList<QAction *> actionlist = dummys->MainActions();
    tbutton[0] = new Pbutton(QString("undo")); // 0
    tbutton[1] = new Pbutton(QString("redo"));  // 1
    
    tbutton[2] = new Pbutton(QString("bold"));  // 2
    tbutton[3] = new Pbutton(QString("italic"));  // 3
    tbutton[4] = new Pbutton(QString("underline"));  // 4
    tbutton[5] = new Pbutton(QString("font"));   // 5
    tbutton[6] = new Pbutton(QString("backcolor"));  // 6
    tbutton[7] = new Pbutton(QString("textcolor"));  // 7
    tbutton[8] = new Pbutton(QString("margin")); // 8
    
    tbutton[9] = new Pbutton(QString("alleft")); // 9
    tbutton[10] = new Pbutton(QString("allright")); // 10
    tbutton[11] = new Pbutton(QString("allcenter")); // 11
    tbutton[12] = new Pbutton(QString("allblock")); // 12
    /* undo redo */
    toolBar_0->addWidget(tbutton[0]);
    toolBar_0->addWidget(tbutton[1]);
    toolBar_0->addSeparator();
    /* text formatting */
    toolBar_1->addWidget(tbutton[2]);
    toolBar_1->addWidget(tbutton[3]);
    toolBar_1->addWidget(tbutton[4]);
    toolBar_1->addWidget(tbutton[5]);
    toolBar_1->addSeparator();
    toolBar_1->addWidget(tbutton[6]);
    toolBar_1->addWidget(tbutton[7]);
    toolBar_1->addWidget(tbutton[8]);
    ///align /////
    toolBar_2->addWidget(tbutton[9]);
    toolBar_2->addWidget(tbutton[10]);
    toolBar_2->addWidget(tbutton[11]);
    toolBar_2->addWidget(tbutton[12]);
    /////toolBar_2->addSeparator();
    toolBar_3->addWidget(xysett);
    toolBar_4->addWidget(scalesett);
    
    Q_ASSERT ( actionlist.size() == NumComandButton ); 

     for (int i = 0; i < NumComandButton; ++i) {
        qDebug() << "### action  " << i;
        tbutton[i]->SetAction(actionlist[i]);
        tbutton[i]->setEnabled (false); 

    }
    
    xysett->setEnabled(false);
    scalesett->setEnabled(false);
		
		

		
		
    pstatus = new QWidget;
    pstatus->setMaximumHeight (38);
		pstatus->setContentsMargins (0,0,0,0);
    hboxLayout2 = new QHBoxLayout();
		hboxLayout2->setContentsMargins (0,0,0,0);
    statuslabel = new QLabel;
    statuslabel->setText(tr("   Loading...."));
    statuslabel->setMaximumWidth(300);
    statuslabel->setMinimumWidth(280);
    statuslabel_1 = new QLabel(tr("Search"));
    searchgui = new  QLineEdit; 
    searchgui->setMinimumHeight ( 18 );
		searchgui->setStatusTip ( tr("Search page") );
    boolemode = new QCheckBox;
    clocke = new DigitalClock;
    clocke->setMinimumWidth(100);
    //////clocke->setMaximumWidth(100);
    boolemode->setChecked(true);
    hboxLayout2->addWidget(statuslabel_1);
    hboxLayout2->addWidget(searchgui);
    hboxLayout2->addWidget(new QLabel(tr("Boolean yes/no")));
    hboxLayout2->addWidget(boolemode);
    stgrid = new QGridLayout(pstatus);   
    stgrid->addLayout(hboxLayout2, 0, 0);
    statusBar()->insertWidget(0,statuslabel,11);
    statusBar()->insertWidget(1,pstatus,22); 
    statusBar()->insertWidget(2,clocke,22); 
    ////////PaintSearchCompleter();
}
		
		

