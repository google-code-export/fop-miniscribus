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
	  #ifndef QT_NO_OPENGL
    /////////setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    #endif
	
	  setRenderHint(QPainter::Antialiasing,true);
	  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	
	  if (Metric(setter.value("gview/chess").toString()) > 0) {
			chessgrid = BruschChess(Metric(setter.value("gview/chess").toString()));
		} else {
			chessgrid = BruschChess(Metric("10mm"));
		}
		
		openGlaction = new QAction(tr("Render OpenGL"),this);
    /////actionLink->setIcon(iconl);
    openGlaction->setCheckable(true);
		openGlaction->setChecked( false );
	  connect(openGlaction, SIGNAL(triggered()),this,SLOT(toggleOpenGL()));
		
		
		
		
		const QString backsS = setter.value("gview/backpix").toString();
		if (backsS.size() > 0) {
			 QList<SPics> li = OpenImageGroup(backsS);
			 if (li.size() > 0) {
					SPics bb = li[0];
					chessgrid = bb.pix();
			 }
		}
		///////QRectF bounds((-width / 2.0) * 150, (-height / 2.0) * 150, width * 150, height * 150);
	  scene = new GraphicsScene(rectToScene(),this);
	  setCacheMode(CacheNone);
	  setScene(scene);
	  fillNullItem();
	  connect(scene, SIGNAL(SelectOn(QGraphicsItem*,qreal)), this, SLOT(WorksOn(QGraphicsItem*,qreal)));
	  connect(scene, SIGNAL(nullitem()), this, SLOT(notselect()));
		pageclear();
		AppendDemo();
	  
}

void GraphicsView::pageclear()
{
		///////items.clear();
		scene->clear();
	  scene->setSceneRect(rectToScene());
	  MarginController = new GMarginScene(1,1,scene);
	  MarginController->setPos(0,0);
	  MarginController->setData(ObjectNameController,1);
	  InitTopPositionAfterBorderPlay = InitTopPosition + MarginController->boundingRect().height();
	  connect(MarginController, SIGNAL(CursorMove(qreal,qreal)),this,SLOT(CursorMargins(qreal,qreal)));
	  emit MenuActivates(false,qMakePair(0,0));
}

void GraphicsView::CursorMargins( qreal left ,qreal right )
{
		if ( Lmm == (int)left && Rmm == (int)right) {
		return;
		}
	
	const QRect viewRe = viewport()->rect(); 
	qDebug() << "### viewRe  " << viewRe;
	
		QList<QGraphicsItem *> listing = scene->l_items();
		for (int e=0;e<listing.size();e++) {
			TextLayer *it = qgraphicsitem_cast<TextLayer *>(listing[e]);
			if (it) {
				         const int idax = it->data(ObjectNameEditor).toUInt();
				
				         if (it->Ltype() != TextLayer::DIV_ABSOLUTE ) {
									 
						     QTextFrame  *Tframe = it->document()->rootFrame();
								 QTextFrameFormat rootformats = Tframe->frameFormat();
								 rootformats.setLeftMargin(left - 1);
								 rootformats.setRightMargin(scene->sceneRect().width() - right + 1);
								 Tframe->setFrameFormat(rootformats);
									if ( viewRe.contains(it->pos().toPoint()) ) {
								    it->update();
									} else {
										qDebug() << "### no update item!! " << idax;
									}
								 }
			}
		}
		scene->update(viewRe);
		Lmm = left;
		Rmm = right;
}


void GraphicsView::PrintDoc()
{
	#ifndef QT_NO_PRINTER
	//////const QRectF area = boundingRect();
	PrintSetup(true);
	PreviewDialog *PrintScene = new PreviewDialog(scene);
	///////////connect(PrintScene,SIGNAL(pageRequested(int,int,QPainter &,QPrinter)),scene, SLOT(printPage(int,int, QPainter &, QPrinter)),Qt::DirectConnection);
	
	PrintScene->exec();
	PrintSetup(false);
	PrintScene->deleteLater();
	#endif
	///////////qDebug() << "### PrintDoc end......... ";
}

void GraphicsView::PrintSetup( bool enable )
{
	MarginController->setVisible(!enable);
	
          QList<QGraphicsItem *> listing = scene->l_items();
				  for (int e=0;e<listing.size();e++) {
					TextLayer *it = qgraphicsitem_cast<TextLayer *>(listing[e]);
						if (it) {
							it->SetPrintModus(enable);
						}
				  }
	update();
}


QRectF GraphicsView::rectToScene()
{
	  QRectF Paper;
	  ///// paperSizeCombo->addItem(tr("A4 (210 x 297 mm, 8.26 x 11.7 inches)"), QPrinter::A4);
	
		if (Metric(setter.value("gviewooooooooo/wi").toString()) > 0) {
		Paper = QRectF(0,0,Metric(setter.value("gview/wi").toString()),Metric(setter.value("gview/hi").toString()));
		} else {
		Paper = QRectF(0,0,Metric("210mm"),Metric("297mm"));
		}
		return Paper;
}

void GraphicsView::setGlobalBrush( QPixmap e )
{
	NextfromY();
	chessgrid = e;
	update();
	scene->update();
	update();
	
	SPics bb;
	      bb.set_pics(e);
	QList<SPics> li;
	             li.append(bb);
	const QString image = SaveImageGroup(li);
	setter.setValue("gview/backpix",image);
}


void GraphicsView::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    setViewport(openGlaction->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
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
	menu->addSeparator();
	menu->addAction(openGlaction);
	a = menu->addAction(tr("Paste Layer from clipboard"), this, SLOT(PasteLayer()));
	a->setIcon(QIcon(":/img/paste.png"));
	a->setEnabled(layeronram);
  menu->addSeparator();
	a = menu->addAction(tr("Page clear"), this, SLOT(pageclear()));
	a->setIcon(QIcon(":/img/filenew.png"));
	
	a = menu->addAction(tr("Page print"), this, SLOT(PrintDoc()));
	a->setIcon(QIcon(":/img/fileprint.png"));
	
	a = menu->addAction(tr("Save as Layer group Page"), this, SLOT(SaveAsPage()));
	a->setIcon(QIcon(":/img/wp.png"));
	
	a = menu->addAction(tr("Open Layer group Page"), this, SLOT(OpenFilePageGroup()));
	a->setIcon(QIcon(":/img/wp.png"));
	
		
	
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
	      ///////items.append(ioq);
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
	      //////items.append(ioq2);
	      ioq2->setZValue(0.1); 
	      ioq2->setPos(QPointF(_DEBUGRANGE_WI_,gotopX));
	      connect(ioq2, SIGNAL(recalcarea() ),this, SLOT(updateauto()));
		    connect(ioq2, SIGNAL(clonehere() ),this, SLOT(CloneCurrent()));
		    connect(ioq2, SIGNAL(remid(int) ),this, SLOT(removelayer(int)));
	}
	
	QApplication::processEvents();
}



void GraphicsView::NewLayer()
{
	updateauto();
	QAction *invoice = qobject_cast<QAction *>(sender());
	const int type = invoice->data().toInt();
	/////////qDebug() << "### new layer type  " << type;
	layercount++;
	NewLayer(type);
}

void GraphicsView::CloneCurrent()
{
	const int idax = CurrentActive->data(ObjectNameEditor).toUInt();
	if (idax < 1) {
	return;
	}
	layercount++;
	      RichDoc rdoc = CurrentActive->ReadActualItem();
				insert(rdoc,true);
}

QMap<int,RichDoc> GraphicsView::read()
{
	   QMap<int,RichDoc> pages;
	   RecordItem();  /* order ObyektSortingTimeline  first item at y */
	
        QList<QGraphicsItem *> listing = scene->l_items();
				  for (int e=0;e<listing.size();e++) {
					TextLayer *it = qgraphicsitem_cast<TextLayer *>(listing[e]);
						if (it) {
								const int Nid = it->data(ObyektSortingTimeline).toInt(); 
							  /* order by y top !!!!!  */
		            RichDoc doc = it->ReadActualItem();
							  if (Nid > 8) {
                pages.insert(Nid,doc);
								}
						}
				  }
		 return pages; 
}

void GraphicsView::insert( RichDoc e , bool cloned )
{
	      layercount++;
	      TextLayer *ioq2 = new TextLayer(layercount,0,scene);
				ioq2->insert(e,cloned);
				ioq2->setModus(TextLayer::Show);
				ioq2->setData (ObjectNameEditor,layercount+10);
	
	      if (!e.isAbsolute()) {
	      ioq2->setPos(_DEBUGRANGE_WI_,NextfromY()); 
				ioq2->setZValue(0.1); 
				}
				
	      connect(ioq2, SIGNAL(recalcarea() ),this, SLOT(updateauto()));
				connect(ioq2, SIGNAL(clonehere() ),this, SLOT(CloneCurrent()));
	      connect(ioq2, SIGNAL(remid(int) ),this, SLOT(removelayer(int)));
	      emit MenuActivates(false,qMakePair(0,0));
	      QTimer::singleShot(0, this, SLOT(updateauto()));
	
	
}

void GraphicsView::PasteLayer()
{
	QClipboard *ramclip = QApplication::clipboard();
	if (ramclip->mimeData()->hasFormat("application/x-layerrichdoc")) {
			QByteArray itemData = ramclip->mimeData()->data("application/x-layerrichdoc");
		  QString daten = QString::fromUtf8(itemData.data());
			RichDoc rdoc = OpenRichDoc(daten);
		  layercount++;
			insert(rdoc);
	} else {
		QMessageBox::information(0, tr("Error Layer"),tr("No Layer on clipboard"));
		
	}
	
	emit MenuActivates(false,qMakePair(0,0));
}



void GraphicsView::removelayer( const int idx )
{
	if (idx < 1) {
	return;
	}
	//////qDebug() << "### removelayer " << idx;
	scene->clearSelection();
  scene->remid(idx);
	updateauto();
	emit MenuActivates(false,qMakePair(0,0));
	fillNullItem();
}





QRectF GraphicsView::boundingRect()
{
	return scene->sceneRect();
	
}



qreal GraphicsView::NextfromY()
{
	updateauto();
	qreal fromtop = SpaceAutoFloatHight;
	qreal bigYall = InitTopPositionAfterBorderPlay;
	
	QList<QGraphicsItem *> listing = scene->l_items();
	for (int e=0;e<listing.size();e++) {
		////////qDebug() << "### tipo " << items[e]->type();
		TextLayer *it = qgraphicsitem_cast<TextLayer *>(listing[e]);
		if (it) {
					bigYall = qMax(bigYall,it->pos().y() + it->boundingRect().height());
		}
	}
	 /* height total all elemenst absolute */
    QRectF resc = scene->sceneRect();
	  if (resc.height() < fromtop) {
			scene->setSceneRect(0,0,resc.width(),fromtop + RectoSceneBottomFooterSpace);
		}
		if (resc.height() < bigYall) {
			scene->setSceneRect(0,0,resc.width(),bigYall + RectoSceneBottomFooterSpace);
		}
		
		if (fromtop < 4) {
			return InitTopPositionAfterBorderPlay;
		} else {
			return fromtop;
		}
}


void GraphicsView::notselect()
{
	/* no active layer selected make disable mainwindow command */
	//////qDebug() << "### WorksOn NULL Pointer! ";
  fillNullItem();
}

void GraphicsView::fillNullItem()
{
	 setProperty("_layer_work_",0);   ////////  
	 CurrentActive = new TextLayer(0,0,0);
	 CurrentActive->setData (ObjectNameEditor,0);
	 scene->clearSelection();
	 QApplication::processEvents();
	 emit MenuActivates(false,qMakePair(0,0));
	 QApplication::processEvents();
	 updateauto();
	 //////qApp->postEvent(this,new LayerEvent(0,false));
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
		QApplication::processEvents();
}


/*  bridge to edit from external */
void GraphicsView::WorksOn(QGraphicsItem * item , qreal zindex )
{
	
	bool canEditLayer = false;
	layerNr  = item->data(ObjectNameEditor).toInt();
	//////////qDebug() << "### WorksOn data " << layerNr;  
	if (layerNr > 0) {
			if (CurrentActive = qgraphicsitem_cast<TextLayer *>(item)) {
				
					QList<QGraphicsItem *> listing = scene->l_items();
				  for (int e=0;e<listing.size();e++) {
					TextLayer *it = qgraphicsitem_cast<TextLayer *>(listing[e]);
						if (it) {
								it->setSelected(false);
						}
				  }
	    }
				 
				 
				 
				 
				 CurrentActive->setSelected(true);
				 canEditLayer = true;
				 //////////////qDebug() << "### layerNr " << layerNr ;
			
	
					setProperty("_layer_work_",layerNr);
					QApplication::processEvents();
					emit MenuActivates(canEditLayer,qMakePair(layerNr,CurrentActive->textCursor().position()));
					QApplication::processEvents();
			
			}
			
	/////////////qDebug() << "### emit bool " << canEditLayer;  
}

void GraphicsView::AppendDemo()
{
				LaunchFile(QString(":/img/master.page"));
}


void GraphicsView::LaunchFile( const QString islayeror )
{
	
	        ////////////qDebug() << "### LaunchFile " << islayeror;
				QFileInfo fix(islayeror);
	      QFile f(islayeror); 
	      QString inside;
  
				if (f.open(QFile::ReadOnly | QFile::Text)) {
            if (fix.completeSuffix().toLower() == "layer") {
            inside = QString::fromUtf8(f.readAll());
            RichDoc addoc = OpenRichDoc(inside); 
            insert(addoc);
            } else if (fix.completeSuffix().toLower() == "page") {
              PageDoc e;
              e.open(f.readAll());
              OpenPage(e); 
            }
            layercount++;
            f.close();
				}  
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
	  //////////painter->save();
	  painter->setPen( QPen(Qt::darkGray,1) );
    painter->setBrush(QBrush(chessgrid));  
		painter->drawRect(scene->sceneRect());  
		/////////painter->restore();
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


void GraphicsView::RecordItem()
{
	  ///// QMap<int,TextLayer*> auto_li;
	  
	  /////SpaceAutoFloatHight = InitTopPositionAfterBorderPlay;
	  auto_li.clear();
	  int ordersorting = 10;
	  int orderabsolutesorting = 800;
	  QList<QGraphicsItem *> listing = scene->l_items();
	  for (int e=0;e<listing.size();e++) {
			TextLayer *it = qgraphicsitem_cast<TextLayer *>(listing[e]);
			if (it) {
				const int idonbegin = it->data(ObjectNameEditor).toInt();

				
						if (it->Ltype() != TextLayer::DIV_ABSOLUTE ) {
							    ///////
							    const int fromtop = it->pos().y();
							    qreal hibecome = it->pointnext();
							    const qreal Linerecthi = hibecome + fromtop;
							    if (!auto_li[fromtop]) {
							    auto_li.insert(fromtop,it);
									} else {
									auto_li.insert(fromtop + 1,it);
									}
									/////////
						}  else {
							  if (idonbegin > 0) {
							  orderabsolutesorting++;
							  it->setData(ObyektSortingTimeline,orderabsolutesorting);
								}
						}
			}
		}
		
			QMapIterator<int,TextLayer*> o(auto_li);
					while (o.hasNext()) {
                 o.next();
						     ordersorting++;
				         TextLayer *ali = o.value();
						     ali->setData(ObyektSortingTimeline,ordersorting);
					}
		
		

}




void GraphicsView::updateauto()
{
	RecordItem();
	qreal ynext = InitTopPositionAfterBorderPlay;
	SpaceAutoFloatHight = InitTopPositionAfterBorderPlay;
	QMapIterator<int,TextLayer*> o(auto_li);
					while (o.hasNext()) {
                 o.next();
				         TextLayer *ali = o.value();
						     ali->setPos(_DEBUGRANGE_WI_,ynext); 
						     ynext = o.key() + ali->pointnext() + InterSpacingFromAutoFloatLayerElements;
						     ////////qDebug() << "### updateauto " << ynext <<  " ... " << ali->data(ObjectNameEditor).toInt();
						     SpaceAutoFloatHight = ynext;
					}
					
					
	TextLayer *layer = qobject_cast<TextLayer *>(sender());
	if (layer) {
		const QRectF needview = layer->viewport_need();
		if (needview.isValid() && needview != viewportLayer) {
			  viewportLayer = needview;
			  layer->setFocus(Qt::MouseFocusReason);
		}
		
	 }
	 
	const qreal HIII = qBound (rectToScene().height(),ynext + RectoSceneBottomFooterSpace,rectToScene().height() * 10);
	scene->setSceneRect(0,0,scene->sceneRect().width(),HIII);
	AreaHiTower = scene->sceneRect().height();
}


void GraphicsView::onOtherInstanceMessage( const QString msg )
{
	QApplication::setActiveWindow(this);
	
	//////////qDebug() << "### onOtherInstanceMessage " << msg;
	
	
	QFileInfo fix(msg);
	          if (fix.exists()) {
							  LaunchFile( fix.absoluteFilePath() );
						 return;
						}
	QMessageBox::warning(this, tr("Application warning..."),msg);
}



 void GraphicsView::closeEvent(QCloseEvent *event)
 {
	  scene->deleteLater();
		event->accept();

 }
 
 PageDoc GraphicsView::getPage()
 {
	 
	 NextfromY();
	 NextfromY();
	 
	 
				PageDoc lpage;
			  lpage.Listening = read();
			  lpage.rect = boundingRect();
	      return lpage;
 }
 
void GraphicsView::OpenPage( PageDoc e )
{
	 pageclear();
	 if (e.Listening.size() > 0) {
		scene->setSceneRect(e.rect);
	  QMapIterator<int,RichDoc> o(e.Listening);
					while (o.hasNext()) {
                 o.next();
				         RichDoc record = o.value();
						     insert(record);
					}
	 }
}
 
void GraphicsView::SaveAsPage()
{
    QString file = QFileDialog::getSaveFileName(0, tr("Save as Page Layer group File"),QString(setter.value("LastDir").toString()),tr("Pagelayer File (*.page)"));
    if (file.size() > 0) {
        setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
				 PageDoc lpage = getPage();
			   QFile f(file);
					if (f.open(QFile::WriteOnly)) {
						  f.write(lpage.save());
						  f.close();
					}
    }
}
 
 
void GraphicsView::OpenFilePageGroup()
{
    QString file = QFileDialog::getOpenFileName(0, tr( "Choose Page Layer group File..." ), QString(setter.value("LastDir").toString()) ,tr("Pagelayer File (*.page)"));
    if (file.size() > 0) {
        setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
        QFile f(file); 
                if (f.open(QFile::ReadOnly)) {
									  PageDoc e;
									  e.open(f.readAll());
									  OpenPage(e); 
                    f.close();
                }
             
    }
}
 

