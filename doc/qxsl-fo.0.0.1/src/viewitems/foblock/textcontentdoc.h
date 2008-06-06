#ifndef TEXTCONTENT_H
#define TEXTCONTENT_H

#include "objectfodoc.h"

class ObjectFoSimpleDoc;
class ObjectFoContainerSimpleDoc;
class QTextDocument;
class ObjectFoContainerDoc;

/**
 * Class which represent text object (it haven't fo tag) in foblock
 */
class TextContentDoc : public ObjectFoDoc
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param pParentFoObject 
	 * @param text 
	 */
	TextContentDoc(ObjectFoContainerSimpleDoc *pParentFoObject,QString text);
	/**
	 * destructor
	 */
	~TextContentDoc();
	/**
	 * return contained string
	 * @return 
	 */
	QString text(){return m_textDoc;}
	/**
	 * return parent container
	 * @return 
	 */
	ObjectFoDoc *parentContainer(){	return (ObjectFoDoc *)m_pParentContainer;}
	/**
	 * return lenght of contained text
	 * @return 
	 */
	int size(){	return m_textDoc.simplified().trimmed().size();}
	/**
	 * replace block of text 
	 * @param startPosition 
	 * @param charsRemoved 
	 * @param blockOfText 
	 */
	void replaceBlockOfText(int startPosition,int charsRemoved,QString blockOfText);
private:
	/**
	 * reffers to parent container
	 */
	ObjectFoContainerDoc *m_pParentContainer;
	/**
	 * contain represented string
	 */
	QString m_textDoc;
};

#endif
