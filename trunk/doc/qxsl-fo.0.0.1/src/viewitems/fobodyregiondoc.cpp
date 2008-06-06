//own include
#include "fobodyregiondoc.h"

//qt include
#include <QDomElement>

//project include
#include "objectfosequenceddoc.h"
#include "pagedoc.h"
#include "objectfodoc.h"
#include "nodefo.h"

FoBodyRegionDoc::FoBodyRegionDoc(QDomElement elFoPgSeqPart,QDomElement elFoLayMastSetPart,PageDoc *pPageDoc)
:ObjectFoSequencedDoc(ObjectFoDoc::typeFoBodyRegion,elFoPgSeqPart,elFoLayMastSetPart,pPageDoc),m_pPageDoc(pPageDoc)
{
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin_top,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin_bottom,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin_left,NULL));
	appendAtributeToFoSimplePageMasterPart(new AttributeFo(AttributeFo::margin_right,NULL));
	
	ObjectFoSequencedDoc::loadChildrenToDoc();
}


FoBodyRegionDoc::~FoBodyRegionDoc()
{
}

int FoBodyRegionDoc::widthValue()
{
	AttributeFoData *attrPageWidth=m_pPageDoc->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::page_width);
	AttributeFoData *attrPageMargin=m_pPageDoc->getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::margin);

	int regionMargLeft = getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::margin_left)->sizeValue().m_value;
	int regionMargRight = getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::margin_right)->sizeValue().m_value;

	int pageWidth=attrPageWidth->sizeValue().m_value;
	int pageMargin=attrPageMargin->sizeValue().m_value;	

	return pageWidth-pageMargin-pageMargin-regionMargLeft-regionMargRight;
}

void FoBodyRegionDoc::setWidthValue(FoSize newWidth)
{
	FoSize currentWidth=widthValue();
	
	int regionMargRight = getFoSimplePageMasterPart()->attributeValueByType(AttributeFo::margin_right)->sizeValue().m_value;
	int newMarginSize =  regionMargRight - (newWidth-currentWidth);
	//we need to set right margin
	QString newValue=QString::number(newMarginSize)+"mm";
	getFoSimplePageMasterPart()->setAttributeValue(AttributeFo::margin_right,newValue);
}

void FoBodyRegionDoc::setHeight(FoSize newHeight)
{
	FoSize currentHeight=height();
	
	int regionMarTop = getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::margin_bottom )->sizeValue().m_value;
	int newMarginSize = regionMarTop - (newHeight-currentHeight);
	
	QString newValue=QString::number(newMarginSize)+"mm";
	getFoSimplePageMasterPart()->setAttributeValue(AttributeFo::margin_bottom,newValue);
}

FoSize FoBodyRegionDoc::height()
{
	AttributeFoData *attrPageHeight=m_pPageDoc->getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::page_height );
	AttributeFoData *attrPageMargin=m_pPageDoc->getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::margin );

	int margTop = getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::margin_top )->sizeValue().m_value;
	int margBottom = getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::margin_bottom )->sizeValue().m_value;
	
	int pageHeight=attrPageHeight->sizeValue().m_value;
	int pageMargin=attrPageMargin->sizeValue().m_value;

	return pageHeight-pageMargin-pageMargin-margTop-margBottom;
}

QPoint FoBodyRegionDoc::positionValue()
{
	AttributeFoData *attrPageMargin=m_pPageDoc->getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::margin );

	int margTop = getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::margin_top )->sizeValue().m_value;
	int margLeft = getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::margin_left )->sizeValue().m_value;

	int pageMargin=attrPageMargin->sizeValue().m_value;

	return QPoint(pageMargin+margLeft,pageMargin+margTop);
}

void FoBodyRegionDoc::setPositionValue(QPoint pos)
{
	QPoint oldPositionValue=positionValue();
	AttributeFoData *attrPageMargin=m_pPageDoc->getFoSimplePageMasterPart()->attributeValueByType ( AttributeFo::margin );
	int pageMargin=attrPageMargin->sizeValue().m_value;
	
	int newX=pos.x()-pageMargin;
	int newY=pos.y()-pageMargin;
	
	//we change left and top margin	and with and height is computed from this attributes
	//so we must change them
	int distX=newX-(oldPositionValue.x()-pageMargin);
	int newWidthValue=widthValue() + distX;	
	setWidthValue(newWidthValue);
	
	int distY=newY-(oldPositionValue.y()-pageMargin);
	int newHeightValue=height() + distY;
	setHeight(newHeightValue);
	
	QString newLeft=QString::number(newX) + "mm";
	QString newTop=QString::number(newY) + "mm";
	
	getFoSimplePageMasterPart()->setAttributeValue( AttributeFo::margin_top,newTop  );
	getFoSimplePageMasterPart()->setAttributeValue( AttributeFo::margin_left,newLeft );
}	

void FoBodyRegionDoc::reset()
{
	//do nothing becouse we dont want set height to null
}
