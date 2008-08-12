#include <QGraphicsScene>
#include "GraphicsScene.h"









GraphicsScene::GraphicsScene( QObject * parent )
	: QGraphicsScene( parent ),bridge(0)
{
	 QApplication::restoreOverrideCursor();
	 QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}
GraphicsScene::GraphicsScene(  const QRectF & sceneRect, QObject * parent )
	: QGraphicsScene( sceneRect,  parent ),bridge(0)
{
	setSceneRect( sceneRect );
    QApplication::restoreOverrideCursor();
	 QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    
    //////QPalette p = palette();
    //////////p.setColor(QPalette::Text,_DOCUMENT_TXT_COLOR_); 
	  //////////setPalette(p);
}
GraphicsScene::GraphicsScene( qreal x, qreal y, qreal width, qreal height, QObject * parent )
	: QGraphicsScene(x,y,width,height,parent ),bridge(0)
{
	setSceneRect(x,y,width,height);
      /////////QPalette p = palette();
     ////////// p.setColor(QPalette::Text,_DOCUMENT_TXT_COLOR_); 
	    ///////////setPalette(p);
    QApplication::restoreOverrideCursor();
	 QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    
}


void GraphicsScene::paste( bool cloneornot )
{
    emit LayerPaste(cloneornot);
}




void GraphicsScene::reload()
{
     update();
}

void GraphicsScene::storno()
{
    bridge = 0;
    emit SelectOn(bridge);
}

void GraphicsScene::clearSelection()
{
    QGraphicsScene::clearSelection();
    
}

void GraphicsScene::clear()
{
    clearSelection();
    const QList<QGraphicsItem*> items =  QGraphicsScene::items();
    QList<QGraphicsItem*> topLevelItems;
    
    foreach(QGraphicsItem* item, items) {
        if (!item->parentItem()) {
            topLevelItems << item;
        }
    }
    foreach(QGraphicsItem* item, topLevelItems) {
        delete item;
    }
    storno();
}

qreal GraphicsScene::zmax()
{
 qreal mValue = 0.00;
 QList<QGraphicsItem *> items = QGraphicsScene::items();
     for (int o=0;o<items.size();o++) {
       mValue = qMax(items[o]->zValue(),mValue);
    }
 return mValue;
}

qreal GraphicsScene::zmin()
{
 qreal mValue = zmax();
 QList<QGraphicsItem *> items = QGraphicsScene::items();
     for (int o=0;o<items.size();o++) {
       mValue = qMin(items[o]->zValue(),mValue);
    }
 return mValue;
}

/* filter only item a top Zindex /  zValue */
bool GraphicsScene::WakeUp( const QPointF incomming ) 
{
       QList<QGraphicsItem *>  listing = QGraphicsScene::items(incomming);
       qreal thebest = 0;  /* zindex on front */
       for (int o=0;o<listing.size();o++) {
           listing[o]->setSelected(false); 
           listing[o]->setFlag(QGraphicsItem::ItemIsSelectable,false);
           thebest = qMax(listing[o]->zValue(),thebest);
       }
       ///////qDebug() << "### thebest->" << thebest;
       /* activate item at top z-index zValue  /  not the down not visible!!!! */
       for (int e=0;e<listing.size();e++) {
           if (listing[e]->zValue() == thebest) {
             listing[e]->setFlag(QGraphicsItem::ItemIsSelectable,true);
             listing[e]->setSelected(true); 
             bridge = listing[e];
             emit SelectOn(listing[e]);
             return true;
           }
       }
       emit SelectOn(bridge);
return false;
}

void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    } else {
    storno();
    }
    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    } else {
    storno();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
void GraphicsScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    } else {
    storno();
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}


void GraphicsScene::setSceneRect( qreal x, qreal y, qreal w, qreal h ) 
{
    ///////Area = QRectF(x,y,w,h);
    QGraphicsScene::setSceneRect(x,y,w,h);
}

void GraphicsScene::setSceneRect(  const QRectF & rect ) 
{
    ///////Area = rect;
    QGraphicsScene::setSceneRect(rect);
}

int GraphicsScene::PrintPageSumm( QPrinter * printer )
{
    bool portrait = printer->orientation() == QPrinter::Portrait ? true : false;
    const qreal wit = qMin(printer->pageRect().width(),printer->paperRect().width());
	  const qreal hei = qMin(printer->pageRect().height(),printer->paperRect().height());
    const qreal faktor_print = qMax(wit,hei) / qMin(wit,hei);
    qreal onepagescene_HI = sceneRect().width() * faktor_print;
    if (!portrait) {
    onepagescene_HI = sceneRect().width() / faktor_print;
    } 
    QRectF rectScenePiece = QRectF (0.,0.,sceneRect().width(),onepagescene_HI); /* first slice from scene */
    const qreal page = sceneRect().height() / onepagescene_HI;   /* page need */
    
    qDebug() << "### page math  " << page;
    
    
      int PageSumm = page;
	    if (page > PageSumm) {
			 PageSumm++;   /* float to next int */
			}
    qDebug() << "### page int " << PageSumm;
    return PageSumm;
}


void GraphicsScene::printPage(int index, int percentual , QPainter &painter, QPrinter * printer)
{
  
    QPixmap pixmap(painter.viewport().width(),painter.viewport().height());   /* virtual paper */
    bool portrait = printer->orientation() == QPrinter::Portrait ? true : false;
    
    const qreal wit = qMin(printer->pageRect().width(),printer->paperRect().width());
	  const qreal hei = qMin(printer->pageRect().height(),printer->paperRect().height());
    const qreal faktor_print = qMax(wit,hei) / qMin(wit,hei);
    
    const QRect Paper_Rect = printer->pageRect();   
    qreal onepagescene_HI = sceneRect().width() * faktor_print;
    if (!portrait) {
    onepagescene_HI = sceneRect().width() / faktor_print;
    } 
    
    QRectF rectScenePiece = QRectF (0.,0.,sceneRect().width(),onepagescene_HI); /* first slice from scene */
    
    const qreal page = sceneRect().height() / onepagescene_HI;   /* page need */
	  int PageSumm = page;
    
	    if (page > PageSumm) {
			 PageSumm++;   /* float to next int */
			}
      
    if ( index > PageSumm ) {
    return;   /* not having this page */
    }
    
    qreal InitOnYtop = 0;
    
    if (index != 0) {
          InitOnYtop = rectScenePiece.height() * index;
    }
    QRect pagepiece = QRect(0,InitOnYtop,rectScenePiece.width(),rectScenePiece.height());
    //////////qDebug() << "### page  " << index << "," << percentual << "," << printer->pageRect();
    const qreal smallpart = qMax(pixmap.width(),pixmap.height()) / faktor_print;
    
    QRectF AFormatPaper;
    /* Paper dimension from printer Faktor run */
    if (portrait) {
    AFormatPaper = QRect(0,0,smallpart,qMax(pixmap.width(),pixmap.height()));
    } else {
    AFormatPaper = QRect(0,0,qMax(pixmap.width(),pixmap.height()),smallpart);    
    }
    
    
    QRectF ZoomFaktorPage = Reduce(AFormatPaper,percentual);   /* zoom rect */
    QRect WhitePaper = CenterRectSlaveFromMaster(pixmap.rect(),AFormatPaper).toRect(); 
    QRect RenderPage = CenterRectSlaveFromMaster(pixmap.rect(),ZoomFaktorPage).toRect(); 
    
    painter.fillRect(painter.viewport(),QBrush(Qt::lightGray));  /* device to cover */
    painter.fillRect(WhitePaper,QBrush(Qt::white));   /* paper */
    /////////painter.fillRect(RenderPage,QBrush(Qt::red));   /* page result */
    render(&painter,RenderPage,pagepiece,Qt::KeepAspectRatio);
}























