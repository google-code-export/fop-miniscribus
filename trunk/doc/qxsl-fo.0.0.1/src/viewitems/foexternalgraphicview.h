#ifndef FOEXTERNALGRAPHICVIEW_H
#define FOEXTERNALGRAPHICVIEW_H

#include "objectfosimpleview.h"

class QGraphicsItemGroup;
class QGraphicsPixmapItem;

class ObjectFoContainerView;
class FoExternalGraphicDoc;
class MainView;

/**
 * represent view part of formating object fo:external-graphic
 */
class FoExternalGraphicView : public ObjectFoSimpleView
{
public:
	/**
	 * constructor
	 * @param parentContainer 
	 * @param doc 
	 * @param pMainView 
	 */
	FoExternalGraphicView(ObjectFoContainerView *parentContainer,FoExternalGraphicDoc *doc,MainView *pMainView);
	/**
	 * destructor
	 */
	~FoExternalGraphicView();
	/**
	 * load picture to view, scale it to content_width size, which is defined in doc part
	 * @param doc 
	 * @param alternatePicture 	true if we want load "empty_picture" usefull for not found image defined in doc part
	 * 				false if we want use picture from doc part (defined in fo:document)
	 */
	void loadPixmapToView(FoExternalGraphicDoc *doc,bool alternatePicture=false);
	/**
	 * return width of loaded pixmap
	 * @return 
	 */
	int pixmapWidth();
	/**
	 * set position of pixmap in fo:block row
	 * @param pos 
	 */
	void setPos(QPoint pos);
	/**
	 * set parent group of object (parent is fo:block row)
	 * @param groupItem 
	 */
	void assignGroup(QGraphicsItemGroup *groupItem);
private:
	/**
	 * reference to pixmap which represent picture
	 */
	QGraphicsPixmapItem *m_pPixmapItem;
	/**
	 * contain scale value of picture, becouse we need scale picture to achieve required width defined by content_width attribute
	 */
	qreal m_scale;
};

#endif
