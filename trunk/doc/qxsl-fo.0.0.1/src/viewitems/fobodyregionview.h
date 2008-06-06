#ifndef FOBODYREGIONVIEW_H
#define FOBODYREGIONVIEW_H

#include <QGraphicsRectItem>
#include "resizehandlewindow.h"
#include "objectfosequencedview.h"

class QGraphicsItem;

class FoObjectSimplView;
class FoSimplePageMaster;
class PageView;
class BorderRegionDoc;
class FoBodyRegionDoc;
class FoBlockDoc;
class FoObjectSelectable;
class FoObjectSequencedDoc;
class ObjectFoSimpleDoc;
class MainView;
class NodeFo;

/**
 * represent view part of formating object fo:body-region
 */

class FoBodyRegionView : public ObjectFoSequencedView
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param pFoSimplePageMaster from parent page to get his attributes
	 * @param parentGroup 
	 * @param pDoc document part of bodyregion
	 */
	FoBodyRegionView(NodeFo *pFoSimplePageMaster,ObjectFoSequencedView *parentView,ObjectFoSequencedDoc *doc,MainView *pMainView);
	/**
	 * destructor
	 */
	~FoBodyRegionView();
	/**
	 * this drag&drop comes from itemtoolboxdoc or from bodyregionview itselfs
	 * @param event 
	 */
	void moveContainerView(QGraphicsSceneDragDropEvent * event);
	/**
	 * acces for doc part of view
	 * @return 
	 */
	FoBodyRegionDoc *bodyRegionDoc(){	return (FoBodyRegionDoc *)ObjectFoView::doc();}
	/**
	 * set rectangle and set position of handler window
	 * @param x 
	 * @param y 
	 * @param width 
	 * @param height 
	 * @param setUpObjectHandler 
	 */
	void slSetRectangle ( qreal x, qreal y, qreal width, qreal height, bool setUpObjectHandler = true);
private:
	/**
	 * draw rectangle of the region
	 */
	void draw();
private:
	/**
	 * node object which has margins attributes, which define left top position
	 */
	NodeFo *m_pFoSimplePageMaster;
	/**
	 * reference to resizehandler window
	 */
	ResizeHandleWindow *m_pResizeHandleWindow;
};

#endif
