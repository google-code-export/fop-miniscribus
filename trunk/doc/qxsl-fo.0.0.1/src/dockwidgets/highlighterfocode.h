#ifndef HIGHLIGHTERFOCODE_H
#define HIGHLIGHTERFOCODE_H

class QTextDocument;

#include <QSyntaxHighlighter>

/**
 * provide highlighting of displayed fo:document code for CodeView class
 */
class HighlighterFoCode : public QSyntaxHighlighter
{
public:
	/**
	 * constructor
	 * @param textDoc 
	 */
	HighlighterFoCode(QTextDocument *textDoc);
	/**
	 * destructor
	 */	
	~HighlighterFoCode();
	/**
	 * is called from qt framework
	 * Highlights the given text block. This function is called when necessary by the rich text engine, i.e. on text blocks which have changed.
	 * @param text 
	 */
	void highlightBlock(const QString &text);
private:
	/**
	 * do highlight for given inputText with textFormat on expression
	 * @param expression 
	 * @param textFormat 
	 * @param inputText 
	 */
	void expHighlightning(QString expression,QTextCharFormat textFormat,const QString &inputText);
	/**
	 * check comment block in inputText and do highlight if is needed
	 * @param inputText 
	 */
	void highlightComment(const QString &inputText);
};

#endif
