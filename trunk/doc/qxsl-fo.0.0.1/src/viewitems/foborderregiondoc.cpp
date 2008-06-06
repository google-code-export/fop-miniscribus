#include "foborderregiondoc.h"

//qt includes
#include <QDomElement>

//proj includes
#include "objectfosequenceddoc.h"
#include "attributefo.h"
#include "objectfodoc.h"
#include "pagedoc.h"
#include "nodefo.h"

FoBorderRegionDoc::FoBorderRegionDoc( QDomElement elFoPgSeqPart,QDomElement elFoLayMastSetPart,EBorderRegionType regionType,PageDoc *pPageDoc)
 : ObjectFoSequencedDoc(ObjectFoDoc::typeFoBorderRegion,elFoPgSeqPart,elFoLayMastSetPart,pPageDoc),m_pPageDoc(pPageDoc),m_regionType(regionType)
{
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::extent,NULL));
	
	//specified attributes for regionBefore and regionAfter
	if ( m_regionType==FoBorderRegionDoc::regionBefore || m_regionType==FoBorderRegionDoc::regionAfter)
	{
		appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::precedence,NULL));
	}
	
}


FoBorderRegionDoc::~FoBorderRegionDoc()
{
}

FoSize FoBorderRegionDoc::xPos()
{
	if (regionType()==FoBorderRegionDoc::regionBefore || regionType()==FoBorderRegionDoc::regionAfter)
	{
		bool precedence=getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::precedence)->boolValue();
		//if precedence == false we have to use extent from regionstart & regionend
		if (precedence==false)
		{
			int precedencedWidth=m_pPrecedenceRegions[0]->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::extent)->sizeValue().m_value;
			int xPos = precedencedWidth;
			return xPos;
		}
	}
	
	return 0;
	
}

FoSize FoBorderRegionDoc::widthValue()
{
	int precedencedWidth=0;
	//width depend on region type, on orientation -- orientation is not implemented yet
	// on extent and on precedence
	if (regionType()==FoBorderRegionDoc::regionStart || regionType()==FoBorderRegionDoc::regionEnd)
	{
		AttributeFoData *attrExtend=getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::extent);
		int regionExtent=attrExtend->sizeValue().m_value;	
		return regionExtent;
	}
	else if (regionType()==FoBorderRegionDoc::regionBefore || regionType()==FoBorderRegionDoc::regionAfter)
	{
		bool precedence=getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::precedence)->boolValue();
		//if precedence == false we have to use extent from regionstart & regionend
		if (precedence==false)
		{
			precedencedWidth=m_pPrecedenceRegions[0]->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::extent)->sizeValue().m_value;
			precedencedWidth+=m_pPrecedenceRegions[1]->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::extent)->sizeValue().m_value;
		}
		int width = m_pPageDoc->widthWithMargin()-precedencedWidth;
		return width;
	}
	else
	{
		qFatal("Unrecognized BorderRegion type");
		return 0;
	}
}

