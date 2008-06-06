#include "nodefo.h"

#include "attributefo.h"
#include "attributefodata.h"

NodeFo::NodeFo(QDomElement data)
 : m_qDomElement(data)
{
}

NodeFo::~NodeFo()
{
}

AttributeFo *NodeFo::attributeByType(AttributeFo::EAttributeType type)
{
	for(unsigned int pos=0;pos<m_vecAttributes.size();pos++)
	{
		if (m_vecAttributes.at(pos)->type()==type)
			return m_vecAttributes.at(pos);
	}
	return NULL;
}

AttributeFoData *NodeFo::attributeValueByType(AttributeFo::EAttributeType type)
{
	AttributeFo *attr = attributeByType(type);
	
	if (attr==NULL)
		qFatal("Can't find attribute id:%d in fodataobject",type);
	
	QString strAttrData = m_qDomElement.attribute(attr->name());
	
	return new AttributeFoData(strAttrData);
}

AttributeFo *NodeFo::attributeByPosition(int attrPos)
{
	return m_vecAttributes.at(attrPos);
}

int NodeFo::countOfAttributes()
{
	return m_vecAttributes.size();
}

bool NodeFo::appendChildElement(QDomElement element)
{
	m_qDomElement.appendChild(element);
	return true;
}

QDomNode NodeFo::insertChildElementAfter(QDomElement newChildElement,QDomElement refElement)
{
	QDomNode ret=m_qDomElement.insertAfter(newChildElement,refElement);
	return ret;
}

void NodeFo::appendAttribute(AttributeFo *pAttribute)
{
	m_vecAttributes.push_back(pAttribute);
}

QDomElement NodeFo::firstChildElement(QString elementName)
{
	return m_qDomElement.firstChildElement(elementName);
}

QDomElement NodeFo::firstElementByAttributeValue(QString attributeName,QString attributeValue)
{

	for(QDomNode node = m_qDomElement.firstChild();!node.isNull(); node = node.nextSibling())
	{
		QDomElement e = node.toElement(); // try to convert the node to an element.
		if(!e.isNull())
		{
			//check if desired attribute has desired value	
			if (e.attribute(attributeName)==attributeValue)
				return e;
		}
	}
	
	qFatal("I havent found specified attribute with speciffied value");
	return QDomElement();
}

void NodeFo::setAttributeValue( AttributeFo::EAttributeType attributeName, QString & value )
{
	QString strName=AttributeFo::typeToString(attributeName);
	m_qDomElement.setAttribute(strName,value);
}

QDomNode NodeFo::xslStyleSheetRoot()
{
	QDomElement parentElement;
	//trace to find xslStyleSheet
	for(QDomNode node = m_qDomElement;!node.isNull();node=node.parentNode())
	{
		parentElement = node.toElement();
		if(parentElement.isNull())
			qFatal("can't convert QDomNode to QDomElement while tracing to xslStyleSheet root");
		qDebug(parentElement.tagName().toAscii());
		if(parentElement.tagName()=="xsl:stylesheet")
			return node;
	}
	
	qFatal("can't find xslStyleSheet root parent");
	return QDomNode();
}
