#ifndef DOMITEM_H
#define DOMITEM_H

#include <QDomNode>
#include <QHash>

/**
 * represent ine item in dommodel, contain reference to parent and to children
 */
class DomItem
{
public:
	/**
	 * constructor
	 * @param node 
	 * @param row 
	 * @param parent 
	 */
	DomItem(QDomNode &node, int row, DomItem *parent = 0);
	/**
	 * destructor
	 */
	~DomItem();
	/**
	 * return child on i position
	 * @param i 
	 * @return 
	 */
	DomItem *child(int i);
	/**
	 * return reference to parent
	 * @return 
	 */
	DomItem *parent();
	/**
	 * return encapsulated QDomNode data
	 * @return 
	 */
	QDomNode node() const;
	/**
	 * return row number
	 * @return 
	 */
	int row();
private:
	/**
	 * node data
	 */
	QDomNode domNode;
	/**
	 * children
	 */
	QHash<int,DomItem*> childItems;
	/**
	 * reference to parent 
	 */
	DomItem *parentItem;
	/**
	 * hold information about row nomber (position in siblings)
	 */
	int rowNumber;
};

#endif
