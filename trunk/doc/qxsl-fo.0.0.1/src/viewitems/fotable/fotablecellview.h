#ifndef FOTABLECELLVIEW_H
#define FOTABLECELLVIEW_H

#include "objectfocontainersimpleview.h"

class QGraphicsItem;
class FoObjectSimple;
class ObjectFoDoc;
class FoTableCellDoc;

class FoTableView;
class FoTableRowView;
class MainView;

/**
 * view part of formating object fo:table-cell
 */
class FoTableCellView : public ObjectFoContainerSimpleView
{
public:
	/**
	 * constructor
	 * @param pRowParent 
	 * @param doc 
	 * @param pMainView 
	 */
	FoTableCellView(FoTableRowView *pRowParent,ObjectFoContainerDoc *doc,MainView *pMainView);
	/**
	 * destructor
	 */
	~FoTableCellView();
	/**
	 * return document part of table cell view
	 * @return 
	 */
	FoTableCellDoc *foTableCellDoc(){	return (FoTableCellDoc *)ObjectFoView::doc();}
	/**
	 * called by Qt framework, currently not used 
	 * @param mouseEvent 
	 */
	void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	/**
	 * show menu, currently not used
	 * @param mouseEvent 
	 */
	void showCellMenu(QGraphicsSceneMouseEvent * mouseEvent);
	/**
	 * return position of cell in table
	 * @return 
	 */
	QPoint calculatePosition();
	/**
	 * add row, not fully implemented yet
	 */
	void addRow();
	/**
	 * add column, not fully implemented yet
	 */
	void addColumn();
	/**
	 * not implemented yet
	 */
	void resizeAfterAppendCell();
private:
	/**
	 * reference to parent table row
	 */	
	FoTableRowView *m_pTableRowView;
};

#endif
