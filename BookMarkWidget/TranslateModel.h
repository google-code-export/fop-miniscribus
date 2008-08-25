#ifndef TRANSLATEMODEL_H
#define TRANSLATEMODEL_H
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include <QtCore>
#include <QtGui>
#include <QDebug>

#include <QDomElement>
#include <QDomDocument>
#include <QDomNode>



QIcon createBookColorIcon( QColor color );
QList<QStandardItem*> childList( QStandardItem *qi );
QMap<QStandardItem*,QModelIndexList> childListmap( QStandardItem *qi );


namespace ApacheFop
{



typedef enum
        {  
          Mfull,
          Mommit,
          Mclean
        } ModusReadModel;




class ModelDomRead : public QObject
{
    Q_OBJECT
    
    public:
        
    ModelDomRead( QStandardItemModel  *mod ,  ModusReadModel  e  = Mfull, const QString xlinktxt = QString() );
    inline QDomDocument *Domdocument() { return dom; }
    
    protected:
        
    void parserChild( QModelIndex index , QDomElement e  );
    void read();
    void appendOnTreeDom();
    QDomElement linkPdf( QModelIndex first ,  QModelIndex second );
    QStandardItemModel *model;
    ModusReadModel modus;
    QString linktxt;
    QDomDocument *dom;
    QDomElement root;
    QDomElement appender;
};


}


class BooksDelegate : public QItemDelegate
{
    Q_OBJECT
   
    
public:
   BooksDelegate( QObject *parent  , QStringList link );
   void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
   QWidget *createEditor(QWidget *parent0, const QStyleOptionViewItem &option, const QModelIndex &index) const;
   void setEditorData(QWidget *editor, const QModelIndex &index);
   void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const ;
   QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;

signals:
public slots:
private:
    QStringList xi;

};
  




#endif // TRANSLATEMODEL_H

