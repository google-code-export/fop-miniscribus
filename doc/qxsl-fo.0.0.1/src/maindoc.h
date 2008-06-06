#ifndef MAINDOC_H
#define MAINDOC_H

// qt includes
#include <QtXml>
#include <QObject>
#include <QUndoStack>
#include <vector>

//project includes


class QUndoGroup;
class QUndoStack;
class QUndoView;
class QString;
class QDomDocument;

class FoSimplePageMaster;
class PageDoc;

/**
 * maindoc contain vector of all fo-pages,undo stack,and memeber of QDomDocument which represent fo-document
 * also contain methods to join two definitions of fo:page one is situated in fo:layout-master-set as fo:simple-page-master
 * and second is in fo:root represented by fo:page-sequence
 * class is doc part for mainview 
 */
class MainDoc :  public QObject{
Q_OBJECT
public:
	/**
	 * constructor
	 */
	MainDoc();
	/**
	 * destructor
	 */
	~MainDoc();
	/**
	 * open xsl-fo file load it to QDomDocument and start creating foobjects
	 * @param fileName 
	 * @return 
	 */
	bool openDocumentFile(const QString &fileName);
	/**
	 * save QDomDocument which represent fo-document to file
	 * @param fileName 
	 * @return 
	 */
	bool saveDocumentFile(const QString &fileName);
	/**
	 * create page - for each simple-page-master element in elementLayoutMasterSet
	 * to find appropriate concrete definition of page in elementRoot it use getPageSequenceElement
	 * @param elementLayoutMasterSet in this element we are looking for simple-page-master-element
	 * @param elementRoot and in this element we are looking for appropriate page-sequence definition
	 */
	void createPagesTemplate(QDomElement &elementLayoutMasterSet,QDomElement &elementRoot);
	/**
	 * push command cmd to undo stack
	 * @param cmd 
	 */
	void pushToUndoStack(QUndoCommand * cmd){	m_pUndoStack->push(cmd);}
	/**
	 * return reference to undo stack
	 * @return 
	 */
	QUndoStack *undoStack(){ return m_pUndoStack;}
	/**
	 * convert data QDomDocument to QString 
	 * @return xsl-fo doc as QString
	 */
	QString toString();
	/**
	 * get appropriate fo:page-sequence element from fo:root of xsl-fo document for single-page-master (if SPM is in page-sequence-master, it could
	 * be complicated becouse we must find PS for PSM)
	 * @param ePageMasterElement 
	 */
	QDomElement getPageSequenceElement(QDomElement &ePageMasterElement,QDomElement &elementRoot);
	/**
	 * return first sibling of node, sibling must have tag name tagName
	 * @param node we search sibling for this child
	 * @param tagName name of tag of searched sibling
	 * @return desired sibling
	 */
	QDomNode findFirstSibling(QDomNode node,QString tagName);
	/**
	 * return current name of file name of fo:document which is loaded to QDomDocument
	 * @return string of current file name
	 */
	QString currentXslFoFileName(){	return m_currentXslFoFileName;}
public slots:
	/**
	 * do undo on undo stack
	 */
	void undo(){	m_pUndoStack->undo();}
	/**
	 * do redo on undo stack
	 */
	void redo(){	m_pUndoStack->redo();}
signals:
	/**
	 * signal intended for MainView, inform view about newly created page
	 * @param  
	 */
	void createPage(PageDoc*);
private:
	/**
	 * vector of all pages
	 */
	std::vector<PageDoc *>m_vecPageDoc;
	/**
	 * qt undo stack
	 */
	QUndoStack *m_pUndoStack;
	/**
	 * pointed to root of document
	 */
	QDomDocument m_domDocument;
	/**
	 * current name of file which is loaded to m_domDocument
	 */
	QString m_currentXslFoFileName;
};

#endif 

///todo:we need join page-sequence master-reference with apropriate  fo:simple-page-master master-name, becouse now we can handle only one page template
