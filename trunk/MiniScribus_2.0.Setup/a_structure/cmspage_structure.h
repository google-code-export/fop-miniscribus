#ifndef CMSPAGE_STRUCTURE_H
#define CMSPAGE_STRUCTURE_H


#include "QmyDom.h"
#include "qdocxhtml.h"
#include "richdoc_structure.h"
#include "swisslang_structure.h"
#include "xmlhighlighter.h"
#include "bestqtidy.h"
#include "GhostFunction.h"

#define TSEARCH "SEARCHPAGE"
#define LITESEARCH "SEARCHLOCAL"

#define INDEXNR 50000
#define INDEXLTNR 50001
#define INDEXRINR 50002

#define KEYWORDMAX_NUMMER 200


/* quote and dequote sql s result */
static inline QString G_Quote( QString xml )
{
   //the following checks are necessary before exporting
  //strings to XML. see http://hdf.ncsa.uiuc.edu/HDF5/XML/xml_escape_chars.html for details
   QString text = xml;
   text.replace("\"","\\\"");  /* 228 ``*/
   text.replace("'","`");
   return text;
}

static inline QString G_DeQuote( QString xml )
{
   //the following checks are necessary before exporting
  //strings to XML. see http://hdf.ncsa.uiuc.edu/HDF5/XML/xml_escape_chars.html for details
  QString text = xml;
   text.replace("\\\"","\"");  /* 228 ``*/
   text.replace("`","\'");
  return text;
}



static inline QString UriConvert( QString touri )
{
    touri = touri.toLower();
    touri = touri.replace(" ","_");
    QStringList piece = touri.split("");
    QString alloweduri = "abcdefghijklmnopqrstuvwxyz1234567890_";
    QStringList alist = alloweduri.split("");
    
     for (int i = 0; i < piece.size(); ++i) {
           QString pin = piece.at(i);
           if (!alist.contains(pin)) {
               touri.replace(pin,"");
           }
       }
    
    return touri;
}





static inline QString datetr(QString form, uint unixtime )
{
     QDateTime fromunix;
     fromunix.setTime_t(unixtime); 
     return fromunix.toString((const QString)form);
}


static inline QString MicroQuote( QString xml )
{
    /////QString bole = "";
   QString text = xml;
   text.replace("\"","\\\"");  /* 228 ``*/
   text.replace("'","`");
   text.replace("\t"," ");
   text.replace("\n"," ");
   text.replace("\r"," ");
   return text;
}


static inline QString KeywordGenerator( QString xml )
{
    QString stream = MicroQuote(xml);
    QStringList eachword = stream.split(" ");
    QStringList metakey;
    metakey.clear();
    for (int i=0;i<eachword.size();i++) {
        QString now = eachword.at(i);
        now = now.toLower();
        /*  && !now.contains(".")  */
        if (now.size() > 3
            && !now.contains(",")
            && !now.contains(":") 
            && !now.contains("&") 
            && !now.contains("=")
            && !now.contains("|")
            && !now.contains(")")
            && !now.contains("(")
            && !now.contains("#")
            && !now.contains("@")
            && !now.contains("*")
            && !now.contains("-")
            && !now.contains("`")
            && !now.contains("_")
            && !now.contains("|")
            && !now.contains("\\")
            && !now.contains(";")
            && !metakey.contains(now,Qt::CaseInsensitive)) 
        {
        metakey.append(now);
        }
        if (metakey.size() > KEYWORDMAX_NUMMER) {
        return metakey.join(" , ");
        }
    }
    
return metakey.join(" , ");  
}


static inline QString GrabRichTxt( QStringList item  )
{ 
    QString  htmls;
    QByteArray stream; 
     for (int i = 0; i < item.size(); ++i)  {
         RichDoc dd = OpenRichDoc(item.at(i));
         stream.append(dd.html);
     }
    return QString(stream);
}


static inline QString DataPathServer( uint filenummer )
{ 
    QString base_0 = QString("data/%1/").arg(datetr("yyyy",filenummer) );
    QString base_1 = QString("%1%2/").arg( base_0).arg(datetr("MM",filenummer) );
    QString base_2 = QString("%1%2/").arg( base_1 ).arg(datetr("dd",filenummer));
    QString base_3 = QString("%1%2/").arg( base_2).arg(filenummer);
    return base_3;
}

static inline QString hantext(QString inside )
{
    if (inside.size() > 0) {
        QTextDocument *doc = new QTextDocument();
				doc->setHtml(inside);
        QString ddd = doc->toPlainText().trimmed();
        ddd = ddd.replace("\n"," ");
        ddd = ddd.replace("\t"," ");
        ddd = ddd.replace("\r"," ");
        ddd = ddd.replace("@"," a#t ");
        return ddd;
    }
   return inside; 
}

static inline QString handletextplain(QString fullFileName )
{
    QByteArray streams;
    QFile f(fullFileName);
    if (f.open(QIODevice::ReadOnly)) {
    streams = f.readAll();
    f.close();
    }
    streams = streams.replace("’","'");
    streams = streams.replace("`","'");
    streams = streams.replace("–","-");
    streams = streams.replace("”","\"");
    streams = streams.replace("“","\"");
    streams = streams.replace("…","...");
    QString inside = QString(streams);
    
    
    
    if (inside.size() > 0) {
        QTextDocument *doc = new QTextDocument();
				doc->setHtml(inside);
        QString ddd = doc->toPlainText().trimmed();
        ddd = ddd.replace("\n"," ");
        ddd = ddd.replace("@"," a#t ");
        return ddd;
    }
   return inside; 
}




class CMSDoc
{
    public:
    enum { MAGICNUMBER = 0xFAAAFFAA, VERSION = 2 };
    CMSDoc();
    void openfile( const QString file );
    void savefile_small(  QString fullFileName  );
    void openfile_small(  QString fullFileName  );
    void Load_Stream( const QByteArray xmlextern );
    void openfile_small(  QDomDocument doc  );
    void RewriteFrominit();
    inline SwissLang get_small() 
    {
    DD_Page.save();        
    return DD_Page; 
    }
    QDomElement Page( QString languagetag );
    QVariant Root_Tag( const QString tag );
    QString  title( QString languagetag );
    QString  desc( QString languagetag );
    QString  notes( QString languagetag );
    QString  xhtml( QString languagetag );
    void RegisterRdoc( QString languagetag );
    RichDoc appendItem( const QDomElement e );
    operator QVariant() const; 
    QString LaTr( QString languagetag );
    
/* must set to write */
QString globaltitle;
QString preurl;
QStringList lasquery;   
QStringList localquery;
QString tmpstream;
QStringList liteup;
    
/* must set to write */
QTidy *cleaner;
QDomElement root;
QDomElement root1;
QDomDocument doc;
QmyDom doc1;
QMap<QString,QDomElement> pager;
QMap<QString,QDomDocument> inxhtml;
SwissLang DD_Page;
QStringList langs;
QStringList lshort;
QString bgcolor;
QString spath;
QString uripage;
bool complete;
int cat;
int topid;
int active;
int album;
uint epoche;
uint start_t;
uint stop_t;
int worksource;
};
 
inline QDebug operator<<(QDebug debug, const CMSDoc& tline)
{
	debug.nospace() << "Nuovo()" 
		    << tline.epoche << "," 
        << tline.active << "," 
        << tline.worksource << ",cat()" 
        << tline.cat << "," 
        << tline.topid << ","
        << tline.bgcolor << ",uripage()"    
        << tline.uripage << ","   
        << tline.spath << ")";
	return debug.space();
}


Q_DECLARE_METATYPE(CMSDoc);


  









//
#endif // CMSPAGE_STRUCTURE_H

