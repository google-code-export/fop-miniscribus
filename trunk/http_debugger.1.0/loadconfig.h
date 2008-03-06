#ifndef LOADCONFIG_H_
#define LOADCONFIG_H_

#include <QtCore>
#include <QtGui>
#include <QtGui>
#include <QtCore>
#include <QWidget>
#include <QPair>
#include <QTextDocument>
#include <QPair>
#include <QMap>
#include <QDateTime>
#include <QFile>
#include <QtCore> 
#include <QCoreApplication>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
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

typedef QMap<QString,QString> HeaderSend;

#include "ModelSwap.h"


static inline void OpenDeskBrowser( QUrl loc )
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









#endif /*CONFIG_H_*/
