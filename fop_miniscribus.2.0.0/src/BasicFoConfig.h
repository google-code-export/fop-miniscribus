#ifndef BASICFOCONFIG_H
#define BASICFOCONFIG_H


#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "Config.h"
#include <QSvgRenderer>

#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())    /* gs tmp */


/* help functions */
QPixmap BruschChess( qreal chesswidth = 50. );

QIcon createPenStyleIco( QPen item);

/* allowed char on file name image to save */
QString Imagename( const  QString txt );
/* 12mm to point , 12cm , 2inch */
/* metric conversion from and to */
qreal FopInt( const QString datain );
qreal Pointo( qreal unit , const QString unita );
qreal ToUnit( qreal unit , const QString unita );
qreal ToPoint( qreal unit , const QString unita );

bool Cache( const QString dirpath );  /* mkdir */
QString PathConvert( QString path );  /* QDir::toNativeSeparators  */
bool fwriteutf8(const QString file , QString xml );
QString ReadFile(const QString fullFileName); /* read utf8 text */
/* read the contenet of a local file as QByteArray*/
QByteArray StreamFromFile(const QString fullFileName);
QString decodeBase64( QString xml );
QString encodeBase64( QString xml );
void OpenDeskBrowser( QUrl loc );  /* open any file or url */

QPixmap RenderPixmapFromSvgByte(  QByteArray streams );
QRectF Reduce( QRectF rect , const int percentual );
QRectF CenterRectSlaveFromMaster( const QRectF Master , QRectF Slave  );

QIcon createColorToolButtonIcon(const QString &imageFile,QColor color);
QIcon createColorIcon( QColor color );


QString getGSLinuxPath( QString apps = QString("gs") );
QString getGSDefaultExeName();
double getGSVersion();
double JavaVersion();
int callGS( const QStringList args );
QPixmap LoadPS( QString fn , const QString arguments_append = QString() );
QPixmap LoadPDF(QString fn, int Page, int w );



















#endif // BASICFOCONFIG_H

