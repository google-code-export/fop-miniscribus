#ifndef FOOBJECTVIEWABLEDOC_H
#define FOOBJECTVIEWABLEDOC_H

#include <QPoint>
#include <QObject>

#include "attributefodata.h"


class QDomElement;
class QPixmap;
class QRect;

class FoObjectType;


/**
 * Base doc class for all formating objects (FO).Contain enum FoObjectType of object types you should use only this enums and 
 * methods stringToType typeToStrin to handle concrete type of foobject, also contain member id to identify object in
 * parent's vector, also contain base methods for move and resize which could be overwritten in inherited classes
 */

class ObjectFoDoc : public QObject
{
Q_OBJECT
public: 
	/**
	* enum of object types,if you add new type you must add it also to case 
	* block in typeToString and stringToType methods
	*/	
	enum FoObjectType
	{
		//simple
		typeNone=0,
		typeFoBlock=1,
		typeFoBlockContainer=2,
		typeFoTable=3,
		typeFoTableCell=4,
		typeFoTableBody=5,
		typeFoTableRow=6,
		typeFoTableHeader=7,
		typeFoExternalGraphic=8,
		//sequenced
		typeFoBorderRegion=10,
		typeFoBodyRegion=11,
		typeFoPage=12,
		//without foobject (without foattributes)
		typeFoTextContent=20
	};

	/**
	 * Construtor
	 * @param objectType type of new FO object
	 */
	ObjectFoDoc(FoObjectType objectType);
	/**
	 * Destructor
	 */
	virtual ~ObjectFoDoc();
	/**
	 * return type of object
	 * @return type of object
	 */
	FoObjectType type() const {return m_objectType;}
	/**
	 * set id of object, id is used to identify object in parent's vector of children
	 * @param id 
	 */
	void setId(int id){	m_id=id;}
	/**
	 * return id of object
	 * @return id of object
	 */
	int getId(){	return m_id;}
	/**
	 * return top right position of FO object, read value from m_position,but  each concrete formating object could use
	 * own rule to get positionValue throught the overwriting
	 * @return return top right position of FO object
	 */
	virtual QPoint positionValue();
	/**
	 * set the top right position of formating object, each concrete formating object could use own rule to set position
	 * it use m_position to save pos value
	 * @param pos 
	 */
	virtual void setPositionValue(QPoint pos){	m_position=pos;}
	/**
	 * return width of formating object, from m_width member, but could be overwritted in sublass
	 * @return 
	 */
	virtual FoSize widthValue();
	/**
	 * set width of formating object, it use m_width
	 * @param width 
	 */
	virtual void setWidthValue(FoSize width){	m_width=width;}
	/**
	 * return height of foblock
	 * @return 
	 */
	virtual FoSize height();
	/**
	 * set height of block
	 * @param height 
	 */
	void setHeight(FoSize height){	m_height=height;}
	/**
	 * method which invoke signal to view, to provide selection (is used in ItemHierarchyTreeDoc to select item in graphicsview)
	 */
	void doSelect();
	/**
	 * method which invoke signal to view, to provide clear slection (is used in ItemHierarchyTreeDoc)
	 */
	void doUnselect();
	/**
	 * set setPositionValue and emit signal to view
	 * @param pos 
	 */
	virtual void moveTo(QPoint pos);
	/**
	 * set setWidthValue and emit singal to view
	 * @param pos 
	 */
	virtual void resize(QPoint newSize);
	/**
	 * check if is position read directly from fodocument - it depend on type of object
	 * @return if value of position is computed (false) or is readed from fodoc -is fixed (true)
	 */
	bool hasAbsolutePositioning();
	/**
	 * this method is overwitten in sublclassed ObjectFoContainer class if isn't overwritten it return NULL
	 * @param  
	 * @return NULL
	 */
	virtual ObjectFoDoc *foObjectChildren(unsigned int){	return NULL;}
	/**
	 * return 0, but is overwitten in ObjectFoContainer to return count of chilren
	 * @return 
	 */
	virtual unsigned int countOfFoObjectsChildren(){	return 0;}
	/**
	 * pure virtual method return parent container
	 * @return 
	 */
	virtual ObjectFoDoc *parentContainer()=0;
	/**
	 * method is used e.g when view is deleted, and we need clear document part (used in move command)
	 */
	virtual void resetDoc();
protected:
	/**
	* represent unique id of object
	*/
	int m_id;
	/**
	* contains height of foobject, it needn't be used if is used value directly from fo-document 
	*/
	int m_height;
	/**
	* contains width of foobject, it needn't be used if is used value directly from fo-document
	*/
	int m_width;
	/**
	* upper left corner position of fo:block, we need this value e.g. for fo:block becouse
	* it don't have attributes in fo document for position
	*/
	QPoint m_position;
private:
	/**
	* type of object - must be set in child constructor
	*/
	FoObjectType m_objectType;
signals:
	/**
	 * signal to view to select object in graphicsview
	 */
	void sigDoSelect();
	/**
	 * signal to view which provide clear selection
	 */
	void sigDoUnselect();
	/**
	 * signal to view which provide move to
	 * @param newPos 
	 */
	void sigMoveTo(QPoint newPos);
	/**
	 * set rectangle values of formating objects, also if is needed setup object handler
	 * @param x 
	 * @param y 
	 * @param width 
	 * @param height 
	 * @param setUpObjectHandler if we want show object handler (small rect on top right to select indication)
	 */
	void sigSetRectangle ( qreal x, qreal y, qreal width, qreal height,bool setUpObjectHandler=true);
public:
	/* ************** */
	/* STATIC METHODS */
	/* ************** */
	/**
	 * convert string to type
	 * @param  type of desired object
	 * @return return null string if can't assign type to string
	 */
	static QString typeToString(const FoObjectType type);
	/**
	 * convert type to string
	 * @param type 
	 * @return 
	 */
	static ObjectFoDoc::FoObjectType stringToType(const QString typeName);
	/**
	 * convert type to pixmap (becouse each formating object has own icon)
	 * @param type 
	 * @return 
	 */
	static QPixmap typeToPixmap(const FoObjectType type);
};

#endif
