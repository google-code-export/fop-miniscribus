
#include <QBrush>
#include <QPen>

//own include
#include "foborderregionview.h"

//qt includes

//project includes
#include "pagedoc.h"
#include "objectfosequencedview.h"
#include "foborderregiondoc.h"
#include "nodefo.h"

FoBorderRegionView::FoBorderRegionView( ObjectFoContainerView *parentContainer,ObjectFoSequencedDoc *doc,MainView *pMainView)
 : ObjectFoSequencedView(parentContainer,doc,pMainView)
{
	QBrush brush(QColor(255,255,255,200));
	setBrush(brush);
	
	QPen penReg(QColor(0,0,0));
	setPen(penReg);

	draw();

	setZValue ( PageView::EFoBorderRegionViewZPosition );
	setFlag(QGraphicsItem::ItemIsSelectable, true);
}

FoBorderRegionView::~FoBorderRegionView()
{

}

void FoBorderRegionView::draw()
{
	int regionWidth,regionHeight,regionX,regionY;
	
	//get width, height and margin from simple page
	AttributeFoData *attrPageHeight=borderRegionDoc()->pageDoc()->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::page_height);
	AttributeFoData *attrPageMargin=borderRegionDoc()->pageDoc()->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::margin);

	int pageWidth=borderRegionDoc()->pageDoc()->widthValue();
	int pageHeight=attrPageHeight->sizeValue().m_value;
	int margin=attrPageMargin->sizeValue().m_value;

	int extent=borderRegionDoc()->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::extent)->sizeValue().m_value;
	
	//and set coordinates according to the type of the region
	switch (borderRegionDoc()->regionType())
	{
		case FoBorderRegionDoc::regionBefore:
			regionX=margin+borderRegionDoc()->xPos();
			regionY=margin;
			regionWidth=borderRegionDoc()->widthValue();//pageWidth-margin-margin;
			regionHeight=extent;
			break;
		case FoBorderRegionDoc::regionAfter:
			regionX=margin+borderRegionDoc()->xPos();
			regionY=pageHeight-margin-extent;
			regionWidth=borderRegionDoc()->widthValue();//pageWidth-margin-margin;
			regionHeight=extent;
			break;
		case FoBorderRegionDoc::regionStart:
			regionX=margin;
			regionY=margin;
			regionWidth=extent;
			regionHeight=pageHeight-margin-margin;
			break;
		case FoBorderRegionDoc::regionEnd:
			regionX=pageWidth-extent-margin;
			regionY=margin;
			regionWidth=extent;
			regionHeight=pageHeight-margin-margin;
			break;
	}
	
	//call method to draw rect
	ObjectFoSequencedView::setPos(regionX,regionY);
	ObjectFoSequencedView::setRect(0,0,regionWidth,regionHeight);
}
