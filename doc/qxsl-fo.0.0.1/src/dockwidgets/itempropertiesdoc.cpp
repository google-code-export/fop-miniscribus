#include <vector>

#include "pagedoc.h"
#include "itempropertiesdoc.h"
#include "objectfosimpledoc.h"
#include "objectfosequenceddoc.h"
#include "objectfocontainersimpledoc.h"
#include "nodefo.h"
#include "attributefo.h"
#include "objectfodoc.h"

ItemPropertiesDoc::ItemPropertiesDoc(QObject *parent)
 : QAbstractTableModel(parent)
{
	m_pNodeFo=NULL;
	m_pCurrentObjectFo=NULL;
}

ItemPropertiesDoc::~ItemPropertiesDoc()
{
}

void ItemPropertiesDoc::UpdatePagePropertiesView(ObjectFoSimpleDoc *pFoObjectSimple)
{
	//disconnect previous conected foobjectsimple
	disconnect(this, SIGNAL(sigDataChangedInPropertiesDoc(AttributeFo::EAttributeType,QVariant)), 0, 0);
	m_pCurrentObjectFo=pFoObjectSimple;
	m_pNodeFo=pFoObjectSimple->getNodeFo();
	reset();
}
void ItemPropertiesDoc::UpdatePagePropertiesView(ObjectFoSequencedDoc *pFoObjectSequenced)
{
	//disconnect previous conected foobjectsequenced
	disconnect(this, SIGNAL(sigDataChangedInPropertiesDoc(AttributeFo::EAttributeType,QVariant)), 0, 0);
	m_pCurrentObjectFo=pFoObjectSequenced;
	m_pNodeFo=pFoObjectSequenced->getFoSimplePageMasterPart();
	reset();
}

void ItemPropertiesDoc::UpdatePagePropertiesView(ObjectFoContainerSimpleDoc *pFoObjectFoContainerSimpleDoc)
{
	//disconnect previous conected foobjectsequenced
	disconnect(this, SIGNAL(sigDataChangedInPropertiesDoc(AttributeFo::EAttributeType,QVariant)), 0, 0);
	m_pCurrentObjectFo=pFoObjectFoContainerSimpleDoc;
	m_pNodeFo=pFoObjectFoContainerSimpleDoc->getNodeFo();
	reset();
}

QVariant ItemPropertiesDoc::data(const QModelIndex &index, int role) const
{
	if (m_pNodeFo == NULL)
		return QVariant();
	
	//user want info about group of indexed item
	if(role==ItemPropertiesDoc::GroupOfFoAttributeRole)
	{
		AttributeFo *attr = m_pNodeFo->attributeByPosition(index.row());
		return attr->group();
	}
	
	//
	if(role!=Qt::DisplayRole)
		return QVariant();
	
	switch (index.column())
	{
		case 0:
			//foobject has vector of attributes and by index we get desired attribute, end we return his name
			if (index.row()==0)
				return QString("type");
			
			return m_pNodeFo->attributeByPosition(index.row()-1)->name();
		case 1:
		{
			//at first position we want show name
			if (index.row()==0)
				return ObjectFoDoc::typeToString(m_pCurrentObjectFo->type());
			//then we want show attributes
			//foobject has vector of attributes and by index we get desired attribute
			AttributeFo *attr = m_pNodeFo->attributeByPosition(index.row()-1);
			//sizeType,colorType,fontFamilyType,alignType,stringType,numberType
			switch (attr->group())
			{
				case AttributeFo::alignGroup:
					return m_pNodeFo->attributeValueByType(attr->type())->stringValue();
				case AttributeFo::sizeGroup:
					return m_pNodeFo->attributeValueByType(attr->type())->sizeValue().m_value;
				default:
					return "unresolved type";
			}
		}
	}
	
	return QVariant();
}

int ItemPropertiesDoc::rowCount(const QModelIndex &) const
{
	if (m_pNodeFo==NULL)
		return 0;
	else
		//+1 means that we add line with "type" of the object
		return m_pNodeFo->countOfAttributes()+1;
}

int ItemPropertiesDoc::columnCount(const QModelIndex &) const
{
	return 2;
}

QVariant ItemPropertiesDoc::headerData(int section, Qt::Orientation orientation,int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0:
				return tr("Attribute name");
			case 1:
				return tr("Value");
			default:
				return QVariant();
		}
	}	
	return QVariant();
}


Qt::ItemFlags ItemPropertiesDoc::flags(const QModelIndex &index) const
{	
	//we want only second column editable
	if (index.column() == 1) 
	{
		return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
	}
	else
	{
		return QAbstractItemModel::flags(index);
	}
}

bool ItemPropertiesDoc::setData(const QModelIndex &index,const QVariant &val, int role)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		QString strAttrName=m_pNodeFo->attributeByPosition(index.row())->name();
		emit sigDataChangedInPropertiesDoc(AttributeFo::stringToType(strAttrName),val);
		return true;
	}
	return false;
}
