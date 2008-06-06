#ifndef FOTABLEROWDOC_H
#define FOTABLEROWDOC_H

class QDomElement;

class FoTableCellDoc;
class ObjectFoContainerDoc;

#include "objectfocontainersimpledoc.h"

/**
 * represent doc part of formating object fo:table-row
 */
class FoTableRowDoc : public ObjectFoContainerSimpleDoc
{
public:
	/**
	 * construtor
	 * @param rowNumber 
	 * @param pElement 
	 * @param parent 
	 */
	FoTableRowDoc(int rowNumber,QDomElement pElement,ObjectFoContainerDoc *parent);
	/**
	 * destructor
	 */
	~FoTableRowDoc();
	/**
	 * it goes throught the pElement and is looking for elements with "fo:table-cell" tagName
	 * for each fo:table-cell create instance of FoTableCell
	 * @param pElement 
	 */
	void loadTableCells(QDomElement pElement);
	/**
	 * return count of cells in current row
	 * @return 
	 */
	int cellCount();
	/**
	 * return cell from i-position
	 * @param i 
	 * @return 
	 */
	FoTableCellDoc *cell(int i){	return m_pFoTableCellDoc[i];}
	/**
	 *  append cell to row
	 * @return 
	 */
	FoTableCellDoc *appendCell();
	/**
	 * return row number
	 * @return 
	 */
	int rowNumber(){	return m_rowNumber;}
	/**
	 * set row number
	 * @param rowNum 
	 */
	void setRowNumber(int rowNum){	m_rowNumber=rowNum;}
	/**
	 * return width, which depend on parent width
	 * @return 
	 */
	FoSize widthValue();
	/**
	 * return height, it depend on highest cell
	 * @return 
	 */
	FoSize height();
private:
	/**
	 * vecotr of cells in current row
	 */
	std::vector<FoTableCellDoc*>m_pFoTableCellDoc;
	/**
	 * row position number in table
	 */
	int m_rowNumber;
};

#endif
