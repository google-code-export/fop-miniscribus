#ifndef DOMMODEL_H
#define DOMMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QModelIndex>
#include <QVariant>

class DomItem;

/**
 * represent doc part of xml tree dock window, it process whole QDomDocument to tree
 */
class DomModel : public QAbstractItemModel
{
    Q_OBJECT
public:
	/**
	 * define own roles for function data 
	 */
	enum EFoRoles
	{
		XPathRole = Qt::UserRole+1	//curently unused
	};
	/**
	 * constructor
	 * @param document 
	 * @param parent 
	 */
	DomModel(QDomDocument document, QObject *parent = 0);
	/**
	 * destructor
	 */
	~DomModel();
	/**
	 * Returns the data stored under the given role for the item referred to by the index.
	 * return name of node,attributes of node and value of node, return type depend on column number
	 * @param index 
	 * @param role 
	 * @return 
	 */
	QVariant data(const QModelIndex &index, int role) const;
	/**
	 * set flags for view (e.g. selectable,dragEnabled)
	 * @param index 
	 * @return 
	 */
	Qt::ItemFlags flags(const QModelIndex &index) const;
	/**
	 * return header data : name, attributes,value
	 * @param section 
	 * @param orientation 
	 * @param role 
	 * @return 
	 */
	QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
	/**
	 * Returns the index of the item in the model specified by the given row, column and parent index.
	 * @param row 
	 * @param column 
	 * @param parent 
	 * @return 
	 */
	QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
	/**
	 * xonstruct and return model index with parent of child
	 * @param child 
	 * @return 
	 */
	QModelIndex parent(const QModelIndex &child) const;
	/**
	 * return count of rows (child count) of parent
	 * @param parent 
	 * @return 
	 */
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	/**
	 * return row count of parent (3 - name,attributes,value)
	 * @param parent 
	 * @return 
	 */
	int columnCount(const QModelIndex &parent = QModelIndex()) const;	
	/**
	 * specified supported drop action : Qt::CopyAction | Qt::MoveAction; 
	 * @return 
	 */
	Qt::DropActions supportedDropActions() const;
	/**
	 * construct data from currently selected item for drop action
	 * @param indexes 
	 * @return 
	 */
	QMimeData *mimeData(const QModelIndexList &indexes) const;
private:
	/**
	 * contain reference to displayed qdomdocument
	 */
	QDomDocument domDocument;
	/**
	 * contian reference to root item
	 */
	DomItem *rootItem;
};

#endif
