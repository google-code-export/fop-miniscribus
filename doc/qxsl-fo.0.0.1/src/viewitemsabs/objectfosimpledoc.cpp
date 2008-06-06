//
// C++ Implementation: foobjectsimple
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//

//own include 
#include "objectfosimpledoc.h"

//qt includes
#include <QDomElement>
#include <QVariant>

//project includes
#include "absobjectfosimpledoc.h"
#include "objectfodoc.h"
#include "objectfocontainerdoc.h"
#include "nodefo.h"
#include "attributefo.h"
#include "focreator.h"

ObjectFoSimpleDoc::ObjectFoSimpleDoc(ObjectFoDoc::FoObjectType type,ObjectFoContainerDoc *parentContainer,QDomElement pElement)
 : ObjectFoDoc(type),AbsObjectFoSimpleDoc(pElement),m_pParentContainer(parentContainer)
{
	
}


ObjectFoSimpleDoc::~ObjectFoSimpleDoc()
{
	
}

///todo: this method should be i foobjectviewvable
void ObjectFoSimpleDoc::slDataChangedInPropertiesDoc(AttributeFo::EAttributeType attributeType,QVariant value)
{
	switch(attributeType)
	{
		case AttributeFo::left:
			move(QPoint(value.toInt(),positionValue().y()));
			break;
		case AttributeFo::top:
			move(QPoint(positionValue().x(),value.toInt()));
			break;
		case AttributeFo::width:
			resize(QPoint(value.toInt(),height()));
			break;
		case AttributeFo::text_align:
			break;
		case AttributeFo::content_width:
			break;
		case AttributeFo::precedence:
			break;
		case AttributeFo::margin_top:
		case AttributeFo::margin_bottom:
		case AttributeFo::margin_left:
		case AttributeFo::margin_right:
		case AttributeFo::margin:
		case AttributeFo::page_height:
		case AttributeFo::page_width:
		case AttributeFo::extent:
		case AttributeFo::font_size:
			break;
		case AttributeFo::font_family:
		case AttributeFo::line_height:
		case AttributeFo::background_color:
		case AttributeFo::color:
		case AttributeFo::padding_top:
		case AttributeFo::height:
		case AttributeFo::src:
		case AttributeFo::unknown:
			break;
	}	

}
