//own include
#include "fotablebodydoc.h"

#include <vector>

//qt include
#include <QDomElement>

//project includes
#include "objectfosimpledoc.h"
#include "fotablerowdoc.h"
#include "objectfocontainerdoc.h"
#include "focreator.h"

FoTableBodyDoc::FoTableBodyDoc(QDomElement pElement,ObjectFoContainerDoc *parent)
 : ObjectFoContainerSimpleDoc(ObjectFoDoc::typeFoTableBody,parent,pElement)
{
	//fill table with table rows according to fodocument
	loadTableRows(pElement);
}

FoTableBodyDoc::~FoTableBodyDoc()
{
	///todo:delete item in vector
}


void FoTableBodyDoc::loadTableRows(QDomElement pElement)
{
	int rowNum=0;
	for(QDomNode n = pElement.firstChild();!n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull())
		{
			if (e.tagName()=="fo:table-row")
			{
				m_pFoTableRowDoc.push_back(new FoTableRowDoc(rowNum,e,this));
				rowNum++;
			}
		}
	}
}

FoTableRowDoc *FoTableBodyDoc::row(unsigned int i)
{
	if (i<m_pFoTableRowDoc.size())
		return m_pFoTableRowDoc[i];
	else
		return NULL;
}

FoTableRowDoc *FoTableBodyDoc::appendRow()
{
	FoCreator *foCreator=FoCreator::GetInstancePtr();
	QDomElement newRowElement=foCreator->foRowElement();
	
	int lastRowPos=m_pFoTableRowDoc.size()-1;
	
	FoTableRowDoc *lastRow=m_pFoTableRowDoc.at(lastRowPos);
	QDomElement lastRowData=lastRow->getNodeFo()->elementData();
	
	FoTableRowDoc *tableRowDoc = new FoTableRowDoc(lastRowPos+1,newRowElement,this);
	m_pFoTableRowDoc.push_back(tableRowDoc);
	
	return tableRowDoc;
}

void FoTableBodyDoc::appendColumn()
{
	for(unsigned int rowNum=0;rowNum<m_pFoTableRowDoc.size();rowNum++)
	{
		m_pFoTableRowDoc.at(rowNum)->appendCell();
	}
}

FoSize FoTableBodyDoc::widthValue()
{
	return m_pParentContainer->widthValue();
}
