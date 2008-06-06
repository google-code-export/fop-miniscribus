#ifndef NODEFO_H
#define NODEFO_H

#include <QDomElement>

#include <vector>
#include "attributefo.h"

class QDomNode;

/**
 *	conaint vector of attributes,method which provide attributte operations and also contain QDomElement data
 */
class NodeFo
{

public:
	/**
	 * constructor
	 * @param data which are encapsulated with curent NodeFo
	 */
	NodeFo(QDomElement data);
	/**
	 * destructor
	 */
	~NodeFo();
	/**
	 * go through all defined attrbutes and return attribute specified by id
	 * @param attrId id of the attribute (use enum from FoAttribute)
	 * @return return attribute
	 */
	AttributeFo *attributeByType(AttributeFo::EAttributeType type);
	/**
	 * return attribute which is on attrPos position im vector m_vecAttributes
	 * @param attrPos index of attribute
	 * @return return attribute
	 */
	AttributeFo *attributeByPosition(int attrPos);
	/**
	 * return count of defined attributes
	 * @return count of attributes
	 */
	int countOfAttributes();
	/**
	 * return attribute data from attribute which has type type
	 * @param type of attribute which contain desired data
	 * @return 
	 */
	AttributeFoData *attributeValueByType(AttributeFo::EAttributeType type);
	/**
	 * add child element to current node
	 * @param element 
	 * @return 
	 */
	bool appendChildElement(QDomElement element);
	/**
	 * add child element newChildElement after refElement
	 * @param newChildElement 
	 * @param refElement 
	 * @return 
	 */
	QDomNode insertChildElementAfter(QDomElement newChildElement,QDomElement refElement);
	/**
	 * append foattribute to the vector of attributes, it means: append attribute to foobject
	 * @param pAttribute pointer to the foattribute
	 * @return 
	 */
	void appendAttribute(AttributeFo *pAttribute);
	/**
	 * return first element in element elementName
	 * @param elementName 
	 * @return 
	 */
	QDomElement firstChildElement(QString elementName);
	/**
	 * return first element with specified attribute name and appropriate value
	 * @param attributeName 
	 * @param attributeValue
	 * @return 
	 */
	QDomElement firstElementByAttributeValue(QString attributeName,QString attributeValue);
	/**
	 * return whole data which is type of QDomElement
	 * @return 
	 */
	QDomElement elementData() {	return m_qDomElement;}
	/**
	 * swt value for attribute with name attributeName
	 * @param attributeName 
	 * @param value 
	 */
	void setAttributeValue( AttributeFo::EAttributeType attributeName, QString & value );
	/**
	 * return parent element with tag name : "xsl:stylesheet"
	 * @return xsl:stylesheet element
	 */
	QDomNode xslStyleSheetRoot();
private:
	/**
	 *	vector of defined atributes.
	 */
	std::vector<AttributeFo *> m_vecAttributes;
	/**
	 *	m_data contain reference to QDomElement in QDomattributeByPosition
	 */
	QDomElement m_qDomElement;
};

#endif

