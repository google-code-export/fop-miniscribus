//own include
#include "objectfosequencedview.h"

//qt includes

//projects include
#include "foblockview.h"
#include "fotableview.h"
#include "foblockcontainerview.h"
#include "mainview.h"
#include "objectfocontainerview.h"

#include "pagedoc.h"
#include "maindoc.h"
#include "objectfosequenceddoc.h"
#include "objectfodoc.h"

ObjectFoSequencedView::ObjectFoSequencedView(ObjectFoContainerView *parentGroup,ObjectFoSequencedDoc *doc,MainView *pMainView,QGraphicsScene *scene)
:ObjectFoContainerView(parentGroup,doc,pMainView,scene)
{
	//we add containers to the page by hand
	if(ObjectFoView::doc()->type()!=ObjectFoDoc::typeFoPage)
	{
		loadChildrenToView(doc);
	}
}


ObjectFoSequencedView::~ObjectFoSequencedView()
{
	
}

void ObjectFoSequencedView::slDoSelect()
{

}

void ObjectFoSequencedView::mousePressEvent ( QGraphicsSceneMouseEvent *event )
{
	m_pMainView->foObjectSequencedSelected(this);
	//emit sigItemSelected((FoObjectSequenced*)doc());
	ObjectFoContainerView::mousePressEvent(event);
}

