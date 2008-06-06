//own include
#include "mainview.h"

//qt includes
#include <QtGui>

//project includes

#include "pageviewmanager.h"
#include "mainwindow.h"

#include "maindoc.h"
#include "pagedoc.h"
#include "objectfosimpledoc.h"
#include "objectfodoc.h"

#include "graphicsview.h"
#include "pageview.h"
#include "objectfosimpleview.h"
#include "objectfoview.h"
#include "objectfosequencedview.h"
#include "objectfocontainersimpleview.h"
//#include "fotableview.h"
#include "fotablecellview.h"
#include "fotablecelldoc.h"
#include "objectfocontainersimpledoc.h"
#include "objectfohandler.h"

MainView::MainView(MainWindow *pMainWindow,MainDoc *pMainDoc)
:QGraphicsScene(pMainWindow),m_pMainWindow(pMainWindow),m_pMainDoc(pMainDoc),m_pSelectedItemView(NULL),m_scale(1,1)
{
	
	m_pGraphicsView = new GraphicsView(this);
	scale(2.85,2.85);
	m_pGraphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	m_pGraphicsView->setAccessibleName ( "widgetView" );
	
	QGraphicsScene::setBackgroundBrush(Qt::gray);
	m_pPageViewMng = new PageViewManager();
	//little rect on top right
	m_pObjectFoHandler = new ObjectFoHandler();
	m_pObjectFoHandler->hide();
}

MainView::~MainView()
{
	
}

void MainView::createPage(PageDoc *pPageDoc)
{
	//create page view 
	PageView *pNewPageView=new PageView(pPageDoc,this,this);
	//add to pageview manager
	m_pPageViewMng->AddPageView( pNewPageView );
	//update item hierarchy dockwidget
	m_pMainWindow->updateItemHierarchyView(pPageDoc);
}

void MainView::foObjectSimpleSelected(ObjectFoSimpleView *pObjectFoSimpleView)
{
	m_pSelectedItemView=pObjectFoSimpleView;
	
	m_pMainWindow->setRulersForPage(pObjectFoSimpleView);
	
	ObjectFoSimpleDoc *simpleDoc=pObjectFoSimpleView->foObjectSimpleDoc();
	
	//refresh properties view
	m_pMainWindow->updatePropertiesView(simpleDoc);
	//if necessary refresh also foeditview
	if(simpleDoc->type()==ObjectFoDoc::typeFoBlock)
	{
		m_pMainWindow->updateXslFoEditView(simpleDoc);
	}
	else
	{
		//m_pMainWindow->clearXslFoEditView();
	}
}

void MainView::objectFoContainerSimpleSelected(ObjectFoContainerSimpleView *pFoObjectContainerSimpleView)
{
	m_pSelectedItemView=pFoObjectContainerSimpleView;
	
	m_pMainWindow->setRulersForPage(m_pSelectedItemView);
	
	ObjectFoContainerSimpleDoc *objFoContainerSimpleDoc=pFoObjectContainerSimpleView->objectFoContainerSimpleDoc();
	
	//refresh properties view
	m_pMainWindow->updatePropertiesView(objFoContainerSimpleDoc);
	//if necessary refresh also foeditview
	if(objFoContainerSimpleDoc->type()==ObjectFoDoc::typeFoBlock)
	{
//		m_pMainWindow->updateXslFoEditView(objFoContainerSimpleDoc);
	}
	else
	{
		//m_pMainWindow->clearXslFoEditView();
	}
}

void MainView::foObjectSequencedSelected(ObjectFoSequencedView *pObjectFoSequencedView)
{
	m_pSelectedItemView=pObjectFoSequencedView;
	
	m_pMainWindow->setRulersForPage(pObjectFoSequencedView);
	
	ObjectFoSequencedDoc *sequencedDoc=pObjectFoSequencedView->foObjectSequencedDoc();
	
	//refresh properties view
	m_pMainWindow->updatePropertiesView(sequencedDoc);
}

void MainView::scale ( qreal sx, qreal sy )
{
	m_scale.setX(m_scale.x()*sx);
	m_scale.setY(m_scale.y()*sy);

	m_pGraphicsView->scale(sx,sy);
}

void MainView::pushToUndoStack(QUndoCommand *cmd)
{
	m_pMainDoc->pushToUndoStack(cmd);
}

int MainView::verticalScrollBarValue(qreal scale)
{
	return m_pGraphicsView->verticalScrollBarValue(scale);
}

int MainView::horizontalScrollBarValue(qreal scale)
{
	return m_pGraphicsView->horizontalScrollBarValue(scale);
}

void MainView::menuQActionTableResize(FoTableView::ETableResizeCmd resizeType)
{
	if (m_pSelectedItemView->doc()->type()==ObjectFoDoc::typeFoTableCell)
	{
		switch(resizeType)
		{
			case FoTableView::addRow:
				((FoTableCellView*)m_pSelectedItemView)->addRow();
				break;
			case FoTableView::addCol:
				((FoTableCellView*)m_pSelectedItemView)->addColumn();
				break;
			case FoTableView::delRow:
			case FoTableView::delCol:
			default:
				break;
		}
	}
}

void MainView::showDockFoTextEdit()
{
	m_pMainWindow->showDockFoTextEdit();
}

void MainView::setUpObjectFoHandler(ObjectFoView *objectFo)
{
	if (m_pObjectFoHandler->currentParent()!=NULL)
		m_pObjectFoHandler->currentParent()->giveBackZPosition();
	
	if (objectFo!=NULL)
	{
		m_pObjectFoHandler->setParentObject(objectFo);
		objectFo->setForthZPosition();
		m_pObjectFoHandler->show();
	}
}

ObjectFoHandler *MainView::objectFoHandler()
{
	return m_pObjectFoHandler;
}
