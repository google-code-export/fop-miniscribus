#include "xslt_convert.h"
//
/*  Save file as xslt_convert.cpp  */
/*  incomming class name Xslt_Convert */
//



void qt_libxml_error_handler(void *ctx, const char *msg, ...)
{
    va_list args;
    QString message;
    QStringList errorlined;
    errorlined.clear();
    int size = 256;
    char * buf = new char[ size ];

    while( 1 ) {
       va_start(args, msg);
       int retval = ::vsnprintf( buf, size, msg, args );
       va_end(args);
     
       if( -1 < retval && retval < size ) { // everything was OK
          message = buf;
           if (message.size() > 0) {
           message.replace("&", "&amp;"); 
           errorlined.append(message);  
            /*qDebug() << "### 1 error captured to insert on class as list or so.... " << ;*/
           }
          break;
       }
       else if( retval > -1 ) { // buffer too small
           size = retval + 1;
           delete [] buf;
           buf = new char[ size ];
       }
       else {  // error
          // ...
          break;
       }
    }

    delete [] buf;
      QFile f( XMLERROR_FILE );
	if ( f.open( QFile::Append | QFile::Text ) )
	{
		QTextStream sw( &f );
		sw << errorlined.join("");
		f.close();
	}
}


/* read the contenet of a local file as qstring */
QString ReadFileUtf8Xml( const QString fullFileName )
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







//
Xslt_Convert::Xslt_Convert()
{
    //////qt_unlink(XMLERROR_FILE);
}
//
void Xslt_Convert::Convert()
{
    if (XML_file.size() < 1) {
    emit ErrorMsg(tr("XML file is not set!"));
    return;
    }
    if (XSLT_file.size() < 1) {
    emit ErrorMsg(tr("XML file is not set!"));
    return;
    }
    
    if (!is_file(XML_file)) {
    emit ErrorMsg(tr("XML file \"%1\" not found!").arg(XML_file));   
    return;
    }
    if (!is_file(XSLT_file)) {
    emit ErrorMsg(tr("XSLT file \"%1\" not found!").arg(XSLT_file));   
    return;
    }
    
    QString resulterxml = XMLRESULT_FILE;
    QString catnummer = QString("\"%1\"").arg(UmanTimeFromUnix(QTime_Null()));
    QByteArray nummero = catnummer.toAscii();
    QString catnummer1 = QString("\"%1\"").arg(QTime_Null());
    QByteArray nummero1 = catnummer1.toAscii();
    QString success = "";
    QStringList nam_M;
    QStringList val_M;
    
    int caseparam = nam_M.size();
    bool eset = false;
    const char* paramsa[caseparam];
    const char* params[6];  
    if (caseparam > 0 && nam_M.size() == val_M.size() ) {
         eset = true;
         
         int numerise = -1;
         for (int i = 0; i < caseparam; ++i)  {
           numerise++;
           QByteArray na = nam_M.at(i).toAscii();    
           QByteArray va = val_M.at(i).toAscii();  
               paramsa[numerise] = na.data();
                      numerise++;
               paramsa[numerise] = va.data();
             
         }
          

        
    } else {
     
    params[0] = "NowTime";
    params[1] = nummero.data();
    params[2] = "UnixTime";
    params[3] = nummero1.data();
    params[4] = NULL;
    params[5] = NULL;  
    }
    
    
    
    
    
    
    /* ######################################### */
         xsltStylesheetPtr cur = NULL;
         xmlDocPtr doc, outputDoc;
         xmlSubstituteEntitiesDefault(1);
         xmlLoadExtDtdDefaultValue = 1;
    /* ######################################### */
        char* xslt_errors;
        qt_unlink(XMLERROR_FILE);
        xsltSetGenericErrorFunc(&xslt_errors, qt_libxml_error_handler);
        xmlSetGenericErrorFunc(&xslt_errors, qt_libxml_error_handler);
        xsltSetGenericDebugFunc(&xslt_errors, qt_libxml_error_handler);
        QByteArray gocharxslt = XSLT_file.toAscii();
        cur = xsltParseStylesheetFile( (const xmlChar*)gocharxslt.data() );
        doc = xmlParseFile( QFile::encodeName(XML_file) );
        if (eset) {
        outputDoc = xsltApplyStylesheet(cur, doc, paramsa);
        } else {
        outputDoc = xsltApplyStylesheet(cur, doc, params);
        }
        xmlFreeDoc( doc ); /* free ram from xml! */
        doc = outputDoc; /* swap input and output */
        FILE* outfile = fopen( QFile::encodeName( resulterxml ), "w" );
        xsltSaveResultToFile( outfile, doc, cur );
        fclose( outfile );
        xsltFreeStylesheet(cur);
        xmlFreeDoc( outputDoc );
        xsltCleanupGlobals();
        xmlCleanupParser();
    
        QString rxml = fopenutf8(XMLRESULT_FILE);
        
        if (!rxml.contains("utf-8",Qt::CaseInsensitive)) {
            rxml = fopeniso(XMLRESULT_FILE);
        }
        
        
        const QString rdedu = fopenutf8(XMLERROR_FILE);
        LAST_RESUL = rxml;
        qt_unlink(XMLERROR_FILE);
        emit Result(rxml,rdedu);  
}


