#ifndef OBJECTFOHANDLER_H
#define OBJECTFOHANDLER_H

#include <QGraphicsRectItem>
#include <QObject>
#include "objectfoview.h"

class QGraphicsItemGroup;
class ObjectFoView;

/**
 * this class represent small top right rect which provide state handle of it's parent formating object
 * it represent state of FO by color, clicking on it, we can change this state
 */
class ObjectFoHandler : public QObject,public QGraphicsRectItem
{

public:
	/**
	 * constructor
	 * @param parent 
	 */
	ObjectFoHandler(ObjectFoView * parent=NULL);
	/**
	 * destructor
	 */
	~ObjectFoHandler();
	/**
	 * this methos is called by qt framework and it change state of his parent and his own state
	 * @param event 
	 */
	void mousePressEvent ( QGraphicsSceneMouseEvent *event );
	/**
	 * 
	 * @param parent 
	 */
	void setParentObject(ObjectFoView * parent);
	/**
	 * 
	 * @return 
	 */
	ObjectFoView *currentParent(){	return m_parent;}
private:
	/**
	 * width of rect handler
	 */
	enum {EHandlerWidth=4};
	/**
	 * set color of rect, color is defined by state
	 * @param state which represent color
	 */
	void setColorState(ObjectFoView::EObjectFoState state);
	/**
	 * set concere color of rect which represent lock state
	 */
	void setLockColor();
	/**
	 * set concrete color of rect which represent unlock state
	 */
	void setUnlockColor();
private:
	/**
	 * parent formating object which is handled with this class
	 */
	ObjectFoView *m_parent;
};

#endif
