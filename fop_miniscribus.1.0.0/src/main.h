#ifndef MAIN_H
#define MAIN_H


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


#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QBrush>
#include <QPixmap>
#include <QColor>
#include <QPalette> 
#include <QGraphicsScene>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "fop_handler.h"
#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QBrush>
#include <QPixmap>
#include <QColor>
#include <QPalette> 
#include <QGraphicsScene>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>








#define PICS_SIZE_LIMIT 740
#define PAGESPACING 20
#define BEGINNATOP 0
#define POINT_TO_CM(cm) ((cm)/28.3465058)

#define FLOATLAYER_INCREMENT 10
#define FLOATLAYER_SUBLIMIT 9

typedef QMap<QString, QString> Appsetting;
typedef  QMap<QString,QVariant> TypImageContainer; 


#define _CURRENT_VERSION_ "1.0.2"

#define _PROGRAM_NAME_ "PPK Fop edit " "_CURRENT_VERSION_
#define _PROGRAM_SHORT_NAME_ "tmp_cache"
#define _DEFAULT_TEXT_ "Write your text..."
#define FIXEDIMAGE_CIZ "http://ciz.ch/map/theme/zest/image/ppk-web-logo.png"
#define FOPIMAGEDIR "Pictures/"




#define _PROGRAM_NAME_ "FOP MiniScribus  Block-Container"
#define _REGISTER_PROGRAM_NAME_ "FOP MiniScribus"
#define _PROGRAM_NAME_DOMAINE_ "fop.ciz.ch"
#define _ORGANIZATION_NAME__ "1-PPK-Schweiz"

#define _PROGRAM_VERSION_ "version " "_CURRENT_VERSION_
#define Q_WORKS_PEND "open"

#define _PROGRAM_TITLE_ \
             QString("FOP MiniScribus  Block-Container version %1").arg(_CURRENT_VERSION_)
#define _BUNDLE_NAME_ \
             QString("MiniScribus")


#if defined Q_WS_MAC
#define WEBAGENTNAME "Mac WebDavExplorer QT4.2 / PPK_W @ciz.ch"
#endif
#if defined Q_WS_WIN
#define WEBAGENTNAME "Windows WebDavExplorer QT4.2 / PPK_W @ciz.ch"
#endif
#if defined Q_WS_X11
#define WEBAGENTNAME "Unix WebDavExplorer QT4.2  / PPK_W @ciz.ch"
#endif











static inline QString ImageSaveName( QString hrefimager  )
{
    hrefimager = FilenameAllow(hrefimager.right(hrefimager.lastIndexOf("-")).toLower());
    return hrefimager;
}

    
        
        
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

/* write a file to utf-8 format */
static inline bool debugwrite(QString fullFileName,QString xml)
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



static inline QString HumanTextQuiet( QString todomdocument , int compression )
{
    QString errorStr;
    int errorLine, errorColumn;
    QDomDocument doc;
    
    if (!doc.setContent(todomdocument,false, &errorStr, &errorLine, &errorColumn)) {
         todomdocument = todomdocument.replace("\n","");
         todomdocument = todomdocument.replace("\t","");
         todomdocument = todomdocument.replace("\r","");
      return todomdocument;
     }
     
     
     QString canse = doc.toString(compression);
     if (compression == 0) {
         canse = canse.replace("\n","");
         canse = canse.replace("\t","");
         canse = canse.replace("\r","");
     }
     return canse;
}










#endif

