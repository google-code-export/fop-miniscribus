#ifndef OBJECTFOCONTAINERDOC_H
#define OBJECTFOCONTAINERDOC_H

#include <vector>

#include "objectfodoc.h"

class QPoint;
class QDomElement;

/**
 * This class represent each formating object which could contain another formating object
 */
class ObjectFoContainerDoc : public ObjectFoDoc
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param type 
	 * @param parentContainer 
	 */
	ObjectFoContainerDoc(ObjectFoDoc::FoObjectType type,ObjectFoContainerDoc *parentContainer);
	/**
	 * destructor
	 */
	~ObjectFoContainerDoc();
	/**
	 * return height of children items, which is position on y axis for new child formating object
	 * @return 
	 */
	QPoint positionForNewChildFoObject();
	/**
	 * return height of all children
	 * @return 
	 */
	int wholeHeightOfChildren();
	/**
	 * load children from QDomElement data
	 * @param elementData 
	 */
	void loadElementChildrenToContainer(QDomElement elementData);
	/**
	 * add foObjectViewable item to container
	 * @param foObjectViewable 
	 */
	void addItem(ObjectFoDoc *foObjectViewable);
	/**
	 * remove item from container (it's removed by id)
	 * @param foObjectViewable 
	 */
	void removeItem(ObjectFoDoc *foObjectViewable);
	/**
	 * return count of children
	 * @return 
	 */
	unsigned int countOfFoObjectsChildren(){	return m_listFoObjects.size();}
	/**
	 * return children from container from pos position
	 * @param pos 
	 * @return 
	 */
	ObjectFoDoc *foObjectChildren(unsigned int pos){	return m_listFoObjects.at(pos);}
	/**
	 * return reference to parent container
	 * @return 
	 */
	ObjectFoDoc *parentContainer(){	return m_pParentContainer;}
	/**
	 * resize object and emit signal to view to reload children (they position and size are also changed becouse they are calculated from parent)
	 * @param newSize 
	 */
	virtual void resize(QPoint newSize);
	/**
	 * clear width or hight for all children, if children is container,delete also it's children
	 * is used when view is deleted, and again created (e.g. resize)
	 */
	virtual void resetDoc();
	/**
	 * return height of container - it height of all children
	 * @return 
	 */
	virtual FoSize height();
protected:
	/**
	* contain reference to parent container
	*/
	ObjectFoContainerDoc *m_pParentContainer;
private:
	/**
	* list of (children) objects
	*/
	std::vector<ObjectFoDoc *>m_listFoObjects;
signals:
	/**
	 * signal to reload view part of children
	 */
	void sigReloadChildrenAfterResize();
};

#endif
