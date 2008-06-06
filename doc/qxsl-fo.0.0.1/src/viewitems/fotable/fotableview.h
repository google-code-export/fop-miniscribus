#ifndef FOTABLEVIEW_H
#define FOTABLEVIEW_H

#include "objectfocontainersimpleview.h"

class QGraphicsItem;
class FoTableDoc;
class ObjectFoSimpleDoc;
class ObjectFoDoc;
class MainView;
class FoTableRowView;
class ObjectFoContainerView;

/**
 * represent view of fo:table formating object	
 */
class FoTableView : public ObjectFoContainerSimpleView
{
Q_OBJECT
public: 
	/**
 	 * represent type of resizing (add,dell,row,col) of table
 	 */	
	enum ETableResizeCmd	{	addRow,	addCol,	delRow,delCol	};
public:	
	/**
	 * constructor
	 * @param parentContainer 
	 * @param doc 
	 * @param pMainView 
	 */
	FoTableView(ObjectFoContainerView *parentContainer,ObjectFoContainerDoc *doc,MainView *pMainView);
	/**
	 * destructor
	 */
	~FoTableView();
	/**
	 * return reference to doc part of table
	 * @return 
	 */
	FoTableDoc *foTableDoc(){	return (FoTableDoc *)ObjectFoView::doc();}
	/**
	 * append column at the end 
	 */
	void appendColumn();
	/**
 	 * append row at the end 
 	 */	
	void appendRow();
	/**
 	 * calculate position of row on rowNum position
 	 */	
	QPoint calculatePositionForRow(unsigned int rowNum);
private:
	/**
 	 * create FoTableRowView instances for current TableView
 	 */	
	void loadDataToTableView();
	/**
 	 * vector of rows
 	 */	
	std::vector<FoTableRowView*>m_pFoTableRowView;
};

#endif

