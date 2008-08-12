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



QString ReadFileUtf8Xml( const QString fullFileName );








class Xslt_Convert : public QObject
{
     Q_OBJECT
//
public:	 
    Xslt_Convert();
    inline void Set_xml( const QString xmlfilein  ) { XML_file =  xmlfilein; }
    inline void Set_xslt( const QString xsltfilein  ) { XSLT_file =  xsltfilein; }
    inline void Set_param( QStringList namet , QStringList valuet ) 
    { 
        nam_M = namet;
        val_M = valuet;
    }
    inline QString  GetLast() 
    { 
       return LAST_RESUL;
    }
    void Convert();
protected:
    QString XML_file;
    QString XSLT_file;
    QStringList nam_M;
    QStringList val_M;
    QString LAST_RESUL;
private:
 
 signals:
 void ErrorMsg(QString);
 void Result(QString,QString);
public slots:

};
//
#endif // XSLT_CONVERT_H

