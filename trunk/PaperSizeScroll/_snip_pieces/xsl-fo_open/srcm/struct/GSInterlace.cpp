#include "GSInterlace.h"



/* encode to name */
QString encodeBase64( QString xml )
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

/* decode to name */
QString decodeBase64( QString xml )
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}

/* read the contenet of a local file as qstring */
QString ReadFile(const QString fullFileName)
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


void OpenDeskBrowser( QUrl loc )
{
    #if defined Q_WS_MAC
    QProcess *m = new QProcess();
    QStringList macs;
    macs << loc.toString();  /* oeffnet der default browser */
    m->startDetached(QString("open") , macs );
    ///////std::cout << "OpenDesktop [" << qPrintable(macs.join(" ")) << "]" << std::endl;
    return;
    #endif 
    
#if defined Q_WS_WIN
    
    QString fullFileName = loc.toString();
    
    if (fullFileName.startsWith("http://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("https://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("ftp://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("news://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("mailto:", Qt::CaseInsensitive) ||
        fullFileName.startsWith("webdav://", Qt::CaseInsensitive)  )
       { 
         /* all ok */
       } else {
         fullFileName.prepend("file:///");  
       }
    bool wr = QDesktopServices::openUrl(QUrl(fullFileName));
    if (!wr) {
    QMessageBox::warning(0, qApp->tr("Error"),qApp->tr("Window Unable to open action file or dir  %1").arg(loc.toString()));
    }
return;
#endif
    
    /* linux */

     bool r = QDesktopServices::openUrl(loc);
    if (!r) {
    QMessageBox::warning(0, qApp->tr("Error"),qApp->tr("Linux Unable to open action file or dir  %1").arg(loc.toString()));
    }
    
}




/* simple mkdir */
bool Cache( const QString dirpath )
{
    QDir dira(dirpath);
    if ( dira.mkpath(dirpath) ) {
    return true;
    } else {
    return false;
    }
}
/* os path from sistem */
QString PathConvert( QString path ) 
{
    return QDir::toNativeSeparators ( path ); 
}

/* write a file to utf-8 format */
bool fwriteutf8( const QString file ,QString xml)
{
    if (file.contains("/", Qt::CaseInsensitive)) {
    QString ultimacartellaaperta = file.left(file.lastIndexOf("/"))+"/";
    QDir dira(ultimacartellaaperta);
    if ( dira.mkpath(ultimacartellaaperta) ) { } else {
    return false;
    }
    }
    
        QTextCodec *codecx;
        codecx = QTextCodec::codecForMib(106);
        QFile f( file );
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



QString getGSLinuxPath( QString apps  )
{
    QStringList potential_paths;
    potential_paths.append("/usr/local/bin");
    potential_paths.append("/sw/bin");   /* to use on mac as same */
    potential_paths.append("/opt/bin");
    QProcess *process = new QProcess(NULL);
    process->setReadChannelMode(QProcess::MergedChannels);
    QStringList env = process->systemEnvironment();
    env.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive), "PATH=\\1;"+potential_paths.join(";"));
    process->setEnvironment(env);
    
        process->start( QString("which") ,  QStringList() << apps , QIODevice::ReadOnly );
          if (!process->waitForFinished()) {
          return QString();
          } else {
              QString finder = process->readAll().trimmed();
              if (finder.endsWith(apps,Qt::CaseInsensitive)) {
                 ///////////// qDebug() << "### finder " <<  finder;
                return finder;  
              } else {
                return QString(); 
              }
          }
}



/* find gpl GhostScript path or exe */
QString getGSDefaultExeName()
{
	QString gsName;
    QString gVersion;
#if defined Q_WS_WIN
    QFileInfo Pinfo;
	// Try to locate GhostScript thanks to the qsetting
	 gsName = "gswin32c.exe";
     QSettings softs("HKEY_LOCAL_MACHINE\\Software",QSettings::NativeFormat);
     QStringList allsoftware = softs.childGroups();
     QStringList gsonly = allsoftware.filter(QRegExp("Ghostscript"));
     //////////qDebug() << "### gsonly " <<  gsonly;
         for (int i = 0; i < gsonly.size(); ++i) {
              const QString RealName = gsonly.at(i);  /* realpath */
                      if (RealName.contains("Ghostscript")) {
                       //////////qDebug() << "### soft " <<  RealName;
                       for(int e=1;e<99;e++)  {
                           /* check version 8 ++ 99 down */
                           gVersion = QString("8.%1").arg(100 - e);
                           if (softs.value(RealName+"/"+gVersion+"/GS_DLL").toString().size() > 6 ) {
                               Pinfo.setFile(softs.value(RealName+"/"+gVersion+"/GS_DLL").toString());
                               return gsName.prepend(Pinfo.absolutePath()+"/");
                           }
                           /* check version 7 ++ 99 down */
                           gVersion = QString("7.%1").arg(100 - e);
                           if (softs.value(RealName+"/"+gVersion+"/GS_DLL").toString().size() > 6 ) {
                               Pinfo.setFile(softs.value(RealName+"/"+gVersion+"/GS_DLL").toString());
                               return gsName.prepend(Pinfo.absolutePath()+"/");
                           }
                       }
               }
         }
    
    /* win not having GPL Ghostscript ! */
    gsName = "";  
    return gsName;         
#endif
#if defined Q_WS_MAC
         
         QString pathfinder = getGSLinuxPath(QString("gs"));  /* which  simple append systems path mac + linux */
         if (pathfinder.size() > 2 && pathfinder.endsWith("gs",Qt::CaseInsensitive)) {
          return pathfinder;
         }
         
     QStringList paths;
     paths << "/usr/local/bin/gs" << "/usr/bin/gs" << "/bin/gs" << "/sw/bin/gs" << "/opt/bin/gs";
         QFileInfo gsp; 
          for (int i = 0; i < paths.size(); ++i)  {
                  gsp.setFile (paths.at(i));
                  if (gsp.exists()) {
                   return paths.at(i);
                  }                      
          }
	gsName = "gs";
    return gsName;
#endif
#if defined Q_WS_X11
    return getGSLinuxPath(); 
#endif
         /* forum http://www.qtcentre.org/forum/f-qt-programming-2/t-qsettings-read-only-avaiable-10254.html */
}


/* find gpl GhostScript version  */
double getGSVersion()
{
        QProcess process;
        QString Stversion;
        bool ok;
        double GS_version = 0.00;
        QRegExp regExp1(" (\\d+)\\.(\\d+).*");
        process.setReadChannelMode(QProcess::MergedChannels);
        process.start( getGSDefaultExeName() ,  QStringList() << "-v");
          if (!process.waitForFinished()) {
          return GS_version;
          } else {
              QString pu = process.readAll();
              QStringList line1 = pu.split("\n");
              int pos1 = regExp1.indexIn(line1.at(0));  /* only first line */
              if (pos1 > 0) {
                        Stversion = QString("%1.%2").arg(regExp1.cap(1).toInt()).arg(regExp1.cap(2).toInt());
                        double GS_version = Stversion.toDouble(&ok);
                         if (ok && GS_version > 5) {
                         return GS_version;
                         }
              } else {
              return GS_version;
              }
          }
return GS_version;
}

double JavaVersion()
{
        QProcess process;
        QString Stversion;
        bool ok;
        double js_version = 0.00;
        QRegExp regExp1(" (\\d+)\\.(\\d+).*");
        process.setReadChannelMode(QProcess::MergedChannels);
        process.start( "java" ,  QStringList() << "-version");
          if (!process.waitForFinished()) {
          return js_version;
          } else {
              QString pu = process.readAll();
              /////QStringList line1 = pu.split("\n");
              ////////qDebug() << "### jsversion " << pu;
              int pos1 = regExp1.indexIn(pu);  /* only first line */
              if (pos1 > 0) {
                        Stversion = QString("%1.%2").arg(regExp1.cap(1).toInt()).arg(regExp1.cap(2).toInt());
                        double js_version = Stversion.toDouble(&ok);
                         if (ok && js_version > 1) {
                         return js_version;
                         }
              } else {
              return js_version;
              }
          }
return js_version;
}

/*
#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())
 remove dir after works 
*/

int callGS( const QStringList args )
{
   const QString startnow = QDir::currentPath();
   const QString GhostScriptPath = getGSDefaultExeName();
   QDir::setCurrent(_GSCACHE_);
   QString  cmd1 =  GhostScriptPath + " ";
   cmd1 += args.join(" ");
   int fax = -1;
   #if defined Q_WS_MAC
   fax = system(cmd1.toLocal8Bit());
   QDir::setCurrent(startnow);
   return fax;
   #endif
   QProcess *process = new QProcess(NULL);
   process->setReadChannelMode(QProcess::MergedChannels);
   process->start( GhostScriptPath ,  args , QIODevice::ReadOnly );
          if (!process->waitForFinished()) {
           fax = -1;
          } else {
            QString ghostcomment = process->readAll().trimmed();
            //////qDebug() << "ghostcomment-> " << ghostcomment; 
            fax = 0;
          }
          
   QDir::setCurrent(startnow);
   return fax;
}


/*
extern inline int Funzionaaaaaaa_______lGS( const QStringList args )
{
   const QString startnow = QDir::currentPath();
   QDir::setCurrent(_GSCACHE_);
   QString  cmd1 = getGSDefaultExeName() + " ";
   cmd1 += args.join(" ");
   int fax = -1;
   fax = system(cmd1.toLocal8Bit());
   QDir::setCurrent(startnow);
   return fax;
}
*/


QPixmap LoadPS( QString fn , const QString arguments_append )
{
	const QString pdfFile = PathConvert(fn);
    QChar letter('A' + (qrand() % 26));
    QDateTime timer1( QDateTime::currentDateTime() );
    const QString qttmpFile = _GSCACHE_+QString("%2_%1.png").arg( timer1.toString("yyyy-MM-dd-HH-mm-ss-zzz") ).arg(letter);
    QFileInfo fitmp(qttmpFile);
    const int VersionGS = getGSVersion();

    QFile lastaction(fitmp.absoluteFilePath());
    lastaction.remove();
    
	QPixmap pm;
	int ret = -1;
	QStringList args;
    
    if (arguments_append.size() > 3) {
    args.append(arguments_append); 
    }
    
    if (VersionGS >=8) {
    args.append("-sDEVICE=png16m");
    args.append("-dGraphicsAlphaBits=4");
    args.append("-r72");
    args.append("-o");
	args.append(fitmp.fileName());
    args.append(pdfFile);
    } else {
    args.append("-sDEVICE=pnggray");
    args.append("-r72x72");
    args.append("-sOutputFile="+fitmp.fileName());
    args.append("-q");
    args.append(pdfFile);
    }
	ret = callGS(args);
    //////////qDebug() << "### ret " << ret << " VersionGS->" << VersionGS;
	if (ret == 0)
	{
		   QPixmap tmpimage(fitmp.absoluteFilePath());
           lastaction.remove();
           return tmpimage;
	}
	return pm;
}




    /*
    version < 8
    args.append("-r72x72");
    args.append("-sOutputFile="+imageiosFileCache);
    args.append("-q");
    args.append(pdfFile);
    */




QPixmap LoadPDF(QString fn, int Page, int w )
{
	QString tmp, cmd1, cmd2;
	const QString pdfFile = PathConvert(fn);
	const QString tmpFile = PathConvert(QDir::homePath()+"/sctodaytmps.png");
    const QString qttmpFile = QDir::homePath()+"/sctodaytmps.png";
	QPixmap pm;
    tmp.setNum(Page);
	int ret = -1;
	tmp.setNum(Page);
	QStringList args;
    args.append("-sDEVICE=png16m");
    args.append("-r72");
    args.append("-dGraphicsAlphaBits=4");
    args.append("-o");
	args.append(tmpFile);
    args.append("-dFirstPage="+tmp);
	args.append("-dLastPage="+tmp);
	args.append(pdfFile);
	ret = callGS(args);
    ////////qDebug() << "### ret " << ret;
	if (ret == 0)
	{
		QPixmap tmpimage(qttmpFile);
        QPixmap penna = tmpimage.scaledToWidth(w);
        tmpimage.detach(); 
        QFile lastaction(qttmpFile);
        lastaction.remove();
        QPainter p;
		p.begin(&penna);
		p.setBrush(Qt::NoBrush);
		p.setPen(QPen(QBrush(Qt::black),2,Qt::SolidLine));
		p.drawRect(0, 0, penna.width(), penna.height());
		p.end();
        return penna;
	}
	return pm;
}





