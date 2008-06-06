#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsScene>

#include "fotableview.h"

class QUndoCommand;

class GraphicsView;
class PageViewManager;
class MainWindow;

class PageDoc;
class MainDoc;
class ObjectFoSimpleDoc;
class ObjectFoSequencedDoc;

class ObjectFoView;
class ObjectFoSimpleView;
class ObjectFoSequencedView;
class ObjectFoContainerSimpleView;
class ObjectFoHandler;

/**
 * represent gray "background" behind pages, it containt also pegeview mamanger, reference to currently selected
 * item and actual scale value of QGraphicView. Also cover actions after selecting fo object like rulers set and 
 * properties dock window update
 */

class MainView : public QGraphicsScene
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param pMainWindow 
	 * @param pMainDoc 
	 */
	MainView(MainWindow *pMainWindow,MainDoc *pMainDoc);
	/**
	 * destructor
	 */
	~MainView();
	/**
	 * this method is called when fo object with ObjectFoSimpleView type is selected
	 *  and set rules for selected object and update properties dock window
	 * @param pObjectFoSimpleView 
	 */
	void foObjectSimpleSelected(ObjectFoSimpleView *pObjectFoSimpleView);
	/**
	 * this method is called when fo object with ObjectFoSequencedView type is selected
	 * and set rules for selected object and update properties dock window 
	 * @param pObjectFoSequencedView 
	 */
	void foObjectSequencedSelected(ObjectFoSequencedView *pObjectFoSequencedView);
	/**
	 * this method is called when fo object with ObjectFoContainerSimpleView type is selected
	 * and set rules for selected object and update properties dock window
	 * @param pFoObjectContainerSimpleView 
	 */
	void objectFoContainerSimpleSelected(ObjectFoContainerSimpleView *pFoObjectContainerSimpleView);
	/**
	 * push command to undo stack
	 * @param cmd 
	 */
	void pushToUndoStack(QUndoCommand * cmd);
	/**
	 * set scale value for all QGraphicsView
	 * @param sx 
	 * @param sy 
	 */
	void scale ( qreal sx, qreal sy );
	/**
	 * return scale value of QGraphcisView
	 * @return 
	 */
	QPointF scale(){	return m_scale;}
	/**
	 * return reference of encapsulated QGraphicsView
	 * @return 
	 */
	GraphicsView *graphicsView(){	return m_pGraphicsView;}
	/**
	 * return scaled value of vertical scroolbar
	 * @param scale 
	 * @return 
	 */
	int verticalScrollBarValue(qreal scale);
	/**
	 * return scaled value of horizontal scroolbar
	 * @param scale 
	 * @return 
	 */
	int horizontalScrollBarValue(qreal scale);
	/**
	 * add column or row to m_pSelectedItemView accortding to resizeType
	 * @param resizeType 
	 */
	void menuQActionTableResize(FoTableView::ETableResizeCmd resizeType);
	/**
	 * unused
	 */
	void showDockFoTextEdit();
	/**
	 * show object handler and give old parent object back and new to front
	 * @param objectFo 
	 */
	void setUpObjectFoHandler(ObjectFoView *objectFo);
	/**
	 * return reference to object handler
	 * @return 
	 */
	ObjectFoHandler *objectFoHandler();
public slots:
	/**
	 * create view part of page
	 * @param  
	 */
	void createPage(PageDoc *);
private:
	/**
	 * reference to encapsulated QGraphicsView
	 */
	GraphicsView *m_pGraphicsView;
	/**
	 * reference to main window
	 */
	MainWindow *m_pMainWindow;
	/**
	 * page mager
	 */
	PageViewManager *m_pPageViewMng;
	/**
	 * reference to maindoc
	 */
	MainDoc	*m_pMainDoc;
	/**
	 * hold actual selected item
	 */
	ObjectFoView *m_pSelectedItemView;
	/**
	 * hold current scale of qgraphicsview
	 */	
	QPointF m_scale;
	/**
	 * reference to fo object handler (small rect on top right)
	 */		
	ObjectFoHandler *m_pObjectFoHandler;
};

#endif
