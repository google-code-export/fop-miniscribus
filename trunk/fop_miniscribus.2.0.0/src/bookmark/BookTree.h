#ifndef BOOKTREE_H
#define BOOKTREE_H

#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "XML_Editor.h"  /* display result */
#include "BookMarkModel.h"
#include <QAbstractItemModel>

#include "SessionManager.h"


/*
get dom elemenst 
void BookTree::showXmlBooks()
{
    
    QStandardItemModel *pm = qobject_cast<QStandardItemModel *>(model());
    ModelDomRead *ponte = new ModelDomRead(pm);
    XMLTextEdit *sHtml = new XMLTextEdit(0);
	sHtml->setWindowFlags ( Qt::Window );
	sHtml->setMinimumSize ( 450 , 500 );
    sHtml->setPlainText( ponte->Domdocument()->toString(1) );
    sHtml->show();
    
}


*/


class BookTree : public QTreeView
{
     Q_OBJECT

public:
    BookTree( QWidget * parent = 0 );
    inline void openDom( const QByteArray x) 
    {
      xmlmode->openStream(x); 
      insertModel( xmlmode->bookModel() , xmlmode->linkList() );
    }
    ~BookTree();

protected:
    void contextMenuEvent ( QContextMenuEvent * e );
    int maximumID();
    void insertModel( QStandardItemModel *m , QStringList avaiablelist = QStringList() );
    BookMarkModelRead *xmlmode;
    QModelIndex touchCell;
    int onRowCurrent;
    QStringList fromdoclink;
    int touchlevel;
private:
    QStringList currentLinker;
signals:
public slots:
    void InsertOnRoot();  /* general insert at root index top tree */
    void insertSubOnCurrent();  /* on cursor cell */
    void removeOnCurrent();  /* on cursor cell */
    void showXmlBooks();
    void incommingBookmark( QStringList interlinks );
    void incommingBookmark( QStringList interlinks , QStandardItemModel * mod);
private slots:
    void activeTree(QModelIndex);

};
//
#endif // BOOKTREE_H

