//own include
#include "textcontentdoc.h"

//qt includes
#include <QObject>
#include <QString>
#include <QDomElement>
#include <QTextDocument>

//project includes
#include "objectfodoc.h"
#include "objectfosimpledoc.h"
#include "objectfocontainersimpledoc.h"
#include "objectfocontainerdoc.h"

TextContentDoc::TextContentDoc(ObjectFoContainerSimpleDoc *pParentFoContainer,QString text)
:ObjectFoDoc(ObjectFoDoc::typeFoTextContent),m_pParentContainer(pParentFoContainer),m_textDoc(NULL)
{
	m_textDoc = text;
}

TextContentDoc::~TextContentDoc()
{
	
}

void TextContentDoc::replaceBlockOfText(int startPosition,int charsRemoved,QString blockOfText)
{
	m_textDoc.replace(startPosition,charsRemoved,blockOfText);
}
