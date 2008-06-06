#ifndef FOBORDERREGIONVIEW_H
#define FOBORDERREGIONVIEW_H

class BorderRegion;

#include "objectfosequencedview.h"
#include "pageview.h"
#include "foborderregiondoc.h"

/**
 * represent view part of fotmating object fo:border-region
 */
class FoBorderRegionView : public ObjectFoSequencedView
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param parentContainer 
	 * @param doc 
	 * @param pMainView 
	 */
	FoBorderRegionView(ObjectFoContainerView *parentContainer,ObjectFoSequencedDoc *doc,MainView *pMainView);
	/**
	 * destructor
	 */
	~FoBorderRegionView();
	/**
	 * return doc part of border region
	 * @return 
	 */
	FoBorderRegionDoc *borderRegionDoc(){	return (FoBorderRegionDoc *)doc();}
private:
	/**
	 * draw rectangle of the region
	 */
	void draw();
};

#endif
