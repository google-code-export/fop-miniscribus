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




class QmyDom : public QDomDocument
{
public:
	QmyDom( bool heading = true ); /* utf-8 - ISO-8859-1 */
    QString decode;
    QString CleanTagAsText( QString body );
    QString CleanTagAsHTML( QString body );
    inline QString Getdecode() { return decode; }
    bool SetSelf( const QString fullFileName );
    bool SetStream( const QString stream );
    QDomElement insertFragmentorFile( QString fragment ); /* insert xml file or tag fragment */
    void insertFragment( QDomElement e , QString fragment );
    void insertFullFragment( QDomElement e , QString fragment );
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
    bool is_file(QString fullFileName);
    bool xml_unlink(QString fullFileName);
    QDomProcessingInstruction header;
    QString stream_on_file; 
    QDomElement ErrorDom();  /* on error return </error> tag */
};
//



Q_DECLARE_METATYPE(QmyDom);






#endif // QXML_H


