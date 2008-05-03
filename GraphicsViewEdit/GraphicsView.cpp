#include "GraphicsView.h"

GraphicsView::GraphicsView(  QWidget * parent )
	: QGraphicsView( parent ),width(16.),height(9.)
{
	  
	
	  chessgrid = BruschChess(Metric("25mm"));
	  QRectF bounds((-width / 2.0) * 150, (-height / 2.0) * 150, width * 150, height * 150);
		QRectF PaperA4(0,0,Metric("211mm"),Metric("297mm"));
	
	  scene = new GraphicsScene(PaperA4,this);
	  setCacheMode(CacheBackground);
	  setScene(scene);
	  TextLayer *ioq = new TextLayer(1,0,scene);
    ioq->setPos(QPointF(0,0));
	  fitInView(ioq, Qt::KeepAspectRatio);
	
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
	fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
	  painter->save();
	  painter->setPen( QPen(Qt::darkGray,Metric("1mm")) );
    painter->setBrush(QBrush(chessgrid));  
		painter->drawRect(scene->sceneRect());  
	  painter->restore();
}



