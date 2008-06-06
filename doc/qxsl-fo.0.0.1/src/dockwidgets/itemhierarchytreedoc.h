#ifndef ITEMHIERARCHYTREEDOC_H
#define ITEMHIERARCHYTREEDOC_H

#include <QAbstractItemModel>

class PageDoc;
class ObjectFoDoc;

/**
 *	provide child hierarchy of all items on page
 */

class ItemHierarchyTreeDoc : public QAbstractItemModel
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param parent 
	 */
	ItemHierarchyTreeDoc(QObject *parent);
	/**
	 * destructor
	 */
	~ItemHierarchyTreeDoc();
	/**
	 * set flags (like ItemIsDragEnabled  Qt::ItemIsDropEnabled) for index 
	 * @param index 
	 * @return 
	 */
	Qt::ItemFlags flags(const QModelIndex &index) const;
	/**
	 * return count of columns : 1 we show only object names
	 * @param  
	 * @return 
	 */
	int columnCount(const QModelIndex &/*parent*/) const;
	/**
	 * set parent page which represent root point for all items
	 * @param pPageDoc 
	 */
	void setPageDoc(PageDoc *pPageDoc);
	/**
	 * Returns the index of the item in the model specified by the given row, column and parent index.
	 * create and return index which represent child of parent on row position
	 * @param row 
	 * @param column 
	 * @param parent 
	 * @return 
	 */
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	/**
	 * return parent of indexed child
	 * @param child 
	 * @return 
	 */
	QModelIndex parent(const QModelIndex &child) const;
	/**
	 * return count of rows (count of children)
	 * @param parent 
	 * @return 
	 */
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	/**
	 * Returns the data stored under the given role for the item referred to by the index.
	 * return name of indexed item and assigned pixmap for nicer look
	 * @param index 
	 * @param role 
	 * @return 
	 */
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
public slots:
	/**
	 * slot which handle click on hierarchy view (we do selection of selected item)
	 * @param index 
	 */
	void slClicked ( const QModelIndex & index );
private:
	/**
	 * reference to current page
	 */
	ObjectFoDoc *m_pCurrentPageDoc;
	/**
	 * reference to actually selected object
	 */
	ObjectFoDoc *m_pSelectedObject;
};

#endif

///todo : regions have multiple reference to parent and page have multiple reference to regions
