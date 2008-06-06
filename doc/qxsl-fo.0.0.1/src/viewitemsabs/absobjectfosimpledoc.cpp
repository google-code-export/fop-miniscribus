//own include
#include "absobjectfosimpledoc.h"

//qt includes
#include <QDomElement>
#include <QDomNode>

//project includes
#include "focreator.h"
#include "nodefo.h"
#include "attributefo.h"
#include "attributefodata.h"
#include "objectfocontainerdoc.h"
#include "nodefo.h"

AbsObjectFoSimpleDoc::AbsObjectFoSimpleDoc(QDomElement pElement)
{
	m_pNodeFo = new NodeFo(pElement);	
}


AbsObjectFoSimpleDoc::~AbsObjectFoSimpleDoc()
{
	delete m_pNodeFo;
}


void AbsObjectFoSimpleDoc::addAttribute(AttributeFo *attribute)
{
	m_pNodeFo->appendAttribute(attribute);
}

QDomNode AbsObjectFoSimpleDoc::insertChildElementAfter(QDomElement newChildElement,QDomElement refElement)
{
	return m_pNodeFo->insertChildElementAfter(newChildElement,refElement);
}

void AbsObjectFoSimpleDoc::setAttributeValue( AttributeFo::EAttributeType attributeName, QString value )
{
	m_pNodeFo->setAttributeValue(attributeName,value);
}

void AbsObjectFoSimpleDoc::setXslTemplate(QString xmlPath)
{
	//get style sheet root
	QDomNode xslStyleSheetRoot=m_pNodeFo->xslStyleSheetRoot();

	//get node of this foobject
	QDomNode currentNode=m_pNodeFo->elementData();

	//get node of parent
	QDomNode parentNode=currentNode.parentNode();

	//remove curent foobjparentElementect node from parent
	QDomNode removedNode=parentNode.removeChild(currentNode);
	if (removedNode.isNull())
		qFatal("Error occurs while moving foobjectsimple to xsltemplate match");

	//and create xsl-template match from it
	QDomNode xslTemplateMatch=FoCreator::GetInstancePtr()->xslTemplateMatch(xmlPath);
	xslTemplateMatch.appendChild(removedNode);
	
	//and append teplate match
	xslStyleSheetRoot.appendChild(xslTemplateMatch);
	
	//and append xsl:apply templates to parent, where was predtym removedNode
	QDomNode xslApplyTemplate=FoCreator::GetInstancePtr()->xslApplyTemplates(xmlPath);
	parentNode.appendChild(xslApplyTemplate);
}
