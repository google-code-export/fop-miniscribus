#ifndef PAGERULER_H
#define PAGERULER_H

#include <QWidget>
#include <QGraphicsRectItem>

class PageView;
class QLabel;
class MainView;

/**
 * ruler - on top or on left,is activated after selecting foobject 
 * and inform about borders of selected formating object
 */
class PageRuler : public QWidget
{
Q_OBJECT
public:
	/**
	 * define ruler position
	 */
	enum ERulerType
	{
		topPostion,leftPosition
	};

	/**
	 * construtor
	 * @param position could be top or left
	 * @param pMainView parent mainview
	 */
	PageRuler(ERulerType position,MainView *pMainView);
	/**
	 * destructor
	 */
	~PageRuler();
	/**
	 * place cursors according to pQGraphicsRect size
	 * @param pQGraphicsRect 
	 */
	void setRulerPosition(QGraphicsRectItem *pQGraphicsRect);
private:
	/**
	 * construt ruler cursors
	 */
	void createCursors();
private:
	/**
	 * parent mainview
	 */	
	MainView *m_pMainView;
	/**
	 * hold type of ruler
	 */	
	ERulerType m_position;
	/**
	 * four cursors which represent borders of selected formating object
	 */	
	QLabel *m_pCursor[4];
};

#endif
