#ifndef FOEXTERNAL_H
#define FOEXTERNAL_H

#include <objectfosimpledoc.h>

class QDomElement;

class ObjectFoDoc;

/**
 * represent doc part of formating object fo:external-graphic which represent pictures in fo-document
 */
class FoExternalGraphicDoc : public ObjectFoSimpleDoc
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param domElement 
	 * @param parent 
	 */
	FoExternalGraphicDoc(QDomElement domElement,ObjectFoContainerDoc *parent);
	/**
	 * destructor
	 */
	~FoExternalGraphicDoc();
	/**
	 * return QString path to file of picture which is displayed 
	 * @return 
	 */
	QString picturePath();
	/**
	 * return width of object - it depend on content-width fo attribute
	 * @return 
	 */
	int contentWidth();
	/**
	 * return content width of picture - ( implemented for convenience )
	 * @return 
	 */
	FoSize widthValue();
};

#endif
