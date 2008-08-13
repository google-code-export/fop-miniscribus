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

Xslt_Convert::~Xslt_Convert()
{
   EndingJob = true;
   qDebug() << "### Destructor ..........................  ->" << UmanTimeFromUnix(QTime_Null());
   qDebug() << "### EndingJob ..........................  ->" << EndingJob;
   qt_unlink(XMLERROR_FILE);  /* remove debug or error message from last */
}

Xslt_Convert::Xslt_Convert( const QString xmlfile , const QString xsltfile ,  QMap<QString,QString> paramsetting )
 :EndingJob(false),xmlcodec(0),xsltcodec(0)
{
    ////////qDebug() << "### Start Xslt_Convert ...................  ->" << UmanTimeFromUnix(QTime_Null());
    qt_unlink(XMLERROR_FILE);  /* remove debug or error message from last */
    QTimer::singleShot(1000, this, SLOT(CheckError()));
    QFile *xfile = new QFile( xmlfile );
    if (!xfile->exists()) {
    emit ErrorMsg(QString("File %1 not exist!").arg(xmlfile));
    return;
    }
    QFile *sfile = new QFile( xsltfile );
    if (!sfile->exists()) {
    emit ErrorMsg(QString("File %1 not exist!").arg(xsltfile));
    return;
    }
    xmlcodec = GetcodecfromXml(xmlfile);
    xsltcodec = GetcodecfromXml(xsltfile);
  
    qDebug() << "### codec ..........................  " << xmlcodec->mibEnum();
    qDebug() << "### codec ..........................  " << xmlcodec->name();
    qDebug() << "### codec ..........................  " << xsltcodec->mibEnum();
    qDebug() << "### codec ..........................  " << xsltcodec->name();
  
    const QString maildate = QString("\"%1\"").arg(UmanTimeFromUnix(QTime_Null()));
    QByteArray param1 = maildate.toAscii();
    const QString unixtime = QString("\"%1\"").arg(QTime_Null());
    QByteArray param2 = unixtime.toAscii();
    /////qDebug() << "### catnummer ..........................  " << param1;
    /////qDebug() << "### catnummer1 ..........................  " << param2;
    const int totparams = paramsetting.size() * 2 + 4;
    //////qDebug() << "### totparams ..........................  " << totparams;
    const char* params[totparams];  
    params[0] = "NowTime";
    params[1] = param1.data();
    params[2] = "UnixTime";
    params[3] = param2.data();
    int loop = 4;
  
         QMapIterator<QString,QString> i(paramsetting);
         while (i.hasNext()) {
             i.next();
             QString name = i.key();
             qDebug() << "### name  " << name << loop;
             params[loop] = "done";
             params[loop + 1 ] = qPrintable(i.value());
             loop = loop + 2;
         }
    QTemporaryFile recfile;
    recfile.setAutoRemove (true);
    QString resulterxml = recfile.fileTemplate();
    
    /* ######################################### */
         xsltStylesheetPtr cur = NULL;
         xmlDocPtr doc, outputDoc;
         xmlSubstituteEntitiesDefault(1);
         xmlLoadExtDtdDefaultValue = 1;
    /* ######################################### */
        char* xslt_errors;
        xsltSetGenericErrorFunc(&xslt_errors, qt_libxml_error_handler);
        xmlSetGenericErrorFunc(&xslt_errors, qt_libxml_error_handler);
        xsltSetGenericDebugFunc(&xslt_errors, qt_libxml_error_handler);
        QByteArray gocharxslt = QFile::encodeName(xsltfile); 
        cur = xsltParseStylesheetFile( (const xmlChar*)gocharxslt.data() );
        doc = xmlParseFile( QFile::encodeName(xmlfile) );
        outputDoc = xsltApplyStylesheet(cur, doc, params);
        xmlFreeDoc( doc ); /* free ram from xml! */
        doc = outputDoc; /* swap input and output */
        FILE* outfile = fopen( QFile::encodeName( resulterxml ), "w" );
        xsltSaveResultToFile( outfile, doc, cur );
        fclose( outfile );
        xsltFreeStylesheet(cur);
        xmlFreeDoc( outputDoc );
        xsltCleanupGlobals();
        xmlCleanupParser();
        qDebug() << "### resulterxml ..........................  " << resulterxml;
        QFile file(resulterxml); 
        if (file.exists()) {
                if (file.open(QIODevice::ReadOnly)) {
                    Rstream = file.readAll();
                    file.close();
                }
        }
        debug_msg = ReadFileUtf8Xml(XMLERROR_FILE);
        qDebug() << "### resulterxml ..........................  " << Rstream;
        qt_unlink(XMLERROR_FILE); 
        EndingJob = true;
        emit DebugMsg(debug_msg);

}



void Xslt_Convert::CheckError()
{

    qDebug() << "### CheckError ..........................  ->" << UmanTimeFromUnix(QTime_Null());
    qDebug() << "### EndingJob ..........................  ->" << EndingJob;

}


