//own include
#include "foblockcontainerview.h"

//qt includes
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QDrag>
#include <QMimeData>

//project includes
#include "foblockcontainerdoc.h"
#include "objectfodoc.h"
#include "objectfocontainersimpleview.h"
#include "objectfosimpledoc.h"
#include "resizehandlewindow.h"
#include "pageview.h"

FoBlockContainerView::FoBlockContainerView(ObjectFoContainerView * parent,ObjectFoContainerDoc *doc,MainView *pMainView)
 : ObjectFoContainerSimpleView(parent,doc,pMainView)
{
	QBrush brush(QColor(255,255,255,200));
	setBrush(brush);

	QPen penReg(QColor(0,0,0));
	setPen(penReg);
	
	//calculate pos according to parent
	setPos(foBlockContainerDoc()->positionValue());
	//calculate width according to parent
	int width=foBlockContainerDoc()->widthValue();
	//calculate width according to children
	int height=foBlockContainerDoc()->height();
	//resize handle window must be crated befor we setRectangle
	m_pResizeHandleWindow=new ResizeHandleWindow(this);
	
	slSetRectangle(0,0,width,height,false);
	
	//parentGraphicsItemGroup()->setZValue ( PageView::ESimpleFoObjectViewZPosition );
	setZValue ( PageView::ESimpleFoBlockContainerViewZPosition);
	
	setAcceptDrops ( true );
}

FoBlockContainerView::~FoBlockContainerView()
{
	
}

void FoBlockContainerView::slSetRectangle ( qreal x, qreal y, qreal width, qreal height , bool setUpObjectHandler)
{
	ObjectFoContainerSimpleView::slSetRectangle(x,y,width,height,setUpObjectHandler);
	m_pResizeHandleWindow->mapPositionToParent(this);
}
