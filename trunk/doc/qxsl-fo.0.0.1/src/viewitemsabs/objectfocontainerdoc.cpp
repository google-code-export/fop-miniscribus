//own include
#include "objectfocontainerdoc.h"

#include <vector>

//qt includes
#include <QPoint>
#include <QDomElement>

//project includes
#include "foblockdoc.h"
#include "foblockcontainerdoc.h"
#include "objectfodoc.h"
#include "fotabledoc.h"

ObjectFoContainerDoc::ObjectFoContainerDoc(ObjectFoDoc::FoObjectType type,ObjectFoContainerDoc *parentContainer)
:ObjectFoDoc(type),m_pParentContainer(parentContainer)
{

}


ObjectFoContainerDoc::~ObjectFoContainerDoc()
{

}

QPoint ObjectFoContainerDoc::positionForNewChildFoObject()
{
	//if we don't have any blocks yet we calculate position according the bodyregion
	if(m_listFoObjects.size()==0)
	{
		return QPoint(0,0);
	}
	else
	{
		int tmpY =wholeHeightOfChildren();
		return QPoint(0,tmpY);
	}
}

int ObjectFoContainerDoc::wholeHeightOfChildren()
{
	int wholeHeight=0;
	///todo:need calcute only to selected item
	//go throught whole list
	for(unsigned int pos=0;pos<m_listFoObjects.size();pos++)
	{
		if (m_listFoObjects.at(pos)->hasAbsolutePositioning()==false)
			wholeHeight+=m_listFoObjects.at(pos)->height();
	}
	
	return wholeHeight;
}

void ObjectFoContainerDoc::addItem(ObjectFoDoc *foObjectViewable)
{
	//mark position
	foObjectViewable->setId(m_listFoObjects.size());
	//add to stack of items
	m_listFoObjects.push_back(foObjectViewable);
}

void ObjectFoContainerDoc::removeItem(ObjectFoDoc *foObjectViewable)
{
	//we know ID and we find it
	//go throught whole list
	for(unsigned int pos=0;pos<m_listFoObjects.size();pos++)
	{
		//if U find object with the same Id, remove it from list - but don't delete it
		if ( foObjectViewable->getId() == m_listFoObjects.at(pos)->getId() )
		{
			m_listFoObjects.erase(m_listFoObjects.begin()+pos);
			return;
		}
	}
}

void ObjectFoContainerDoc::loadElementChildrenToContainer(QDomElement elementData)
{
	
	for(QDomNode node = elementData.firstChild();!node.isNull(); node = node.nextSibling())
	{
		QDomElement eChildFoObject = node.toElement(); // try to convert the node to an element.
		if(!eChildFoObject.isNull())
		{
			if (eChildFoObject.tagName()=="fo:block")
			{
				//we want only add item to vector we have data curently added in dom
				//fo we use addItem from ObjectFoDoc
				ObjectFoContainerDoc::addItem(new FoBlockDoc(eChildFoObject,this));
			}
			//if node is picture
			else if (eChildFoObject.tagName()=="fo:block-container")
			{
				ObjectFoContainerDoc::addItem(new FoBlockContainerDoc(eChildFoObject,this));
			}	
			else if (eChildFoObject.tagName()=="fo:table")
			{
				ObjectFoContainerDoc::addItem(new FoTableDoc(eChildFoObject,this));
			}
			else
			{
				qDebug(QString("FoObject %1 not implemented yet").arg(eChildFoObject.tagName()).toAscii());
			}
		}
	}
}

void ObjectFoContainerDoc::resetDoc()
{
	ObjectFoDoc *foChildObj;
	for(unsigned int a=0;a<m_listFoObjects.size();a++)
	{
		foChildObj=m_listFoObjects.at(a);
		foChildObj->resetDoc();
	}
	ObjectFoDoc::resetDoc();
}

void ObjectFoContainerDoc::resize(QPoint newSize)
{
	resetDoc();
	ObjectFoDoc::resize(newSize);
	emit sigReloadChildrenAfterResize();
}

FoSize ObjectFoContainerDoc::height()
{
	return wholeHeightOfChildren();
}
