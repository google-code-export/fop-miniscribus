#ifndef CODEVIEW_H
#define CODEVIEW_H

#include <QTextEdit>

/**
 * show code of currently procesed fo:document
 */
class CodeView : public QTextEdit
{
public:
	/**
	 * constructor
	 */
	CodeView();
	/**
	 * destructor
	 */
	~CodeView();
	/**
	 * load qstring (fo code data) to codeview window
	 * @param foData 
	 */
	void load(QString foData);
};

#endif
