//own include
#include "textcontentview.h"

//qt include
#include <QGraphicsTextItem>
#include <QString>
#include <QGraphicsRectItem>
#include <QFont>
#include <QTextDocument>
#include <QGraphicsSceneMouseEvent>

//project include
#include "textcontentdoc.h" 
#include "attributefo.h"
#include "pageview.h"
#include "mainview.h"
#include "rowinblockview.h"

TextContentView::TextContentView(TextContentDoc *pTextContentDoc,MainView *pMainView,RowInBlockView *parentRow,int startPosInText)
:	QGraphicsTextItem(parentRow),m_pTextContentDoc(pTextContentDoc),
	m_startPositionInText(startPosInText),m_pMainView(pMainView),m_pParentRow(parentRow)
{
	
	QGraphicsTextItem::scale(constTextScale,constTextScale);
	
	setZValue ( PageView::ESimpleFoObjectViewZPosition);
	
	//setTextInteractionFlags(Qt::TextEditorInteraction);
}

bool TextContentView::fillViewFromDocText(int maxWidth)
{
	QString appendedText="";
	
	int offset=m_startPositionInText;
	
	QString text=QString(m_pTextContentDoc->text().simplified().trimmed ().mid(offset).trimmed());

	QStringList listOfWords(text.split(" "));
	
	//hide becouse wi will fill qgraphicstext char by char
	hide();
	
	//this loop go char by char and in each iteration append one char to
	//qgraphicstext, in loop we compare width of qgraphicstext with doc with which is calculated 
	//according to parent)
	for(int i = 0; i < listOfWords.size(); ++i)
	{
		//set qgraphicstext with text from textdocument
		setHtml(appendedText+listOfWords.at(i));
		//comapare extended qgraphcistext with doc width
		int width=boundingRect().width();
		
		if (maxWidth < width)
		{	
			//if we don't have a place to resize stop loop
			//end return end position for this textcontent in document text 
			setHtml(appendedText);
			showAndConnectWithDoc();
			
			if (appendedText=="")
				return false;
			else
				return true;
		}
		else
		{
			appendedText+=listOfWords.at(i);
			//try to put space after word - after each word should be space, only word on
			//the end should have exception
			setHtml(appendedText+" ");
			int widthWithSpace=boundingRect().width();
			if(maxWidth >= widthWithSpace)
				appendedText=appendedText+" ";
			else
				setHtml(appendedText);
		}
	}
	
	showAndConnectWithDoc();
	
	return true;
}


QRectF TextContentView::boundingRect ()
{
	QRectF rect=QGraphicsTextItem::boundingRect();
	QRectF scaledRect(0,0,rect.width()*constTextScale,rect.height()*constTextScale);
	return scaledRect;
}

void TextContentView::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsTextItem::mousePressEvent(event);
}

TextContentView::~TextContentView()
{
	
}

void TextContentView::slContentsChange ( int position, int charsRemoved, int charsAdded )
{
	QString newTextContent = document()->toPlainText().mid(position,charsAdded);
	
	m_pTextContentDoc->replaceBlockOfText(position + m_startPositionInText,charsRemoved,newTextContent);
}

void TextContentView::showAndConnectWithDoc()
{
	connect( document(),SIGNAL(contentsChange(int,int,int)),this,SLOT(slContentsChange(int,int,int)) );
	show();
}

int TextContentView::countOfChars()
{
	int size=toPlainText().size();
	return size;
}
