#ifndef FOBLOCKDOC_H
#define FOBLOCKDOC_H

#include "objectfocontainersimpledoc.h"

#include <vector>

class QDomElement;
class QPoint;

class PageDoc;
class FoBodyRegionDoc;
class TextContentDoc;
class ObjectFoDoc;
class ObjectFoContainerDoc;

/**
 *	class which represent fo:block from fo language
 */
class FoBlockDoc : public ObjectFoContainerSimpleDoc
{
public:	
	FoBlockDoc(QDomElement domElement,ObjectFoContainerDoc *parentContainer);
	~FoBlockDoc();
	/**
	 * go through the foBLockElement and create appropriate blocks - not fully implemented yet
	 * @param foBlockElement 
	 */
	void loadViewableChildBlocks(QDomElement foBlockElement);
	/**
	 * return width, depend on parent width
	 * @return 
	 */
	FoSize widthValue();
	/**
	 * return height of foblock, depend on chilren
	 * @return 
	 */
	FoSize height();
};

#endif
