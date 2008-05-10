#ifndef OO_DOCUMET_H
#define OO_DOCUMET_H

#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QVariant>

namespace OOO {

class Document
{
  public:
  Document() 
  { 
     
  }
  ////operator QVariant() const {
  ////return QVariant::fromValue(*this);
  ////}   
  QByteArray content() const
  {
    return mContent;
  }

  QByteArray meta() const
  {
    return mMeta;
  }

  QByteArray styles() const
  {
    return mStyles;
  }
  QString lastErrorString() const
  {
    return mErrorString;
  }

QByteArray mStyles;
QByteArray mMeta;
QByteArray mContent;
QString mErrorString;
  
};


///////Q_DECLARE_METATYPE(Document); 


}
  
  
  
  
  
#endif // OO_DOCUMET_H

