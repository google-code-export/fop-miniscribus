//own include
#include "fotablerowdoc.h"

//qt includes
#include <QDomElement>

//project includes
#include "focreator.h"
#include "fotablecelldoc.h"
#include "objectfodoc.h"
#include "objectfocontainersimpledoc.h"
#include "objectfocontainerdoc.h"

FoTableRowDoc::FoTableRowDoc(int rowNumber,QDomElement pElement,ObjectFoContainerDoc *parent)
 : ObjectFoContainerSimpleDoc(ObjectFoDoc::typeFoTableRow,parent,pElement),m_rowNumber(rowNumber)
{
	loadTableCells(pElement);
}


FoTableRowDoc::~FoTableRowDoc()
{
	///todo: delete vector of cells
}


void FoTableRowDoc::loadTableCells(QDomElement pElement)
{
	int column=0;
	for(QDomNode n = pElement.firstChild();!n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull())
		{
			if (e.tagName()=="fo:table-cell")
			{
				m_pFoTableCellDoc.push_back(new FoTableCellDoc(column,e,this));
				column++;
			}
		}
	}
}

FoTableCellDoc *FoTableRowDoc::appendCell()
{
	FoCreator *foCreator=FoCreator::GetInstancePtr();
	QDomElement newCellElement=foCreator->foCellElement();
	
	FoTableCellDoc *lastCellDoc=m_pFoTableCellDoc.at(m_pFoTableCellDoc.size()-1);
	QDomElement lastCellData=lastCellDoc->getNodeFo()->elementData();
	
	int lastCellPosition=lastCellDoc->cellNumber();
	
//s	ObjectFoSimpleDoc::insertChildElementAfter(newCellElement,lastCellData);
	
	FoTableCellDoc *cellDoc = new FoTableCellDoc(lastCellPosition+1,newCellElement,this);
	m_pFoTableCellDoc.push_back(cellDoc);
	
	return cellDoc;
}

int  FoTableRowDoc::cellCount()
{
	int cell=0;
	QDomElement pElement=getNodeFo()->elementData();
	
	for(QDomNode n = pElement.firstChild();!n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull())
		{
			if (e.tagName()=="fo:table-cell")
			{
				cell++;
			}
		}
	}
	return cell;
}

FoSize FoTableRowDoc::widthValue()
{
	return m_pParentContainer->widthValue();
}

FoSize FoTableRowDoc::height()
{
	return ObjectFoDoc::height();
}
