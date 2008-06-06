#ifndef DOMITEMATTRIBUTESDOC_H
#define DOMITEMATTRIBUTESDOC_H

#include <QAbstractTableModel>

class DomItem;
class QModelIndex;

/**
 * doc part of tableview, it handle (display) attributes of currently selected item in dommondel
 */
class DomItemAttributesDoc : public QAbstractTableModel
{
Q_OBJECT
public:
	/**
	 * constructor
	 */
	DomItemAttributesDoc();	
	/**
	 * destructor
	 */
	~DomItemAttributesDoc();
	/**
	 * Returns the data stored under the given role for the item referred to by the index.
	 * return attrbiute name and value
	 * @param index 
	 * @param role 
	 * @return 
	 */
	QVariant data(const QModelIndex &index, int role) const;
	/**
	 * return count of rows in table (it's count of attributes)
	 * @param  
	 * @return 
	 */
	int rowCount(const QModelIndex &) const;
	/**
	 * return count of columns (it value 2 - attrbiute name and attribute value)
	 * @param  
	 * @return 
	 */
	int columnCount(const QModelIndex &) const;
	/**
	 * return header data of table (attribute and value)
	 * @param section 
	 * @param orientation 
	 * @param role 
	 * @return 
	 */
	QVariant headerData(int section, Qt::Orientation orientation,int role) const;
public slots:
	/**
	 * slot which handle item selection in view part of dom model
	 * @param index 
	 */
	void slItemSelected(const QModelIndex& index);
private:
	/**
	 * currently selected dom item
	 * @param index 
	 */
	DomItem *m_pDomItem;
};

#endif
