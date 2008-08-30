#ifndef XSLT_CONVERT_H
#define XSLT_CONVERT_H

#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QApplication>
#include <QtCore> 
#include <QTextCodec>
#include <QCoreApplication>
#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>

#include <libxml/nanoftp.h>
#include <libxml/nanohttp.h>
#include <libxml/parser.h>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>


#include "Tools_Basic.h"   /* on global include */

#define XMLERROR_FILE \
              QString( "%1/libxmlerror.dat" ).arg( QDir::homePath() ) 

class Xslt_Convert : public QObject
{
     Q_OBJECT
//
public:	 
    Xslt_Convert( const QString xmlfile , 
                  const QString xsltfile ,  
                  QString resulterxml );
    ~Xslt_Convert();
    inline QString dmg() { return debug_msg; }
    inline QByteArray stream() { return Rstream; }
    QTextCodec *xmlcodec;
    QTextCodec *xsltcodec;
    
protected:
    QString debug_msg;
    QString XML_file;
    QString XSLT_file;
    QMap<QString,QString> Params;
private:
    QByteArray Rstream;
    bool EndingJob;
 signals:
 void ErrorMsg(QString);
 void DebugMsg(QString);
public slots:

private slots:
void CheckError();

};
//
#endif // XSLT_CONVERT_H

