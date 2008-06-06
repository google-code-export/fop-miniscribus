//own include
#include "foblockview.h"

#include <vector>

//qt includes
#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>

//project includes
#include "pageview.h"
#include "foblockdoc.h"
#include "objectfocontainersimpleview.h"
#include "textcontentview.h"
#include "foexternalgraphicdoc.h"
#include "foexternalgraphicview.h"
#include "rowinblockview.h"
#include "objectfocontainerview.h"
#include "textcontentdoc.h"

FoBlockView::FoBlockView(ObjectFoContainerView *parentContainer,FoBlockDoc *doc,MainView *pMainView)
 : ObjectFoContainerSimpleView(parentContainer,doc,pMainView)
{
	QBrush brush(QColor(255,255,255,200));
	setBrush(brush);
	QPen penReg(QColor(0,0,0));
	setPen(penReg);
	setZValue ( PageView::ESimpleFoObjectViewZPosition );
	//set item selectable
	setFlag(QGraphicsItem::ItemIsSelectable, true);
		
	loadChildrenToView(doc);
	//get new position for our block from parent
	doc->setPositionValue(parentContainer->objectFoContainerDoc()->positionForNewChildFoObject());
	
	setPos(doc->positionValue());
	
	doc->setHeight(heightOfBlockView());
	
	slSetRectangle(0,0,doc->widthValue(),doc->height(),false);
}

FoBlockView::~FoBlockView()
{
	
}

void FoBlockView::loadChildrenToView(ObjectFoContainerDoc *doc)
{
	//create first row
	createAndAppendNewRow();
	
	ObjectFoDoc *viewableBlock;
	//go trought all block in foblock
	for(unsigned int a=0;a<doc->countOfFoObjectsChildren();a++)
	{
		//get doc from child vector
		viewableBlock = doc->foObjectChildren(a);
		//and resolve type of child
		if (viewableBlock->type()==ObjectFoDoc::typeFoTextContent)
		{
			appendTextContent((TextContentDoc*)viewableBlock);
		}
		else if (viewableBlock->type()==ObjectFoDoc::typeFoExternalGraphic)
		{
			appendFoExternalGraphic((FoExternalGraphicDoc*)viewableBlock);
		}
	}
}

void FoBlockView::appendTextContent(TextContentDoc *pTextDoc)
{
	int offsetOfUnProcessedText=0;
	
	while (offsetOfUnProcessedText<pTextDoc->size())
	{
		//check free space in row
		if ( lastRow()->remainWidthOfRow() <= 0 )
			//if we haven't free space in last row, create new row
			createAndAppendNewRow();
		//try to add text from textcontentdoc and use start from (pTextDoc->size()-notAddedRemainOfText)
		offsetOfUnProcessedText=lastRow()->addTextToRow(pTextDoc,offsetOfUnProcessedText);
	}
}

void FoBlockView::appendFoExternalGraphic(FoExternalGraphicDoc *pGraphicDoc)
{
	//create view for text and put it to the last row
	FoExternalGraphicView *pGraphicView = new FoExternalGraphicView(this,pGraphicDoc,m_pMainView);
	
	if (lastRow()->addGraphicToRow(pGraphicView)==false)
	{
		RowInBlockView *newRow=createAndAppendNewRow();
		if (newRow->addGraphicToRow(pGraphicView))
		{
			qFatal("Ugh, inserted picture is larger than width of FoBlockView");
		}
	}
}

int FoBlockView::heightOfBlockView()
{
	int heightOfBlockView=0;
	
	//go throught all rows,and count all their height - it's position for new row
	for(unsigned int a=0;a<m_rowsInBlock.size();a++)
	{
		QRectF rect=m_rowsInBlock.at(a)->childrenBoundingRect();
		//QPointF pos=m_rowsOfBlock.at(a)->pos();
		heightOfBlockView+=rect.height();
		//qDebug(QString(">%1<>%2").arg(width).arg(height).toAscii());
	}
	
	return heightOfBlockView;
}

RowInBlockView *FoBlockView::createAndAppendNewRow()
{
	RowInBlockView *newRow = new RowInBlockView(this);
	//get height of actual blockview (height all rows together) - it's position for new row
	int newPositionOn_Y = heightOfBlockView();
	//set position for new row
	newRow->setPos(0,newPositionOn_Y);
	//append row to vector
	m_rowsInBlock.push_back(newRow);
	
	return newRow;
}
