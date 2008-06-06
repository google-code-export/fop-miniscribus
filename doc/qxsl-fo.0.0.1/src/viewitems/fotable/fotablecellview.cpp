//own include
#include "fotablecellview.h"

//qt include
#include <QGraphicsItem>
#include <QPen>
#include <QMenu>
#include <QAction>

//project include
#include "objectfosimpledoc.h"
#include "objectfodoc.h"
#include "fotablecelldoc.h"
#include "fotablerowdoc.h"

#include "fotablerowview.h"
#include "fotableview.h"
#include "pageview.h"
#include "objectfocontainersimpleview.h"
#include "mainview.h"

FoTableCellView::FoTableCellView(FoTableRowView *pRowParent,ObjectFoContainerDoc *doc,MainView *pMainView)
:ObjectFoContainerSimpleView(pRowParent,doc,pMainView),m_pTableRowView(pRowParent)
{
	//calculate pos according to parent
	QGraphicsRectItem::setPos(calculatePosition());
	
	//calculate width according to parent
	int width=foTableCellDoc()->widthValue();
	
	int height=foTableCellDoc()->height();
	setRect(0,0,width,height);
	
	setZValue ( PageView::ESimpleFoObjectViewZPosition );
	
	//set item selectable
	setFlag(QGraphicsItem::ItemIsSelectable, true);
}

FoTableCellView::~FoTableCellView()
{
	
}

void FoTableCellView::resizeAfterAppendCell()
{
}

QPoint FoTableCellView::calculatePosition()
{
	int width=foTableCellDoc()->widthValue();
	int celNum=foTableCellDoc()->cellNumber();
	//get topleft of row (we need y axis) 
	QPoint pointTopLeft = m_pTableRowView->calculateTopLeftPosition();
	//create qpoint position
	QPoint cellPos(celNum*width,pointTopLeft.y());
	//return position
	return cellPos;
}

void FoTableCellView::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
	ObjectFoContainerSimpleView::mousePressEvent(mouseEvent);
	
	if (mouseEvent->button()==Qt::RightButton)
	{
		showCellMenu(mouseEvent);
	}
}

void FoTableCellView::addRow()
{
	//in parent table view create rowview
	m_pTableRowView->parentTableView()->appendRow();
}

void FoTableCellView::addColumn()
{
	m_pTableRowView->parentTableView()->appendColumn();
}

void FoTableCellView::showCellMenu(QGraphicsSceneMouseEvent * /*mouseEvent*/)
{
// 	QMenu cellMenu;
// 	cellMenu.addAction(pActAddRow);
// 	cellMenu.addAction(pActAddColumn);
// 	cellMenu.addSeparator();
// 	cellMenu.addAction(pActRemoveRow);
// 	cellMenu.addAction(pActRemoveColumn);
// 	cellMenu.exec(mouseEvent->screenPos());	
}

