#ifndef OBJECTFOSEQUENCEDDOC_H
#define OBJECTFOSEQUENCEDDOC_H

#include "objectfodoc.h"
#include "objectfocontainerdoc.h"
#include "attributefo.h"



class QDomElement;

class NodeFo;
class ObjectFoSimpleDoc;


/**
 * base class for all formating objects which have definition in page-seauence node and in layout-master-set node,
 * these object are containers for simple formating objects
 * 
 */

class ObjectFoSequencedDoc : public ObjectFoContainerDoc
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param type 
	 * @param elFoPgSeqPart 
	 * @param elFoSimplePgMastPart 
	 * @param parent 
	 */
	ObjectFoSequencedDoc(ObjectFoDoc::FoObjectType type,QDomElement elFoPgSeqPart,QDomElement elFoSimplePgMastPart,ObjectFoContainerDoc *parent);
	/**
	 * destructor
	 */
	~ObjectFoSequencedDoc();
	/**
	 * add atributes to simple-page-master part of fo-document
	 * @param pAttribute 
	 */
	void appendAtributeToFoSimplePageMasterPart(AttributeFo *pAttribute);
	/**
	 * add element to simple-page-master part of fo-document
	 * @param attr 
	 */
	void appendElementToFoSimplePageMasterPart(QDomElement element);
	/**
	 * add attribute to page-sequence part of fo-document
	 * @param pAttribute 
	 */
	void appendAtributeToPageSequencePart(AttributeFo *pAttribute);
	/**
	 * add element to page-sequence part of fo-document
	 * @param attr 
	 */
	void appendElementToPageSequencePart(QDomElement element);
	/**
	 * return reference to simple-page-master node in fo-document
	 * @return 
	 */
	NodeFo *getFoSimplePageMasterPart(){	return m_pFoSimplePageMasterPart;}
	/**
	 * return reference to page-sequence node in fo-document
	 * @return 
	 */
	NodeFo *getFoPageSequencePart(){	return m_pFoPageSequencePart;}
	/**
	 * currently unused
	 * @return 
	 */
	void setXslTemplate(QString xmlPath);
	/**
	 * create appropriate chilren docs like foblock foexternal-graphics etc ...
	 * @param elFoPgSeqPart 
	 */
	void loadChildrenToDoc();
public slots:	
	/**
	 * currently unused
	 * @param attributeType 
	 * @param value 
	 */
	void slDataChangedInPropertiesDoc(AttributeFo::EAttributeType attributeType,QVariant value);
private:
	/**
	*	data of FoObject which are in FoLayouMasterSet
	*/
	NodeFo *m_pFoSimplePageMasterPart;
	/**
	*	data of FoObject which are in FoPageSequence
	*/
	NodeFo *m_pFoPageSequencePart;
};

#endif
