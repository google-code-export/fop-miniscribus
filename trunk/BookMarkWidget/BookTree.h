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



class BookTree : public QTreeView
{
     Q_OBJECT

public:
    BookTree( QWidget * parent = 0 );
    ~BookTree();

protected:
    void contextMenuEvent ( QContextMenuEvent * e );
    int maximumID();
    void insertModel( QAbstractItemModel *m , QStringList avaiablelist = QStringList() );
    BookMarkModelRead *xmlmode;
    QModelIndex touchCell;
    int onRowCurrent;
    int touchlevel;
private:
    QStringList currentLinker;
signals:
public slots:
    void InsertOnRoot();  /* general insert at root index top tree */
    void insertSubOnCurrent();  /* on cursor cell */
    void removeOnCurrent();  /* on cursor cell */
    void showXmlBooks();
private slots:
    void activeTree(QModelIndex);

};
//
#endif // BOOKTREE_H

