//own include
#include "foexternalgraphicview.h"

//qt include
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPixmap>

//project includes
#include "objectfosimpleview.h"
#include "objectfocontainerview.h"
#include "foexternalgraphicdoc.h"
#include "mainview.h"
#include "pageview.h"

FoExternalGraphicView::FoExternalGraphicView(ObjectFoContainerView *parentContainer,FoExternalGraphicDoc *doc,MainView *pMainView)
 : ObjectFoSimpleView(parentContainer,doc,pMainView),m_scale(1)
{	
	m_pPixmapItem =  new QGraphicsPixmapItem(parentGraphicsItemGroup());
	
	m_pPixmapItem->setZValue ( PageView::ESimpleFoObjectViewZPosition);
	
	setFlag(QGraphicsItem::ItemIsFocusable);	
	setFlag(QGraphicsItem::ItemIsSelectable);

	loadPixmapToView(doc);

	setRect(0,0,m_pPixmapItem->boundingRect().width()*m_scale,m_pPixmapItem->boundingRect().height()*m_scale);
	
}

void FoExternalGraphicView::loadPixmapToView(FoExternalGraphicDoc *doc,bool alternatePicture)
{
	const QString pixPath = doc->picturePath();
	////////////if (alternatePicture==false)
		////////pixPath=doc->picturePath();
	////////else
		///////pixPath=QString(":/images/empty_picture.png");

	QPixmap pixmap(pixPath);
	
	////////if (pixmap.isNull())
	//////////{
		//////////if(alternatePicture==false)
		/////////{
			 //////////qWarning(qPrintable("Error occurs while loading pixmap, used alternate picture from resources path " + pixPath));
			///////////loadPixmapToView(doc,true);
		////////}
		///////////else
			//////////qWarning(qPrintable("Error occurs while loading pixmap" + pixPath));
		
		/////////return;
	///////////}
	
	if (!pixmap.isNull()) {
	m_pPixmapItem->setPixmap(pixmap);
	} else {
	qWarning(qPrintable("Error occurs while loading pixmap path/url " + pixPath));
	m_pPixmapItem->setPixmap(QPixmap(QString(":/images/empty_picture.png")));
	}
	
	if ( m_pPixmapItem->pixmap().isNull() )
	{
		qDebug("QGraphicsPixmapItem can't set pixmap");
	}
	
	QRectF pixmapRect=m_pPixmapItem->boundingRect();
	int contWidth=doc->contentWidth();
	m_scale=(qreal)contWidth/pixmapRect.width();
	m_pPixmapItem->scale(m_scale,m_scale);
}

int FoExternalGraphicView::pixmapWidth()
{
	int width=m_pPixmapItem->boundingRect().width()*m_scale;
	return width;
}

void FoExternalGraphicView::setPos(QPoint pos)
{
	ObjectFoSimpleView::setPos(pos);
}

void FoExternalGraphicView::assignGroup(QGraphicsItemGroup *groupItem)
{
	groupItem->addToGroup(this);
}

FoExternalGraphicView::~FoExternalGraphicView()
{
	
}
