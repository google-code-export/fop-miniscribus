//own include
#include "foexternalgraphicdoc.h"

//qt includes
#include <QDomElement>
#include <QUrl>
#include <QFileInfo>
//project includes
#include "objectfodoc.h"
#include "attributefo.h"

FoExternalGraphicDoc::FoExternalGraphicDoc(QDomElement domElement,ObjectFoContainerDoc *parentContainer)
: ObjectFoSimpleDoc(ObjectFoDoc::typeFoExternalGraphic,parentContainer,domElement)
{
	
	//<fo:external-graphic src="url(mzr90.jpg)" width="6cm" height="6cm" content-width="4cm" display-align="center" text-align="center"/>^
	addAttribute(new AttributeFo(AttributeFo::src,NULL));
	addAttribute(new AttributeFo(AttributeFo::content_width,NULL));
	
}


FoExternalGraphicDoc::~FoExternalGraphicDoc()
{
	
}

int FoExternalGraphicDoc::contentWidth()
{
	int contentWidth = attributeValueByType(AttributeFo::content_width)->sizeValue().m_value;
	return contentWidth;
}

FoSize FoExternalGraphicDoc::widthValue()
{
	return contentWidth();
}

QString FoExternalGraphicDoc::picturePath()
{
	QString pixPath = attributeValueByType(AttributeFo::src)->stringValue();
	if (pixPath.startsWith("url("))
	{
		pixPath=pixPath.mid(4,pixPath.length()-5);
	}
	
	/* file start from ./ current dir? from file */
	
	QFileInfo local;
	QUrl url(pixPath);
	if (url.scheme().isEmpty()) {
		local.setFile(pixPath);
		return local.absoluteFilePath();
	} else {
		return url.toString(QUrl::RemovePassword);
	}
}
