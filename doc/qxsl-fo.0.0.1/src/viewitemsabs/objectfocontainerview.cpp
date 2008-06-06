//own include
#include "objectfocontainerview.h"

//qt includes
#include <QGraphicsSceneDragDropEvent>
#include <QDataStream>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsItem>


//project includes
#include "objectfosequenceddoc.h"
#include "objectfocontainerdoc.h"

#include "foblockview.h"
#include "fotableview.h"
#include "foblockcontainerview.h"
#include "mainview.h"

#include "foblockdoc.h"
#include "foblockcontainerdoc.h"
#include "fotabledoc.h"

#include "focreator.h"

#include "cmdmovefoobject.h"
#include "cmdaddfoobject.h"

ObjectFoContainerView::ObjectFoContainerView(ObjectFoContainerView *parentContainer,ObjectFoContainerDoc *doc,MainView *pMainView,QGraphicsScene *scene)
 : ObjectFoView(parentContainer,doc,pMainView,scene)
{
	connect(doc,SIGNAL(sigReloadChildrenAfterResize()),this,SLOT(slReloadChildrenAfterResize()));
}


ObjectFoContainerView::~ObjectFoContainerView()
{
}

void ObjectFoContainerView::loadChildrenToView(ObjectFoContainerDoc *foContainerDoc)
{
	ObjectFoDoc *foChildDoc=0;
		
	//go throught whole list of child foobjects
	for(unsigned int pos=0;pos<foContainerDoc->countOfFoObjectsChildren();pos++)
	{
		foChildDoc=foContainerDoc->foObjectChildren(pos);
		slAddItemToViewContainer(foChildDoc);
	}

}


void ObjectFoContainerView::slAddItemToViewContainer(ObjectFoDoc *foObjectDoc)
{
	//create desired foblock
	switch(foObjectDoc->type())
	{
		case ObjectFoDoc::typeFoBlock:
		{
			new FoBlockView(this,(FoBlockDoc*)foObjectDoc,m_pMainView);
			break;
		}
		case ObjectFoDoc::typeFoTable:
		{
			new FoTableView(this,(ObjectFoContainerDoc*)foObjectDoc,m_pMainView);
			break;
		}
		case ObjectFoDoc::typeFoBlockContainer:
		{
			/*FoBlockContainerView *foBlockContainer=*/new FoBlockContainerView(this,(ObjectFoContainerDoc*)foObjectDoc,m_pMainView);
			break;
		}
		case ObjectFoDoc::typeNone:
		default:
		{
			qFatal("I cant recognize which type of object you want add to body region container");
			break;
		}
	}
}

void ObjectFoContainerView::removeItemFromViewContainer(ObjectFoDoc */*foObjectViewable*/)
{
	QList <QGraphicsItem*> listOfChildren;
	listOfChildren=QGraphicsItem::children();

	qDebug("not implemented yet");
}

void ObjectFoContainerView::createObjectFoSimpleDoc(int typeOfNewFoObject,QPoint newPos)
{
		//and chose what kind of fo object we want create	
		switch(typeOfNewFoObject)
		{
			case (ObjectFoDoc::typeFoBlock):
			{
				//get focreator
				FoCreator *foCreator=FoCreator::GetInstancePtr();
				//genereate foblockelement
				QDomElement elementBlock=foCreator->foBlockElement();
				//create doc part of block
				FoBlockDoc *foBlockDoc = new FoBlockDoc ( elementBlock,objectFoContainerDoc() );
				//add command to add block to undostack
				m_pMainView->pushToUndoStack ( new CmdAddFoObject ( foBlockDoc,this ) );
				break;
			}
			
			case (ObjectFoDoc::typeFoBlockContainer):
			{
				//get focreator
				FoCreator *foCreator=FoCreator::GetInstancePtr();
				//genereate foblockelement
				QDomElement elementBlock=foCreator->foBlockContainerElement();
				//create doc part of block
				FoBlockContainerDoc *foBlockContainerDoc = new FoBlockContainerDoc ( elementBlock,objectFoContainerDoc(),newPos );
				//add command to add block to undostack
				ObjectFoView::m_pMainView->pushToUndoStack ( new CmdAddFoObject ( foBlockContainerDoc,this ) );
				break;
			}
			
			case (ObjectFoDoc::typeFoTable):
			{
				//get focreator
				FoCreator *foCreator=FoCreator::GetInstancePtr();
				//genereate foblockelement
				QDomElement elementBlock=foCreator->foTableElement();
				//create doc part of block
				FoTableDoc *foTableDoc = new FoTableDoc ( elementBlock,objectFoContainerDoc() );
				//add command to add block to undostack
				ObjectFoView::m_pMainView->pushToUndoStack ( new CmdAddFoObject ( foTableDoc,this ) );
				break;
			}
			
			case (ObjectFoDoc::typeNone):
			default:
				qCritical("Unrecognized mime data,cant decide what kind of foobject you want to create!");
		}
}

void ObjectFoContainerView::dropEvent ( QGraphicsSceneDragDropEvent * event )
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

			QPoint newPoint=event->scenePos().toPoint();
			newPoint-=pos().toPoint();
			m_pMainView->pushToUndoStack( new CmdMoveFoObject ( objectFoDoc->positionValue(),newPoint-offset,objectFoDoc ) );
			return;
		}
	
	}
	else if (event->mimeData()->hasFormat("application/createnewfoobject"))
	{
		if( doc()->type()==FoBlockDoc::typeFoBodyRegion || doc()->type()==FoBlockDoc::typeFoBorderRegion )
		{
			//if mimedata has desired format load data from it
			QByteArray itemData = event->mimeData()->data("application/createnewfoobject");
			QDataStream dataStream(&itemData, QIODevice::ReadOnly);
			dataStream >> /*dragType >> offset >> intFoSimple >>*/ typeOfNewFoObject;
			//create simpledoc
			createObjectFoSimpleDoc(typeOfNewFoObject,event->pos().toPoint());
		}
	}
	else
		ObjectFoView::dropEvent(event);
}

void ObjectFoContainerView::slReloadChildrenAfterResize()
{
	//at first we delete all children
	QList<QGraphicsItem *> list =  parentGraphicsItemGroup()->children();
	
	for(int a=0;a<list.size();a++)
	{
		delete list.at(a);
	}
	
	loadChildrenToView(objectFoContainerDoc());
	
	int width=doc()->widthValue();
	//calculate width according to children
	int height=doc()->height();
	slSetRectangle(0,0,width,height);

}

void ObjectFoContainerView::startDrag(QGraphicsSceneMouseEvent *event)
{
	if ( doc()->type()!=ObjectFoDoc::typeFoBlockContainer || doc()->type()!=ObjectFoDoc::typeFoBodyRegion)
		ObjectFoView::startDrag(event);
		
	QByteArray dragAttributes;
	QDataStream stream(&dragAttributes, QIODevice::WriteOnly);
	
	//push all atrubutes to stream
	stream << ObjectFoView::dragMoveItem << event->pos().toPoint() << reinterpret_cast<int> (doc()) << int(ObjectFoDoc::typeNone);
	
	QDrag *drag = new QDrag(event->widget());
	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-dnditemdata", dragAttributes);
	
	QPointF scale=m_pMainView->scale();
	
	int w=(int)(rect().width()*scale.x());
	int h=(int)(rect().height()*scale.y());
	QPixmap pixMap(w,h);
	pixMap.fill(QColor(219,219,219));
	drag->setPixmap(pixMap);
	drag->setMimeData(mimeData);
	
	QPointF hotSpotPoint=QPointF(event->pos().x()*scale.x(),event->pos().y()*scale.y());
	drag->setHotSpot(hotSpotPoint.toPoint());
	
	ObjectFoView::parentContainer()->setStateForDrop();
	
	hide();
	//start drag for move item
	/*Qt::DropAction res=*/drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
	parentContainer()->setStateAfterDrop();
	show();
}
