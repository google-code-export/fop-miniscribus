#ifndef FOTABLEROWVIEW_H
#define FOTABLEROWVIEW_H

#include "objectfocontainersimpleview.h"

class QGraphicsItem;
class FoObjectSimple;
class ObjectFoDoc;
class MainView;
class FoTableRowDoc;
class FoTableView;
class FoTableCellView;

/**
 * represent view part of formating object fo:table-row
 */
class FoTableRowView : public ObjectFoContainerSimpleView
{
public:
	/**
	 * constructor
	 * @param parent 
	 * @param doc 
	 * @param pMainView 
	 */
	FoTableRowView(FoTableView *parent,ObjectFoContainerDoc *doc,MainView *pMainView);
	/**
	 * destructor
	 */
	~FoTableRowView();
	/**
	 * return reference to doc part of table
	 * @return 
	 */
	FoTableRowDoc *foTableRowDoc(){	return (FoTableRowDoc *)ObjectFoView::doc();}
	/**
	 * calculate of row position in table
	 * @return 
	 */
	QPoint calculateTopLeftPosition();
	/**
	 * create CellView's which belong to current row
	 */
	void loadDataToRowView();
	/**
	 * return reference to parent table
	 * @return 
	 */
	FoTableView *parentTableView(){	return m_pParentTableView;}
	/**
	 * append cell by cloning of last cell
	 */
	void appendCell();
private:
	/**
	 * reference to parent table
	 */
	FoTableView *m_pParentTableView;
	/**
	 * vector of cells in curent row
	 */
	std::vector<FoTableCellView*>m_pFoTableCellView;
};

#endif
