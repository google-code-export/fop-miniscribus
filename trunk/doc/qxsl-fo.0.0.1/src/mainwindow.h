#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomDocument>

class ObjectFoSimpleDoc;
class ObjectFoSequencedDoc;
class ObjectFoContainerSimpleDoc;
class ObjectFoDoc;

class QAbstractItemModel;
class QTextEdit;
class QUndoView;
class QUndoGroup;
class QGraphicsView;
class QTreeView;
class QTableView;
class QListView;
class QGraphicsRectItem;

class PageDoc;
class FoTextEdit;
class DomModel;
class PageRuler;
class MainDoc;
class MainView;
class CodeView;
class FoTableView;
class DomItemAttributesDoc;

class ItemToolBoxDoc;
class ItemPropertiesDoc;
class ItemHierarchyTreeDoc;

/**
 * this class represent main window part, here are created all docked windows, toolbars and menus
 * also do processing CLI args
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	/**
	 * constructor
	 * @param argc 
	 * @param argv[] 
	 */
	MainWindow(int argc, char *argv[]);
	/**
	 * desturctor
	 */
	~MainWindow();
	/**
	 * open file fileName and load xml to DomModel
	 * @param fileName 
	 */
	void loadXmlFile(QString fileName);
	/**
	 * is called by qt framework, it is used to save window size
	 */
	void closeEvent(QCloseEvent *event);
	/**
	 * read saved window size (using QSetting) and set it
	 */
	void readSettings();
	/**
	 * write actual window size (using QSetting) to settings
	 */
	void writeSettings();
	/**
	 * set pFoObjectSequenced as a inspected object in dock window ItemPropertiesDoc
	 * @param pFoObjectSequenced 
	 */
	void updatePropertiesView(ObjectFoSequencedDoc *pFoObjectSequenced);
	/**
	 * set pFoObjectSimple as a inspected object in dock window ItemPropertiesDoc
	 * @param pFoObjectSimple 
	 */
	void updatePropertiesView(ObjectFoSimpleDoc *pFoObjectSimple);
	/**
	 * set pObjectFoContainerSimpleDoc as a inspected object in dock window ItemPropertiesDoc
	 * @param pObjectFoContainerSimpleDoc 
	 */
	void updatePropertiesView(ObjectFoContainerSimpleDoc *pObjectFoContainerSimpleDoc);
	/**
	 * currently not used
	 * @param pFoObjectSimple 
	 */
	void updateXslFoEditView(ObjectFoSimpleDoc *pFoObjectSimple);
	/**
	 * set pPageDoc as a root node and refresh dock window itemhierarchy
	 * @param pPageDoc 
	 */
	void updateItemHierarchyView(PageDoc *pPageDoc);
	/**
	 * currently not used
	 */
	void clearXslFoEditView();
	/**
	 * set position for rulers (two small rects on top and two small rects on left)
	 * @param pQGraphicsRect 
	 */
	void setRulersForPage(QGraphicsRectItem *pQGraphicsRect);
	/**
	 * currently not used
	 */
	void showDockFoTextEdit();
private slots:
	/**
	 * show dialog about 
	 */
	void about();
	/**
	 * open xml file and call loadxmlfile, slot is called from menu
	 */
	void openXmlFile();
	/**
	 * open and load xsl file, slot is called from menu
	 */
	void openXslFoFile();
	/**
	 * save fo file to default.fo
	 */
	void saveFoFile();
	/**
	 * do zoom in by 1.2
	 */
	void zoomIn();
	/**
	 * do zoom out by 1.2
	 */
	void zoomOut();
	/**
	 * add row to table (slot is inicialized from menu) 
	 */
	void slAddTableRow();
	/**
	 * add column to table (slot is inicialized from menu)
	 */
	void slAddTableCol();
	/**
	 * del row from table (slot is inicialized from menu)
	 */
	void slDelTableRow();
	/**
	 * del column from table (slot is inicialized from menu)
	 */
	void slDelTableCol();
	/**
	 * set main window title (it's usefull for identify opened file)
	 * @param fileName 
	 */
	void setWindowTitle(QString fileName="");
private:
	/**
	 * encapsulation for m_pCodeView->load, load code to codeView dockwindow
	 */
	void loadFoCode();
	/**
	 * init command line arguments - this should be in singleton and init should be in main.cpp
	 * @param argc 
	 * @param argv[] 
	 */
	void initCliArgs( int argc, char *argv[]);
	/**
	 * create mainview (center area, workplace)
	 */
	void createMainView(MainDoc *pMainDoc);
	/**
	 * create menus
	 */
	void createMenus();
	/**
	 * create toolbars
	 */
	void createToolBars();
	/**
	 * create status bar
	 */
	void createStatusBar();
	/**
	 * create dock windows
	 */
	void createDockWindows();
	/**
	 * create actios
	 */
	void createActions();
private:
	/// ////////////// ///
	/// //mainview//// ///
	/// ////////////// ///
	MainView *m_pMainView;// we have two views .. QGraphicsView and MainView

	//dockwidgets pointers
	QDockWidget *m_pDockFoTextEdit;
	QDockWidget *m_pDockCodeView;
	
	/// /////////////////////// ///
	/// //dockwidget's view /// ///
	/// /////////////////////// ///
	QTreeView *m_pDomTreeView;
	QTableView *m_pDomItemAttributesView;
	QUndoView *m_pUndoView;
	QTreeView *m_pItemHierarchyTreeView;
	QTableView *m_pItemPropertiesView;
	QListView *m_pItemToolBoxView;
	
	CodeView *m_pCodeView;//widget which display xsl-fo code
	FoTextEdit *m_pFoTextEdit;
	
	/// //////////////////// ///
	/// //dockwidget's doc// ///
	/// //////////////////// ///
	MainDoc *m_pMainDoc;
	DomModel *m_pDomModel;
	ItemToolBoxDoc *m_pItemToolBoxDoc;
	ItemPropertiesDoc *m_pItemPropertiesDoc;
	ItemHierarchyTreeDoc *m_pItemHierarchyTreeDoc;
	QUndoGroup *m_pUndoGroup;
	
	/// //////////// ///
	/// //toolbars// ///
	/// //////////// ///
	QToolBar *m_pToolBarView;
	QToolBar *m_pToolBarUndo;

	/// ///////////////////////// ///
	/// //rulers round mainview// ///
	/// ///////////////////////// ///
	PageRuler *m_pLeftRuler;
	PageRuler *m_pTopRuler;
	
	/// //////////// ///
	/// ///menus//// ///
	/// //////////// ///
	QMenu *m_pHelpMenu;
	QAction *m_pAboutQtAct;

	QMenu *m_pEditMenu;
	QAction *m_pQActUndo;
	QAction *m_pQActRedo;
	
	QMenu *m_pViewMenu;
	QAction *m_pQActZoomIn;
	QAction *m_pQActZoomOut;

	QMenu *m_pFileMenu;
	QAction *m_pQActOpenXmlFile;
	QAction *m_pQActOpenXslFoFile;
	QAction *m_pQActSaveFoFile;
	
	QMenu *m_pItemMenu;
	QAction *m_pQActAddRow;
	QAction *m_pQActDeleteRow;
	QAction *m_pQActAddColumn;
	QAction *m_pQActDeleteColumn;
};

#endif
