#ifndef BODYREGIONDOC_H
#define BODYREGIONDOC_H

#include <vector>

#include "objectfosequenceddoc.h"

class QDomElement;
class QPoint;

class PageDoc;
class FoBlockDoc;
class ObjectFoDoc;

/**
 * represent doc part of formating object fo:body-region
 */
class FoBodyRegionDoc : public ObjectFoSequencedDoc
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param elFoPgSeqPart 
	 * @param elFoLayMastSetPart 
	 * @param pPageDoc 
	 */
	FoBodyRegionDoc(QDomElement elFoPgSeqPart,QDomElement elFoLayMastSetPart,PageDoc *pPageDoc);
	/**
	 * destructor
	 */
	~FoBodyRegionDoc();
	/**
	 * calculate width of bodyregion (from page width and margins)
	 * @return width
	 */
	int widthValue();
	/**
	 * return doc part of parent page
	 * @return 
	 */
	PageDoc *pageDoc(){	return m_pPageDoc;}
	/**
	 * calculate and set width (it set right margin) in fo-document
	 * @param newWidth 
	 */
	void setWidthValue(FoSize newWidth);
	/**
	 * calculate and set height (it set bottom margin) in fo-document
	 * @param height 
	 */
	void setHeight(FoSize height);
	/**
	 * calculate and return height (it's computed from margins and page height)
	 * @return 
	 */
	FoSize height();
	/**
	 * it's used when view is deleted to reset doc (e.g. reset width height etc.)
	 */
	void reset();
	/**
	 * compute and return position, (it's computed from body region margins margin and page margins)
	 * @return 
	 */
	QPoint positionValue();
	/**
	 * compute and set position directly in fo-document (it's set margin top and margin left)
	 * @param pos 
	 */
	void setPositionValue(QPoint pos);
private:
	/**
	 * reference to parent page
	 */
	PageDoc *m_pPageDoc;
};

#endif

///todo: we dont need reference to pagedoc .. we have reference to it in class ObjectFoDoc
