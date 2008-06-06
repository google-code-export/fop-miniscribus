#ifndef FOTEXTEDIT_H
#define FOTEXTEDIT_H

#include <QTextEdit>

class ObjectFoSimpleDoc;

/**
 * allow edit text of formating objects
 * currently unused
 */

class FoTextEdit : public QTextEdit
{
Q_OBJECT
public:
	/**
	 * constructor
	 * @param mainWindow 
	 */
	FoTextEdit(QWidget* mainWindow);
	/**
	 * destructor
	 */
	~FoTextEdit();
	/**
	 * 
	 * @param foObjectSimple 
	 */
	void setFoObjectSimple(ObjectFoSimpleDoc *foObjectSimple);
	/**
	 * 
	 * @param source 
	 * @return 
	 */
	bool canInsertFromMimeData( const QMimeData *source ) const;
	/**
	 * 
	 * @param source 
	 */
	void insertFromMimeData( const QMimeData *source );
public slots:
	/**
	 * 
	 */
	void slTextChanged();
private:
	/**
	 * 
	 */
	ObjectFoSimpleDoc *m_pFoObjectSimple;
};

#endif

///todo: this class is deprecated and should be compete removed from project

