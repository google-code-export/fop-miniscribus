//own include
#include "objectfosequenceddoc.h"

//qt include
#include <QDomElement>
#include <QVariant>

//project includes
#include "nodefo.h"
#include "attributefo.h"
#include "objectfodoc.h"
#include "objectfosimpledoc.h"

#include "foblockdoc.h"
#include "foblockcontainerdoc.h"
#include "objectfocontainerdoc.h"

ObjectFoSequencedDoc::ObjectFoSequencedDoc(ObjectFoDoc::FoObjectType type,QDomElement elFoPgSeqPart,QDomElement elFoSimplePgMastPart,ObjectFoContainerDoc *parentContainer)
 : ObjectFoContainerDoc(type,parentContainer)
{
	m_pFoSimplePageMasterPart = new NodeFo(elFoSimplePgMastPart);
	m_pFoPageSequencePart = new NodeFo(elFoPgSeqPart);
}

ObjectFoSequencedDoc::~ObjectFoSequencedDoc()
{
	
}

void ObjectFoSequencedDoc::loadChildrenToDoc()
{
	ObjectFoContainerDoc::loadElementChildrenToContainer(m_pFoPageSequencePart->elementData());
}

void ObjectFoSequencedDoc::appendAtributeToFoSimplePageMasterPart(AttributeFo *pAttribute)
{
	m_pFoSimplePageMasterPart->appendAttribute(pAttribute);
}

void ObjectFoSequencedDoc::appendElementToFoSimplePageMasterPart(QDomElement element)
{
	m_pFoSimplePageMasterPart->appendChildElement(element);
}

void ObjectFoSequencedDoc::appendAtributeToPageSequencePart(AttributeFo *pAttribute)
{
	m_pFoPageSequencePart->appendAttribute(pAttribute);
}

void ObjectFoSequencedDoc::appendElementToPageSequencePart(QDomElement element)
{
	m_pFoPageSequencePart->appendChildElement(element);
}

void ObjectFoSequencedDoc::slDataChangedInPropertiesDoc(AttributeFo::EAttributeType /*attributeType*/,QVariant /*value*/)
{
	qDebug("void ObjectFoSequencedDoc::slDataChangedInPropertiesDoc(AttributeFo::EAttributeType /*attributeType*/,QVariant /*value*/) not implemented yet");
}

void ObjectFoSequencedDoc::setXslTemplate(QString /*xmlPath*/)
{
	qDebug("void ObjectFoSequencedDoc::setXslTemplate(QString /*xmlPath*/) not implemented yet");
}
