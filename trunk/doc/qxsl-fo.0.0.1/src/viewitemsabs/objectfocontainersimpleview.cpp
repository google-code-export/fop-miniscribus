//own include
#include "objectfocontainersimpleview.h"

//qt includes
#include <QGraphicsItem>

//project includes
#include "foblockview.h"
#include "objectfodoc.h"
#include "mainview.h"
#include "objectfocontainerview.h"
#include "objectfocontainersimpledoc.h"
#include "pageview.h"

ObjectFoContainerSimpleView::ObjectFoContainerSimpleView(ObjectFoContainerView *parentContainer,ObjectFoContainerDoc *doc,MainView *pMainView,QGraphicsScene *scene)
 : ObjectFoContainerView(parentContainer,doc,pMainView,scene)
{
	//we fill foblock with different approach
	if(ObjectFoView::doc()->type()!=ObjectFoDoc::typeFoBlock)
	{
		loadChildrenToView(doc);
	}
}


ObjectFoContainerSimpleView::~ObjectFoContainerSimpleView()
{
	
}

void ObjectFoContainerSimpleView::mousePressEvent ( QGraphicsSceneMouseEvent *event )
{
	//we select item, and we need inform mainview about this action,
	//main view show propeties of this foobject then
	m_pMainView->objectFoContainerSimpleSelected(this);

	ObjectFoContainerView::mousePressEvent(event);
}
