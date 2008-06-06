//own include
#include "pagedoc.h"

//qt include
#include <QPointF>

//project include
#include "maindoc.h"
#include "foblockdoc.h"
#include "foborderregiondoc.h"
#include "fobodyregiondoc.h"

PageDoc::PageDoc(MainDoc *pMainDoc,QDomElement elFoPagegSeqPart,QDomElement elFoLayoutMasterSetPart)
 : ObjectFoSequencedDoc(ObjectFoDoc::typeFoPage,elFoPagegSeqPart,elFoLayoutMasterSetPart,NULL),m_pMainDoc(pMainDoc)
{
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::page_height,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::page_width,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin_top,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin_bottom,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin_left,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin_right,NULL));
	
	//create body region	
	m_pBodyRegion=createBodyRegion();
	ObjectFoContainerDoc::addItem(m_pBodyRegion);
	
	//create borderregions
	m_pBorderRegion[FoBorderRegionDoc::regionBefore]=createBorderRegion("fo:region-before","xsl-region-before",FoBorderRegionDoc::regionBefore);
	m_pBorderRegion[FoBorderRegionDoc::regionAfter]=createBorderRegion("fo:region-after","xsl-region-after",FoBorderRegionDoc::regionAfter);
	m_pBorderRegion[FoBorderRegionDoc::regionStart]=createBorderRegion("fo:region-start","xsl-region-start",FoBorderRegionDoc::regionStart);
	m_pBorderRegion[FoBorderRegionDoc::regionEnd]=createBorderRegion("fo:region-end","xsl-region-end",FoBorderRegionDoc::regionEnd);
	//add to container
	ObjectFoContainerDoc::addItem(m_pBorderRegion[FoBorderRegionDoc::regionBefore]);
	ObjectFoContainerDoc::addItem(m_pBorderRegion[FoBorderRegionDoc::regionAfter]);
	ObjectFoContainerDoc::addItem(m_pBorderRegion[FoBorderRegionDoc::regionStart]);
	ObjectFoContainerDoc::addItem(m_pBorderRegion[FoBorderRegionDoc::regionEnd]);
	
	//set precedences for border regions
	setPrecedenceForRegions();
	
	//load children to border regions	
	loadChildrenToRegions();	
}

PageDoc::~PageDoc()
{
}

FoBodyRegionDoc *PageDoc::createBodyRegion()
{
	//get body region from simplepagedata
	QDomElement tmpSimplePage=getFoSimplePageMasterPart()->firstChildElement("fo:region-body");;
	//get body region from page sequence
	QDomElement tmpPageSequence=getFoPageSequencePart()->firstElementByAttributeValue("flow-name","xsl-region-body");
	
	//create body region and use data from page sequence and simple page
	return new FoBodyRegionDoc(tmpPageSequence,tmpSimplePage,this);
}

void PageDoc::loadChildrenToRegions()
{
	m_pBorderRegion[FoBorderRegionDoc::regionBefore]->loadChildrenToDoc();
	m_pBorderRegion[FoBorderRegionDoc::regionAfter]->loadChildrenToDoc();
	m_pBorderRegion[FoBorderRegionDoc::regionStart]->loadChildrenToDoc();
	m_pBorderRegion[FoBorderRegionDoc::regionEnd]->loadChildrenToDoc();
}

void PageDoc::setPrecedenceForRegions()
{
	m_pBorderRegion[FoBorderRegionDoc::regionBefore]->addPrecedenceRegions(m_pBorderRegion[FoBorderRegionDoc::regionStart],m_pBorderRegion[FoBorderRegionDoc::regionEnd]);
	m_pBorderRegion[FoBorderRegionDoc::regionAfter]->addPrecedenceRegions(m_pBorderRegion[FoBorderRegionDoc::regionStart],m_pBorderRegion[FoBorderRegionDoc::regionEnd]);
	
	m_pBorderRegion[FoBorderRegionDoc::regionStart]->addPrecedenceRegions(m_pBorderRegion[FoBorderRegionDoc::regionBefore],m_pBorderRegion[FoBorderRegionDoc::regionAfter]);
	m_pBorderRegion[FoBorderRegionDoc::regionEnd]->addPrecedenceRegions(m_pBorderRegion[FoBorderRegionDoc::regionBefore],m_pBorderRegion[FoBorderRegionDoc::regionAfter]);
}

FoBorderRegionDoc *PageDoc::createBorderRegion(QString regionNameInSPM,QString regionNameInPS,FoBorderRegionDoc::EBorderRegionType regionType)
{
	//get specified border region element from simplepage part 
	QDomElement tmpSimplePage=getFoSimplePageMasterPart()->firstChildElement(regionNameInSPM);
	
	//get specified border region element from pageseqence part
	QDomElement tmpPageSequence=getFoPageSequencePart()->firstElementByAttributeValue("flow-name",regionNameInPS);
	
	if (!tmpSimplePage.isNull())
	{
		return new FoBorderRegionDoc(tmpPageSequence,tmpSimplePage,regionType,this);
	}
	else
	{
		qDebug("type:%d border region in simple page not defined",regionType);
		return NULL;	
	}
}

FoBorderRegionDoc *PageDoc::borderRegion(FoBorderRegionDoc::EBorderRegionType regionType)
{
	return m_pBorderRegion[regionType];
}

FoSize PageDoc::widthValue()
{
	AttributeFoData *attrWidth=getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::page_width);
	
	int width=attrWidth->sizeValue().m_value;
	
	return width;
}

FoSize PageDoc::widthWithMargin()
{
	AttributeFoData *attrWidth=getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::page_width);
	AttributeFoData *attrMargin=getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::margin);
	
	int width=attrWidth->sizeValue().m_value;
	int margin=attrMargin->sizeValue().m_value;

	return width-margin-margin;
}

