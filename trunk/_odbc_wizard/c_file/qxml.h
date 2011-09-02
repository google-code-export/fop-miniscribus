#ifndef QXML_H
#define QXML_H
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

class Qxml : public QDomDocument
{
public:
    Qxml(); /* utf-8 - ISO-8859-1 */
    QString decode;
    inline QString Getdecode() { return decode; }
    QDomElement insertFragmentorFile( QString fragment ); /* insert xml file or tag fragment */
    QString StringtoXML( QString t );  /* xml_escape_chars */
    void SetstreamFile( QString sfile );  /* set a work file */
    bool saveXML(QString fullFileName);  /* save to external file */
    bool saveXML();  /* save to work file if exist */
    void Print();  /* print to console */
    QString GetAtt(QDomElement e , QString name ); /* get attribute value from element */
    QDomElement root();
    QString FilterAttribute( QDomElement element , QString attribute );
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


