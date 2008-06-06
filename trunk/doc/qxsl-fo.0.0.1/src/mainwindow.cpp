// own include
#include "mainwindow.h"

#include <getopt.h>

// qt include
#include <QtGui>
#include <QtCore>

// project include

//fo part
#include "objectfosimpledoc.h"
#include "objectfosequenceddoc.h"
#include "objectfocontainersimpledoc.h"
#include "objectfodoc.h"

//dom&xml
#include "dommodel.h"

//main
#include "maindoc.h"
#include "mainview.h"
#include "graphicsview.h"

#include "fotextedit.h"
#include "maindoc.h"
#include "itemtoolboxdoc.h"
#include "itempropertiesdoc.h"
#include "itemhierarchytreedoc.h"
#include "itempropertiesdelegate.h"
#include "codeview.h"
#include "highlighterfocode.h"
#include "domitemattributesdoc.h"
//#include "fotableview.h"

#include "pageruler.h"

MainWindow::MainWindow(int argc, char *argv[])
{
	readSettings();
	
	/*xml tree widget*/
	/*****************/
	m_pDomTreeView = new QTreeView(this);
	m_pDomTreeView->setDragEnabled(true);
	m_pDomTreeView->setAcceptDrops(true);
	m_pDomTreeView->setDropIndicatorShown(true);
	m_pDomModel = new DomModel(QDomDocument(), this);
	m_pDomTreeView->setModel(m_pDomModel);
	
	m_pDomItemAttributesView = new QTableView(this);
	DomItemAttributesDoc *pDomItemAttributesDoc=new DomItemAttributesDoc;
	m_pDomItemAttributesView->setModel(pDomItemAttributesDoc);
	m_pDomItemAttributesView->verticalHeader()->hide();
	m_pDomItemAttributesView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);//resizeMode must be after setModel
	
	connect(m_pDomTreeView,SIGNAL(clicked (QModelIndex)),pDomItemAttributesDoc,SLOT(slItemSelected (QModelIndex)));
	
	/*item properties view*/
	/**********************/
	m_pItemPropertiesView = new QTableView;
	m_pItemPropertiesDoc = new ItemPropertiesDoc(this);
	//m_pDomItemAttributesView->setModel(m_pItemPropertiesDoc);
	m_pItemPropertiesView->setModel(m_pItemPropertiesDoc);
	m_pItemPropertiesView->verticalHeader()->hide();
	m_pItemPropertiesView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);//resizeMode must be after setModel
	ItemPropertiesDelegate *delegate=new ItemPropertiesDelegate;
	m_pItemPropertiesView->setItemDelegate(delegate);
	
	/*item toolbox widget view*/
	/**************************/
	m_pItemToolBoxView = new QListView(this);
	m_pItemToolBoxDoc = new ItemToolBoxDoc();
	m_pItemToolBoxView->setDragEnabled(true);
	m_pItemToolBoxView->setModel(m_pItemToolBoxDoc);
	
	/*item hierarchy tree view*/
	/**************************/
	m_pItemHierarchyTreeView = new QTreeView(this);
	m_pItemHierarchyTreeView->header()->hide();
	m_pItemHierarchyTreeDoc = new ItemHierarchyTreeDoc(this);
	m_pItemHierarchyTreeView->setModel(m_pItemHierarchyTreeDoc);
	connect(m_pItemHierarchyTreeView,SIGNAL(clicked (QModelIndex)),m_pItemHierarchyTreeDoc,SLOT(slClicked (QModelIndex)));
	
	/*create fotexteditor*/
	/*********************/
	m_pFoTextEdit = new FoTextEdit(this);
	
	/*code view*/
	/***********/
	m_pCodeView = new CodeView();
	new HighlighterFoCode(m_pCodeView->document());
	
	//create main doc, we need created befor UndoView use his pointer 
	m_pMainDoc=new MainDoc();//maindoc need handle propertydoc to show properties 
	
	/*undo command*/
	/**************/
	m_pUndoView = new QUndoView();
	m_pUndoGroup = new QUndoGroup(this);
	m_pUndoView->setGroup(m_pUndoGroup);//connect undogroup with view
	m_pUndoGroup->setActiveStack(m_pMainDoc->undoStack());//connect undo group with undo stack

	createMainView(m_pMainDoc);
	
	createActions();
	createMenus();
	createToolBars();
	createDockWindows();
	createStatusBar();

	/*process command line arguments*/
	initCliArgs(argc,argv);
}

MainWindow::~MainWindow()
{
	delete m_pMainDoc;
	//delete m_pItemToolBoxDoc;
}

/////////////////////////////////////////////////////////////////////////////
////////////methods for CREATE (actions,menu,toolbats, etc) ////////////////
/////////////////////////////////////////////////////////////////////////////

void MainWindow::createMainView(MainDoc *pMainDoc)
{
	//main central widget
	m_pMainView = new MainView(this,pMainDoc);
	
	
	//create left and top part 
	m_pLeftRuler=new PageRuler(PageRuler::leftPosition,m_pMainView);//left
	m_pTopRuler=new PageRuler(PageRuler::topPostion,m_pMainView);
		
	//connect doc and view
	QObject::connect(m_pMainDoc,SIGNAL(createPage(PageDoc*)),m_pMainView,SLOT(createPage(PageDoc*)));
	
	//create group box
	QGroupBox *pMainBox = new QGroupBox();
	QGridLayout *pMainLayout = new QGridLayout;
	
	//and add widgets to groupbox
	pMainLayout->addWidget(m_pMainView->graphicsView(),1,1);
	pMainLayout->addWidget(m_pLeftRuler,1,0);
	pMainLayout->addWidget(m_pTopRuler,0,1);
	
	pMainBox->setLayout(pMainLayout);
	setCentralWidget(pMainBox);
}

void MainWindow::createActions()
{
	/////////About/////////////////////////////////////
	m_pAboutQtAct = new QAction(tr("About &Qt"), this);
	m_pAboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(m_pAboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	//////////File/////////////////////////////////////
	m_pQActOpenXmlFile = new QAction(QIcon(":/images/file_open.png"),tr("Load xml file"), this);
	m_pQActOpenXmlFile->setStatusTip(tr("Load xml file"));
	connect(m_pQActOpenXmlFile, SIGNAL(triggered()), this, SLOT(openXmlFile()));
	m_pQActOpenXslFoFile = new QAction(QIcon(":/images/file_open.png"),tr("Load xsl-fo file"), this);
	m_pQActOpenXslFoFile->setStatusTip(tr("Load xsl-fo file"));
	connect(m_pQActOpenXslFoFile, SIGNAL(triggered()), this, SLOT(openXslFoFile()));
	m_pQActSaveFoFile = new QAction(QIcon(":/images/file_save.png"),tr("Save fo file"), this);
	m_pQActSaveFoFile->setStatusTip(tr("Save fo file"));
	connect(m_pQActSaveFoFile, SIGNAL(triggered()), this, SLOT(saveFoFile()));

	//////Undo Redo///////////////////////////////////////
	m_pQActUndo = new QAction(QIcon(":/images/undo.png"),tr("Undo"), this);
	m_pQActUndo->setStatusTip(tr("Undo"));
	connect(m_pQActUndo, SIGNAL(triggered()), m_pMainDoc, SLOT(undo()));	
	m_pQActRedo = new QAction(QIcon(":/images/redo.png"),tr("Redo"), this);
	m_pQActRedo->setStatusTip(tr("Redo"));
	connect(m_pQActRedo, SIGNAL(triggered()), m_pMainDoc, SLOT(redo()));
	
	//////Zoom////////////////////////////////////////
	m_pQActZoomIn = new QAction(QIcon(":/images/zoom_in.png"),tr("Zoom &in"), this);
	//m_pQActZoomIn->setShortcut(tr("Ctrl+i"));
	m_pQActZoomIn->setStatusTip(tr("Zoom in"));
	connect(m_pQActZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
	m_pQActZoomOut = new QAction(QIcon(":/images/zoom_out.png"),tr("Zoom out"), this);
	m_pQActZoomOut->setStatusTip(tr("Zoom out"));
	connect(m_pQActZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
	
	////////Table resize//////////////////////////////////////	
	m_pQActAddRow=new QAction(QIcon(":/images/insert_table_row.png"), tr("Add row"),this);
	connect(m_pQActAddRow, SIGNAL(triggered()), this, SLOT(slAddTableRow()));
	m_pQActAddColumn=new QAction(QIcon(":/images/insert_table_col.png"), tr("Add column"),this);
	connect(m_pQActAddColumn, SIGNAL(triggered()), this, SLOT(slAddTableCol()));
	m_pQActDeleteRow=new QAction(QIcon(":/images/delete_table_row.png"), tr("Remove row"),this);
	connect(m_pQActDeleteRow, SIGNAL(triggered()), this, SLOT(slDelTableRow()));
	m_pQActDeleteColumn=new QAction(QIcon(":/images/delete_table_col.png"), tr("Remove column"),this);
	connect(m_pQActDeleteColumn, SIGNAL(triggered()), this, SLOT(slDelTableCol()));
	
	//connect(qActEdit, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	m_pFileMenu = menuBar()->addMenu(tr("&File"));
	m_pFileMenu->addAction(m_pQActOpenXmlFile);
	m_pFileMenu->addAction(m_pQActOpenXslFoFile);
	m_pFileMenu->addAction(m_pQActSaveFoFile);

	m_pEditMenu = menuBar()->addMenu(tr("&Edit"));
	m_pEditMenu->addAction(m_pQActUndo);
	m_pEditMenu->addAction(m_pQActRedo);

	m_pViewMenu = menuBar()->addMenu(tr("&View"));
	m_pViewMenu->addAction(m_pQActZoomIn);
	m_pViewMenu->addAction(m_pQActZoomOut);

	m_pItemMenu = menuBar()->addMenu(tr("&Item"));
	m_pItemMenu->addAction(m_pQActAddRow);
	m_pItemMenu->addAction(m_pQActDeleteRow);
 	m_pItemMenu->addSeparator();
	m_pItemMenu->addAction(m_pQActAddColumn);
	m_pItemMenu->addAction(m_pQActDeleteColumn);
	
	m_pHelpMenu = menuBar()->addMenu(tr("&Help"));
	m_pHelpMenu->addAction(m_pAboutQtAct);

}

void MainWindow::createToolBars()
{
	m_pToolBarView = addToolBar("View Toolbar");
	m_pToolBarView->addAction(m_pQActZoomIn);
	m_pToolBarView->addAction(m_pQActZoomOut);
	
	m_pToolBarUndo = addToolBar("Undo Toolbar");
	m_pToolBarUndo->addAction(m_pQActUndo);
	m_pToolBarUndo->addAction(m_pQActRedo);

}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
	//////////////////////////////////////////////
	////left dock
	//////////////////////////////////////////////
	
	QDockWidget *dockXMLTree = new QDockWidget(tr("Xml Tree"), this);
	dockXMLTree->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_pDomTreeView);
	vbox->addWidget(m_pDomItemAttributesView);
	vbox->setContentsMargins(0,0,0,0);
	
	QGroupBox *groupBox = new QGroupBox();
	groupBox->setLayout(vbox);

	dockXMLTree->setWidget(groupBox);
	addDockWidget(Qt::LeftDockWidgetArea, dockXMLTree);
	////////////////////////	
	QDockWidget *dockToolBoxOfItems = new QDockWidget(tr("Item Box"), this);
	dockToolBoxOfItems->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	dockToolBoxOfItems->setWidget(m_pItemToolBoxView);
	addDockWidget(Qt::LeftDockWidgetArea, dockToolBoxOfItems);
	////////////////////////	
	QDockWidget *dockTreeOfItems = new QDockWidget(tr("Items Hierarchy"), this);
	dockTreeOfItems->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	dockTreeOfItems->setWidget(m_pItemHierarchyTreeView);
	addDockWidget(Qt::LeftDockWidgetArea, dockTreeOfItems);
	
	tabifyDockWidget(dockXMLTree,dockToolBoxOfItems);
	tabifyDockWidget(dockXMLTree,dockTreeOfItems);
	//////////////////////////////////////////////
	////right dock
	//////////////////////////////////////////////
	
	QDockWidget *dockProperties = new QDockWidget(tr("Item Properties"), this);
	dockProperties->setAllowedAreas(Qt::RightDockWidgetArea | Qt::RightDockWidgetArea);
	dockProperties->setWidget(m_pItemPropertiesView);
	addDockWidget(Qt::RightDockWidgetArea, dockProperties);
	
	QDockWidget *dockUndo = new QDockWidget(tr("Commands Info"), this);
	dockUndo->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	dockUndo->setWidget(m_pUndoView);
	addDockWidget(Qt::RightDockWidgetArea, dockUndo);
	
//	tabifyDockWidget(dockXMLTree,dockProperties);
	
	//////////////////////////////////////////////
	////bottom dock
	//////////////////////////////////////////////

	m_pDockCodeView = new QDockWidget(tr("Xsl-Fo Code"), this);
	m_pDockCodeView->setAllowedAreas(Qt::BottomDockWidgetArea);
	m_pDockCodeView->setWidget(m_pCodeView);
	addDockWidget(Qt::BottomDockWidgetArea, m_pDockCodeView);
	
	m_pDockFoTextEdit = new QDockWidget(tr("FoText Editor"), this);
	m_pDockFoTextEdit->setAllowedAreas(Qt::BottomDockWidgetArea);
	m_pDockFoTextEdit->setWidget(m_pFoTextEdit);
	addDockWidget(Qt::BottomDockWidgetArea, m_pDockFoTextEdit);
	
	tabifyDockWidget(m_pDockFoTextEdit,m_pDockCodeView);
}

void MainWindow::showDockFoTextEdit()
{
	QMainWindow::tabifyDockWidget(m_pDockCodeView,m_pDockFoTextEdit);
	m_pFoTextEdit->setFocus();
}

void MainWindow::initCliArgs( int argc, char *argv[])
{
	int next_option;

	/* A string listing valid short options letters.  */
	const char* const short_options = ":x:f";
	/* An array describing valid long options.  */
	const struct option long_options[] = 
	{
		{	"xml",		1,	NULL,	'x'	},
		{	"xslfo",	1,	NULL,	'f'	},
		{	NULL,		0,	NULL,	0	}   /* Required at end of array.  */
	};
	/* The name of the file to  */
	//const char* fileName = NULL;
	do {
		next_option = getopt_long (argc, argv, short_options,long_options, NULL);
		switch (next_option)
		{
			case 'x':
				loadXmlFile(optarg);//open specified xml document
				break;
			case 'f':
				m_pMainDoc->openDocumentFile(optarg);//open specified xsl-fo document
				loadFoCode();//init codeview
				setWindowTitle(m_pMainDoc->currentXslFoFileName());
				break;
			case -1:    /* Done with options.  */
				break;	
			default:    /* Something else: unexpected.  */
				qFatal("Something si wrong with command line arguments!");
		}
	}
	while (next_option != -1);	
}

/////////////////////////////////////////////////////////////////////////////
////////////menu slots///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void MainWindow::about()
{
	QMessageBox::about(this, tr("About qxsl-fo"),tr("http://sourceforge.net/projects/qxsl-fo/"));
}

void MainWindow::openXmlFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"),"./data/",tr("xml (*.xml)"));
	
	if (fileName.isEmpty())
		return;
		
	loadXmlFile(fileName);
}

void MainWindow::openXslFoFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open XslFo File"),"./data/",tr("xsl-fo (*.xsl)"));

	if (fileName.isEmpty())
		return;

	if (m_pMainDoc->openDocumentFile(fileName)==true)
	{
		loadFoCode();
		setWindowTitle(m_pMainDoc->currentXslFoFileName());
	}
}

void MainWindow::saveFoFile()
{
	m_pMainDoc->saveDocumentFile("default.fo");
}

void MainWindow::zoomIn()
{
	m_pMainView->scale(1.2, 1.2);
}

void MainWindow::zoomOut()
{
	m_pMainView->scale(1/1.2, 1/1.2);
}

void MainWindow::loadFoCode()
{
	m_pCodeView->load(m_pMainDoc->toString());
}

///////////////////////////////////
void MainWindow::slAddTableRow()
{
	m_pMainView->menuQActionTableResize(FoTableView::addRow);
}

void MainWindow::slAddTableCol()
{
	m_pMainView->menuQActionTableResize(FoTableView::addCol);
}

void MainWindow::slDelTableRow()
{
	m_pMainView->menuQActionTableResize(FoTableView::delRow);
}
void MainWindow::slDelTableCol()
{
	m_pMainView->menuQActionTableResize(FoTableView::delCol);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void MainWindow::setWindowTitle(QString fileName)
{
	QMainWindow::setWindowTitle(QString("QXsl-Fo file:%1").arg(fileName));
}

void MainWindow::loadXmlFile(QString fileName)
{
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		QDomDocument doc;
		if (doc.setContent(&file))
		{
			DomModel *newModel = new DomModel(doc, this);
			m_pDomTreeView->setModel(newModel);
			delete m_pDomModel;
			m_pDomModel = newModel;
		}
		file.close();
	}
}
void MainWindow::clearXslFoEditView()
{
	m_pFoTextEdit->clear();
}

void MainWindow::updatePropertiesView(ObjectFoSequencedDoc *pFoObjectSequenced)
{
	m_pItemPropertiesDoc->UpdatePagePropertiesView(pFoObjectSequenced);
}

void MainWindow::updatePropertiesView(ObjectFoSimpleDoc *pFoObjectSimple)
{
	m_pItemPropertiesDoc->UpdatePagePropertiesView(pFoObjectSimple);
}

void MainWindow::updatePropertiesView(ObjectFoContainerSimpleDoc *pObjectFoContainerSimpleDoc)
{
	m_pItemPropertiesDoc->UpdatePagePropertiesView(pObjectFoContainerSimpleDoc);
}

void MainWindow::updateItemHierarchyView(PageDoc *pPageDoc)
{
	m_pItemHierarchyTreeDoc->setPageDoc(pPageDoc);
}

void MainWindow::updateXslFoEditView(ObjectFoSimpleDoc *pFoObjectSimple)
{
	m_pFoTextEdit->setFoObjectSimple(pFoObjectSimple);
}

void MainWindow::closeEvent(QCloseEvent */*event*/)
{
	writeSettings();
}

void MainWindow::readSettings()
 {
	QSettings settings("QXslFo", "QXslFo");
	QPoint pos = settings.value("pos", QPoint(0, 0)).toPoint();
	QSize size = settings.value("size", QSize(800, 400)).toSize();
	resize(size);
	move(pos);
 }

 void MainWindow::writeSettings()
 {
	QSettings settings("QXslFo", "QXslFo");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
 }

void MainWindow::setRulersForPage(QGraphicsRectItem *pQGraphicsRect)
{
	m_pTopRuler->setRulerPosition(pQGraphicsRect);
	m_pLeftRuler->setRulerPosition(pQGraphicsRect);
}
