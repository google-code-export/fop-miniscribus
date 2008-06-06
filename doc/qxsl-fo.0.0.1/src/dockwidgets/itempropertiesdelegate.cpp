//own include
#include "itempropertiesdelegate.h"

//qt include
#include <QtGui>

//project includes
#include "attributefo.h"
#include "itempropertiesdoc.h"

ItemPropertiesDelegate::ItemPropertiesDelegate(QObject *parent)
: QItemDelegate(parent)
{
	
}

ItemPropertiesDelegate::~ItemPropertiesDelegate()
{
	
}

QWidget *ItemPropertiesDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &,const QModelIndex &index) const
{
	//we want work only with 1st column where are values of foattributes
	if (index.column() != 1)
		return NULL;
	
	AttributeFo::EAttributeGroup groupOfAttribute;
	//we get group of indexed attribute and we use GroupOfFoAttributeRole role
	//to get it look at QVariant ItemPropertiesDoc::data(const QModelIndex &index, int role) const
	//use typecast to convert int to EAttributeGroup
	groupOfAttribute=AttributeFo::EAttributeGroup(index.model()->data(index, ItemPropertiesDoc::GroupOfFoAttributeRole).toInt());
	
	switch (groupOfAttribute)
	{
		//for sizeGroup we create spinbox
		case AttributeFo::sizeGroup:
		{
			QSpinBox *editor = new QSpinBox(parent);
			editor->setMinimum(0);
			editor->setMaximum(500);
			return editor;
		}
		//for align group create combobox
		case AttributeFo::alignGroup:
		{
			QComboBox *cmbWidget = new QComboBox(parent);
			cmbWidget->addItem("center");
			cmbWidget->addItem("right");
			cmbWidget->addItem("left");
			return cmbWidget;
		}
		case AttributeFo::colorGroup:
		case AttributeFo::fontFamilyGroup:
		case AttributeFo::stringGroup:
		case AttributeFo::noneGroup:
		default:
			return NULL;
	}
}

void ItemPropertiesDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
	AttributeFo::EAttributeGroup groupOfAttribute;
	//we get group of indexed attribute and we use GroupOfFoAttributeRole role
	//to get it look at QVariant ItemPropertiesDoc::data(const QModelIndex &index, int role) const
	//use typecast to convert int to EAttributeGroup
	groupOfAttribute=AttributeFo::EAttributeGroup(index.model()->data(index, ItemPropertiesDoc::GroupOfFoAttributeRole).toInt());
	
	switch (groupOfAttribute)
	{
		case AttributeFo::sizeGroup:
		{
			int value = index.model()->data(index, Qt::DisplayRole).toInt();
			
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			spinBox->setValue(value);
			break;
		}
		
		case AttributeFo::alignGroup:
		{
			QString value = index.model()->data(index, Qt::DisplayRole).toString();
			
			QComboBox *cmbWidget = static_cast<QComboBox*>(editor);
			int indexOfSelectedText=cmbWidget->findText(value);//bed bed ugly code
			cmbWidget->setCurrentIndex(indexOfSelectedText);
			break;
		}
		case AttributeFo::colorGroup:
		case AttributeFo::fontFamilyGroup:
		case AttributeFo::stringGroup:
		case AttributeFo::numberGroup:
		case AttributeFo::noneGroup:
		default:
			qFatal("cant set editor data for specific type of foObject");
			break;
	}
}

void ItemPropertiesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
	AttributeFo::EAttributeGroup groupOfAttribute;
	//we get group of indexed attribute and we use GroupOfFoAttributeRole role
	//to get it look at QVariant ItemPropertiesDoc::data(const QModelIndex &index, int role) const
	//use typecast to convert int to EAttributeGroup
	groupOfAttribute=AttributeFo::EAttributeGroup(index.model()->data(index, ItemPropertiesDoc::GroupOfFoAttributeRole).toInt());
	
	switch (groupOfAttribute)
	{
		case AttributeFo::sizeGroup:
		{
			QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
			spinBox->interpretText();
			int value = spinBox->value();
			
			model->setData(index, value);
			break;
		}
		case AttributeFo::alignGroup:
		{
			QComboBox *cmbWidget = static_cast<QComboBox*>(editor);
			QString value = cmbWidget->currentText();
			
			model->setData(index, value);
			break;
		}
		case AttributeFo::colorGroup:
		case AttributeFo::fontFamilyGroup:
		case AttributeFo::stringGroup:
		case AttributeFo::numberGroup:
		case AttributeFo::noneGroup:
		default:
			qFatal("cant set model data for specific type of foObject");
			break;
	}
}
