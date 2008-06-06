#ifndef RESIZEHANDLEWINDOW_H
#define RESIZEHANDLEWINDOW_H

#include <QGraphicsRectItem>

class ObjectFoView;

/**
 *	little small rect window on rightbottom side which provide resize of formating object using drag&drop
 */

class ResizeHandleWindow : public QObject,public QGraphicsRectItem
{
Q_OBJECT
public:
	/**
	 * width of rect
	 */
	enum {EResizerWidth=4};
	/**
	 * constructor
	 * @param parent 
	 */
	ResizeHandleWindow(ObjectFoView * parent);
	/**
	 * destructor
	 */
	~ResizeHandleWindow();
	/**
	 * this method call qt framework and we use it to start drag
	 * @param event 
	 */
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	/**
	 * set position of rect according to position of parent
	 * @param parent 
	 */
	void mapPositionToParent(ObjectFoView * parent);
	/**
	 * this method call qt framework and we it becouse we need stop event on this widget
	 * @param  
	 */
	void mousePressEvent( QGraphicsSceneMouseEvent */*event*/);
private:
	/**
	 * parent formating object
	 */	
	ObjectFoView *m_parent;
};

#endif
