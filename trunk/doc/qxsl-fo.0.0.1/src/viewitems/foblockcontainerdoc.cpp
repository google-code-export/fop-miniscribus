//http://www.zvon.org/xxl/xslfoReference/Output/index.html
/*
| absolute-position | top | right | bottom | left 
| background-attachment | background-color | background-image | background-repeat | background-position-horizontal 
| background-position-vertical | border-before-color | border-before-style | border-before-width | border-after-color 
| border-after-style | border-after-width | border-start-color | border-start-style | border-start-width 
| border-end-color | border-end-style | border-end-width | border-top-color | border-top-style | border-top-width 
| border-bottom-color | border-bottom-style | border-bottom-width | border-left-color | border-left-style 
| border-left-width | border-right-color | border-right-style | border-right-width | padding-before 
| padding-after | padding-start | padding-end | padding-top | padding-bottom | padding-left | padding-right 
| margin-top | margin-bottom | margin-left | margin-right | space-before | space-after | start-indent 
| end-indent | block-progression-dimension | break-after | break-before | clip | display-align | height | id 
| inline-progression-dimension | intrusion-displace | keep-together | keep-with-next | keep-with-previous 
| overflow | reference-orientation | span | width | writing-mode | z-index |
*/

//position="auto"
//position="absolute"
//position="fixed"

//own include
#include "foblockcontainerdoc.h"

//qt include
#include <QPoint>
#include <QDomElement>
// #include <QVariant>
#include <QRect>

//project includes
#include "objectfocontainersimpledoc.h"
#include "attributefodata.h"
#include "textcontentdoc.h"

FoBlockContainerDoc::FoBlockContainerDoc(QDomElement pElement,ObjectFoContainerDoc *parentContainer,QPoint pos)
 : ObjectFoContainerSimpleDoc(ObjectFoDoc::typeFoBlockContainer,parentContainer,pElement)
{	

	//<fo:block font-size="18pt" font-family="sans-serif" line-height="24pt" background-color="blue" color="white" text-align="center" padding-top="0pt">
//	addAttribute(new AttributeFo(AttributeFo::font_size,NULL));
// 	addAttribute(new AttributeFo(AttributeFo::font_family,NULL));
// 	addAttribute(new AttributeFo(AttributeFo::line_height,NULL));
// 	addAttribute(new AttributeFo(AttributeFo::background_color,NULL));
// 	addAttribute(new AttributeFo(AttributeFo::color,NULL));
// 	addAttribute(new AttributeFo(AttributeFo::padding_top,NULL));
	addAttribute(new AttributeFo(AttributeFo::top,NULL));
	addAttribute(new AttributeFo(AttributeFo::left,NULL));
	addAttribute(new AttributeFo(AttributeFo::width,NULL));
	
	ObjectFoContainerSimpleDoc::loadChildrenToDoc();
	
	//set position
	if (pos==QPoint(-1,-1))
	{//object is loaded from file
		int left=attributeValueByType(AttributeFo::left)->sizeValue().m_value;
		int top=attributeValueByType(AttributeFo::top)->sizeValue().m_value;
		setPositionValue(QPoint(left,top));
	}
	else
	{//object is crated after drop action
		setPositionValue(pos);
	}
	
	//set width from fo document
	int width=attributeValueByType(AttributeFo::width)->sizeValue().m_value;
	setWidthValue(width);

	
	//set textcontent for foblockconrainer
//	ObjectFoSimpleDoc::setTextContent(new TextContentDoc(this));

}

FoBlockContainerDoc::~FoBlockContainerDoc()
{

}

QPoint FoBlockContainerDoc::positionValue()
{
	QPoint ret;
	
	FoSize x=ObjectFoContainerSimpleDoc::attributeValueByType(AttributeFo::left)->sizeValue().m_value;
	FoSize y=ObjectFoContainerSimpleDoc::attributeValueByType(AttributeFo::top)->sizeValue().m_value;
	
	ret.setX(x);
	ret.setY(y);
	
	return ret;
}

void FoBlockContainerDoc::setPositionValue(QPoint pos)
{
	///todo: remove mm
	setAttributeValue(AttributeFo::left,QString::number(pos.x())+"mm");
	setAttributeValue(AttributeFo::top,QString::number(pos.y())+"mm");
}

void FoBlockContainerDoc::setWidthValue(FoSize width)
{
	//todo: remove mm
	setAttributeValue(AttributeFo::width,QString::number(width)+"mm");
}

FoSize FoBlockContainerDoc::widthValue()
{
	FoSize width=ObjectFoContainerSimpleDoc::attributeValueByType(AttributeFo::width)->sizeValue().m_value;
	return width;
}


