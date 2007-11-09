#ifndef QMYDOM_H
#define QMYDOM_H
/* save this file as qxml.h dont forget to set QT += xml on your pro file */
#include <QDomDocument>
#include <QtDebug>
#include <QDebug> 
#include <QSettings>
#include <QDomDocument>
#include <QDomElement>
#include <QDomImplementation>
#include <QDomProcessingInstruction>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QTextDocument>
/*

sample read :

 Qxml doc;
 doc.SetSelf( const QString fullFileName );
 QDomElement root = doc.root();
 
 new doc 
 Qxml doc;
 doc.SetstreamFile( QString sfile );
QDomElement root = doc.root();

append to root 


#include "domelementcontainer.h"

const char *xml = "<test><tag>content 1</tag><tag>content 2</tag> "
                  "<othertag>wrongcontent</othertag>"
                  "<tag>content 3</tag></test>";

int main(){
  QDomDocument doc;
  doc.setContent(XMLSTREAM);
  DomElementContainer c(doc, "tag");
  foreach(QDomElement e, c)
    qDebug("%s", qPrintable(e.text()));
  return 0;
}

from http://www.qtcentre.org/ amministrator;
http://blog.wysota.eu.org/  http://www.wysota.eu.org/

*/



class QmyDom : public QDomDocument
{
public:
	QmyDom(); /* utf-8 - ISO-8859-1 */
    QString decode;
    QString CleanTagAsText( QString body );
    QString CleanTagAsHTML( QString body );
    inline QString Getdecode() { return decode; }
    bool SetSelf( const QString fullFileName );
    QDomElement insertFragmentorFile( QString fragment ); /* insert xml file or tag fragment */
    QString StringtoXML( QString t );  /* xml_escape_chars */
    void SetstreamFile( const QString fullFileName );  /* set a work file */
    bool saveXML(const QString fullFileName);  /* save to external file */
    bool saveXML();  /* save to work file if exist */
    void Print();  /* print to console */
    QString GetAtt(QDomElement e , QString name ); /* get attribute value from element */
    QDomElement root();
    QString FilterAttribute( QDomElement element , QString attribute );
    QDomElement insertTagValue( const QString name ,  QString value );
    bool saveStream( const QString fullFileName , QString xml );
protected:
    bool is_file(QString fullFileName);
    bool xml_unlink(QString fullFileName);
private:
    QDomProcessingInstruction header;
    QString stream_on_file; 
    QDomElement ErrorDom();  /* on error return </error> tag */
};
//











#endif // QXML_H


