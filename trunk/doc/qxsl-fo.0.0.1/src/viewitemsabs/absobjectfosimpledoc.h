#ifndef ABSOBJECTFOSIMPLEDOC_H
#define ABSOBJECTFOSIMPLEDOC_H

#include "attributefo.h"
#include "nodefo.h"

class QDomElement;
class QDomNode;
class AttributeFoData;
class ObjectFoContainerDoc;

/**
 * represent abstract doc part of formating  object which has definition inly in page-sequence part of fo-document
 */

class AbsObjectFoSimpleDoc
{
public:
	/**
	 * constructor
	 * @param pElement 
	 */
	AbsObjectFoSimpleDoc(QDomElement pElement);
	/**
	 * destructor
	 */
	~AbsObjectFoSimpleDoc();
	/**
	 * add attribute to formating object
	 * @param attribute 
	 */
	void addAttribute(AttributeFo *attribute);
	/**
	 * insert child element after element refElement
	 * @param newChildElement 
	 * @param refElement 
	 * @return 
	 */
	QDomNode insertChildElementAfter(QDomElement newChildElement,QDomElement refElement);
	/**
	 * return attribute from vector of attributes specified by position in vector
	 * @param attrPos 
	 * @return 
	 */
	AttributeFo *attributeByPosition(int attrPos){	return m_pNodeFo->attributeByPosition(attrPos);}
	/**
	 * return attribute from vector of attributes specified by type
	 * @param type 
	 * @return 
	 */
	AttributeFoData *attributeValueByType(AttributeFo::EAttributeType type){	return m_pNodeFo->attributeValueByType(type);}
	/**
	 * return count of attributes
	 * @return 
	 */
	int countOfAttributes(){	return m_pNodeFo->countOfAttributes();}
	/**
	 * return reference to encapsulated fo:node
	 * @return 
	 */
	NodeFo *getNodeFo(){	return m_pNodeFo;}
	/**
	 * set attrbite which is type of attributeName to value
	 * @param attributeName 
	 * @param value 
	 */
	void setAttributeValue( AttributeFo::EAttributeType attributeName, QString value );
	/**
	 * overwritted from foobjectvievabledoc - currently not used
	 * @param xmlPath 
	 */
	void setXslTemplate(QString xmlPath);
private:
	/**
	*	data part of foblock, it handle FoAttributes
	*/
	NodeFo *m_pNodeFo;
};

#endif
