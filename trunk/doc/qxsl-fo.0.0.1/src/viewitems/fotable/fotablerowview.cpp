//own include
#include "fotablerowview.h"

#include <vector>

//qt includes
#include <QGraphicsItem>

//project includes
#include "objectfocontainersimpleview.h"
#include "objectfodoc.h"
#include "mainview.h"
#include "fotableview.h"
#include "fotablerowdoc.h"
#include "fotablecellview.h"
#include "fotablecelldoc.h"

FoTableRowView::FoTableRowView(FoTableView *parent,ObjectFoContainerDoc *doc,MainView *pMainView)
:ObjectFoContainerSimpleView(parent,doc,pMainView),m_pParentTableView(parent)
{
	setHandlesChildEvents (false);
	loadDataToRowView();
}


FoTableRowView::~FoTableRowView()
{
}

QPoint FoTableRowView::calculateTopLeftPosition()
{
	int actualRow=foTableRowDoc()->rowNumber();
	QPoint pos=m_pParentTableView->calculatePositionForRow(actualRow);
	return pos;
}

void FoTableRowView::loadDataToRowView()
{
	int heightOfRow=0;
	FoTableRowDoc *rowDoc=foTableRowDoc();
	int countOfCell=rowDoc->cellCount();
	//go through the all cells in row
	for(int iCol=0;iCol<countOfCell;iCol++)
	{
		//get celldoc
		FoTableCellDoc *cellDoc=rowDoc->cell(iCol);
		FoTableCellView *cellView=new FoTableCellView(this,cellDoc,m_pMainView);
		//put new row to vector
		m_pFoTableCellView.push_back(cellView);
		//find the highest cell
		if (cellView->doc()->height() > heightOfRow)
			heightOfRow=cellView->doc()->height();
	}
	
	foTableRowDoc()->setHeight(heightOfRow);
}

void FoTableRowView::appendCell()
{
	//create new celldoc
	FoTableCellDoc *cellDoc = foTableRowDoc()->appendCell();
	
	//create view
	FoTableCellView *cellView=new FoTableCellView(this,cellDoc,m_pMainView);
	//append it to vector
	m_pFoTableCellView.push_back(cellView);
	
	//go through the whole vector of view and resize it to the new width
	int width=0;int height=0;
	for(int iCol=0;iCol<foTableRowDoc()->cellCount();iCol++)
	{
		width=m_pFoTableCellView.at(iCol)->foTableCellDoc()->widthValue();
		height=m_pFoTableCellView.at(iCol)->foTableCellDoc()->height();
		m_pFoTableCellView.at(iCol)->setPos(m_pFoTableCellView.at(iCol)->calculatePosition());
		m_pFoTableCellView.at(iCol)->setRect(0,0,width,height);
	}
}
