// own include
#include "maindoc.h"

// qt includes
#include <QtCore>
#include <QUndoGroup>
#include <QUndoStack>

#include "pagedoc.h"

MainDoc::MainDoc()
 :  QObject()
{
 	m_pUndoStack = new QUndoStack(this);
}


MainDoc::~MainDoc()
{
	delete m_pUndoStack;

	long size = m_vecPageDoc.size();
	for (long i = 0; i < size ;i++ )
	{
		if ( m_vecPageDoc[i] != NULL ) {
			delete m_vecPageDoc[i];
		}
	}
	m_vecPageDoc.clear();
}

QString MainDoc::toString()
{
	return m_domDocument.toString();
}

bool MainDoc::saveDocumentFile(const QString &fileName)
{
	QFile file( fileName );
	if( !file.open( QIODevice::WriteOnly ) )
		return false;
	
	QTextStream ts( &file );
	ts << m_domDocument.toString();
	
	file.close();
	return true;
}

bool MainDoc::openDocumentFile(const QString &fileName)
{
	m_currentXslFoFileName=fileName;
	
	QFile file(fileName);
	
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug(QString("cant open file %1").arg(fileName).toAscii());
		return false;
	}
	
	if (!m_domDocument.setContent(&file))
	{
		file.close();
		return false;
	}

	file.close();
	
	//get first child - we are looking for "xsl:stylesheet"
	QDomNode nodeRoot = m_domDocument.firstChild();
	nodeRoot=findFirstSibling(nodeRoot,"xsl:stylesheet");
	//next we are looking for "xsl:template"
	nodeRoot=nodeRoot.firstChild();
	nodeRoot=findFirstSibling(nodeRoot,"xsl:template");
	//next we are looking for "fo:root"
	nodeRoot=nodeRoot.firstChild();
	nodeRoot=findFirstSibling(nodeRoot,"fo:root");
	
	QDomElement elementRoot = nodeRoot.toElement(); // try to convert the node to an element.
	if(elementRoot.isNull())
	{
		qDebug("elementRoot of fo-document is null");
		return false;
	}
	
	/*first child must be fo:root*/
	if (elementRoot.tagName()!="fo:root")
	{
		qDebug("cant find fo:root element in fo-document");
		return false;
	}
	
	//find fo:layout-master-set
	for(QDomNode n = nodeRoot.firstChild();!n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull())
		{
			if (e.tagName()=="fo:layout-master-set")
			{
				createPagesTemplate(e,elementRoot);
			}
		}
	}
	
	
	
	return true;
}

QDomNode MainDoc::findFirstSibling(QDomNode node,QString tagName)
{
	for(QDomNode n = node;!n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull())
		{
			if (e.tagName()==tagName)
			{
				return n;
			}
		}
	}
	
	qDebug(QString("cant find sibling node with tagName %1").arg(tagName).toAscii());
	return QDomNode();
}

void MainDoc::createPagesTemplate(QDomElement &elementLayoutMasterSet,QDomElement &elementRoot)
{
	QDomNode nodePageSeqMast;

	//in this loop we try to find each fo:simple-page-master
	for(QDomNode nodeSimplePage = elementLayoutMasterSet.firstChild();!nodeSimplePage.isNull(); nodeSimplePage = nodeSimplePage.nextSibling())
	{
		//try to convert the node to an element
		QDomElement ePageMaster = nodeSimplePage.toElement();
		if(!ePageMaster.isNull())
			//find fo:simple-page-master it represent one page layout
			if(ePageMaster.tagName()=="fo:simple-page-master")
			{
				//get page sequence element according to pagemaster
				QDomElement ePageSequence=getPageSequenceElement(ePageMaster,elementRoot);
//				qDebug(ePageSequence.attribute("master-reference").toAscii());
				PageDoc *temp = new PageDoc(this,ePageSequence,ePageMaster);
				m_vecPageDoc.push_back(temp);
				emit createPage(temp);
			}
	}
}

QDomElement MainDoc::getPageSequenceElement(QDomElement &/*ePageMasterElement*/,QDomElement &elementRoot)
{
	for(QDomNode n = elementRoot.firstChild();!n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element
		if(!e.isNull())
		{
			if (e.tagName()=="fo:page-sequence")
			{
				return e;
			}
		}
	}
	qFatal("I cant find fo:page-sequence element in xsl-fo document");
	return QDomElement();
}
