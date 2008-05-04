#include "GraphicsView.h"

GraphicsView::GraphicsView(  QWidget * parent )
	: QGraphicsView( parent ),width(16.),height(9.),layercount(10)
{
	  chessgrid = BruschChess(Metric("25mm"));
	  QRectF bounds((-width / 2.0) * 150, (-height / 2.0) * 150, width * 150, height * 150);
		QRectF PaperA4(0,0,Metric("200mm"),Metric("200mm"));
	  scene = new GraphicsScene(PaperA4,this);
	  setCacheMode(CacheBackground);
	  setScene(scene);
	  fillNullItem();
	  connect(scene, SIGNAL(SelectOn(QGraphicsItem*,qreal)), this, SLOT(WorksOn(QGraphicsItem*,qreal)));
	  connect(scene, SIGNAL(nullitem()), this, SLOT(notselect()));
	  AppendDemo();
	  
}

void GraphicsView::fillNullItem()
{
	 CurrentActive = new TextLayer(0,0,0);
	 CurrentActive->setData (ObjectNameEditor,0);
}

/*  bridge to edit from external */
void GraphicsView::WorksOn(QGraphicsItem * item , qreal zindex )
{
	qDebug() << "### WorksOn " << zindex ;
	qDebug() << "### data " << item->data(ObjectNameEditor).toInt();  
	if (item->data(ObjectNameEditor).toUInt() > 0) {
	return;
	}
	if (CurrentActive = qgraphicsitem_cast<TextLayer *>(item)) {
		 scene->clearSelection();
		 CurrentActive->setSelected(true);
	} else {
		fillNullItem();
	}
}

void GraphicsView::AppendDemo()
{
	      layercount++;
        QString inside;
        QFile f(QString(":/img/_default_layer.layer")); 
				if (f.open(QFile::ReadOnly | QFile::Text)) {
				inside = QString::fromUtf8(f.readAll());
				f.close();
        RichDoc addoc = OpenRichDoc(inside); 
				TextLayer *ioq = new TextLayer(layercount,0,scene);
				ioq->insert(addoc);
				ioq->setModus(TextLayer::Show);
				ioq->setData (ObjectNameEditor,layercount);
	      //////////////fitInView(ioq, Qt::KeepAspectRatio);
				}

}


void GraphicsView::wheelEvent (QWheelEvent * event)
{
	/////if (setter.value("wheel/zoom").toInt() == 1) {
  scaleView(pow ((double) 2, event->delta() / 240.0));
	/////return;
	/////////}
	//////////QGraphicsView::wheelEvent(event);
}

/* scale the scene view */
void GraphicsView::scaleView (qreal scaleFactor)
{
	qreal factor = matrix().scale (scaleFactor,scaleFactor).mapRect (QRectF (0, 0, 1, 1)).width();
	if (factor < 0.15 || factor > 8)
	{
		return;
	}
	QGraphicsView::scale(scaleFactor, scaleFactor);
	DisplayTop();
}

void GraphicsView::DisplayTop()
{
	///////////////qDebug() << "### DisplayTop #############################";
	horizontalScrollBar()->setValue(0); 
	verticalScrollBar()->setValue(0);
	QGraphicsView::horizontalScrollBar()->setValue(0); 
	QGraphicsView::verticalScrollBar()->setValue(0);
	////if (Xhtml.size()  > 0) {
		//////////emit NewSource(Xhtml);
	////////}
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
	///////////////fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
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



