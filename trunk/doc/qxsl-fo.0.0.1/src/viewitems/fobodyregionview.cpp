//own include
#include "fobodyregionview.h"

//qt includes


//project includes
#include "pagedoc.h"
#include "maindoc.h"
#include "fobodyregiondoc.h"
#include "focreator.h"
#include "foblockdoc.h"
#include "foblockcontainerdoc.h"
#include "fotabledoc.h"
#include "objectfosimpledoc.h"
#include "objectfosequenceddoc.h"
#include "nodefo.h"

#include "resizehandlewindow.h"
#include "pageview.h"
#include "foblockcontainerview.h"
#include "objectfosequencedview.h"
#include "mainview.h"

FoBodyRegionView::FoBodyRegionView (NodeFo *pFoSimplePageMaster,ObjectFoSequencedView *parentView,ObjectFoSequencedDoc *doc,MainView *pMainView )
: ObjectFoSequencedView(parentView,doc,pMainView),m_pFoSimplePageMaster ( pFoSimplePageMaster )
{
	QBrush brush ( QColor ( 255, 255, 255, 200 ) );
	setBrush ( brush );
	
	QPen penReg(QColor(0,0,0));
	setPen ( penReg );
	
	//accept drop from others widgets
	setAcceptDrops ( true );
	
	//draw rect of body
	draw();
	
	m_pResizeHandleWindow=new ResizeHandleWindow(this);
		
	//set z posittion
	setZValue ( PageView::EFoBodyRegionViewZPosition );
	setFlag ( QGraphicsItem::ItemIsSelectable, true );
}


FoBodyRegionView::~FoBodyRegionView()
{
}

void FoBodyRegionView::draw()
{
	
	setPos(doc()->positionValue());
	
	setRect (0,0,doc()->widthValue(),doc()->height());
}

void FoBodyRegionView::slSetRectangle ( qreal x, qreal y, qreal width, qreal height , bool setUpObjectHandler)
{
	ObjectFoSequencedView::slSetRectangle(x,y,width,height,setUpObjectHandler);
	m_pResizeHandleWindow->mapPositionToParent(this);
}
