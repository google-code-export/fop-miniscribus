//own include
#include "objectfocontainersimpledoc.h"

//qt includes
#include <QDomElement>

//project includes
#include "objectfocontainerdoc.h"
#include "objectfodoc.h"

ObjectFoContainerSimpleDoc::ObjectFoContainerSimpleDoc(ObjectFoDoc::FoObjectType type,ObjectFoContainerDoc *parentContainer,QDomElement pElement)
 : AbsObjectFoSimpleDoc(pElement), ObjectFoContainerDoc(type,parentContainer)
{

}


ObjectFoContainerSimpleDoc::~ObjectFoContainerSimpleDoc()
{

}

void ObjectFoContainerSimpleDoc::loadChildrenToDoc()
{
	ObjectFoContainerDoc::loadElementChildrenToContainer(AbsObjectFoSimpleDoc::getNodeFo()->elementData());
}

