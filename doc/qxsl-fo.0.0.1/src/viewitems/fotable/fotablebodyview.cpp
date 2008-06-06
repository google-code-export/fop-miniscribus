//own include
#include "fotablebodyview.h"

//qt includes
#include <QGraphicsItem>

//project includes
#include "objectfosimpleview.h"
#include "objectfodoc.h"
#include "mainview.h"
#include "objectfocontainerdoc.h"

FoTableBodyView::FoTableBodyView(ObjectFoContainerView *parent,ObjectFoContainerDoc *doc,MainView *pMainView)
:ObjectFoSimpleView(parent,doc,pMainView)
{
}


FoTableBodyView::~FoTableBodyView()
{
}


