//own include
#include "fotableview.h"

//qt include
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>

//project include
#include "objectfocontainersimpleview.h"
#include "objectfosimpledoc.h"
#include "fotablerowdoc.h"
#include "fotabledoc.h"
#include "fotablecelldoc.h"
#include "fotablecellview.h"
#include "fotablerowview.h"
#include "objectfodoc.h"
#include "fotablerowdoc.h"
#include "objectfocontainerview.h"

FoTableView::FoTableView(ObjectFoContainerView *parentContainer,ObjectFoContainerDoc *doc,MainView *pMainView)
:ObjectFoContainerSimpleView(parentContainer,doc,pMainView)
{

	setHandlesChildEvents (false);	
	
	QPen penReg(QColor(0,0,0));
	setPen(penReg);
		
	doc->setPositionValue(parentContainer->objectFoContainerDoc()->positionForNewChildFoObject());
	setPos(foTableDoc()->positionValue());
		
	//create table row and cell views and fill them with qdom data
	loadDataToTableView();
	
	//calculate width according to parent
	int width=foTableDoc()->widthValue();
	//height of rows was counted in loadDataToTableView to get height of table 
	int height=foTableDoc()->height();
	setRect(0,0,width,height);
	
	//set item selectable
	setFlag(QGraphicsItem::ItemIsSelectable, true);
}

FoTableView::~FoTableView()
{
///todo: delete tablecells
}

void FoTableView::loadDataToTableView()
{
	int tableHeight=0;
	
	FoTableBodyDoc *pTableBodyDoc=foTableDoc()->foTableBodyDoc();
	//go through the rows
	for(unsigned int iY=0;iY<pTableBodyDoc->rowCount();iY++)
	{
		//create rowview
		FoTableRowView *rowView= new FoTableRowView(this,pTableBodyDoc->row(iY),m_pMainView);
		//put new row to vector
		m_pFoTableRowView.push_back(rowView);
		//compute tableHeight
		tableHeight+=rowView->doc()->height();
	}
	
	foTableDoc()->setHeight(tableHeight);
}

QPoint FoTableView::calculatePositionForRow(unsigned int rowNum)
{
	FoTableBodyDoc *pTableBodyDoc=foTableDoc()->foTableBodyDoc();
	int height=0;
	unsigned int rowCount=pTableBodyDoc->rowCount();
	
	for(unsigned int i=0;i<rowNum&&i<rowCount;i++)
	{
		height+=m_pFoTableRowView.at(i)->doc()->height();
	}
	
	return QPoint(0,height);
}

void FoTableView::appendColumn()
{
	//get count of rows
	FoTableBodyDoc *pTableBodyDoc=foTableDoc()->foTableBodyDoc();
	unsigned int rowCount=pTableBodyDoc->rowCount();
	
	
	//go throught the all rows and append cell to the end of each row	
	for(unsigned int i=0;i<rowCount;i++)
	{
		m_pFoTableRowView.at(i)->appendCell();
	}
	
}

void FoTableView::appendRow()
{
	//create doc part of row - it is append after last row
	FoTableRowDoc *rowDoc = foTableDoc()->appendRow();
	//we have now doc, so create view
	FoTableRowView *rowView = new FoTableRowView(this,rowDoc,m_pMainView);
	//and append it to vector of tablerowViews
	m_pFoTableRowView.push_back(rowView);
}
