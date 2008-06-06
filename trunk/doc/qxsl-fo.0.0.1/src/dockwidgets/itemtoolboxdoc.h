#ifndef ITEMTOOLBOXDOC_H
#define ITEMTOOLBOXDOC_H

#include <QAbstractListModel>
#include <QList>
#include "objectfodoc.h"

class QMimeData;

/**
 * show list of formating objects which could user insert using d&d to workplace
 */
class ItemToolBoxDoc : public QAbstractListModel
{
public:
	/**
	 * constructor
	 * @param parent 
	 */
	ItemToolBoxDoc(QObject *parent = 0);
	/**
	 * destructor
	 */
	~ItemToolBoxDoc();
	/**
	 * return mime data for drag&drop purpose
	 * @param indexes 
	 * @return 
	 */
	QMimeData *mimeData(const QModelIndexList &indexes) const;
	/**
	 * return count of rows (count of displayed formating objects)
	 * @param parent 
	 * @return 
	 */
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	/**
	 * return string name and pixmap which represent indexed object for DisplayRole
	 * for ETypeRole return unum define of name of formating object
	 * @param index 
	 * @param role 
	 * @return 
	 */
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	/**
	 * 
	 * @param index 
	 * @return 
	 */
	Qt::ItemFlags flags(const QModelIndex &index) const;
private:
	/**
	 * spcify own role for data method
	 * @param index 
	 * @return 
	 */
	enum {ETypeRole = Qt::UserRole+1};
	/**
	 * contain list of displayed names of formating objects
	 * @param index 
	 * @return 
	 */
	QList<ObjectFoDoc::FoObjectType> m_list;
};

#endif
