#ifndef BASICCONFIG_H
#define BASICCONFIG_H

#include <QInputDialog>
#include <QMenu>
#include <QString>
#include <QtDebug>
#include <QDebug> 
#include <QDesktopServices>
#include <QDebug>
#include <QString>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QObject>
#include <QDateTime>
#include <QDate>
#include <QImageReader>
#include <QPixmap>
#include <QSettings>
#include <QTimer>
#include <QProgressDialog>
#include <QPainter>
#include <QPixmap>
#include <QUrl>
#include <QColorDialog>
#include <QMessageBox>
#include <QWorkspace>
#include <QNetworkInterface>
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
#include <QTextCursor>
#include <QTextBrowser>
#include <QTextBlockFormat>
#include <QTextFrame>
#include <QTextTable>
#include <QSvgRenderer>


#define _PROGRAM_NAME_LIB_ "FOP converter to QT-TextDocument"

#include "QmyDom.h"

#if defined Q_WS_MAC
#define WEBAGENTNAME "Mac WebDavExplorer QT4.2 / PPK_W @ciz.ch"
#endif
#if defined Q_WS_WIN
#define WEBAGENTNAME "Windows WebDavExplorer QT4.2 / PPK_W @ciz.ch"
#endif
#if defined Q_WS_X11
#define WEBAGENTNAME "Unix WebDavExplorer QT4.2  / PPK_W @ciz.ch"
#endif


#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)
#define POINT_TO_PI(pi) ((pi)/12)
#define POINT_TO_DD(dd) ((dd)/154.08124)
#define POINT_TO_CC(cc) ((cc)/12.840103)



#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
#define PI_TO_POINT(pi) ((pi)*12)
#define DD_TO_POINT(dd) ((dd)*154.08124)
#define CC_TO_POINT(cc) ((cc)*12.840103)
  
#define BASECOLOR "#ffffff"
#define FORETXTCOLOR "#000000"

#define _DEFAULT_TEXT_ "Write your text..."
#define FIXEDIMAGE_CIZ "http://ciz.ch/map/theme/zest/image/ppk-web-logo.png"
#define FOPIMAGEDIR "Pictures/"



#define PICS_SIZE_LIMIT 740
#define PAGESPACING 20


#define FLOATLAYER_INCREMENT 2

typedef QMap<QString, QString> Appsetting;
typedef  QMap<QString,QVariant> TypImageContainer; 




static inline QString FilenameAllow( QString touri )
{
    touri = touri.replace(" ","_");
    QStringList piece = touri.split("");
    QString alloweduri = "abcdefghijklmnopqrstuvwxyz1234567890_.-ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QStringList alist = alloweduri.split("");
    
     for (int i = 0; i < piece.size(); ++i) {
           QString pin = piece.at(i);
           if (!alist.contains(pin)) {
               touri.replace(pin,"");
           }
       }
    
    return touri;
}



extern inline QString  BiteorMega( int peso  )
{
    QString humanread;
    qreal  faktor = 1024.00;

    qreal bytesizefile = peso / faktor;
    int kilo = 0;
    
    if (bytesizefile > 0) {
        if (bytesizefile < 1) {
           kilo = 1; 
        } else {
           kilo = bytesizefile;
        }
    }
    
   if (kilo < 1025) {
   humanread = QString("Kb.%1").arg(kilo);
   return humanread;
   }
   
   qreal megad = bytesizefile / faktor;
   
   if (megad < 1025) {
   humanread = QString("MB.%1").arg(megad, 0, 'f', 2);
   return humanread;
   } else {
            qreal gigab = megad / faktor;
            humanread = QString("GB.%1").arg(gigab, 0, 'f', 2);
       
       
   }
   
return humanread;
}










/* check if a net file to download */
static inline bool IsNetFile( QString fullFileName )
{
    if (fullFileName.startsWith("http://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("https://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("ftp://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("webdav://", Qt::CaseInsensitive)  )
       { 
         return true;
       } else {
         return false;  
       }
}



static inline QPixmap RenderPixmapFromSvgByte(  QByteArray streams ) 
{
        QSvgRenderer  svgRenderer( streams );
        QPixmap pix(  svgRenderer.defaultSize() );
        pix.fill(Qt::transparent);
        QPainter paint(&pix);
        svgRenderer.render(&paint);
        return pix;
}




static inline QString ImagesrcUrl( const QDomElement e )
{
    QString hrefimageplace = e.attribute( "src" ,"");
    if (hrefimageplace.startsWith("url(",Qt::CaseInsensitive)) {
        hrefimageplace = hrefimageplace.right(hrefimageplace.size() - 4);
        hrefimageplace = hrefimageplace.left(hrefimageplace.size() - 1).trimmed();
    }
    if (hrefimageplace.startsWith("file:",Qt::CaseInsensitive)) {
        hrefimageplace =  hrefimageplace.replace("file:","",Qt::CaseInsensitive);
    }
    return hrefimageplace;
}







static inline qreal PointMetric( qreal dimension   )
{
    qreal millimetr = POINT_TO_CM( dimension  );
    return millimetr;
}
static inline qreal Pointo( qreal unit , const QString unita )  {
    qreal ri = 0;
    if (unita == "cm") {
    ri = POINT_TO_CM( unit );
    } else if (unita == "pt") {
    ri = unit;
    } else if (unita == "mm") {
    ri = POINT_TO_MM( unit );
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
    ri = 10000;
    }
    
    return ri;
}

static inline qreal ToPoint( qreal unit , const QString unita )  {
    qreal ri = 0;
    if (unita == "cm") {
    ri = CM_TO_POINT( unit );
    } else if (unita == "pt") {
    ri = unit;
    } else if (unita == "mm") {
    ri = MM_TO_POINT( unit );
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
    ri = 10000;
    }
    
    return ri;
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
  
  if ( data.endsWith( "pt" ) || data.endsWith( "px" ) ) {
    points = data.left( data.length() - 2 ).toDouble();
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
  return points;
} 



static inline bool Cache( const QString dirpath )
{
    
    QTextCodec *codecx;
    codecx = QTextCodec::codecForMib(106);
    QTextCodec::setCodecForLocale( codecx );
    
    
     QDir dira(dirpath);
    if ( dira.mkpath(dirpath) ) {
    return true;
    } else {
    return false;
    }
}





/* grep nummer on [77] */
static inline int  BraketNummer( QString braket )
{
    int esanummer = 0;
    if (braket.startsWith("[")) {
       braket.replace(QString("["), QString("")); 
       int zool = braket.indexOf("]",0); 
           if (zool > 0) {
              QString   xid = braket.left(zool);  
               esanummer = xid.toInt();
           }
    }
return esanummer;
}

























































#endif