//own include
#include "itemtoolboxdoc.h"

//qt includes
#include <QMimeData>

//project includes
#include "objectfodoc.h"
#include "objectfoview.h" ///todo:view include in doc part ? it becouse of enum ... enum should by in foobjectviewalbe

ItemToolBoxDoc::ItemToolBoxDoc(QObject *parent)
 : QAbstractListModel(parent)
{
	///this text should be editable, maybe some ./home/file ? ->qsettings

	m_list	<< ObjectFoDoc::typeFoBlock
		<< ObjectFoDoc::typeFoBlockContainer
		<< ObjectFoDoc::typeFoTable
		<< ObjectFoDoc::typeFoExternalGraphic;
}


ItemToolBoxDoc::~ItemToolBoxDoc()
{
}

QVariant ItemToolBoxDoc::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	
	if (index.row() >= m_list.size())
		return QVariant();
	
	ObjectFoDoc::FoObjectType type=m_list.at(index.row());
	
	if (role == Qt::DisplayRole)
	{
		QString strType=ObjectFoDoc::typeToString(type);
		return strType;
	}
	if (role == Qt::DecorationRole)
	{
		return QPixmap(ObjectFoDoc::typeToPixmap(type));
	}
	if (role == ETypeRole)
	{
		return type;
	}
	else
		return QVariant();
}


Qt::ItemFlags ItemToolBoxDoc::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
	
	if (index.isValid())
         	return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	else
		return Qt::ItemIsDropEnabled | defaultFlags;
}

QMimeData * ItemToolBoxDoc::mimeData ( const QModelIndexList & indexes ) const
{
	QMimeData *mimeData = new QMimeData();
	
	int newFoObjectType = data(indexes.at(0), ETypeRole).toInt();
	QByteArray dragAttributes;
	QDataStream stream(&dragAttributes, QIODevice::WriteOnly);
	
	stream << newFoObjectType;
	
	//create qdrag object and mime data
	mimeData->setData("application/createnewfoobject", dragAttributes);
	
	return mimeData;
}

int ItemToolBoxDoc::rowCount(const QModelIndex &) const
{
	return m_list.count();
}
