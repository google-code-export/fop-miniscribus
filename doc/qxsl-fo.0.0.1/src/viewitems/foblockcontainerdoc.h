#ifndef FOBLOCKCONTAINERDOC_H
#define FOBLOCKCONTAINERDOC_H

#include "objectfocontainersimpledoc.h"

class QPoint;
class QDomElement;

/**
 * represent doc part of formating object fo:block-container
 */
class FoBlockContainerDoc : public ObjectFoContainerSimpleDoc
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param pElement 
	 * @param parentContainer 
	 * @param pos 
	 */
	FoBlockContainerDoc(QDomElement pElement,ObjectFoContainerDoc *parentContainer,QPoint pos=QPoint(-1,-1));
	/**
	 * destructor
	 */
	~FoBlockContainerDoc();
	/**
	 * return width value, it's defined in fo-document (overloaded from foobjectviewable)
	 */
	FoSize widthValue();
	/**
	 * return position, it's defined in fo-document (overwrtited from foobjectviewable)
	 * @return 
	 */
	QPoint positionValue();
	/**
	 * set position directly in fo-document (overwrtited from foobjectviewable)
	 * @param pos 
	 */
	void setPositionValue(QPoint pos);
	/**
	 * set width of object directly in fo-document (overwrited from foobjectviewalbe)
	 * @param width 
	 */
	void setWidthValue(FoSize width);
};

#endif
