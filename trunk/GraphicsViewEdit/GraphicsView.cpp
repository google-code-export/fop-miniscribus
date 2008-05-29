/*******************************************************************************
 * class GraphicsView
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/

#include "GraphicsView.h"

GraphicsView::~GraphicsView()
{
	pageclear();
	layercount = 0;
}


GraphicsView::GraphicsView(  QWidget * parent )
	: QGraphicsView( parent ),width(16.),height(9.),layercount(10)
{
	  if (Metric(setter.value("gview/chess").toString()) > 0) {
			chessgrid = BruschChess(Metric(setter.value("gview/chess").toString()));
		} else {
			chessgrid = BruschChess(Metric("10mm"));
		}
		QRectF bounds((-width / 2.0) * 150, (-height / 2.0) * 150, width * 150, height * 150);
		QRectF Paper;
		if (Metric(setter.value("gview/wi").toString()) > 0) {
		Paper = QRectF(0,0,Metric(setter.value("gview/wi").toString()),Metric(setter.value("gview/hi").toString()));
		} else {
		Paper = QRectF(0,0,Metric("150mm"),Metric("200mm"));
		}
	  scene = new GraphicsScene(Paper,this);
	  setCacheMode(CacheNone);
	  setScene(scene);
	  fillNullItem();
	  connect(scene, SIGNAL(SelectOn(QGraphicsItem*,qreal)), this, SLOT(WorksOn(QGraphicsItem*,qreal)));
	  connect(scene, SIGNAL(nullitem()), this, SLOT(notselect()));
	  AppendDemo();
	  
}

void GraphicsView::setGlobalBrush( QPixmap e )
{
	chessgrid = e;
	update();
}

void GraphicsView::pageclear()
{
		items.clear();
		scene->clear();
	  emit LayerEditor(false,0);
}

void GraphicsView::contextMenuEvent ( QContextMenuEvent * e )
{
	const int idax = CurrentActive->data(ObjectNameEditor).toUInt();
	if (idax > 0 ) {
		return QGraphicsView::contextMenuEvent(e);
	}
	bool layeronram = false;
	QClipboard *ramclip = QApplication::clipboard();
	if (ramclip->mimeData()->hasFormat("application/x-layerrichdoc")) {
		layeronram = true;
	}
	
	QAction *a;
	QMenu *menu = new QMenu(this);
	
	a = menu->addAction(tr("New flow text Layer"), this, SLOT(NewLayer()));
	a->setData(51);
	
	a = menu->addAction(tr("New absolute Layer"), this, SLOT(NewLayer()));
	a->setData(50);
	
	
	a = menu->addAction(tr("Paste Layer from clipboard"), this, SLOT(PasteLayer()));
	a->setIcon(QIcon(":/img/paste.png"));
	a->setEnabled(layeronram);

	a = menu->addAction(tr("Page clear"), this, SLOT(pageclear()));
	a->setIcon(QIcon(":/img/filenew.png"));
	
	menu->exec(QCursor::pos());
	delete menu;
}


void GraphicsView::NewLayer( const int type )
{
	RichDoc Adoc;
	const int gotopX = NextfromY();
	
	if (type == 50) {
		
		    QFile f(QString(":/img/layerabsolute.layer")); 
				if (f.open(QFile::ReadOnly | QFile::Text)) {
        RichDoc addoc = OpenRichDoc(QString::fromUtf8(f.readAll()));
				Adoc.margin(false);
				f.close();
				TextLayer *ioq = new TextLayer(layercount,0,scene);
				ioq->insert(addoc);
				ioq->setModus(TextLayer::Show);
				ioq->setData (ObjectNameEditor,layercount);
	      items.append(ioq);
				ioq->setZValue(scene->zmax()); 
				connect(ioq, SIGNAL(recalcarea() ),this, SLOT(updateauto()));
				connect(ioq, SIGNAL(clonehere() ),this, SLOT(CloneCurrent()));
				connect(ioq, SIGNAL(remid(int) ),this, SLOT(removelayer(int)));
				}
		
	} else {
	      Adoc.margin(true);
	      TextLayer *ioq2 = new TextLayer(layercount,0,scene);
				ioq2->insert(Adoc);
				ioq2->setModus(TextLayer::Show);
				ioq2->setData (ObjectNameEditor,layercount);
	      items.append(ioq2);
	      ioq2->setZValue(0.1); 
	      ioq2->setPos(QPointF(_DEBUGRANGE_WI_,gotopX));
	      connect(ioq2, SIGNAL(recalcarea() ),this, SLOT(updateauto()));
		    connect(ioq2, SIGNAL(clonehere() ),this, SLOT(CloneCurrent()));
		    connect(ioq2, SIGNAL(remid(int) ),this, SLOT(removelayer(int)));
	}
	
	
}



void GraphicsView::NewLayer()
{
	updateauto();
	QAction *invoice = qobject_cast<QAction *>(sender());
	const int type = invoice->data().toInt();
	qDebug() << "### new layer type  " << type;
	layercount++;
	NewLayer(type);
}

void GraphicsView::CloneCurrent()
{
	const int idax = CurrentActive->data(ObjectNameEditor).toUInt();
	
	//////qDebug() << "### idax " << idax;
	
	if (idax < 1) {
	return;
	}
	layercount++;
	      RichDoc rdoc = CurrentActive->ReadActualItem();
				TextLayer *ioq2 = new TextLayer(layercount,0,scene);
				ioq2->insert(rdoc,true);
				ioq2->setModus(TextLayer::Show);
				ioq2->setData (ObjectNameEditor,layercount);
	      items.append(ioq2);
	      connect(ioq2, SIGNAL(recalcarea() ),this, SLOT(updateauto()));
				connect(ioq2, SIGNAL(clonehere() ),this, SLOT(CloneCurrent()));
	      connect(ioq2, SIGNAL(remid(int) ),this, SLOT(removelayer(int)));
}

QMap<int,RichDoc> GraphicsView::read()
{
	   QMap<int,RichDoc> pages;
     for (int e=0;e<items.size();e++) {
			    const int Nid = items[e]->data(ObjectNameEditor).toInt();
		      RichDoc doc = items[e]->ReadActualItem();
			    pages.insert(Nid,doc);
	   }
		 return pages; 
}

void GraphicsView::insert( RichDoc e , bool cloned )
{
	      layercount++;
	      TextLayer *ioq2 = new TextLayer(layercount,0,scene);
				ioq2->insert(e,cloned);
				ioq2->setModus(TextLayer::Show);
				ioq2->setData (ObjectNameEditor,layercount);
	      items.append(ioq2);
	      connect(ioq2, SIGNAL(recalcarea() ),this, SLOT(updateauto()));
				connect(ioq2, SIGNAL(clonehere() ),this, SLOT(CloneCurrent()));
	      connect(ioq2, SIGNAL(remid(int) ),this, SLOT(removelayer(int)));
	      emit LayerEditor(false,0);
	
	
	    
	     QTimer::singleShot(600, this, SLOT(updateauto()));
	
}

void GraphicsView::PasteLayer()
{
	QClipboard *ramclip = QApplication::clipboard();
	if (ramclip->mimeData()->hasFormat("application/x-layerrichdoc")) {
			QByteArray itemData = ramclip->mimeData()->data("application/x-layerrichdoc");
		  QString daten = QString::fromUtf8(itemData.data());
			RichDoc rdoc = OpenRichDoc(daten);
			insert(rdoc);
	} else {
		QMessageBox::information(0, tr("Error Layer"),tr("No Layer on clipboard"));
		
	}
	
	emit LayerEditor(false,0);
}



void GraphicsView::removelayer( const int idx )
{
	const int idax = CurrentActive->data(ObjectNameEditor).toUInt();
	if (idax < 1) {
	return;
	}
	if (idx < 1) {
	return;
	}
	scene->clearSelection();
	   for (int e=0;e<items.size();e++) {
		 items[e]->setSelected(false);
	   }
	
	
	for (int e=0;e<items.size();e++) {
		if (items[e]->data(ObjectNameEditor).toInt() == idx ) {
			    items.removeAt(e);
			    scene->remid(idx);
		}
	}
	
	updateauto();
	emit LayerEditor(false,0);
	fillNullItem();
}





QRectF GraphicsView::boundingRect()
{
	return scene->sceneRect();
	
}



qreal GraphicsView::NextfromY()
{
	qreal fromtop = 0;
	qreal bigYall = 0;
	for (int e=0;e<items.size();e++) {
		////////qDebug() << "### tipo " << items[e]->type();
		if (items[e]->Ltype() != TextLayer::DIV_ABSOLUTE ) {
			fromtop += items[e]->pointnext();
		}
	 /* height total all elemenst absolute */
	 bigYall = qMax(bigYall,items[e]->pos().y() + items[e]->boundingRect().height());
		
	}
    QRectF resc = scene->sceneRect();
	  if (resc.height() < fromtop) {
			scene->setSceneRect(0,0,resc.width(),fromtop + 100);
		}
		if (resc.height() < bigYall) {
			scene->setSceneRect(0,0,resc.width(),bigYall + 100);
		}
	return fromtop;
}


void GraphicsView::notselect()
{
	/* no active layer selected make disable mainwindow command */
	//////qDebug() << "### WorksOn NULL Pointer! ";
  fillNullItem();
}

void GraphicsView::fillNullItem()
{
	 CurrentActive = new TextLayer(0,0,0);
	 CurrentActive->setData (ObjectNameEditor,0);
	 scene->clearSelection();
	 for (int e=0;e<items.size();e++) {
		 items[e]->setSelected(false);
	 }
	 
	 emit LayerEditor(false,0);
	 qApp->postEvent(this,new LayerEvent(0,false));
}


void GraphicsView::GoEditCurrentLayer()
{
    const int idax = CurrentActive->data(ObjectNameEditor).toUInt();
    ////////////qDebug() << "### activlayer  " << idax;
    if (idax < 1) {
    QMessageBox::information(0,tr("Layer status"),tr("Select a layer to begin edit."));
    return;
    }
    CurrentActive->SwapEdit();
}


/*  bridge to edit from external */
void GraphicsView::WorksOn(QGraphicsItem * item , qreal zindex )
{
	/////qDebug() << "### WorksOn " << zindex ;
	bool canEditLayer = false;
	layerNr  = item->data(ObjectNameEditor).toInt();
	//////////qDebug() << "### WorksOn data " << layerNr;  
	if (layerNr > 0) {
			if (CurrentActive = qgraphicsitem_cast<TextLayer *>(item)) {
				 for (int e=0;e<items.size();e++) {
				 items[e]->setSelected(false);
				 }
				 CurrentActive->setSelected(true);
				 canEditLayer = true;
				 qApp->postEvent(this,new LayerEvent(layerNr,canEditLayer));
			}
	}
	
	emit LayerEditor(canEditLayer,layerNr);
	/////////////qDebug() << "### emit bool " << canEditLayer;  
}

void GraphicsView::AppendDemo()
{
        QString inside;
        QFile f(QString(":/img/_default_layer.layer")); 
				if (f.open(QFile::ReadOnly | QFile::Text)) {
				inside = QString::fromUtf8(f.readAll());
				f.close();
        RichDoc addoc = OpenRichDoc(inside); 
				insert(addoc);
				}
				
				RichDoc xx;
				
				insert(xx);
				
}


void GraphicsView::wheelEvent (QWheelEvent * event)
{
	if (setter.value("wheel/zoom").toInt() == 0) {
  scaleView(pow ((double) 2, event->delta() / 240.0));
	return;
	}
	QGraphicsView::wheelEvent(event);
}

void GraphicsView::sceneScaleChanged(const QString &scale)
{
	  if (scale.left(scale.indexOf("%")).toDouble() < 20) {
			fitInView(boundingRect(),Qt::KeepAspectRatioByExpanding);
			DisplayTop();
			return;
		}
	
    double newScale = scale.left(scale.indexOf("%")).toDouble() / 100.0;
    QMatrix oldMatrix = matrix();
    resetMatrix();
    translate(oldMatrix.dx(), oldMatrix.dy());
    QGraphicsView::scale(newScale, newScale);
		DisplayTop();
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
	  painter->setPen( QPen(Qt::darkGray,1) );
    painter->setBrush(QBrush(chessgrid));  
		painter->drawRect(scene->sceneRect());  
		painter->restore();
}
/*
void GraphicsView::drawForeground ( QPainter * painter, const QRectF & rect )  
{
	 if (!viewportLayer.isNull()) {
		    painter->setPen( QPen(Qt::red,6,Qt::DashLine) );
			  painter->setBrush(Qt::NoBrush);
			  painter->drawRect(viewportLayer); 
		}
}
*/







void GraphicsView::updateauto()
{
	qreal fromtop = 0;
	for (int e=0;e<items.size();e++) {
		if (items[e]->Ltype() != TextLayer::DIV_ABSOLUTE ) {
			 items[e]->setPos(QPointF(_DEBUGRANGE_WI_,fromtop));
			 qreal hibecome = items[e]->pointnext();
			 items[e]->update();
			 fromtop += hibecome;
		}
	}
	
	NextfromY();  /* make rect scene big as needed */
	
	TextLayer *layer = qobject_cast<TextLayer *>(sender());
	if (layer) {
		 const QRectF needview = layer->viewport_need();
		/////////////////qDebug() << "### view needview " << needview;
		
		if (needview.isValid() && needview != viewportLayer) {
			  ///////ensureVisible(needview,100,100);
			  viewportLayer = needview;
			  layer->setFocus(Qt::MouseFocusReason);
		}
	 
	}
}

















