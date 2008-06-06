//own include
#include "itemhierarchytreedoc.h"

//qt include
#include <QPixmap>

//project includes
#include "pagedoc.h"
#include "objectfodoc.h"
#include "objectfosimpledoc.h"

ItemHierarchyTreeDoc::ItemHierarchyTreeDoc(QObject *parent)
 : QAbstractItemModel(parent),m_pCurrentPageDoc(NULL),m_pSelectedObject(NULL)
{
}

ItemHierarchyTreeDoc::~ItemHierarchyTreeDoc()
{
}

int ItemHierarchyTreeDoc::columnCount(const QModelIndex &/*parent*/) const
{
	return 1;
}


Qt::ItemFlags ItemHierarchyTreeDoc::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
	
	if (index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	else
		return Qt::ItemIsDropEnabled | defaultFlags;
}

void ItemHierarchyTreeDoc::setPageDoc(PageDoc *pPageDoc)
{
	m_pCurrentPageDoc=pPageDoc;
	reset();
}

QModelIndex ItemHierarchyTreeDoc::index(int row, int column, const QModelIndex &parent) const
{
	if (m_pCurrentPageDoc==NULL)
		return QModelIndex();
	
	if (!parent.isValid())
		return createIndex(row, column, m_pCurrentPageDoc);
	
	ObjectFoDoc *foObject = static_cast<ObjectFoDoc*>(parent.internalPointer());
	
	if (foObject!=0)
	{
		if (foObject->countOfFoObjectsChildren()==0)
			return QModelIndex();
			
		if (foObject->type()==ObjectFoDoc::typeFoTextContent)
			return QModelIndex();
			
		ObjectFoDoc *childObjectFo=foObject->foObjectChildren(row);
		
		if(childObjectFo==NULL)
			qFatal("Can't handle child of objectfo in ItemHierarchyTreeDoc");
		
		return createIndex(row, column, childObjectFo);
	}	
	
	return QModelIndex();
}	


QModelIndex ItemHierarchyTreeDoc::parent(const QModelIndex &child) const
{
	ObjectFoDoc *childItem = static_cast<ObjectFoDoc*>(child.internalPointer());
	
	if (childItem==NULL)
		return QModelIndex();
	
	if (childItem->type()==ObjectFoDoc::typeFoPage)
		return QModelIndex();
	
	ObjectFoDoc *parent=childItem->parentContainer();
	if (parent==NULL)
		qFatal("Not handled parent in ItemHierarchyTreeDoc");
	
	int id=parent->getId();
	return createIndex(id,0,parent);
}

QVariant ItemHierarchyTreeDoc::data(const QModelIndex &index, int role) const
{
	if (m_pCurrentPageDoc==NULL)
		return QVariant();

	ObjectFoDoc *item = static_cast<ObjectFoDoc*>(index.internalPointer());
	
	if (role == Qt::DisplayRole)
	{
		if (item==NULL)
			qFatal("Item is empty in Model");
		return ObjectFoDoc::typeToString(item->type());
	}
	if (role == Qt::DecorationRole)
	{
		return QPixmap(ObjectFoDoc::typeToPixmap(item->type()));
	}
	else
		return QVariant();
}

int ItemHierarchyTreeDoc::rowCount(const QModelIndex &parent) const
{
	//this handle first node (page)
	if (!parent.isValid())
		return 1;
	
	ObjectFoDoc *container = static_cast<ObjectFoDoc*>(parent.internalPointer());
		
		
	if (container!=0)
	{
		int countOfChildren=container->countOfFoObjectsChildren();
		return countOfChildren;
	}
	
	return 0;
}

void ItemHierarchyTreeDoc::slClicked ( const QModelIndex & index )
{
	//if was some object selected,do unselect first
	if(m_pSelectedObject!=NULL)
		m_pSelectedObject->doUnselect();
	//get clicked obj
	m_pSelectedObject = static_cast<ObjectFoDoc*>(index.internalPointer());
	//do select
	m_pSelectedObject->doSelect();
}
