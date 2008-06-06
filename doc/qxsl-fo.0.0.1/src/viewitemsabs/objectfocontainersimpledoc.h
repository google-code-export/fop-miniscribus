#ifndef OBJECTFOCONTAINERSIMPLEDOC_H
#define OBJECTFOCONTAINERSIMPLEDOC_H

#include "absobjectfosimpledoc.h"
#include "objectfocontainerdoc.h"
#include "objectfodoc.h"

class QDomElement;

/**
 * represent abstract doc part of formating  object which has definition inly in page-sequence part of fo-document and is also container
 */
class ObjectFoContainerSimpleDoc : public AbsObjectFoSimpleDoc, public ObjectFoContainerDoc
{

public:
	/**
	 * constructor
	 * @param type 
	 * @param parentContainer 
	 * @param pElement 
	 */
	ObjectFoContainerSimpleDoc(ObjectFoDoc::FoObjectType type,ObjectFoContainerDoc *parentContainer,QDomElement pElement);
	/**
	 * destructor
	 */
	~ObjectFoContainerSimpleDoc();
	/**
	 * create instances of all children of formating object
	 */
	void loadChildrenToDoc();
};

#endif
