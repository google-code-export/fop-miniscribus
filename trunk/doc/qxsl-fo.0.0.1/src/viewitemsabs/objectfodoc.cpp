//own include
#include "objectfodoc.h"

//qt include
#include <QPixmap>

//project includes

ObjectFoDoc::ObjectFoDoc(FoObjectType objectType)
:m_id(0),m_height(0),m_width(0),m_objectType(objectType)
{

}


ObjectFoDoc::~ObjectFoDoc()
{

}

FoSize ObjectFoDoc::widthValue()
{
	return m_width;
}

FoSize ObjectFoDoc::height()
{
	return m_height;
}

QPoint ObjectFoDoc::positionValue()
{
	return m_position;
}


bool ObjectFoDoc::hasAbsolutePositioning()
{
	//check if value of position is computed (false) or is readed from fodoc (true)
	//it depend on type of object
	if(type()==ObjectFoDoc::typeFoBlockContainer)
	{
		return true;
	}
	else if	(	type()==ObjectFoDoc::typeFoBlock || 
			type()==ObjectFoDoc::typeFoTable
		)
	{
		return false;
	}
	
	qFatal("Positioning not handled in current objectfo");
	return false;
}

void ObjectFoDoc::doSelect()
{
	emit  sigDoSelect();
}

void ObjectFoDoc::doUnselect()
{
	emit  sigDoUnselect();
}

/* ******************************************************** */
/*                  STATIC METHODS                          */
/* ******************************************************** */


QString ObjectFoDoc::typeToString(const FoObjectType type)
{
	QString result;//this string is null
	
	switch(type)
	{
		case typeFoBlock:
			result="fo:block";
			break;
		case typeFoBlockContainer:
			result="fo:block-container";
			break;
		case typeFoBorderRegion:
			result="fo:border-region";
			break;
		case typeFoBodyRegion:
			result="fo:body-region";
			break;
		case typeFoTable:
			result="fo:table";
			break;
		case typeFoTableCell:
			result="fo:table-cell";
			break;
		case typeFoTableRow:
			result="fo:table-row";
			break;
		case typeFoTableBody:
			result="fo:table-body";
			break;
		case typeFoTableHeader:
			result="fo:table-header";
			break;
		case typeFoExternalGraphic:
			result="fo:external-graphic";
			break;
		case typeFoPage:
			result="page";
			break;
		case typeFoTextContent:
			result="text";
			break;
		case typeNone:
			break;
	}
	//return null string if can't assign type to string
	return result;
}

ObjectFoDoc::FoObjectType ObjectFoDoc::stringToType(const QString typeName)
{	
	if (typeName=="fo:block")
		return typeFoBlock;
	else if (typeName=="fo:block-container")
		return typeFoBlockContainer;
	else if (typeName=="fo:border-region")
		return typeFoBorderRegion;
	else if (typeName=="fo:body-region")
		return typeFoBodyRegion;
	else if (typeName=="fo:table")
		return typeFoTable;
	else if (typeName=="fo:table-row")
		return typeFoTableRow;
	else if (typeName=="fo:table-cell")
		return typeFoTableCell;
	else if (typeName=="fo:table-body")
		return typeFoTableBody;
	else if (typeName=="fo:table-header")
		return typeFoTableHeader;
	else
		return typeNone;
}

QPixmap ObjectFoDoc::typeToPixmap(const FoObjectType type)
{
	switch(type)
	{
		case typeFoBlock:
			return QPixmap(":images/foobjects/foblock.png");
			break;
		case typeFoBlockContainer:
			return QPixmap(":images/foobjects/foblockcontainer.png");
			break;
		case typeFoTable:
			return QPixmap(":images/foobjects/fotable.png");
			break;
		case typeFoExternalGraphic:
			return QPixmap(":images/foobjects/foexternalgraphics.png");
			break;
		case typeFoTextContent:
			return QPixmap(":images/foobjects/text.png");
			break;
		case typeFoBorderRegion:
		case typeFoBodyRegion:
			return QPixmap(":images/foobjects/foregion.png");
			break;
		case typeFoPage:
			return QPixmap(":images/foobjects/page.png");
			break;
		default:
			return QPixmap();
	}
}

void ObjectFoDoc::moveTo(QPoint newPos)
{
	//set position
	setPositionValue(newPos);
	//emit signal for view
	emit sigMoveTo(newPos);
}

void ObjectFoDoc::resize(QPoint newSize)
{
	//set attribute value
	setWidthValue(newSize.x());
	//emit signal for view
	emit sigSetRectangle(0,0,newSize.x(),newSize.y(),true);
}

void ObjectFoDoc::resetDoc()
{
	m_height=0;
}
