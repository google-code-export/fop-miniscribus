#include "attributefo.h"

AttributeFo::AttributeFo(EAttributeType type,QString defaultValue)
{
	m_type=type;
	m_defaultValue=defaultValue;
}


AttributeFo::~AttributeFo()
{
	
}

QString AttributeFo::name() const
{
	return typeToString(m_type);
}

AttributeFo::EAttributeGroup AttributeFo::group() const
{
	switch(m_type)
	{
		case margin_top:
		case margin_bottom:
		case margin_left:
		case margin_right:
		case margin:
		case page_height:
		case page_width:
		case extent:
		case line_height:
		case padding_top:
		case top:
		case left:
		case width:
		case font_size:
		case content_width:
			return sizeGroup;
		case font_family:
			return fontFamilyGroup;
		case background_color:
		case color:
			return colorGroup;
		case text_align:
			return alignGroup;
		case src:
			return stringGroup;
		case precedence:
			return boolGroup;
		default:
			return noneGroup;
	}
}
	
////////////////////
// static methods //
////////////////////
AttributeFo::EAlignValues AttributeFo::stringToAlignType(QString name)
{
	if(name=="left")
		return left_align;
	if(name=="right")
		return right_align;
	if(name=="center")
		return center_align;
	else
		return unknown_align;
}

AttributeFo::EAttributeType AttributeFo::stringToType(QString name)
{
	if(name=="margin-top")
		return margin_top;
	else if(name=="margin-bottom")
		return margin_bottom;
	else if(name=="margin-left")
		return margin_left;
	else if(name=="margin-right")
		return margin_right;
	else if(name=="left")
		return left;
	else if(name=="top")
		return top;
	else if(name=="width")
		return width;
	else if(name=="text-align")
		return text_align;
	else if(name=="font-size")
		return font_size;
	else if(name=="src")
		return src;
	else if(name=="content-width")
		return content_width;
	else if(name=="precedence")
		return precedence;
	else 
		return unknown;
}

QString AttributeFo::typeToString(EAttributeType enumName)
{
	switch(enumName)
	{
		case margin_top:
			return QString("margin-top");
			break;
		case margin_bottom:
			return QString("margin-bottom");
			break;
		case margin_left:
			return QString("margin-left");
			break;
		case margin_right:
			return QString("margin-right");
			break;
		case margin:
			return QString("margin");
			break;
		case page_height:
			return QString("page-height");
		case page_width:
			return QString("page-width");
		case extent:
			return QString("extent");
		case font_size:
			return QString("font-size");
		case font_family:
			return QString("font-family");
		case line_height:
			return QString("line-height");
		case background_color:
			return QString("background-color");
		case color:
			return QString("color");
		case text_align:
			return QString("text-align");
		case padding_top:
			return QString("padding-top");
		case width:
			return QString("width");
		case left:
			return QString("left");
		case top:
			return QString("top");
		case src:
			return QString("src");
		case content_width:
			return QString("content-width");
		case precedence:
			return QString("precedence");
		default:
			return QString();
	}
}
