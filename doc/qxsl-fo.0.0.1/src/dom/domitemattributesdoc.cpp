#include "domitemattributesdoc.h"
#include "domitem.h"

DomItemAttributesDoc::DomItemAttributesDoc()
 : QAbstractTableModel(),m_pDomItem(NULL)
{
}


DomItemAttributesDoc::~DomItemAttributesDoc()
{
	
}

void DomItemAttributesDoc::slItemSelected(const QModelIndex& index)
{
	m_pDomItem = static_cast<DomItem*>(index.internalPointer());
	reset();
}

QVariant DomItemAttributesDoc::data(const QModelIndex &index, int role) const
{
	if (m_pDomItem==NULL)
	{
		return QVariant();
	}
	
	if(role!=Qt::DisplayRole)
		return QVariant();
	
	QDomNode node = m_pDomItem->node();
	QDomNamedNodeMap attributeMap = node.attributes();
	
	QDomNode attribute = attributeMap.item(index.row());
	
	switch (index.column())
	{
		case 0:
			return QString(attribute.nodeName());
		case 1:
			return QString(attribute.nodeValue());
	}
	
	return QVariant();
}

int DomItemAttributesDoc::columnCount(const QModelIndex &) const
{
	return 2;
}

int DomItemAttributesDoc::rowCount(const QModelIndex &) const
{
	if(m_pDomItem!=0)
	{
		return m_pDomItem->node().attributes().count();
	}
	else
	{
		return 0;
	}
}


QVariant DomItemAttributesDoc::headerData(int section, Qt::Orientation orientation,int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0:
				return tr("Attribute");
			case 1:
				return tr("Value");
			default:
				return QVariant();
		}
	}	
	return QVariant();
}
