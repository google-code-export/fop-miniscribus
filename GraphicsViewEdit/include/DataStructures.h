#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QtGui>
#include <QtCore>


#include <stdlib.h>
#if defined Q_WS_WIN
#include <windows.h>
#include <ddeml.h>
#endif
#include <QtCore>
#include <QSettings>
#include <QApplication>

#include <QPixmap>

#define _DRAWMODUS_WEB_ 1

#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())    /* gs tmp */
             

#include "pics_structure.h"
#include "richdoc_structure.h"


#include <QPixmap>   /* find dpi actual to faktor 72 */

/* 
  default translate units to 72 DPI find difference 
  faktor to real DPI !
  #define INCH_TO_POINT(inch) ((inch)*72.0)  
  #include <QPixmap>
 */
 
/* fix from Peter Hohl to make image running on dpi unit! */
static inline qreal DPIactualDiff()
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    int hi = qMax(pixmap.logicalDpiY(),72);
    int mi = qMin(pixmap.logicalDpiY(),72);
    if (hi == mi) {
    return 1; /* no difference increase or degrease 1:1 running 72 DPI  */
    }
    const qreal precision = 0.09741;   /* gimp precision to image */
    ////////return 1;   /* 1:1 to test */
    int diff = hi - mi;
    qreal increments = 1.;
    if (mi == 72) {  //////  more >>>
    return increments + (diff / 100.0) + precision;
    } else {   ///////////// down <<<
    return increments - (diff / 100.0) + precision;
    }
}

#define PDIFIX(ii) ((ii)*DPIactualDiff())
#define BDIFIX(ii) ((ii)/DPIactualDiff())







#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_PX(px) ((px)*1.3333333)   /* error dpi problem */
#define POINT_TO_MM(mm) ((mm)/2.83465058)
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)
#define POINT_TO_PI(pi) ((pi)/12)
#define POINT_TO_DD(dd) ((dd)/154.08124)
#define POINT_TO_CC(cc) ((cc)/12.840103)

#define PX_TO_POINT(px) ((px)/1.3333333)  /* error dpi problem */
#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
#define PI_TO_POINT(pi) ((pi)*12)
#define DD_TO_POINT(dd) ((dd)*154.08124)
#define CC_TO_POINT(cc) ((cc)*12.840103)



static inline QPixmap BruschChess( qreal chesswidth )  
{
    QPixmap m_tile = QPixmap(chesswidth,chesswidth);
    qreal piece = chesswidth / 2;
    m_tile.fill(Qt::white);
    QPainter pt(&m_tile);
    QColor color(230, 230, 230);
    pt.fillRect(0, 0, piece, piece, color);
    pt.fillRect(piece, piece, piece, piece, color);
    pt.end();
    return m_tile;
}


static inline qreal FopInt( const QString datain )
{
  QString ctmp = datain;
  const QString data = ctmp.replace(" ","").trimmed();
  //////////qDebug() << "### request unit data->" << datain << " size->" << datain.size();   
  qreal points = 0;
  if (data.size() < 1) {
  return points;
  }
  if (datain == "0") {
  return points;
  }
  if ( data.endsWith( "pt" ))  {
    points = data.left( data.length() - 2 ).toDouble();
    return points;
  }  else if ( data.endsWith( "px" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = PX_TO_POINT( value );
  }  else if ( data.endsWith( "cm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = CM_TO_POINT( value );
  } else if ( data.endsWith( "em" ) ) {
    points = data.left( data.length() - 2 ).toDouble();
  } else if ( data.endsWith( "mm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = MM_TO_POINT( value );
  } else if ( data.endsWith( "dm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = DM_TO_POINT( value );
  } else if ( data.endsWith( "in" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = INCH_TO_POINT( value );
  } else if ( data.endsWith( "inch" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = INCH_TO_POINT( value );
  } else if ( data.endsWith( "pi" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = PI_TO_POINT( value );
  } else if ( data.endsWith( "dd" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = DD_TO_POINT( value );
  } else if ( data.endsWith( "cc" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = CC_TO_POINT( value );
  } else {
    points = 0;
  }
  
  
  return PDIFIX(points);
  
} 


static inline qreal Pointo( qreal unit , const QString unita )  {
    qreal ri = 0;
    if (unita == "cm") {
    ri = POINT_TO_CM( unit );
    } else if (unita == "pt") {
    ri = unit;
    return ri;
        
    } else if (unita == "mm") {
    ri = POINT_TO_MM( unit );
    } else if (unita == "px") {
    ri = POINT_TO_PX( unit );
    } else if (unita == "dm") {
    ri = POINT_TO_DM( unit );
    } else if (unita == "inch") {
    ri = POINT_TO_INCH( unit );
    } else if (unita == "pi") {
    ri = POINT_TO_PI( unit );
    } else if (unita == "dd") {
    ri = POINT_TO_DD( unit );
    }  else if (unita == "cc") {
    ri = POINT_TO_CC( unit );
    } else {
    ri = 10;
    }
    return BDIFIX(ri);
}


static inline qreal ToPoint( qreal unit , const QString unita )  {
    qreal ri = 0;
    if (unita == "cm") {
    ri = CM_TO_POINT( unit );
    } else if (unita == "pt") {
    ri = unit;
    return ri;
    } else if (unita == "mm") {
    ri = MM_TO_POINT( unit );
    } else if (unita == "px") {
    ri = PX_TO_POINT( unit );
    } else if (unita == "dm") {
    ri = DM_TO_POINT( unit );
    } else if (unita == "inch") {
    ri = INCH_TO_POINT( unit );
    } else if (unita == "pi") {
    ri = PI_TO_POINT( unit );
    } else if (unita == "dd") {
    ri = DD_TO_POINT( unit );
    }  else if (unita == "cc") {
    ri = CC_TO_POINT( unit );
    } else {
    ri = 10;
    }
    return PDIFIX(ri);
}


extern inline qreal Metric( const QString datain  ) /////  pixel in point out 
{
  const qreal pointm = FopInt(datain);
  //////////////qDebug() << "### Metric " << datain << " point " << Faktor(pointm);
  return pointm;
}

extern inline qreal ToUnit( qreal misura , const QString units )  ///// point int pixel out 
{
	qreal conis = Pointo(misura,units);
  return  conis;
}

/* simple mkdir */
static inline bool Cache( const QString dirpath )
{
    QDir dira(dirpath);
    if ( dira.mkpath(dirpath) ) {
    return true;
    } else {
    return false;
    }
}
/* os path from sistem */
extern inline QString PathConvert( QString path ) 
{
    QString resolvepath;
    #if defined Q_WS_WIN
    //////resolvepath = QDir::convertSeparators(path);
    resolvepath = path.replace('/','\\');
    #endif
    resolvepath = path;
   ///////////// qDebug() << "### resolvepath " << resolvepath; 
    return resolvepath;
}

/* write a file to utf-8 format */
static inline bool fwriteutf8(QString fullFileName,QString xml)
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



extern inline QString getGSLinuxPath( QString apps = QString("gs") )
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
extern inline QString getGSDefaultExeName()
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
extern inline double getGSVersion()
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

/*
#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())
 remove dir after works 
*/

extern inline int callGS( const QStringList args )
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


extern inline QPixmap LoadPS( QString fn , const QString arguments_append = QString() )
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




extern inline QPixmap LoadPDF(QString fn, int Page, int w )
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




#endif // DATASTRUCTURES_H
