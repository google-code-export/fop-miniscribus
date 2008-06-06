#ifndef FOOBJECTSEQUENCEDVIEW_H
#define FOOBJECTSEQUENCEDVIEW_H

#include "objectfocontainerview.h"

#include "resizehandlewindow.h"
//#include "foobjectsimpleview.h"

class QGraphicsItem;
class QGraphicsScene;

class ObjectFoSimpleView;
class ObjectFoSequencedDoc;
class ObjectFoDoc;

/**
 * base class for all formating objects which have definition in page-seauence node and in layout-master-set node,
 * these object are containers for simple formating objects
 */ 
class ObjectFoSequencedView : public ObjectFoContainerView
{
Q_OBJECT
public:
	/**
	 * contstructor
	 * @param parentGroup 
	 * @param doc 
	 * @param pMainView 
	 * @param scene 
	 */
	ObjectFoSequencedView(ObjectFoContainerView *parentGroup,ObjectFoSequencedDoc *doc,MainView *pMainView,QGraphicsScene *scene=NULL);
	/**
	 * destructor
	 */
	~ObjectFoSequencedView();
	/**
	 * called by qt framework and inform main view about selection
	 * @param event 
	 */	
	void mousePressEvent ( QGraphicsSceneMouseEvent *event );	
	/**
	 * return doc part of formating object
	 * @return 
	 */
	ObjectFoSequencedDoc *foObjectSequencedDoc(){	return (ObjectFoSequencedDoc *)ObjectFoView::doc();}	
public slots:
	/**
	 * currently unused
	 */
	void slDoSelect();
};

#endif
