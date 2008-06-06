//http://www.zvon.org/xxl/xslfoReference/Output/index.html
/*
| source-document | role | azimuth | cue-after | cue-before | elevation | pause-after | pause-before | pitch 
| pitch-range | play-during | richness | speak | speak-header | speak-numeral | speak-punctuation 
| speech-rate | stress | voice-family | volume | background-attachment | background-color | background-image 
| background-repeat | background-position-horizontal | background-position-vertical | border-before-color 
| border-before-style | border-before-width | border-after-color | border-after-style | border-after-width 
| border-start-color | border-start-style | border-start-width | border-end-color | border-end-style 
| border-end-width | border-top-color | border-top-style | border-top-width | border-bottom-color 
| border-bottom-style | border-bottom-width | border-left-color | border-left-style | border-left-width 
| border-right-color | border-right-style | border-right-width | padding-before | padding-after 
| padding-start | padding-end | padding-top | padding-bottom | padding-left | padding-right | margin-top 
| margin-bottom | margin-left | margin-right | space-before | space-after | start-indent | end-indent 
| relative-position | block-progression-dimension | border-after-precedence | border-before-precedence 
| border-collapse | border-end-precedence | border-separation | border-start-precedence | break-after 
| break-before | id | inline-progression-dimension | intrusion-displace | height | keep-together 
| keep-with-next | keep-with-previous | table-layout | table-omit-footer-at-break | table-omit-header-at-break 
| width | writing-mode |
*/

//own include
#include "fotabledoc.h"

//qt includes
#include <QDomElement>

//project inceludes
#include "objectfodoc.h"

#include "fotablebodydoc.h"
#include "fotablerowdoc.h"


FoTableDoc::FoTableDoc(QDomElement pElement,ObjectFoContainerDoc *parentContainer)
 : ObjectFoContainerSimpleDoc(ObjectFoDoc::typeFoTable,parentContainer,pElement)
{
	loadTableBody(pElement);	
	//get new position for our block
	setPositionValue(parentContainer->positionForNewChildFoObject());
	
// 	addAttribute(new AttributeFo(AttributeFo::font_size,NULL));
// 	addAttribute(new AttributeFo(AttributeFo::font_family,NULL));
}

FoTableDoc::~FoTableDoc()
{
	
}

void FoTableDoc::loadTableBody(QDomElement pElement)
{
	for(QDomNode n = pElement.firstChild();!n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull())
		{
			if (e.tagName()=="fo:table-body")
			{
				m_pFoTableBodyDoc=new FoTableBodyDoc(e,this);
			}
		}
	}
}

FoSize FoTableDoc::widthValue()
{
	return m_pParentContainer->widthValue();
}

FoTableRowDoc *FoTableDoc::row(int i)
{
	return m_pFoTableBodyDoc->row(i);
}

FoTableRowDoc* FoTableDoc::appendRow()
{
	//create rowdoc in tabledoc and return it
	return m_pFoTableBodyDoc->appendRow();
}

void FoTableDoc::appendColumn()
{
	m_pFoTableBodyDoc->appendColumn();
}

FoSize FoTableDoc::height()
{
	return ObjectFoDoc::height();
}

