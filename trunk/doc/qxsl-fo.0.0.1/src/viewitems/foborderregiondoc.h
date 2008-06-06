#ifndef FOBORDERREGIONDOC_H
#define FOBORDERREGIONDOC_H

#include <QDomElement>

#include "objectfosequenceddoc.h"

class PageDoc;

/**
 * represent doc part of formating objet fo:border-region, borders could be overlayed to each other - it depend on precedence attribute which 
 * is defined in fo-document
 */
class FoBorderRegionDoc  : public ObjectFoSequencedDoc
{
Q_OBJECT
public:	
	/**
	 * define type of region
	 */	
	enum EBorderRegionType
	{
		regionBefore,regionAfter,regionStart,regionEnd
	};
	/**
	 * constructor
	 * @param elFoPgSeqPart 
	 * @param elFoLayMastSetPart 
	 * @param regionType 
	 * @param pPageDoc 
	 */
	FoBorderRegionDoc( QDomElement elFoPgSeqPart,QDomElement elFoLayMastSetPart,EBorderRegionType regionType,PageDoc *pPageDoc);
	/**
	 * destructor
	 */
	~FoBorderRegionDoc();
	/**
	 * return width of border region, it's calculeted and it depend on type of region
	 * @return 
	 */
	FoSize widthValue();
	/**
	 * return type of current region
	 * @return 
	 */
	EBorderRegionType regionType(){	return m_regionType;}
	/**
	 * return reference to parent page
	 * @return 
	 */
	PageDoc *pageDoc(){	return m_pPageDoc;}
	/**
	 * on attribute precedence depend region overlay, if is set, we need regions to calculate width of region
	 * @param doc1 
	 * @param doc2 
	 */
	void addPrecedenceRegions(FoBorderRegionDoc *doc1,FoBorderRegionDoc *doc2){m_pPrecedenceRegions[0]=doc1;m_pPrecedenceRegions[1]=doc2;}
	/**
	 * this method is used for regionBefore and region after to calculate shifted position on x axis, position depened on precedence attribute
	 * @return 
	 */
	FoSize xPos();
private:
	/**
	 * contain reference to regions, which could overlay current region, they are used in depend of precedence attribute
	 */
	FoBorderRegionDoc *m_pPrecedenceRegions[2];
	/**
	 * contain reference to parent page
	 */
	PageDoc *m_pPageDoc;
	/**
	 * define type of current region
	 */
	EBorderRegionType m_regionType;
	
};


#endif
