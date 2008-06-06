#ifndef ITEMPROPERTIESDOC_H
#define ITEMPROPERTIESDOC_H

#include <QtGui>
#include <vector>

#include "attributefo.h"

class MainDoc;
class PageDoc;
class ObjectFoSimpleDoc;
class ObjectFoContainerSimpleDoc;
class ObjectFoSequencedDoc;
class NodeFo;
class ObjectFoDoc;

/**
 * represent data part of dock window which show attributes values of currently selected object
 */
class ItemPropertiesDoc : public QAbstractTableModel
{
	 Q_OBJECT
public:
	/**
	 * own role types which specify type of returned data from function data
	 */	
	enum EFoRoles
	{
		GroupOfFoAttributeRole = Qt::UserRole+1
	};	
	/**
	 * constructor
	 * @param parent 
	 */
	ItemPropertiesDoc(QObject *parent = 0);	
	/**
	 * destructor
	 */
	~ItemPropertiesDoc();
	/**
	 * return data : atribute name and value for role DisplayRole
	 * and group of indexed object for role GroupOfFoAttributeRole
	 * @param index 
	 * @param role 
	 * @return 
	 */
	QVariant data(const QModelIndex &index, int role) const;
	/**
	 * return count of rows (it's represented by count of attributes)
	 * @param parent 
	 * @return 
	 */
	int rowCount(const QModelIndex &parent) const;
	/**
	 * return count of columns (2 : attribute name and value)
	 * @param parent 
	 * @return 
	 */
	int columnCount(const QModelIndex &parent) const;
	/**
	 * this method is called when we want show properties of formating object of type ObjectFoSimpleDoc
	 * @param pFoObjectSimple 
	 */
	void UpdatePagePropertiesView(ObjectFoSimpleDoc *pFoObjectSimple);
	/**
	 * this method is called when we want show properties of formating object of type ObjectFoSequencedDoc
	 * @param pFoObjectSequenced 
	 */
	void UpdatePagePropertiesView(ObjectFoSequencedDoc *pFoObjectSequenced);
	/**
	 * this method is called when we want show properties of formating object of type ObjectFoContainerSimpleDoc 
	 * @param pFoObjectSequenced 
	 */
	void UpdatePagePropertiesView(ObjectFoContainerSimpleDoc *pObjectFoConatinerSimpleDoc);
	/**
	 * return header data (attribute name,value) of table
	 * @param section 
	 * @param orientation 
	 * @param role 
	 * @return 
	 */
	QVariant headerData(int section, Qt::Orientation orientation,int role) const;
	/**
	 * set editable flags for items
	 * @param index 
	 * @return 
	 */
	Qt::ItemFlags flags(const QModelIndex &index) const;
	/**
	 * overloaded method from qt, it set data on m_pFoObject (qdomelement), it use sigDataChangedInPropertiesDoc signal
	 * @param index 
	 * @param value 
	 * @param role 
	 * @return 
	 */
	bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole); 
signals:
	/**
	 * signal which inform about value change of attribute
	 * @param attributeType 
	 * @param val 
	 */
	void sigDataChangedInPropertiesDoc(AttributeFo::EAttributeType attributeType,QVariant val);
private:
	/**
	 * curently displayed node, we need it becouse ObjectFoDoc doesn't contain reference to nodefo
	 */
	NodeFo *m_pNodeFo;
	/**
	 * currently displayed formating object
	 */
	ObjectFoDoc *m_pCurrentObjectFo;
};

#endif

///handle sequenced formating objects
