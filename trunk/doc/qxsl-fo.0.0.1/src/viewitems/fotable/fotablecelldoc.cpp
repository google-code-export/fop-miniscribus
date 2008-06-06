//own include
#include "fotablecelldoc.h"

//qt includes
#include <QDomElement>

//project includes
#include "objectfocontainersimpledoc.h"
#include "fotablerowdoc.h"

FoTableCellDoc::FoTableCellDoc(int cellNum,QDomElement pElement,FoTableRowDoc *parentRowDoc)
:ObjectFoContainerSimpleDoc(ObjectFoDoc::typeFoTableCell,parentRowDoc,pElement),m_cellNumber(cellNum),m_pParentRowDoc(parentRowDoc)
{
	ObjectFoContainerDoc::loadElementChildrenToContainer(pElement);
}


FoTableCellDoc::~FoTableCellDoc()
{
	
}


FoSize FoTableCellDoc::widthValue()
{
	int width = m_pParentRowDoc->widthValue()/m_pParentRowDoc->cellCount();
	
	return width;
}

FoSize FoTableCellDoc::height()
{
	return ObjectFoContainerDoc::wholeHeightOfChildren();
}

void FoTableCellDoc::setXslTemplate(QString xmlPath)
{
	m_pParentRowDoc->setXslTemplate(xmlPath);
/*	//get style sheet root
	QDomNode xslStyleSheetRoot=m_pFoObject->xslStyleSheetRoot();

	//get node of this foobject
	QDomNode currentNode=m_pFoObject->elementData();
	//get parent node
	QDomNode parentNode=currentNode.parentNode();
	//remove curent foobjparentElementect node from parent
	QDomNode removedNode=parentNode.removeChild(currentNode);
	if (removedNode.isNull())
		qFatal("Error occurs while moving foobjectsimple to xsltemplate match");
	//and create xsl-template match from it
	QDomNode xslTemplateMatch=FoCreator::GetInstancePtr()->xslTemplateMatch(xmlPath);
	xslTemplateMatch.appendChild(removedNode);
	
	//and append teplate match to it
	xslStyleSheetRoot.appendChild(xslTemplateMatch);
	
	//and append xsl:apply templates to parent, where was predtym removedNode
	QDomNode xslApplyTemplate=FoCreator::GetInstancePtr()->xslApplyTemplates(xmlPath);
	parentNode.appendChild(xslApplyTemplate);
*/
}

