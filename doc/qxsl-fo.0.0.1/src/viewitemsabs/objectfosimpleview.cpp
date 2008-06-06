//own include
#include "objectfosimpleview.h"

//qt include
#include <QApplication>
#include <QDrag>
#include <QMimeData>

//project includes
#include "foblockdoc.h"
#include "fotableview.h"
#include "foblockview.h"
#include "objectfoview.h"
#include "mainview.h"
#include "pageview.h"
#include "objectfocontainerview.h"

ObjectFoSimpleView::ObjectFoSimpleView(ObjectFoContainerView *parentContainer,ObjectFoDoc *doc,MainView *pMainView,QGraphicsScene *scene)
:ObjectFoView(parentContainer,doc,pMainView,scene)
{

}


ObjectFoSimpleView::~ObjectFoSimpleView()
{

}

void ObjectFoSimpleView::mousePressEvent ( QGraphicsSceneMouseEvent *event )
{
	//we select item, and we need inform mainview about this action,
	//main view show propeties of this foobject then
	m_pMainView->foObjectSimpleSelected(this);

	ObjectFoView::mousePressEvent(event);
}
