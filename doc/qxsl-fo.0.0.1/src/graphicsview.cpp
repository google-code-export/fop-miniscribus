//own include
#include "graphicsview.h"

//qt includes
#include <QScrollBar>

//project includes


GraphicsView::GraphicsView(QGraphicsScene * scene)
 : QGraphicsView(scene)
{

}

GraphicsView::~GraphicsView()
{
}

void GraphicsView::scale ( qreal sx, qreal sy )
{
	QGraphicsView::scale(sx,sy);
}

int GraphicsView::verticalScrollBarValue(qreal scale)
{
	QRectF sceneRect=scene()->sceneRect();	
	int max=verticalScrollBar()->maximum();
	
	if (max==0)
		return 0;
	
	qreal viewPortHeight=viewport()->height();
	qreal ww=sceneRect.height()*scale-viewPortHeight;
	qreal one=ww/qreal(max);
	int val=verticalScrollBar()->value();
	int retVal=one*val;

	return retVal;
}


int GraphicsView::horizontalScrollBarValue(qreal scale)
{
	QRectF rect=scene()->sceneRect();	
	int max=horizontalScrollBar()->maximum();
	if (max==0) return 0;
	qreal viewporth=viewport()->width();
	qreal ww=rect.width()*scale-viewporth;
	qreal one=ww/qreal(max);
	int val=horizontalScrollBar()->value();
	int retVal=one*val;

	return retVal;
}

///todo: check verticalScrollBarValue and horizontalScrollBarValue algorithm it's looks suspicious