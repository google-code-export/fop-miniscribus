//own iclude
#include "highlighterfocode.h"

//project include
#include <QTextDocument>

HighlighterFoCode::HighlighterFoCode(QTextDocument *textDoc)
 : QSyntaxHighlighter(textDoc)
{
	
}


HighlighterFoCode::~HighlighterFoCode()
{
	
}

void HighlighterFoCode::highlightBlock(const QString &inputText)
{
	QTextCharFormat blockFormat;
	QString blockPattern;
	
	//do highlighting in whole block
	blockFormat.setForeground(Qt::blue);
	blockPattern = "<.*>";
	expHighlightning(blockPattern,blockFormat,inputText);
	
	//do highlighting in quotation
	blockFormat.setForeground(Qt::darkGreen);
	blockPattern = "\".*\"";
	expHighlightning(blockPattern,blockFormat,inputText);
	
	//do highhlighting for parameter names
	blockFormat.setForeground(Qt::red);
	blockPattern = " [a-z-:]*=";
	expHighlightning(blockPattern,blockFormat,inputText);

	//do highlighting for comment	
	highlightComment(inputText);
	
}

void HighlighterFoCode::expHighlightning(QString blockPattern,QTextCharFormat textFormat,const QString &inputText)
{	
	QRegExp expression(blockPattern);
	expression.setMinimal(true);
	int index = inputText.indexOf(expression);
	while (index >= 0)
	{
		int length = expression.matchedLength();
		setFormat(index, length, textFormat);
		index = inputText.indexOf(expression, index + length);
	}
}

void HighlighterFoCode::highlightComment(const QString &inputText)
{
	QTextCharFormat multiLineCommentFormat;
	multiLineCommentFormat.setForeground(Qt::green);
	
	QRegExp startExpression("<!--");
	QRegExp endExpression("-->");
	
	setCurrentBlockState(0);
	
	int startIndex = 0; 
	if (previousBlockState() != 1)
		startIndex = inputText.indexOf(startExpression);
	
	while (startIndex >= 0)
	{
		int endIndex = inputText.indexOf(endExpression, startIndex);
		int commentLength;
		if (endIndex == -1)
		{
			setCurrentBlockState(1);
			commentLength = inputText.length() - startIndex;
		}
		else
		{
			commentLength = endIndex - startIndex+ endExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = inputText.indexOf(startExpression,startIndex + commentLength);
	}
}
