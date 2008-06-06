//own include
#include "resizehandlewindow.h"

//qt include
#include <QCursor>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>

//project includes
#include "objectfodoc.h"
#include "foblockcontainerview.h"
#include "pageview.h"

ResizeHandleWindow::ResizeHandleWindow(ObjectFoView * parent)
 : QObject(),QGraphicsRectItem(parent),m_parent(parent)
{
	QPen penReg(QColor(100,171,0,200));
	setPen(penReg);

	QBrush brush(QColor(149,255,0,200));
	setBrush(brush);
	
	mapPositionToParent(parent);
	
	//we want resizer window allways with the same size	
	QGraphicsRectItem::setZValue(PageView::EResizehandlerWindow);
	
	QGraphicsRectItem::setCursor(Qt::SizeFDiagCursor);
}

ResizeHandleWindow::~ResizeHandleWindow()
{
}

void ResizeHandleWindow::mapPositionToParent(ObjectFoView * parent)
{
	QPointF scale=parent->mainView()->scale();
	QPoint resizer=QPoint(qreal(EResizerWidth)/scale.x(), qreal(EResizerWidth)/scale.y());
	
	QPoint br = parent->rect().bottomRight().toPoint();
	QGraphicsRectItem::setRect(QRect(br - resizer ,br + resizer));

}

void ResizeHandleWindow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{	
	//check minimal distance required for drag
	if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length() < QApplication::startDragDistance())
	{
		return;
	}	

	QByteArray dragAttributes;
	QDataStream stream(&dragAttributes, QIODevice::WriteOnly);
	
	QPoint distPoint=event->pos().toPoint() - m_parent->rect().bottomRight().toPoint();
	
	QRectF parentRect=m_parent->rect();
	
	stream << parentRect << event->scenePos().toPoint() << reinterpret_cast<int> (m_parent);
	
	QDrag *drag = new QDrag(event->widget());
	QMimeData *mimeData = new QMimeData;
	
	mimeData->setData("application/resizeItem", dragAttributes);
	drag->setMimeData(mimeData);
	
	//start drag here, drop event is intented for pageview
	m_parent->parentContainer()->setStateForDrop();
	drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
	m_parent->parentContainer()->setStateAfterDrop();	
}


void ResizeHandleWindow::mousePressEvent( QGraphicsSceneMouseEvent */*event*/)
{
	//must be here
}
