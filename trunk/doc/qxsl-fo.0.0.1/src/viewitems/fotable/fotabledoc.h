#ifndef FOTABLEDOC_H
#define FOTABLEDOC_H

#include "objectfosimpledoc.h"
#include "fotablebodydoc.h"

#include <QPoint>

class QDomElement;

class FoTableRow;

/**
 * doc part of formating object fo:table
 */
class FoTableDoc : public ObjectFoContainerSimpleDoc
{
public:
	/**
	 * constructor
	 * @param pElement 
	 * @param parent 
	 */
	FoTableDoc(QDomElement pElement,ObjectFoContainerDoc *parent);
	/**
	 * destructor
	 */
	~FoTableDoc();
	/**
	 * find element with tag name "fo:table-body" in pElement and create instance of FoTableBodyDoc
	 * @param pElement 
	 */
	void loadTableBody(QDomElement pElement);
	/**
	 * return count of rows
	 * @return 
	 */
	int rowCount(){	return m_pFoTableBodyDoc->rowCount();}
	/**
	 * return row on i-position
	 * @param i 
	 * @return 
	 */
	FoTableRowDoc *row(int i);
	/**
	 * append row to body
	 * @return 
	 */
	FoTableRowDoc* appendRow();
	/**
	 * append column to body
	 */
	void appendColumn();
	/**
	 * return doc part of table
	 * @return 
	 */
	FoTableBodyDoc *foTableBodyDoc(){	return m_pFoTableBodyDoc;}
	/**
	 * return width, it depend on parent width
	 * @return 
	 */
	FoSize widthValue();
	/**
	 * return height, it depend on hieght of all rows
	 * @return 
	 */
	FoSize height();
private:
	/**
	 * reference to child fotablebody
	 */
	FoTableBodyDoc *m_pFoTableBodyDoc;
};

#endif

