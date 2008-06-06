//own include
#include "rowinblockview.h"

//qt includes
#include <QGraphicsItemGroup>
#include <QPoint>

//project includes
#include "foblockview.h"
#include "textcontentview.h"
#include "foexternalgraphicview.h"
#include "objectfodoc.h"
#include "textcontentdoc.h"

RowInBlockView::RowInBlockView(FoBlockView *foBlockView)
:QGraphicsItemGroup(foBlockView->parentGraphicsItemGroup()),m_currentlyFilledRowWidth(0),m_pFoBlockView(foBlockView)
{
	setHandlesChildEvents (false);
}

RowInBlockView::~RowInBlockView()
{
	
}

int RowInBlockView::rowWidth()
{
	int width=m_pFoBlockView->doc()->widthValue();
	return width;
}

bool RowInBlockView::addGraphicToRow(FoExternalGraphicView *pGraphicView)
{
	int remainPlaceInRow=rowWidth()-m_currentlyFilledRowWidth;
	if (remainPlaceInRow>=pGraphicView->pixmapWidth())
	{
		pGraphicView->assignGroup(this);
		pGraphicView->setPos(QPoint(m_currentlyFilledRowWidth,0));
		m_currentlyFilledRowWidth+=pGraphicView->pixmapWidth();
		return true;
	}
	else
	{
		return false;
	}
}

int RowInBlockView::addTextToRow(TextContentDoc *pTextContentDoc,int offsetOfUnProcessedText)
{
	TextContentView *pTextContentView = new TextContentView(pTextContentDoc,m_pFoBlockView->mainView(),this,offsetOfUnProcessedText);
	
	pTextContentView->setPos(QPoint(m_currentlyFilledRowWidth,0));
	int widthOfFreePlaceInRow=rowWidth()-m_currentlyFilledRowWidth;
	
	bool appendState = pTextContentView->fillViewFromDocText(widthOfFreePlaceInRow);
	
	if (appendState==false)
		m_currentlyFilledRowWidth=rowWidth();
	else
		m_currentlyFilledRowWidth+=pTextContentView->boundingRect().width();
	
	offsetOfUnProcessedText+=pTextContentView->countOfChars();
	
	return offsetOfUnProcessedText;
}

int RowInBlockView::remainWidthOfRow()
{
	return rowWidth()-m_currentlyFilledRowWidth;
}

