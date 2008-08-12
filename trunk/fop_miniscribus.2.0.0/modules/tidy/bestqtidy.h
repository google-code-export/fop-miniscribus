#ifndef BESTQTIDY_H
#define BESTQTIDY_H
//
//
/*  Save file as qtidy.h  */
/*  incomming class name QTidy */

#include "tidy.h"
#include <QFile>
#include <QtCore> 
#include <QCoreApplication>
#include <QSettings>
#include <QDomDocument>








class QTidy : public QObject
{
    Q_OBJECT
    
public:
    QString ErrorMSG;
    QString TMP_Dir;
    QString TMP_Config;
    QTextCodec *setcurrentcodec;
    QStringList  tidiconfigfile;
    int status;
    TidyDoc doc;

QTidy()
{
    
}

~QTidy()
{
    //////////DownDir_RM(TMP_Dir);
}

void Init( QString dircache )
{
    if (!dircache.endsWith("/")) {
      TMP_Dir = dircache + "/";  
    } else {
      TMP_Dir = dircache; 
    }
    
    ////////////DownDir_RM(TMP_Dir);
    
    QDateTime timer1( QDateTime::currentDateTime() ); 
    uint timert =  timer1.toTime_t();
    
    TMP_Config = QString( "%1tidy.conf" ).arg( TMP_Dir );
    setcurrentcodec = QTextCodec::codecForMib(106);  /* utf8 */ 
    QDir dira(TMP_Dir);
    if ( dira.mkpath(TMP_Dir) ) { } else {
    ErrorMSG = QString("ERROR! on %1 not Possibel to make dir \"%2\"\n").arg(timert).arg( TMP_Dir );
    }
}


QString TidyCleanfullxhtml( QString body )
{
    QString newbody;
    bool ok = false;;
    int processid = qrand() % 10000;
    QString sucleanfile = QString( "%1tidy_tmp_%2_in.xml" ).arg( TMP_Dir ).arg(processid);
    QString sucleanfileout = QString( "%1tidy_tmp_%2_out.xml" ).arg( TMP_Dir ).arg(processid);
    
    
              ok = file_put_utf8contents(sucleanfile,body);
    
    /* file_put_utf8contents   Readutf8File  */
    if (!ok) {
    return body;
    } else {
    SetUp_xhtml_full_page();
    CleanTidy(sucleanfile,sucleanfileout);
    newbody =  Readutf8File(sucleanfileout);
        
        qt4unlink(sucleanfile);
        qt4unlink(sucleanfileout);
        
            if (newbody.size() > 2) {
            return newbody;
            } else {
            return body;   
            }
    }        
}




/* prepare config tidy config file xhtml clean */
bool SetUp_xhtml_full_page()
{
    status = 0;
    ErrorMSG ="";
    doc = tidyCreate();
    /////////qDebug() << "### load config. xhtml ...  ";
    /* --output-xhtml y --indent "auto" --wrap "90" */
    bool ok;
    tidiconfigfile.clear();
    tidiconfigfile.append("output-xhtml: yes");
    tidiconfigfile.append("indent: auto");
    tidiconfigfile.append("wrap: 550");
    tidiconfigfile.append("indent-spaces: 1");
    tidiconfigfile.append("char-encoding: utf8");
    tidiconfigfile.append("output-encoding: utf8");
    tidiconfigfile.append("wrap-attributes: yes");
    tidiconfigfile.append("hide-comments: no");
    tidiconfigfile.append("numeric-entities: yes");
    tidiconfigfile.append("drop-proprietary-attributes: no");
    tidiconfigfile.append("word-2000: yes");
    
    //////tidiconfigfile.append("show-body-only: yes");  /* only body checks */
    QString configtotsl = tidiconfigfile.join("\n");
    ok = tidy_file_set_config(configtotsl);   /* TIDY_CONF */
    if (ok) {
         QByteArray configfileti = TMP_Config.toAscii();
         status = tidyLoadConfig( doc, configfileti.data() );  /* http://ch2.php.net/manual/de/function.tidy-load-config.php */
        if ( status != 0 ) {
         ErrorMSG ="Not possibel to load Config File!";
         return false;
        } else { 
        return true;
        }
    } else {
      ErrorMSG ="Not possibel to load Config File!";
     return false; 
    } 
}

/* prepare config tidy config file xhtml clean */
bool SetUp_xhtml()
{
    status = 0;
    ErrorMSG ="";
    doc = tidyCreate();
    ///////qDebug() << "### load config. xhtml ...  ";
    bool ok;
    tidiconfigfile.clear();
    tidiconfigfile.append("error-file: pase.dat");
    tidiconfigfile.append("quiet: yes");
    tidiconfigfile.append("output-xhtml: yes");
    tidiconfigfile.append("clean: yes");
    tidiconfigfile.append("wrap: 550");
    tidiconfigfile.append("indent-spaces: 1");
    tidiconfigfile.append("char-encoding: utf8");
    tidiconfigfile.append("output-encoding: utf8");
    tidiconfigfile.append("wrap-attributes: yes");
    tidiconfigfile.append("hide-comments: no");
    tidiconfigfile.append("numeric-entities: yes");
    tidiconfigfile.append("drop-proprietary-attributes: no");
    tidiconfigfile.append("word-2000: yes");
    //////tidiconfigfile.append("bare: yes");
    tidiconfigfile.append("show-body-only: yes");  /* only body checks */
    QString configtotsl = tidiconfigfile.join("\n");
    ok = tidy_file_set_config(configtotsl);   /* TIDY_CONF */
    if (ok) {
         QByteArray configfileti = TMP_Config.toAscii();
         status = tidyLoadConfig( doc, configfileti.data() );  /* http://ch2.php.net/manual/de/function.tidy-load-config.php */
        if ( status != 0 ) {
         ErrorMSG ="Not possibel to load Config File!";
         return false;
        } else { 
        return true;
        }
    } else {
      ErrorMSG ="Not possibel to load Config File!";
     return false; 
    } 
}

/* prepare config tidy config file xml clean */
bool SetUp_xml()
{
    status = 0;
    ErrorMSG ="";
    doc = tidyCreate();
    /*qDebug() << "### load config xml ....  ";*/
    bool ok;
    tidiconfigfile.clear();
    tidiconfigfile.append("error-file: pase.dat");
    tidiconfigfile.append("quiet: yes");
    tidiconfigfile.append("input-xml: yes");
    tidiconfigfile.append("char-encoding: utf8");
    tidiconfigfile.append("output-encoding: ascii");
    tidiconfigfile.append("output-xml: yes");
    tidiconfigfile.append("hide-comments: no");
    tidiconfigfile.append("numeric-entities: yes");
    tidiconfigfile.append("add-xml-space: no");
    QString configtotsl = tidiconfigfile.join("\n");
    ok = tidy_file_set_config(configtotsl);   /* TIDY_CONF */
    if (ok) {
         QByteArray configfileti = TMP_Config.toAscii();
         status = tidyLoadConfig( doc, configfileti.data() );  /* http://ch2.php.net/manual/de/function.tidy-load-config.php */
        if ( status != 0 ) {
         ErrorMSG ="Not possibel to load Config File!";
         return false;
        } else { 
        return true;
        }
    } else {
      ErrorMSG ="Not possibel to load Config File!";
     return false; 
    } 
}



QString TidyExternalHtml( QString body )
{
    QString prehtml = TidyCleanhtml(body);  /* base clean to stay on minimal standard xhtml */
    QStringList notneed;
    notneed.clear();
    ///////////////width="456" lang  class
    QRegExp expression( "width=[\"\'](.*)[\"\']", Qt::CaseInsensitive );  /* table td tr width image amen */
    QRegExp expression2( "style=[\"\'](.*)[\"\']", Qt::CaseInsensitive );
    QRegExp expression3( "lang=[\"\'](.*)[\"\']", Qt::CaseInsensitive );
    QRegExp expression4( "class=[\"\'](.*)[\"\']", Qt::CaseInsensitive );
    
    
    expression.setMinimal(true);
    expression2.setMinimal(true);
    expression3.setMinimal(true);
    expression4.setMinimal(true);
    
    int iPosition = 0;
    while( (iPosition = expression.indexIn( prehtml , iPosition )) != -1 ) {
        QString semi1 = expression.cap( 1 );
        notneed.append(QString("width=\"%1\"").arg(semi1));
        notneed.append(QString("width='%1'").arg(semi1));
        iPosition += expression.matchedLength();
    }
    
    iPosition = 0;
       while( (iPosition = expression2.indexIn( prehtml , iPosition )) != -1 ) {
        QString semi2 = expression2.cap( 1 );
        notneed.append(QString("style=\"%1\"").arg(semi2));
        notneed.append(QString("style='%1'").arg(semi2));
        iPosition += expression2.matchedLength();
    }
    
    iPosition = 0;
       while( (iPosition = expression3.indexIn( prehtml , iPosition )) != -1 ) {
        QString semi3 = expression3.cap( 1 );
        notneed.append(QString("lang=\"%1\"").arg(semi3));
        notneed.append(QString("lang='%1'").arg(semi3));
        iPosition += expression3.matchedLength();
    }
    
     iPosition = 0;
       while( (iPosition = expression4.indexIn( prehtml , iPosition )) != -1 ) {
        QString semi4 = expression4.cap( 1 );
        notneed.append(QString("class=\"%1\"").arg(semi4));
        notneed.append(QString("class='%1'").arg(semi4));
        iPosition += expression4.matchedLength();
    }
    
    for (int i = 0; i < notneed.size(); ++i)  {
          const QString fluteremove = notneed.at(i);
          prehtml = prehtml.replace(fluteremove,"", Qt::CaseInsensitive );
    }
    
    return prehtml;
}

QString TidyCleanhtml( QString body )
{
    QString newbody;
    bool ok = false;;
    int processid = qrand() % 10000;
    QString sucleanfile = QString( "%1tidy_tmp_%2_in.xml" ).arg( TMP_Dir ).arg(processid);
    QString sucleanfileout = QString( "%1tidy_tmp_%2_out.xml" ).arg( TMP_Dir ).arg(processid);
    
           ok = file_put_utf8contents(sucleanfile,body);
    
    if (!ok) {
    return body;
    } else {
    SetUp_xhtml();
    CleanTidy(sucleanfile,sucleanfileout);
    newbody =  Readutf8File(sucleanfileout);
        
        
        qt4unlink(sucleanfile);
        qt4unlink(sucleanfileout);
        
            if (newbody.size() > 2) {
            return newbody;
            } else {
            return body;   
            }
    }        
}



QString TidyCleanPage( QString body )
{
    QString newbody;
    bool ok = false;;
    int processid = qrand() % 10000;
    QString sucleanfile = QString( "%1tidy_tmppa_%2_in.xml" ).arg( TMP_Dir ).arg(processid);
    QString sucleanfileout = QString( "%1tidy_tmppa_%2_out.xml" ).arg( TMP_Dir ).arg(processid);
    
           ok = file_put_utf8contents(sucleanfile,body);
    
    if (!ok) {
    return body;
    } else {
    SetUp_xhtml_full_page();
    CleanTidy(sucleanfile,sucleanfileout);
    newbody =  Readutf8File(sucleanfileout);
        
        
        qt4unlink(sucleanfile);
        qt4unlink(sucleanfileout);
        
            if (newbody.size() > 2) {
            return newbody;
            } else {
            return body;   
            }
    }        
}










/* remove a file */
bool qt4unlink(QString fullFileName)
{
    if (fullFileName.size() > 0) {
    QFile f( fullFileName );
	if ( f.exists(fullFileName) ) {
       if (f.remove()) {
        return true;
       }
	}
    }
return false;
}

QString TidyCleanxml( QString body )
{
    QString newbody;
    bool ok = false;
    int processid = qrand() % 10000;
    QString sucleanfile = QString( "%1tidy_tmp_%2_in.xml" ).arg( TMP_Dir ).arg(processid);
    QString sucleanfileout = QString( "%1tidy_tmp_%2_out.xml" ).arg( TMP_Dir ).arg(processid);
    
    
              ok = file_put_utf8contents(sucleanfile,body);
    
    /* file_put_utf8contents   Readutf8File  */
    if (!ok) {
    return body;
    } else {
    SetUp_xml();
    CleanTidy(sucleanfile,sucleanfileout);
    newbody =  Readutf8File(sucleanfileout);
        
        
        qt4unlink(sucleanfile);
        qt4unlink(sucleanfileout);
        
        
            if (newbody.size() > 2) {
            return newbody;
            } else {
            return body;   
            }
    }        
}



bool Copy_To(QString inputfile, QString outfile)
{
    bool actioncpy = false;
    QFile Imagedi(inputfile);
    QFile Imagedd(outfile);
    if (Imagedd.exists()) {
           Imagedd.remove();
            
               if (!Imagedi.copy(outfile)) { 
                   ErrorMSG ="Not possibel to copy!";         
                } else {
                   actioncpy = true; 
                }
    }
return actioncpy;
}

/*  Readutf8File file_put_utf8contents */
/* read the contenet of a local file as qstring */
QString Readutf8File(QString fullFileName)
{
    QString inside = "";
    QFile file(fullFileName); 
    if (file.exists()) {
                if (file.open(QFile::ReadOnly | QFile::Text)) {
                    inside = QString::fromUtf8(file.readAll());   
                    file.close();
                }
    }

return inside;
}

bool file_put_utf8contents(QString fullFileName,QString xml)
{
    if (fullFileName.contains("/", Qt::CaseInsensitive)) {
    QString ultimacartellaaperta = fullFileName.left(fullFileName.lastIndexOf("/"))+"/";
    QDir dira(ultimacartellaaperta);
    if ( dira.mkpath(ultimacartellaaperta) ) { } else {
    return false;
    }
    }
    
        QTextCodec *codecx;
        codecx = QTextCodec::codecForMib(106);
        QFile f( fullFileName );
        if ( f.open( QFile::WriteOnly | QFile::Text ) )
        {
        QTextStream sw( &f );
        sw.setCodec(codecx);
        sw << xml;
        f.close();
        return true;
        }
        return false;
}










/* write the config file */
bool tidy_file_set_config(QString xml)
{
return file_put_utf8contents(TMP_Config,xml);
    /* file_put_utf8contents   Readutf8File  */
} 


/* QString clean file inputfile and put to outfile */
bool CleanTidy(QString inputfile, QString outfile)
{
    /*qDebug() << "### load inputfile....  "  << inputfile;*/
    /*qDebug() << "### load outfile....  "  << outfile;*/
    Bool ok;
    ctmbstr cfgfil = NULL, outputfil = NULL, htmlfil = NULL;
    int rc = 0 - 10;
    int base = 0 - 10;
    int status = 0;
    QByteArray infile = inputfile.toAscii();
    QByteArray outfi = outfile.toAscii();
    htmlfil = infile.data();   /* incomming file entra */
    outputfil = outfi.data();
    rc = tidyParseFile( doc, htmlfil );
   
                if ( outputfil ) {
                    status = tidySaveFile( doc, outputfil );
                    ///////////qDebug() << "### save to out file ... ";
                } else {
                    status = tidySaveStdout( doc );
                }
               
    /* check if file exist if not copy in to out */
    QFile f( outfile );
   if (!f.exists()) {
    //////////qDebug() << "### file out not exist copy in to out... ";
       QFile Imaged(inputfile); 
      if (!Imaged.copy(outfile)) { 
       ErrorMSG ="Not possibel to copy!";         
       }
   } else {
    ///////////qDebug() << "### file found SUCCESS!!!!!!!! bravo ... ";
    return true;
    }
    /* check if file exist if not copy in to out */

return false;
}








};











#endif // QTIDY_H

