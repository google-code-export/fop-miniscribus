#ifndef FOTABLEBODYDOC_H
#define FOTABLEBODYDOC_H

#include <vector>

class QDomElement;

#include "objectfosimpledoc.h"
#include "fotablerowdoc.h"

/**
 *	represent formating object fo:table-body, contain vector of rows (fo:table-rows)
 */
class FoTableBodyDoc : public ObjectFoContainerSimpleDoc
{
public:
	/**
	 * constructor
	 * @param pElement 
	 * @param parent 
	 */
	FoTableBodyDoc(QDomElement pElement,ObjectFoContainerDoc *parent);
	/**
	 * destructor
	 */
	~FoTableBodyDoc();
	/**
	 * go throught tje pElement, search elements fo:table-row and creates appropriate instances
	 * @param pElement 
	 */
	void loadTableRows(QDomElement pElement);
	/**
	 * return row on i position in vector
	 * @param i 
	 * @return 
	 */
	FoTableRowDoc *row(unsigned int i);
	/**
	 * return count of rows
	 * @return 
	 */
	unsigned int rowCount(){	return m_pFoTableRowDoc.size();}
	/**
	 * this method create new row doc and append it to the vector, 
	 * new row is cloned from last row in table
	 * @return 
	 */
	FoTableRowDoc *appendRow();
	/**
	 * append column to the table
	 */
	void appendColumn();
	/**
	 * return width of tablebody, which is calculated from its parent fo:table
	 * @return 
	 */
	FoSize widthValue();
private:
	/**
	 * vector of rows
	 */
	std::vector<FoTableRowDoc*>m_pFoTableRowDoc;
};

#endif

///todo:fix append methods and destructor
