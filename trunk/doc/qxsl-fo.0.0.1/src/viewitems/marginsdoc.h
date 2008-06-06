#ifndef MARGINSDOC_H
#define MARGINSDOC_H

#include <QObject>

/**
 * currently unused
 * represent doc part of page margins 
 */
class MarginsDoc : public QObject
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param parent 
	 */
	MarginsDoc(QObject *parent = 0);
	/**
	 * destructor
	 */
	~MarginsDoc();
	/**
	 * define type of margin
	 */
	enum EMarginType
	{
		left,right,top,bottom
	};
};

#endif
