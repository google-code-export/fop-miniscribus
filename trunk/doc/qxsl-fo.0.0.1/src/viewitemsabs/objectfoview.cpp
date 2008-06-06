//own include
#include "objectfoview.h"

//qt includes
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QFocusEvent>
#include <QApplication>
#include <QGraphicsSceneDragDropEvent>

//project includes
#include "mainview.h"
#include "pageview.h"
#include "resizehandlewindow.h"

#include "foblockdoc.h"
#include "foblockcontainerdoc.h"
#include "fotabledoc.h"
#include "objectfodoc.h"
#include "objectfohandler.h"

#include "focreator.h"

#include "cmdaddfoobject.h"
#include "cmdresizefoobject.h"

ObjectFoView::ObjectFoView(ObjectFoView *parentContainer,ObjectFoDoc *doc,MainView *pMainView,QGraphicsScene *scene)
:QGraphicsRectItem(parentContainer?parentContainer->parentGraphicsItemGroup():NULL,scene),m_pParent(parentContainer)
,m_objectFoState(editState),m_doc(doc),m_pMainView(pMainView)
{
	m_oldZValue=0;
	
	//create graphicsgroup for children
	m_parentGroup=new QGraphicsItemGroup(this);
	m_parentGroup->setHandlesChildEvents (false);
	
	setHandlesChildEvents (false);
	
	//we want active focusInEvent focusOutEvent
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setAcceptDrops(true);
	
	connect(doc,SIGNAL(sigDoSelect()),this,SLOT(slDoSelect()));
	connect(doc,SIGNAL(sigDoUnselect()),this,SLOT(slDoUnselect()));
	connect(doc,SIGNAL(sigMoveTo(QPoint)),this,SLOT(slMoveTo(QPoint)));
	connect(doc,SIGNAL(sigSetRectangle(qreal,qreal,qreal,qreal,bool)),this,SLOT(slSetRectangle(qreal,qreal,qreal,qreal,bool)));
}

ObjectFoView::~ObjectFoView()
{

}

void ObjectFoView::setForthZPosition()
{
	m_oldZValue=QGraphicsRectItem::zValue();
	setZValue(PageView::ESelectedFoObjectViewZPosition);
	//and call this method for parent
	if (m_pParent!=NULL)
		m_pParent->setForthZPosition();
}

void ObjectFoView::giveBackZPosition()
{
	setZValue(m_oldZValue);
	//and call this method for parent
	if (m_pParent!=NULL)
		m_pParent->giveBackZPosition();
}

void ObjectFoView::slSizeOfChildChanged(ObjectFoDoc */*doc*/)
{

}

void ObjectFoView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{	
	//check minimal distance required for drag
	if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length() < QApplication::startDragDistance())
	{
		return;
	}	
	
	//this method is overwrited in inherited class
	startDrag(event);
	
}

void ObjectFoView::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	QPoint oldPosition;
	QPoint distPoint;
	int intContainerView;
	ObjectFoView *pObjectFoView;
	QRectF startRect;
	
	//this dragdtop provide item resizing with small rect on right bottom
	if (event->mimeData()->hasFormat("application/resizeItem"))
	{
		QByteArray itemData = event->mimeData()->data("application/resizeItem");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);
		dataStream >> startRect >> oldPosition >> intContainerView;
	}
	else 
		return;
	
	pObjectFoView = reinterpret_cast <ObjectFoView *> (intContainerView);
	if (pObjectFoView==NULL)
		qFatal("Can't convert item in mime data to FoSimple object");
	
	QPoint r=startRect.bottomRight().toPoint();
	QPoint newPoss=event->scenePos().toPoint();

	QPoint diffPos=newPoss-oldPosition;
	QPoint newPos=r+diffPos;
	
	m_pMainView->pushToUndoStack( new CmdResizeFoObject ( r,newPos,pObjectFoView->doc() ) );
	
	QGraphicsRectItem::dragMoveEvent(event);
}

void ObjectFoView::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	QPoint oldPosition;
	QPoint distPoint;
	int intContainerView;
	ObjectFoView *pObjectFoView;
	QRectF startRect;
	
	//this dragdtop provide item resizing with small rect on right bottom
	if (event->mimeData()->hasFormat("application/resizeItem"))
	{
		QByteArray itemData = event->mimeData()->data("application/resizeItem");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);
		dataStream >> startRect >> oldPosition >> intContainerView;
	}
	else 
		return;
	
	pObjectFoView = reinterpret_cast <ObjectFoView *> (intContainerView);
	if (pObjectFoView==NULL)
		qFatal("Can't convert item in mime data to FoSimple object");
	
	QPoint r=startRect.bottomRight().toPoint();
	QPoint newPoss=event->scenePos().toPoint();

	QPoint diffPos=newPoss-oldPosition;
	QPoint newPos=r+diffPos;
	
	pObjectFoView->slSetRectangle(0,0,newPos.x(),newPos.y());
	
	QGraphicsRectItem::dragMoveEvent(event);
}

void ObjectFoView::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
{
	int dragType;
	QPoint offset;
	int intFoDoc;
	int typeOfNewFoObject;
	ObjectFoDoc *objectFoDoc;

	if (event->mimeData()->hasFormat("application/x-dnditemdata"))
	{
		//if mimedata has desired format load data from it
		QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);
		dataStream >> dragType >> offset >> intFoDoc >> typeOfNewFoObject;
		
		objectFoDoc = reinterpret_cast <ObjectFoDoc *> (intFoDoc);
			
		//check if we want move container view
		if (ObjectFoView::EDragType(dragType)==dragMoveItem)
		{
			if (objectFoDoc==NULL)
				qFatal("Pointer to foSimpleObject from mimedata is null !");
			
			//check if we start drag on parent, if no, we deny drop
			if((ObjectFoDoc*)objectFoDoc->parentContainer()==doc())
			{
				event->accept();
			}
			else
			{
				event->ignore();
			}
		}
	
	}
	
	QGraphicsRectItem::dragMoveEvent(event);
}

void ObjectFoView::mousePressEvent ( QGraphicsSceneMouseEvent *event)
{
	m_pMainView->setUpObjectFoHandler(this);
	QGraphicsRectItem::mousePressEvent(event);
}

void ObjectFoView::slDoSelect()
{
	m_pMainView->clearSelection();
	setSelected(true);
	
	setFocus();
	
	mousePressEvent(new QGraphicsSceneMouseEvent());
}

void ObjectFoView::slDoUnselect()
{
	//giveBackZPosition();
	//delete m_pObjectFoHandler;
}

ObjectFoView::EObjectFoState ObjectFoView::changeState()
{
	if(m_objectFoState==editState)
	{
		m_objectFoState=dragState;
		m_parentGroup->setHandlesChildEvents (true);
	}
	else if(m_objectFoState==dragState)
	{
		m_objectFoState=editState;
		m_parentGroup->setHandlesChildEvents (false);
	}
	else
		qFatal("Unhandled state of ObjectFoView");
		
	return m_objectFoState;
}

void ObjectFoView::setStateForDrop()
{
	setZValue(PageView::EPageViewPreparedForDrop);
	m_parentGroup->setHandlesChildEvents (true);
	setHandlesChildEvents (true);

}

void ObjectFoView::setStateAfterDrop()
{
	setZValue(PageView::EPageViewPreparedForDrop);
	m_parentGroup->setHandlesChildEvents (false);
	setHandlesChildEvents (false);	
}

void ObjectFoView::slMoveTo(QPoint newPos)
{
	QGraphicsRectItem::setPos(newPos);
}

void ObjectFoView::slSetRectangle ( qreal x, qreal y, qreal width, qreal height,bool setUpObjectHandler)
{
	QGraphicsRectItem::setRect(x,y,width,height);
	
	if (setUpObjectHandler==true)
		m_pMainView->setUpObjectFoHandler(this);

}
