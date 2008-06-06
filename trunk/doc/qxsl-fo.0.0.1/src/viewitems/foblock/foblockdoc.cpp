//http://www.zvon.org/xxl/xslfoReference/Output/index.html
//| source-document | role | azimuth | cue-after | cue-before | elevation | pause-after | pause-before | pitch | pitch-range | play-during | richness 
//| speak | speak-header | speak-numeral | speak-punctuation | speech-rate | stress | voice-family | volume | background-attachment | background-color 
//| background-image | background-repeat | background-position-horizontal | background-position-vertical | border-before-color | border-before-style 
//| border-before-width | border-after-color | border-after-style | border-after-width | border-start-color | border-start-style | border-start-width 
//| border-end-color | border-end-style | border-end-width | border-top-color | border-top-style | border-top-width | border-bottom-color 
//| border-bottom-style | border-bottom-width | border-left-color | border-left-style | border-left-width | border-right-color | border-right-style 
//| border-right-width | padding-before | padding-after | padding-start | padding-end | padding-top | padding-bottom | padding-left | padding-right 
//| font-model | font-family | font-selection-strategy | font-size | font-stretch | font-size-adjust | font-style | font-variant | font-weight 
//| country | language | script | hyphenate | hyphenation-character | hyphenation-push-character-count | hyphenation-remain-character-count 
//| margin-top | margin-bottom | margin-left | margin-right | space-before | space-after | start-indent | end-indent | relative-position 
//| break-after | break-before | color | text-depth | text-altitude | hyphenation-keep | hyphenation-ladder-count | id | intrusion-displace 
//| keep-together | keep-with-next | keep-with-previous | last-line-end-indent | linefeed-treatment | line-height | line-height-shift-adjustment 
//| line-stacking-strategy | orphans | white-space-treatment | span | text-align | text-align-last | text-indent | visibility | white-space-collapse 
//| widows | wrap-option |
 
//own include
#include "foblockdoc.h"

#include <vector>

//qt includes
#include <QPoint>
#include <QDomElement>
#include <QDomNodeList>

//project inceludes
#include "objectfodoc.h"
#include "objectfocontainersimpledoc.h"
#include "fobodyregiondoc.h"
#include "textcontentdoc.h"
#include "foexternalgraphicdoc.h"
#include "objectfocontainerdoc.h"

FoBlockDoc::FoBlockDoc(QDomElement domElement,ObjectFoContainerDoc *parentContainer)
 : ObjectFoContainerSimpleDoc(ObjectFoDoc::typeFoBlock,parentContainer,domElement)
{
	//set default height for block
	setHeight(0);

	//<fo:block font-size="18pt" font-family="sans-serif" line-height="24pt" background-color="blue" color="white" text-align="center" padding-top="0pt">
	addAttribute(new AttributeFo(AttributeFo::font_size,NULL));
	//addAttribute(new AttributeFo(AttributeFo::font_family,NULL));
	//addAttribute(new AttributeFo(AttributeFo::line_height,NULL));
	//addAttribute(new AttributeFo(AttributeFo::background_color,NULL));
	//addAttribute(new AttributeFo(AttributeFo::color,NULL));
	addAttribute(new AttributeFo(AttributeFo::text_align,NULL));
	//addAttribute(new AttributeFo(AttributeFo::padding_top,NULL));
	
	loadViewableChildBlocks(domElement);
}

FoBlockDoc::~FoBlockDoc()
{
	
}

FoSize FoBlockDoc::widthValue()
{
	return m_pParentContainer->widthValue();
}

FoSize FoBlockDoc::height()
{
	return ObjectFoDoc::height();
}

void FoBlockDoc::loadViewableChildBlocks(QDomElement foBlockElement)
{
	// go throught the whole foblock and split it to the small blocks
	for(QDomNode n = foBlockElement.firstChild();!n.isNull(); n = n.nextSibling())
	{
		//if node is text node
		if (n.isText())
		{
			QDomText t = n.toText();
			if (!t.isNull())
				ObjectFoContainerDoc::addItem( new TextContentDoc(this,t.data()) );
			continue;
		}
		
		QDomElement eFoBlockPart = n.toElement();
		
		if(eFoBlockPart.isNull())
		{
			qFatal("One part of the FobBlock data is NULL !");
		}
		else if (eFoBlockPart.tagName()=="fo:inline")
		{
			qDebug("fo:inline not implemented yet");
		}
		//if node is picture
		else if (eFoBlockPart.tagName()=="fo:external-graphic")
		{
			ObjectFoContainerDoc::addItem( new FoExternalGraphicDoc(eFoBlockPart, this) );
		}
	}
}

