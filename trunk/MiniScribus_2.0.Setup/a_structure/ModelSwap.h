
#ifndef MODELSWAPPER_H
#define MODELSWAPPER_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QTextDocument>
#include <QTextTableCell>
#include <QApplication>
#include <QTableView>
#include <QTableWidget>
#include <QSqlRecord>
#include <QTextCursor>
#include <QDomDocument>



class ModelSwap 
{

public:
 ModelSwap();
 ~ModelSwap();
 void set( const QAbstractItemModel  *in_model , bool style = true );
 void set( const QStandardItemModel  *in_model , bool style = true );
 void set( const QSqlQueryModel  *in_model );
 void set( QTableView  *table , bool style = true );
 void set( QTableWidget  *table , bool style = true );
 void increment( const int i );
 QTextDocument *TabletoDom( int remove_last_line = 0 );
 inline QStandardItemModel *get() { return model; }
 inline int lines() const  { return model->rowCount(); }
 void nulldata();
private:
    QStandardItem *Style( const QModelIndex index , const QString txt  , const int line );
    QStandardItemModel  *model;
    QStringList headers;
    int row;
    Qt::ItemFlags flags;
    QFont bold_base_font;
};





#endif 




