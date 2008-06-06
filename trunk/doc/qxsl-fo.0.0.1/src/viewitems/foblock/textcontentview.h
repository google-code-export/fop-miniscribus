#ifndef TEXTCONTENTVIEW_H
#define TEXTCONTENTVIEW_H

#include <QObject>
#include <QRectF>
#include <QGraphicsTextItem>

#include "attributefo.h"

class QString;
class QGraphicsRectItem;
class QGraphicsSceneMouseEvent;

class TextContentDoc;
class MainView;
class RowInBlockView;

/**
 * object which hadle text view for foblockView 
 * contain reference to TextContentDoc of whole text block, but it's possible that TextContentView show only part of them
 */
class TextContentView: public QGraphicsTextItem
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param pTextContent 
	 * @param pMainView 
	 * @param parentRow 
	 * @param startPosInText 
	 */
	TextContentView(TextContentDoc *pTextContent,MainView *pMainView,RowInBlockView * parentRow,int startPosInText=0);
	/**
	 * destructor
	 */
	~TextContentView();
	/**
	 *	this method go char by char and in each iteration append one char to
	 *	qgraphicstext, in loop we compare width of qgraphicstext with doc with which is calculated 
	 *	according to parent
	 * @param maxWidth 
	 * @return true if fill was ok, false if we havn't place to remain text and we need new row
	 */
	bool fillViewFromDocText(int maxWidth);
	/**
	 * 
	 * @param event 
	 */
	void mousePressEvent ( QGraphicsSceneMouseEvent * event );	
	/**
	 * show view and connect it with doc (contentsChange signal)
	 */
	void showAndConnectWithDoc();	
	/**
	 * return bounding rect scaled with constTextScale constant
	 * @return 
	 */
	QRectF boundingRect ();
	/**
	 * return count of chars
	 * @return 
	 */
	int countOfChars();
public slots:
	/**
	 * sl connected with doc, it's called when contents of doc part has changes
	 * @param position 
	 * @param charsRemoved 
	 * @param charsAdded 
	 */
	void slContentsChange ( int position, int charsRemoved, int charsAdded );
private:
	/**
	 * text content of whole text block but textcontent view could show only part of them
	 */
	TextContentDoc *m_pTextContentDoc;
	/**
	 * start position of TextContnentDoc form which is represented by this view
	 */
	int m_startPositionInText;
	/**
	 * reference to MainView
	 */
	MainView *m_pMainView;
	/**
	 * reference to parentrow
	 */
	RowInBlockView *m_pParentRow;
	/**
	 * scale value to scale text, becouse font size was too big
	 */
	static const double constTextScale=0.287;
};

#endif
