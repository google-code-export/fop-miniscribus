#ifndef PAGEDOC_H
#define PAGEDOC_H

#include <QObject>

#include "foborderregiondoc.h"
#include "objectfosequenceddoc.h"

class QPointF;

class FoBlockDoc;
class FoBodyRegionDoc;
class MainDoc;

/**
 * doc part of page, it contain four borderregion and one body region
 */
class PageDoc : public ObjectFoSequencedDoc
{

public:
	/**
	 * constructor
	 * @param pMainDoc 
	 * @param elFoPagegSeqPart 
	 * @param elFoLayoutMasterSetPart 
	 */
	PageDoc(MainDoc *pMainDoc,QDomElement elFoPagegSeqPart,QDomElement elFoLayoutMasterSetPart);
	/**
	 * destructor
	 */
	~PageDoc();
	/**
	 * return one of the four border region
	 * @param regionType type of the desired region
	 * @return return one of the four border region (BorderRegion *)
	 */
	FoBorderRegionDoc *borderRegion(FoBorderRegionDoc::EBorderRegionType regionType);
	/**
	 * return body region
	 * @return return BodyRegion (BodyRegion *)
	 */
	FoBodyRegionDoc *bodyRegion(){	return m_pBodyRegion;}
	/**
	 * return parent maindoc
	 * @return MainDoc
	 */
	MainDoc *mainDoc(){	return m_pMainDoc;}
	/**
	 * return width of page
	 * @return 
	 */
	FoSize widthValue();
	/**
	 * return width of page between margins
	 * @return 
	 */
	FoSize widthWithMargin();
	/**
	 * do reference between region, becouse of precedence issue
	 */
	void setPrecedenceForRegions();
	/**
	 * if all regions are created then it's used this method to load children from fo-document to appropriate borders
	 */
	void loadChildrenToRegions();
private:
	/**
	 * create new region and fill it with simplepage element and pagesequence element
	 * @param regionNameInSPM name of node in SimplePageMaster (e.g. fo:region-body)
	 * @param regionNameInPS attribute value in foPageSequence should be in simple 
	 *			page master this is not handledyet, we use default xsl values (e.g. xsl-region-before)
	 * @param regionType 
	 * @return newly created border region
	 */
	FoBorderRegionDoc *createBorderRegion(QString regionNameInSPM,QString regionNameInPS,FoBorderRegionDoc::EBorderRegionType regionType);
	/**
	 * create body region
	 * @return 
	 */
	FoBodyRegionDoc *createBodyRegion();
	/**
	 * reference to main doc
	 * @return 
	 */
	MainDoc *m_pMainDoc;
	/**
	 * reference to border regions
	 */
	FoBorderRegionDoc *m_pBorderRegion[4];
	/**
	 * body region
	 */
	FoBodyRegionDoc *m_pBodyRegion;
};

#endif
