#ifndef ROWINBLOCKVIEW_H
#define ROWINBLOCKVIEW_H

#include <QGraphicsItemGroup>

class FoBlockView;
class TextContentView;
class QGraphicsItemGroup;
class FoExternalGraphicView;
class TextContentDoc;

/**
 * represent row in FoBlockView object, class handle block insert
 */
class RowInBlockView : public QGraphicsItemGroup
{
public:
	/**
	 * constructor
	 * @param foBlockView 
	 */
	RowInBlockView(FoBlockView *foBlockView);
	/**
	 * destructor
	 */
	~RowInBlockView();
	/**
	 * add FoExternalGraphic (e.g. picture) to row
	 * @param pGraphicView 
	 * @return 
	 */
	bool addGraphicToRow(FoExternalGraphicView *pGraphicView);
	/**
	 * add text to row
	 * @param pTextContentDoc 
	 * @param offsetOfUnProcessedText insert text from this offset
	 * @return 
	 */
	int addTextToRow(TextContentDoc *pTextContentDoc,int offsetOfUnProcessedText);
	/**
	 * return width of row, depend on parent foblock
	 * @return 
	 */
	int rowWidth();
	/**
	 *  return width of row which is empty (part which is free for insert)
	 * @return 
	 */
	int remainWidthOfRow();
private:
	/**
	 * represent width of filled part of row
	 */
	int m_currentlyFilledRowWidth;
	/**
	 * reference to parent FoBlockView
	 */
	FoBlockView *m_pFoBlockView;
	
};

#endif
