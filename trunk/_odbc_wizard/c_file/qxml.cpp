#include "qxml.h"

/* save this file as qxml.cpp */
/* constructor */
Qxml::Qxml()
{
   decode = "utf-8"; 
   header = QDomDocument::createProcessingInstruction( "xml",QString("version=\"1.0\" encoding=\"%1\"" ).arg(decode));
   appendChild( header );
}


QDomElement Qxml::root()
{
   QDomElement root = documentElement();
   return root;
}



/* save doc if is a streamfiele */
bool Qxml::saveXML()
{
   if (is_file(stream_on_file)) {
   return saveXML(stream_on_file);
   }  
return false;   
}
/* print on cosole the result */
void Qxml::Print()
{
    QTextStream out(stdout);
    out << Qxml::toString(); 
}

/* Append fragment or a real xml file */
QDomElement Qxml::insertFragmentorFile( QString fragment )
{
    bool check;
    /* if is a file insert dom tree and convert to element */
    if (is_file(fragment)) {
        QFile ext_file(fragment);
        ext_file.open( QIODevice::ReadOnly);
        QDomDocument dom_external;
        check = dom_external.setContent(&ext_file); 
           if ( check ) {
            QDomElement root_extern = dom_external.documentElement(); 
            QDomNode newnode = root_extern.firstChild();
            QDomNode treallsub = dom_external.importNode(newnode,true);
            ext_file.close(); 
            return treallsub.toElement(); 
           } else {
            ext_file.close(); 
            return ErrorDom();
           }               
    } else {
    /* is not a file */
    QTextCodec *setcurrentcodec;
    if (decode == "utf-8") {
    setcurrentcodec = QTextCodec::codecForMib(106);  /* utf8 */  
    } else {
    setcurrentcodec = QTextCodec::codecForMib(4);  /* latin iso */
    }
        if (stream_on_file.size() > 0) {
                QFile f( stream_on_file );
                if ( f.open( QFile::WriteOnly | QFile::Text ) ) {
                QTextStream sw( &f );
                sw.setCodec(setcurrentcodec);
                sw << QString("<?xml version=\"1.0\"?>\n<dummyroot>%1</dummyroot>").arg(fragment);
                f.close();
                    if (is_file(stream_on_file)) {
                    return insertFragmentorFile(stream_on_file);
                    }
                }
         } 
    return ErrorDom();
    }
}
/* return on error an error tag */
QDomElement Qxml::ErrorDom()
{
    QDomElement errorelement = QDomDocument::createElement( "error" );
    return errorelement;
}
/* setup a file to stream fragments */
void Qxml::SetstreamFile( QString sfile )
{
    stream_on_file = sfile;
}


QString Qxml::StringtoXML( QString t )
{
  //the following checks are necessary before exporting
  //strings to XML. see http://hdf.ncsa.uiuc.edu/HDF5/XML/xml_escape_chars.html for details
  QString text = t;
  text.replace("&", "&amp;");   /*  qt4 toUtf8 dont replace && */
  text.replace("\"","&quot;");
  text.replace("'", "&apos;");
  text.replace("<", "&lt;");
  text.replace(">", "&gt;");
  text.replace("\n", "&#10;");
  text.replace("\r", "&#13;");
  return text;
}
/* save to a specific file */
bool Qxml::saveXML(QString fullFileName)
{
    QTextCodec *setcurrentcodec;
    if (decode == "utf-8") {
    setcurrentcodec = QTextCodec::codecForMib(106);  /* utf8 */  
    } else {
    setcurrentcodec = QTextCodec::codecForMib(4);  /* latin iso */
    }
        QFile f( fullFileName );
                if ( f.open( QFile::WriteOnly | QFile::Text ) ) {
                QTextStream sw( &f );
                sw.setCodec(setcurrentcodec);
                sw << Qxml::toString();
                f.close();
                    if (is_file(fullFileName)) {
                    return true;
                    }
                }
return false;
}


/* remove file */
QString Qxml::GetAtt(QDomElement e , QString name )
{
    QString textvalue;
    QString errorvalue = "ERROR";
    QDomAttr a1 = e.attributeNode(name);
    textvalue = a1.value();
    if (textvalue.size() > 0) {
      return textvalue;  
    } else {
      return errorvalue;  
    }
}


/* loop  to find attribute name xx */
QString Qxml::FilterAttribute( QDomElement element , QString attribute ) 
{
    QString base = "error";
    QDomNamedNodeMap attlist = element.attributes();
    int bigint = attlist.count();
    if ( bigint > 0 ) {
        for (int i=0; i<bigint; i++){
             QDomNode nod = attlist.item(i);
             if (nod.nodeName() == attribute) {
                base = QString(nod.nodeValue());
                return base;
             }
        } 
    }
    return base; 
}

/* checker if is file exist */
bool Qxml::is_file(QString fullFileName)
{
    QFile f( fullFileName );
	if ( f.exists() ) {
    return true;
	} else {
	return false;
    }
}
/* remove file */
bool Qxml::xml_unlink(QString fullFileName)
{
    QFile f( fullFileName );
	if ( is_file( fullFileName ) ) {
       if (f.remove()) {
        return true;
       }
	}
return false;
}







