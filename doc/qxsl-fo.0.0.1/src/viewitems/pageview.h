#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QGraphicsRectItem>

#include "mainview.h"
#include "objectfodoc.h"
#include "objectfosequencedview.h"
#include "resizehandlewindow.h"
#include "foborderregiondoc.h"

class PageDoc;

/**
 * View part of page, contain method for create margins and regions, also contain enumerator to define 
 * Z values of all foobjects
 */

class PageView : public ObjectFoSequencedView
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param doc 
	 * @param pMainView 
	 * @param scene 
	 */
	PageView(ObjectFoSequencedDoc *doc,MainView *pMainView, MainView * scene = 0);
	/**
	 * destructor
	 */
	~PageView();
	/**
	 * define Z values for all foobjects
	 */
	enum EZValues
	{
		EPageViewZPosition = 1,
		EMarginsViewZPosition = 2,
		EFoBorderRegionViewZPosition = 3,
		EFoBodyRegionViewZPosition = 4,
		
		ESimpleFoObjectViewZPosition = 10,
		
		ESimpleFoBlockContainerViewZPosition = 11,
		
		ESelectedFoObjectViewZPosition = 500,
		EResizehandlerWindow = 101, //small rect on bottom right for resizing
		EHandlerWindow = 102, //small rect on bottom right for resizing
		
		EPageViewPreparedForDrop = 400, //when user want resize foobjectsimple we use PageView::dropMove event to handle resizing so pageview on top is needed
	};
	/**
	 * page distance between pages
	 */
	enum {	EPageDistance = 0 };
	/**
	 * define type for object type recognize in MainView (QGraphicsScene)
	 */
	enum { Type = UserType + 1 };
	/**
	 * return type userType
	 */
	int type() const{return Type;}
	/**
	 * create page
	 * @param shift 
	 * @return 
	 */
	int CreatePageView(int shift);
	/**
	 * return reference to doc part of page
	 * @return 
	 */
	PageDoc *pageDoc(){	return (PageDoc *)doc();}
	
public slots:
	/**
	 *	I use this slot to send information about some action on foobjectsimple to mainview
	 *	e.g. click on fosimpleview -> show properties info about clicked fosimpleview
	 * @param objectSimple 
	 * @param eventType type of event performed on objectSimple
	 */
private:
	/**
	 * create margins of page
	 */
	void CreateMargins();
	/**
	 * create body region
	 */
	void CreateBodyRegion();
	/**
	 * create border region of type regioType
	 * @param regionType 
	 */
	void CreateBorderRegions(FoBorderRegionDoc::EBorderRegionType regionType);
};

#endif
