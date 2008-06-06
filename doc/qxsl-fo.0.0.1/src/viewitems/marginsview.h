#ifndef MARGINSVIEW_H
#define MARGINSVIEW_H

#include <QObject>

/**
 * currently unused
 * represent margins of page 
 */
class MarginsView : public QObject
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param parent 
	 */
	MarginsView(QObject *parent = 0);
	/**
	 * destructor
	 */
	~MarginsView();
};

#endif
