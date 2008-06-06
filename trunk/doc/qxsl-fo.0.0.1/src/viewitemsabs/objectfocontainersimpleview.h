#ifndef OBJECTFOCONTAINERSIMPLEVIEW_H
#define OBJECTFOCONTAINERSIMPLEVIEW_H

#include "objectfocontainerview.h"

class QGraphicsItem;

class ObjectFoContainerSimpleDoc;
class MainView;
class ObjectFoDoc;

/**
 * represent abstract view part of formating  object which has definition inly in page-sequence part of fo-document and contain also container
 */
class ObjectFoContainerSimpleView : public ObjectFoContainerView
{
public:
	/**
	 * constructor
	 * @param parentContainer 
	 * @param doc 
	 * @param pMainView 
	 * @param scene 
	 */
	ObjectFoContainerSimpleView(ObjectFoContainerView *parentContainer,ObjectFoContainerDoc *doc,MainView *pMainView,QGraphicsScene *scene=NULL);
	/**
	 * destructor
	 */
	~ObjectFoContainerSimpleView();
	/**
	 * return reference to doc part of formating object
	 * @return 
	 */
	ObjectFoContainerSimpleDoc* objectFoContainerSimpleDoc(){	return (ObjectFoContainerSimpleDoc *)ObjectFoView::doc();}
	/**
	 * called by qt framework and inform main view about selection
	 * @param event 
	 */
	void mousePressEvent ( QGraphicsSceneMouseEvent *event );

};

#endif
