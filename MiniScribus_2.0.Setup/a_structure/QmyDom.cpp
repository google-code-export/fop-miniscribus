#include "QmyDom.h"

/* save this file as QmyDom.cpp */
/* constructor */
QmyDom::QmyDom( bool heading )
{
   decode = "utf-8";   /* forever save to utf-8 format */
   header = QDomDocument::createProcessingInstruction( "xml",QString("version=\"1.0\" encoding=\"%1\"" ).arg(decode));
   if (heading) {
   appendChild( header );
   }
}


QDomElement QmyDom::root()
{
   QDomElement root = documentElement();
   return root;
}


QString QmyDom::CleanTagAsText( QString body )
{
    QTextDocument *xhtmldoc = new QTextDocument();
    xhtmldoc->setHtml( body );
    QString wbody = xhtmldoc->toPlainText();
    wbody = wbody.replace("\n"," ");
    wbody = wbody.replace("\r","");
    wbody = wbody.replace("\t"," ");
 return wbody.trimmed();
} 

QString QmyDom::CleanTagAsHTML( QString body )
{
    QTextDocument *xhtmldoc = new QTextDocument();
    xhtmldoc->setHtml( body );
 return xhtmldoc->toHtml();
} 



bool QmyDom::SetSelf( const QString fullFileName )
{
    QString errorStr;
    int errorLine, errorColumn, position;
    
    QFile f( fullFileName );
	if ( !f.exists(fullFileName) ) {
    return false;   
    }
    if(!f.open(QIODevice::ReadOnly)) {
    return false;
    }
    /* no namen space ! */
    if (!setContent(&f,false, &errorStr, &errorLine, &errorColumn)) {
    return false;
    } else {
    return true; 
    }
    
}


bool QmyDom::SetStream( const QString stream )
{
    QString errorStr;
    int errorLine, errorColumn, position;
    /* no namen space ! */
    if (!setContent(stream,false, &errorStr, &errorLine, &errorColumn)) {
    return false;
    } else {
    return true; 
    }
    
}








/* save doc if is a streamfiele */
bool QmyDom::saveXML()
{
   if (is_file(stream_on_file)) {
   return saveXML(stream_on_file);
   }  
return false;   
}
/* print on cosole the result */
void QmyDom::Print()
{
    QTextStream out(stdout);
    out << QmyDom::toString(); 
}

QDomElement QmyDom::insertTagValue( const QString name ,  QString value )
{
    QDomElement yourtag = QDomDocument::createElement( name );
    ////QDomNode::setNodeValue( value ) ;
    yourtag.appendChild(QmyDom::createTextNode(value));
    return yourtag;
}

void QmyDom::insertFragment( QDomElement e , QString fragment )
{
        bool check;
        QDomDocument dom_external;
        check = dom_external.setContent(QString("<?xml version=\"1.0\"?>\n<dummyroot>%1</dummyroot>").arg(fragment)); 
           if ( check ) {
            QDomElement root_extern = dom_external.documentElement(); /* dummy */
            QDomNode child = root_extern.firstChild();
                   while ( !child.isNull() ) {
                       if ( child.isElement() ) {
                        e.appendChild(importNode(child,true).toElement());
                       }
                     child = child.nextSibling();            
                   }
           } else {
               qDebug() << "### no fragment   " << e.tagName();
           }
    
}

void QmyDom::insertFullFragment( QDomElement e , QString fragment )
{
        bool check;
        QDomDocument dom_external;
        check = dom_external.setContent(fragment); 
           if ( check ) {
            QDomElement root_extern = dom_external.documentElement();
            QDomNode child = root_extern.firstChild();
                   while ( !child.isNull() ) {
                       if ( child.isElement() ) {
                        e.appendChild(importNode(child,true).toElement());
                       }
                     child = child.nextSibling();            
                   }
           } else {
               qDebug() << "### no fragment   " << e.tagName();
           }
    
}

/* Append fragment or a real xml file */
QDomElement QmyDom::insertFragmentorFile( QString fragment )
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
QDomElement QmyDom::ErrorDom()
{
    QDomElement errorelement = QDomDocument::createElement( "error" );
    return errorelement;
}
/* setup a file to stream fragments */
void QmyDom::SetstreamFile( const QString fullFileName )
{
    stream_on_file = fullFileName;
}


QString QmyDom::StringtoXML( QString t )
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
bool QmyDom::saveXML( const QString fullFileName )
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
                sw << QmyDom::toString();
                f.close();
                    if (is_file(fullFileName)) {
                    return true;
                    }
                }
return false;
}


bool QmyDom::saveStream( const QString fullFileName , QString xml )
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
                sw << xml;
                f.close();
                    if (is_file(fullFileName)) {
                    return true;
                    }
                }
return false;
}


/* remove file */
QString QmyDom::GetAtt(QDomElement e , QString name )
{
    QString textvalue;
    QString errorvalue = "";
    QDomAttr a1 = e.attributeNode(name);
    textvalue = a1.value();
    if (textvalue.size() > 0) {
      return textvalue;  
    } else {
      return errorvalue;  
    }
}





/* loop  to find attribute name xx */
QString QmyDom::FilterAttribute( QDomElement element , QString attribute ) 
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
bool QmyDom::is_file(QString fullFileName)
{
    QFile f( fullFileName );
	if ( f.exists() ) {
    return true;
	} else {
	return false;
    }
}
/* remove file */
bool QmyDom::xml_unlink(QString fullFileName)
{
    QFile f( fullFileName );
	if ( is_file( fullFileName ) ) {
       if (f.remove()) {
        return true;
       }
	}
return false;
}







