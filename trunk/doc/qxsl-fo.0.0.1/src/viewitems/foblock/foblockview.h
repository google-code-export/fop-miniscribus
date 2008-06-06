#ifndef FOBLOCKVIEW_H
#define FOBLOCKVIEW_H

#include <vector>

#include "objectfocontainersimpleview.h"

class QGraphicsItemGroup;
class ObjectFoDoc;
class ObjectFoContainerView;
class FoBlockDoc;
class TextContentDoc;
class FoExternalGraphicDoc;
class RowInBlockView;

/**
 * definition of view part of formating object fo:block, it could contain another formating object, byt 
 * like pictures, tables or fo:block's and could contain also text (rpresented by TextContnnt), all these block are formated to
 * rows, each row is represented by instance of RowInBlockView and blocks are insert to it
 */
class FoBlockView : public ObjectFoContainerSimpleView
{
Q_OBJECT
public:
	/**
	 * constuctor
	 * @param parentContainer 
	 * @param  
	 * @param pMainView 
	 */
	FoBlockView(ObjectFoContainerView * parentContainer,FoBlockDoc *,MainView *pMainView);
	/**
	 * destructor
	 */
	~FoBlockView();
private:
	/**
	 * go throught the foblockdoc and create appropriate block views
	 * @param doc 
	 */
	void loadChildrenToView(ObjectFoContainerDoc *doc);
	/**
	 * append text content to the last row in foblock if isn't place in row, it create new row
	 * @param pTextDoc 
	 */
	void appendTextContent(TextContentDoc *pTextDoc);
	/**
	 * append formating object foexternal (e.g. picture) to last row of current foblock
	 * @param pGraphicDoc 
	 */
	void appendFoExternalGraphic(FoExternalGraphicDoc *pGraphicDoc);
	/**
	 * return last added row
	 * @return 
	 */
	RowInBlockView *lastRow(){	return m_rowsInBlock.at(m_rowsInBlock.size()-1);}
	/**
	 * create and append new row
	 * @return 
	 */
	RowInBlockView * createAndAppendNewRow();
	/**
	 * return count of rows
	 * @return 
	 */
	int countOfRows(){ return m_rowsInBlock.size();}	
	/**
	 * return height of foblock, depent on sum of foblock
	 * @return 
	 */
	int heightOfBlockView();
private:
	/**
	 * vector of rows in foblock
	 */
	std::vector<RowInBlockView *>m_rowsInBlock;
};

#endif
