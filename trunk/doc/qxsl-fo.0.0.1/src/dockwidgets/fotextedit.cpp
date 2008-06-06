//own include
#include "fotextedit.h"

//qt includes
#include <QMimeData>
#include <QTextStream>

//project includes
#include "textcontentdoc.h"
#include "objectfosimpledoc.h"
#include "objectfoview.h"
#include "focreator.h"

FoTextEdit::FoTextEdit(QWidget* mainWindow)
 : QTextEdit(mainWindow)
{
	m_pFoObjectSimple=NULL;
	connect(this, SIGNAL(textChanged()), this, SLOT(slTextChanged()));
}


FoTextEdit::~FoTextEdit()
{
}

void FoTextEdit::slTextChanged()
{
}

void FoTextEdit::setFoObjectSimple(ObjectFoSimpleDoc */*foObjectSimple*/)
{
}

bool FoTextEdit::canInsertFromMimeData( const QMimeData *mimeData ) const
{
	if (mimeData->hasFormat("application/x-dndxmldata"))
		return true;
	else
		return QTextEdit::canInsertFromMimeData(mimeData);
}

void FoTextEdit::insertFromMimeData( const QMimeData *mimeData )
{
	int dragType;
	QString wholeXmlPath;
		
	if (mimeData->hasFormat("application/x-dndxmldata"))
	{
		//if mimedata has desired format load data from it
		QByteArray itemData = mimeData->data("application/x-dndxmldata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);
		dataStream >> dragType >> wholeXmlPath;
		
		if(dragType==ObjectFoView::dragAssignXMLToItem)
		{
			qDebug(wholeXmlPath.toAscii());
         		QTextCursor cursor = this->textCursor();
			QDomElement el=FoCreator::GetInstancePtr()->xslValueOf(wholeXmlPath);
			QString strEl;
			QTextStream stream(&strEl);
			stream << el;
			cursor.insertText("<");
			cursor.insertText(strEl);
		}
	}
	
}

