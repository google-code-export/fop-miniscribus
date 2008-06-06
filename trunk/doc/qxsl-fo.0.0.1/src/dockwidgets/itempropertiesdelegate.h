#ifndef ITEMPROPERTIESDELEGATE_H
#define ITEMPROPERTIESDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>

/**
 * class represent delegate of itemproperties doc , it handle different tytes of attributes for edit and display
 * e.g. it use combobox (for align) and spinbox (for size values) to data display and edit data
 */
class ItemPropertiesDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	/**
	 * constructor
	 * @param parent 
	 */
	ItemPropertiesDelegate(QObject *parent=0);
	/**
	 * destructor
	 */
	~ItemPropertiesDelegate();
	/**
	 * create editor, type of editor depend on type of indexed data
	 * @param parent 
	 * @param  
	 * @param  
	 * @return 
	 */
	QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &,const QModelIndex &) const;
	/**
	 * set editor data, 
	 * @param editor 
	 * @param index 
	 */
	void setEditorData(QWidget *editor,const QModelIndex &index) const;
	/**
	 * set data in model
	 * @param editor 
	 * @param model 
	 * @param index 
	 */
	void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
};

#endif
