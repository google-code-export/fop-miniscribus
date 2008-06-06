#ifndef FOTABLECELLDOC_H
#define FOTABLECELLDOC_H

#include "objectfocontainersimpledoc.h"

class QDomElement;
class ObjectFoDoc;
class FoTableRowDoc;

/**
 * represent formating object fo:table-cell
 */
class FoTableCellDoc : public ObjectFoContainerSimpleDoc
{
public:	
	/**
	 * constructor
	 * @param cellNum 
	 * @param pElement 
	 * @param parentRowDoc 
	 */
	FoTableCellDoc(int cellNum,QDomElement pElement,FoTableRowDoc *parentRowDoc);
	/**
	 * destructor
	 */
	~FoTableCellDoc();
	/**
	 * return width value it's computed from parent's width and count of sibling cell
	 * @return 
	 */
	FoSize widthValue();
	/**
	 * return height of cell it's represented by height of all children
	 * @return 
	 */
	FoSize height();	
	/**
	 * return cellNumber position
	 * @return 
	 */
	int cellNumber(){	return m_cellNumber;}
	/**
	 * set cell number position
	 * @param cellNum 
	 */
	void setCellNumber(int cellNum){	m_cellNumber=cellNum;}
	/**
	 * currently unused 
	 * @param xmlPath 
	 */
	void setXslTemplate(QString xmlPath);
private:
	/**
	 * number which inform about cell position in row
	 */
	int m_cellNumber;
	/**
	 * hold parent row
	 */
	FoTableRowDoc *m_pParentRowDoc;
};

#endif

///todo: m_celNumber could be replaced with id member from objectfo class
