//own include
#include "pageview.h"

//qt include
#include <QtGui>
#include <QUndoStack>

//project includevoid MainView::mousePressEvent ( QGraphicsSceneMouseEvent * event )
#include "foblockview.h"
#include "foblockdoc.h"
#include "pagedoc.h"
#include "objectfosequencedview.h"
#include "mainview.h"
#include "maindoc.h"
#include "objectfodoc.h"
#include "foborderregionview.h"
#include "fobodyregionview.h"
#include "resizehandlewindow.h"
#include "cmdresizefoobject.h"

PageView::PageView(ObjectFoSequencedDoc *doc,MainView *pMainView,MainView * scene)
 : ObjectFoSequencedView(NULL,doc,pMainView,scene)
{
	setPen(QPen(Qt::black));/*set black border */
	setBrush(QBrush(Qt::white));/*set fill color*/
	
	setZValue ( PageView::EPageViewZPosition );

	setFlag(QGraphicsItem::ItemIsSelectable, true);
		
	CreatePageView(PageView::EPageDistance);
	
}

PageView::~PageView()
{

}

//return new vertical offset for next new page
int PageView::CreatePageView(int verticalShift)
{
	//get width and height of the page
	AttributeFoData *attrWidth=pageDoc()->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::page_width);
	AttributeFoData *attrHeight=pageDoc()->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::page_height);
	int height=attrHeight->sizeValue().m_value;
	int width=attrWidth->sizeValue().m_value;

	//draw rect of the page
	setRect(0,EPageDistance+verticalShift,width,height);

	//create group for margins and regions
	parentGraphicsItemGroup()->setPos(rect().x(),rect().y());
	
	//draw margins left,right,bottom.top
	CreateMargins();
	
	//create fobodyregion
	CreateBodyRegion();
	
	//create all four border regions
	CreateBorderRegions(FoBorderRegionDoc::regionBefore);
	CreateBorderRegions(FoBorderRegionDoc::regionAfter);
	CreateBorderRegions(FoBorderRegionDoc::regionStart);
	CreateBorderRegions(FoBorderRegionDoc::regionEnd);
	
	//return coorditnates for next page
	return EPageDistance+verticalShift+height;
}

///todo: margins should have own classes
void PageView::CreateMargins()
{
	AttributeFoData *attrWidth=pageDoc()->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::page_width);
	AttributeFoData *attrHeight=pageDoc()->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::page_height);
	AttributeFoData *attrMargin=pageDoc()->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::margin);

	int width=attrWidth->sizeValue().m_value;
	int height=attrHeight->sizeValue().m_value;
	int margin=attrMargin->sizeValue().m_value;

	//create margins
	QPen pen(QColor(0,0,0),0);//width pen is 0 ->  pen width is always drawn one pixel wide, useful while zooming
	
	QGraphicsLineItem *l1= new QGraphicsLineItem (0,margin,width,margin,parentGraphicsItemGroup());//top
	l1->setPen(pen);
	l1->setZValue(PageView::EMarginsViewZPosition);
	QGraphicsLineItem *l2= new QGraphicsLineItem (0,height-margin,width,height-margin,parentGraphicsItemGroup());//bottom
	l2->setPen(pen);
	l2->setZValue(PageView::EMarginsViewZPosition);
	QGraphicsLineItem *l3= new QGraphicsLineItem (margin,height,margin,0,parentGraphicsItemGroup());//left
	l3->setPen(pen);
	l3->setZValue(PageView::EMarginsViewZPosition);
	QGraphicsLineItem *l4= new QGraphicsLineItem (width-margin,0,width-margin,height,parentGraphicsItemGroup());//right
	l4->setPen(pen);
	l4->setZValue(PageView::EMarginsViewZPosition);

}

void PageView::CreateBodyRegion()
{
	new FoBodyRegionView(pageDoc()->getFoSimplePageMasterPart(),this,(ObjectFoSequencedDoc*)pageDoc()->bodyRegion(),m_pMainView);
}

void PageView::CreateBorderRegions(FoBorderRegionDoc::EBorderRegionType regionType)
{
	//create view for border regions
	FoBorderRegionView *borderRegionView;
	borderRegionView = new FoBorderRegionView(this,pageDoc()->borderRegion(regionType),m_pMainView);
}
