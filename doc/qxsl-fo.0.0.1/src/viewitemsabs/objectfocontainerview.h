#ifndef OBJECTFOCONTAINERVIEW_H
#define OBJECTFOCONTAINERVIEW_H


#include "objectfoview.h"

class QGraphicsItem;
class QGraphicsScene;

class MainView;
class ObjectFoSequencedDoc;
class ObjectFoDoc;
class ObjectFoContainerDoc;

/**
 * represent view part of formating object which contain containers to handle children
 */
class ObjectFoContainerView : public ObjectFoView
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param parentContainer 
	 * @param doc 
	 * @param pMainView 
	 * @param scene 
	 */
	ObjectFoContainerView(ObjectFoContainerView *parentContainer,ObjectFoContainerDoc *doc,MainView *pMainView,QGraphicsScene *scene);
	/**
	 *  destructor
	 */
	~ObjectFoContainerView();
	/**
	 * load children to view from doc part, we overwrite this method in FoBlockView
	 * @param foContainerDoc 
	 */
	void loadChildrenToView(ObjectFoContainerDoc *foContainerDoc);
	/**
	 * remove item fromcontainer
	 * @param foObjectViewable 
	 */
	void removeItemFromViewContainer(ObjectFoDoc *foObjectViewable);
	/**
	 * return doc part of container
	 * @return 
	 */
	ObjectFoContainerDoc *objectFoContainerDoc(){	return (ObjectFoContainerDoc *)ObjectFoView::doc();}
	/**
	 * create simple formating object specified by type typeOfNewFoObject 
	 * @param typeOfNewFoObject 
	 * @param newPos 
	 */
	void createObjectFoSimpleDoc(int typeOfNewFoObject,QPoint newPos);
	/**
	 * is called by qt framework, it used to d&d from itemtoolbox or for move item
	 * @param event 
	 */
	void dropEvent ( QGraphicsSceneDragDropEvent * event );
public slots:
	/**
	 * create drag object fill mime data with specific attributes and execute dragging
	 * @param event 
	 */
	virtual void startDrag(QGraphicsSceneMouseEvent *event);
	/**
	 * slot which is called from command pattern to create new object
	 * @param foObjectViewable 
	 */
	void slAddItemToViewContainer(ObjectFoDoc *foObjectViewable);
	/**
	 * delete all children and create it again
	 */
	void slReloadChildrenAfterResize();
};

#endif

///todo : befor reloading children are all children delete ...
