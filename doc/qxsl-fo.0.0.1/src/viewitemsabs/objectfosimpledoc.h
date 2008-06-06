//
// C++ Interface: foobjectsimple
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FOOBJECTSIMPLEDOC_H
#define FOOBJECTSIMPLEDOC_H

#include <QRect>

#include "absobjectfosimpledoc.h"
#include "objectfodoc.h"
#include "attributefo.h"
#include "nodefo.h"

class QDomElement;
class QDomNode;
class QVariant;

class TextContentDoc;
class ObjectFoDoc;
class ObjectFoContainerDoc;

/**
 * simple fo object which contain definition only in page-sequence part in fo-document and isn't container
 */
class ObjectFoSimpleDoc : public ObjectFoDoc, public AbsObjectFoSimpleDoc
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param type 
	 * @param pElement 
	 * @param parent 
	 */
	ObjectFoSimpleDoc(ObjectFoDoc::FoObjectType type,ObjectFoContainerDoc *parentContainer,QDomElement pElement);
	/**
	 * destructor
	 */
	~ObjectFoSimpleDoc();
	/**
	 * created for convenience
	 * @param  
	 */
	virtual void move(QPoint /*newPos*/){;}
	/**
	 * return doc part of formating object
	 * @return 
	 */
	ObjectFoDoc *parentContainer(){	return (ObjectFoDoc *)m_pParentContainer;}
public slots:
	/**
	 * slot which handle changes in proprties dock window 
	 * currently unused
	 * @param attributeType 
	 * @param value 
	 */
	void slDataChangedInPropertiesDoc(AttributeFo::EAttributeType attributeType,QVariant value);
signals:
	/**
	 * emit signal to view to change geometry
	 * @param rect 
	 * @param setUpObjectHandler 
	 */
	void sigSetRect(QRect rect,bool setUpObjectHandler);
protected:
	/**
	 * contain reference to parent container
	 */
	ObjectFoContainerDoc *m_pParentContainer;
};

#endif
