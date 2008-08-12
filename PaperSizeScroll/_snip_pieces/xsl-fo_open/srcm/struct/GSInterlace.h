#ifndef GSINTERLACE_H
#define GSINTERLACE_H


#include <QtGui>
#include <QtCore>
#include <QPixmap>


#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())    /* gs tmp */
             
#define _DUMMY_SPACE_DOCUMENT_ \
             QString("dummy_space_please") 
             
             


bool Cache( const QString dirpath );  /* mkdir */
QString PathConvert( QString path );  /* QDir::toNativeSeparators  */
bool fwriteutf8(const QString file , QString xml );
QString ReadFile(const QString fullFileName); /* read utf8 text */
QString decodeBase64( QString xml );
QString encodeBase64( QString xml );

void OpenDeskBrowser( QUrl loc );  /* open any file or url */


QString getGSLinuxPath( QString apps = QString("gs") );
QString getGSDefaultExeName();
double getGSVersion();
double JavaVersion();
int callGS( const QStringList args );
QPixmap LoadPS( QString fn , const QString arguments_append = QString() );
QPixmap LoadPDF(QString fn, int Page, int w );


//
#endif // GSINTERLACE_H

