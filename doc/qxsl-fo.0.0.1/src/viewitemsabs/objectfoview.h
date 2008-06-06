#ifndef OBJECTFOVIEW_H
#define OBJECTFOVIEW_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class QGraphicsSceneDragDropEvent;

class ResizeHandleWindow;
class ObjectFoSimpleDoc;
class ObjectFoDoc;
class MainView;
class ObjectFoHandler;

/**
 * base view class for all viewables formating objects
 */

class ObjectFoView : public QObject,public QGraphicsRectItem
{
Q_OBJECT
public:
	/**
	 * define type of drag (it we want create new item or move item ...)
	 */		
	enum EDragType 
	{
		dragMoveItem,
		dragCreateItem,
		dragAssignXMLToItem //currently unsused
	};

	/**
	 * state of formating object could be edit -> setHandlesChildEvents (false) or 
	 * dragState -> setHandlesChildEvents (false)
	 */	
	enum EObjectFoState
	{
		editState,
		dragState
	};
	/**
	 * 
	 * @param parentContainer 
	 * @param doc 
	 * @param pMainView 
	 * @param scene 
	 */
	ObjectFoView(ObjectFoView *parentContainer,ObjectFoDoc *doc,MainView *pMainView,QGraphicsScene *scene=NULL);
	/**
	 * destructor
	 */
	virtual ~ObjectFoView();	
	/**
	 * return document part of formating object
	 * @return 
	 */
	ObjectFoDoc* doc(){	return m_doc;}
	/**
	 * 
	 * @param typeOfNewFoObject 
	 * @param newPos 
	 */
	void createObjectFoSimpleDoc(int typeOfNewFoObject,QPoint newPos);
	/**
	 * return mainview, it's useful to handle toolbar, docks, menu
	 * @return 
	 */
	MainView *mainView(){	return m_pMainView;}
	/**
	 * return parent GraphicsItemGroup -> it's parent of all chilren of 
	 * @return 
	 */
	QGraphicsItemGroup *parentGraphicsItemGroup(){	return m_parentGroup;}
	/**
	 * set Z position to forth, it's usefull after selection to overwrite other graphicsitems
	 */
	void setForthZPosition();
	/**
	 * when item lost selection, we need set Z position back to original values
	 */
	void giveBackZPosition();
	/**
	 * item have EObjectFoState states we can seitch between values using this method
	 * @return 
	 */
	ObjectFoView::EObjectFoState changeState();
	/**
	 * return current state of item
	 * @return 
	 */
	ObjectFoView::EObjectFoState state(){	return m_objectFoState;}
	/**
	 * return parent container of this object, only page have m_pParent set to NULL
	 * @return 
	 */
	ObjectFoView *parentContainer(){	return m_pParent;}
	/**
	 * this method is intendend for catch drop with parent, it set Z position of item to front
	 */
	void setStateForDrop();
	/**
	 * set Z position back to his original value
	 */
	void setStateAfterDrop();
	/**
	 * handle drop event, do resize of child which called darg action with command pattern
	 * @param event 
	 */
	virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
	/**
	 * handle startDrag event, used to rewrite in inherited class
	 * @param  
	 */
	virtual void startDrag(QGraphicsSceneMouseEvent */*event*/){;}
	/**
	 * handle dragMove event, provide resizing of object
	 * @param event 
	 */
	virtual void dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
	/**
	 * handle dragEnterEvent, check if mime data are send from child object, becouse we wont drop only to parent directly object
	 * @param event 
	 */
	virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	/**
	 * handle mousePressEvent, set up fo handler (smal rect on top right)
	 * @param event 
	 */
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	/**
	 * handle mouseMoveEvent, used to identify start action drag
	 * @param event 
	 */
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
public slots:	
	/**
	 * currently unsused
	 * @param doc 
	 */
	void slSizeOfChildChanged(ObjectFoDoc *doc);
	/**
	 * select item, also setup object handler throught the mousepress method
	 */
	void slDoSelect();
	/**
	 * unused method
	 */
	void slDoUnselect();
	/**
	 * move item to pos
	 * @param  
	 */
	void slMoveTo(QPoint pos);
	/**
	 * call QGraphicsRectItem::setRect and after then call refreshItemHandles which
	 * refresh item handles (small rects on bottomright and topright for handle properties item)
	 * @param x 
	 * @param y 
	 * @param width 
	 * @param height 
	 */
	virtual void slSetRectangle ( qreal x, qreal y, qreal width, qreal height,bool setUpObjectHandler = true);
private:
	/**
	 * this memeber is used to hold original position e.g. while selecting (then it's bring to front)
	 */
	qreal m_oldZValue;
	/**
	 * hold reference to QGraphicsItemGroup, parent of this group is current object, all children of current object are added to
	 * this memeber
	 */
	QGraphicsItemGroup *m_parentGroup;
	/**
	 * refers to parent object
	 */
	ObjectFoView *m_pParent;
	/**
	 * represent state of the object
	 */
	EObjectFoState m_objectFoState;
protected:
	/**
	 * holds doc part of objectfo
	 */
	ObjectFoDoc *m_doc;
	/**
	 * holds reference to mainview
	 */
	MainView *m_pMainView;
};

#endif

///todo: parentGraphicsItemGroup should be in cotnainer
///todo: drag type enum isn't in the right header
