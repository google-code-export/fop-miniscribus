
//own include
#include "dommodel.h"

//qt include
#include <QtGui>
#include <QtXml>

//project's include
#include "domitem.h"
#include "objectfoview.h"

DomModel::DomModel(QDomDocument document, QObject *parent)
    : QAbstractItemModel(parent), domDocument(document)
{
	rootItem = new DomItem(domDocument, 0);
}

DomModel::~DomModel()
{
	delete rootItem;
}

int DomModel::columnCount(const QModelIndex &/*parent*/) const
{
	return 3;
}

QVariant DomModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	
	//role to get full xml path of index
	if(role == XPathRole)
	{
		QString wholeXmlPath;
		DomItem *item = static_cast<DomItem*>(index.internalPointer());
		if (item==NULL)
			qFatal("can't convert domitem from datamodel");
		
		for(;item->parent()!=NULL;item=item->parent())
		{
			wholeXmlPath=item->node().nodeName()+"/"+wholeXmlPath;
		}
		
		wholeXmlPath="/"+wholeXmlPath;
		return wholeXmlPath;
	}	
	else if (role == Qt::DisplayRole)
	{
		DomItem *item = static_cast<DomItem*>(index.internalPointer());
		
		QDomNode node = item->node();
		QStringList attributes;
		QDomNamedNodeMap attributeMap = node.attributes();
		
		switch (index.column())
		{
			//name
			case 0:
				return node.nodeName();
			//attributes
			case 1:
				for (int i = 0; i < attributeMap.count(); ++i)
				{
					QDomNode attribute = attributeMap.item(i);
					attributes << attribute.nodeName() + "=\""  +attribute.nodeValue() + "\"";
				}
				return attributes.join(" ");
			//value
			case 2:
				return node.nodeValue().split("\n").join(" ");
			default:
				return QVariant();
		}
	}
	else
		return QVariant();
}

Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
	if (index.isValid())
	{
		return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable);
	}
	
	return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
}

QVariant DomModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
			case 0:
				return tr("Name");
			case 1:
				return tr("Attributes");
			case 2:
				return tr("Value");
			default:
				return QVariant();
		}
	}
	
	return QVariant();
}

QModelIndex DomModel::index(int row, int column, const QModelIndex &parent) const
{
	DomItem *parentItem;
		
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<DomItem*>(parent.internalPointer());
	
	DomItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex DomModel::parent(const QModelIndex &child) const
{
	if (!child.isValid())
		return QModelIndex();
	
	DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
	DomItem *parentItem = childItem->parent();
	
	if (!parentItem || parentItem == rootItem)
		return QModelIndex();
	
	return createIndex(parentItem->row(), 0, parentItem);
}

int DomModel::rowCount(const QModelIndex &parent) const
{
	DomItem *parentItem;
	
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<DomItem*>(parent.internalPointer());
	
	return parentItem->node().childNodes().count();
}

Qt::DropActions DomModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QMimeData *DomModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	
	QString wholeXmlPath=data(indexes.at(0),DomModel::XPathRole).toString();
	
	QByteArray dragAttributes;
	QDataStream stream(&dragAttributes, QIODevice::WriteOnly);
	
	stream << ObjectFoView::dragAssignXMLToItem << wholeXmlPath;
	
	//create qdrag object and mime data
	mimeData->setData("application/x-dndxmldata", dragAttributes);
	
	return mimeData;
}
