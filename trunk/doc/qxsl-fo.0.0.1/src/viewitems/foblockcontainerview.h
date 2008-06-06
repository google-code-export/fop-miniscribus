#ifndef FOBLOCKCONTAINERVIEW_H
#define FOBLOCKCONTAINERVIEW_H

#include "objectfocontainersimpleview.h"

#include "objectfosimpleview.h"
#include "objectfosimpledoc.h"
#include "resizehandlewindow.h"

class QGraphicsSceneMouseEvent;

class ObjectFoDoc;
class FoBlockContainerDoc;

/**
 * represent view of fo:block-container object
 */
class FoBlockContainerView : public ObjectFoContainerSimpleView
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param parent 
	 * @param doc 
	 * @param pMainView 
	 */
	FoBlockContainerView(ObjectFoContainerView * parent, ObjectFoContainerDoc *doc,MainView *pMainView);
	/**
	 * destructor
	 */
	~FoBlockContainerView();
	/**
	 * return FoBlockContainerDoc - doc part of view
	 * @return 
	 */
	FoBlockContainerDoc *foBlockContainerDoc(){	return (FoBlockContainerDoc *)ObjectFoView::doc();}
	/**
	 * set size of object and also map resizer window
	 * @param x 
	 * @param y 
	 * @param width 
	 * @param height 
	 * @param setUpObjectHandler 
	 */
	void slSetRectangle ( qreal x, qreal y, qreal width, qreal height, bool setUpObjectHandler = true);
private:
	/**
	 * refers to resizer window (small rect on bottom right)
	 * @param event 
	 */
	ResizeHandleWindow *m_pResizeHandleWindow;
};

#endif
