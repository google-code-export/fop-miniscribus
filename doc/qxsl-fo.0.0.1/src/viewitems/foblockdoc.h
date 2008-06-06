//
// C++ Interface: foblockdoc
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

///todo:think out new name of the class

#ifndef FOBLOCKDOC_H
#define FOBLOCKDOC_H

#include "objectfosimpledoc.h"

#include <QPoint>
#include <vector>

#include "textcontentdoc.h"

class QDomElement;

class PageDoc;
class BodyRegionDoc;

/**
 *	class which represent fo:block from fo language
 */

class FoBlockDoc : public ObjectFoSimpleDoc
{	
public:
	FoBlockDoc(QDomElement domElement,ObjectFoDoc *parent);
	~FoBlockDoc();
private:
	//rich text blocks
	std::vector<ObjectFoDoc *>m_blocks;
};

#endif
