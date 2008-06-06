#ifndef OBJECTFOSIMPLEVIEW_H
#define OBJECTFOSIMPLEVIEW_H

#include "objectfoview.h"

class ObjectFoSimpleDoc;
class ObjectFoContainerView;

/**
 *	base view class for all foobjects which have definition only in page-sequence part of fo-document and are not containers for others objects
 */
class ObjectFoSimpleView : public ObjectFoView
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
	ObjectFoSimpleView(ObjectFoContainerView *parentContainer,ObjectFoDoc *doc,MainView *pMainView,QGraphicsScene *scene=NULL);
	/**
	 * destructor
	 */
	virtual ~ObjectFoSimpleView();
	/**
	 * is called by qt framework and inform main view about selection
	 * @param event 
	 */
	void mousePressEvent ( QGraphicsSceneMouseEvent *event );
	/**
	 * return reference to doc part of object
	 * @return 
	 */
	ObjectFoSimpleDoc* foObjectSimpleDoc(){	return (ObjectFoSimpleDoc *)ObjectFoView::doc();}
};

#endif
