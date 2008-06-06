//own include
#include "objectfohandler.h"

//qt include
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QObject>
#include <QPen>
#include <QBrush>

//project includes
#include "objectfoview.h"
#include "mainview.h"
#include "pageview.h"

ObjectFoHandler::ObjectFoHandler(ObjectFoView * parent)
:QObject(),QGraphicsRectItem(parent),m_parent(parent)
{	
	setFlag(QGraphicsItem::ItemIsFocusable);
	setZValue(PageView::EHandlerWindow);
}


ObjectFoHandler::~ObjectFoHandler()
{
}

void ObjectFoHandler::setColorState(ObjectFoView::EObjectFoState state)
{
	if (state==ObjectFoView::editState)
		setUnlockColor();
	else if (state==ObjectFoView::dragState)
		setLockColor();
	else 
		qFatal("Unhandled state of parent object");
}

void ObjectFoHandler::setUnlockColor()
{
	QPen penReg(QColor(0,0,0));
	setPen(penReg);
	QBrush brush(QColor(255,127,127,200));
	setBrush(brush);
}

void ObjectFoHandler::setLockColor()
{
	QPen penReg(QColor(0,0,0));
	setPen(penReg);
	QBrush brush(QColor(127,127,255,200));
	setBrush(brush);	
}

void ObjectFoHandler::mousePressEvent ( QGraphicsSceneMouseEvent */*event*/ )
{
	if(m_parent!=NULL)
	{
		ObjectFoView::EObjectFoState state=m_parent->changeState();
		setColorState(state);
	}
	else
		qFatal("ObjectFoHandler is without parent FoObjectView");
}

void ObjectFoHandler::setParentObject(ObjectFoView * parent)
{
	//save parent for events like mousepress
	m_parent=parent;
	
	//set parent to NULL is needed in some casees, becouse m_parentGroup->setHandlesChildEvents (true);
	//didn't work correctly
	setParentItem(0);
	//set parent fort graphicsitem
	setParentItem(m_parent);

	//set color for rect
	setColorState(m_parent->state());
	
	//calculate size and position for rect
	QPointF scale=m_parent->mainView()->scale();
	QPoint resizer=QPoint(qreal(EHandlerWidth)/scale.x(), qreal(EHandlerWidth)/scale.y());
	QPoint br = parent->rect().topRight().toPoint();
	//set rect
	setRect(QRect(br - resizer ,br + resizer));

}
