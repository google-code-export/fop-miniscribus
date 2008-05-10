#ifndef OO_HANDLER_H
#define OO_HANDLER_H

#include "xmlhighlighter.h"

#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include "quazip.h"
#include "quazipfile.h"
#include "oo_document.h"
#include "styleparser.h"
#include "styleinformation.h"

namespace OOO {




//
class OO_Handler : public QObject
{
     Q_OBJECT
//
public:
OO_Handler( const QString filedoc );
QString  text();
bool ParseFiles();
inline QTextDocument *document() { return mTextDocument; }
protected:
   void OpenFile( const QString filedoc );
   
   bool havingdoc;
   QString log;
   QStringList filelist;
   QMap<QString,QByteArray> ooFile; 
private:
   bool convertBody( const QDomElement &element );
   bool convertText( const QDomElement &element );
   bool convertHeader( QTextCursor *cursor, const QDomElement &element );
   bool convertParagraph( QTextCursor *cursor, const QDomElement &element, const QTextBlockFormat &format = QTextBlockFormat(), bool merge = false );
   bool convertTextNode( QTextCursor *cursor, const QDomText &element, const QTextCharFormat &format );
   bool convertSpan( QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format );
   bool convertList( const QDomElement &element );
   bool convertTable( const QDomElement &element );
   bool convertFrame( const QDomElement &element );
   bool convertLink( QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format );
   bool convertAnnotation( QTextCursor *cursor, const QDomElement &element );
    QTextDocument *mTextDocument;
    QTextCursor *mCursor;
    StyleInformation *mStyleInformation;
    void setError( QString msg );
signals:
   void errorFound(QString);
public slots:

};



}
//
#endif // OO_HANDLER_H

