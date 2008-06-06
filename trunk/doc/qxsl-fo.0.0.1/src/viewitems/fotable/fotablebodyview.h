#ifndef FOTABLEBODYVIEW_H
#define FOTABLEBODYVIEW_H

#include "objectfosimpleview.h"

class ObjectFoDoc;
class MainView;
class ObjectFoContainerDoc;

/**
 * represent 
 */
class FoTableBodyView : public ObjectFoSimpleView
{
public:
	FoTableBodyView(ObjectFoContainerView *parent,ObjectFoContainerDoc *doc,MainView *pMainView);
	~FoTableBodyView();
};

#endif
