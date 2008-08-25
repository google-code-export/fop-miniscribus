#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include <QDomElement>
#include <QDomDocument>



#include "TranslateModel.h"







        
        
        


class StreamMark
{
  public:
  StreamMark() 
  :d(new QBuffer())
  {
  d->open(QIODevice::ReadWrite);
  start();
  }
  ~StreamMark()
  {
   d->close();
  }
  bool clear()
  {
    d->write(QByteArray()); 
    return d->bytesAvailable() == 0 ? true : false;
  }
  void start() { 
    d->seek(0);
  }
  bool LoadFile( const QString file ) { 
    if (clear()) {
     QFile f(file); 
        if (f.exists()) {
          if (f.open(QFile::ReadOnly)) {
           d->write(f.readAll());
           f.close();
           start();
           return true;
          }
        }
    }
    return false;
  }
  bool PutOnFile( const QString file ) { 
      QFile f(file);
      start();
      if (f.open(QFile::WriteOnly)) {
        uint bi = f.write(d->readAll());
        f.close();
        start();
        return bi > 0 ? true : false;
      }
   return false; 
  }
  QDomDocument Dom() { return doc; }
  QBuffer *device() { return d; }
  bool isValid() { return doc.setContent(stream(),false,0,0,0);  }
  QByteArray stream() { return d->data(); }
  QDomDocument doc;
  QBuffer *d;
}; 






class BookMarkModelRead : public QObject
{
     Q_OBJECT
//
public:
       BookMarkModelRead( const QString file  );
       ~BookMarkModelRead();
       void openStream( const QByteArray xml );
       QAbstractItemModel *bookModel() { return model; }
       QList<QStandardItem *> newLine( int level );
       inline QStringList linkList() { return internalLinkFound; }
protected:
       bool foundTree;
       QStringList internalLinkFound;
       int treeLoop;
       void openRootBookmark( const QDomElement e );
       void setHeader();
       void read();
       void read( const QDomElement e );
       QList<QStandardItem *> Compose( const QDomElement e , const int leveldeep = 0 );
       StreamMark *dev;
       QDomDocument doc;
       QStandardItemModel *model;
        Qt::ItemFlags flags;
       QFont bold_base_font;

private:
signals:
public slots:
};







//
#endif // BOOKMARKMODEL_H

