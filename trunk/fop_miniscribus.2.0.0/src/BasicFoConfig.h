#ifndef BASICFOCONFIG_H
#define BASICFOCONFIG_H


#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "Config.h"
#include <QSvgRenderer>

#include "FoColorName.h"   /* unit cm mm point ecc...*/



#if QT_VERSION >= 0x04050
#include <QXmlQuery>
#include "OOReader.h"

class StreamBuf
{
  public:
  StreamBuf() 
  :d(new QBuffer())
  {
  d->open(QIODevice::ReadWrite);
  }
  ~StreamBuf() { d->close(); }
  bool clear() {
    d->write(QByteArray()); 
    return d->bytesAvailable() == 0 ? true : false;
  }
  QIODevice *device() { return d; }
  QByteArray stream() { return d->data(); }
  /* <?xml version="1.0" encoding="utf-8"?> */
  QString data() { return QString::fromUtf8(stream()); }
  QBuffer *d;
}; 


class XsltConText
{
public:
 XsltConText( const QString xml , const QString xslt )
  :d(new StreamBuf())
{
  xquery = QXmlQuery(QXmlQuery::XSLT20);
  xquery.setFocus(QUrl(xml));
  xquery.setQuery(QUrl(xslt));
}

~XsltConText()
{
  delete d;
}

void setVariable( const QString name , QVariant value )
{
    xquery.bindVariable(name,value);
}

QString read()
{ 
  xquery.evaluateTo(d->device());
  return d->data();
}


QXmlQuery xquery;
StreamBuf *d;

};

////////QXmlQuery xquery(QXmlQuery::XSLT20);
/////// xquery.setFocus(QUrl("http://fop-miniscribus.googlecode.com/svn/trunk/doc/Xsltqt5/doc/odtdoc.xml"));
/* variable  <xsl:param name="unixtime" select="'0000000'" />  on style */
/////xquery.bindVariable("unixtime", QVariant(timer1.toTime_t()));
/* other variable */
////////xquery.setQuery(QUrl("http://fop-miniscribus.googlecode.com/svn/trunk/doc/odt-fo/ooo2xslfo-writer.xsl"));
//////////xquery.evaluateTo(buf->device());
#else
#include "xslt_convert.h"
#endif



#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())    /* gs tmp */


/* help functions */
QPixmap BruschChess( qreal chesswidth = 50. );

QIcon createPenStyleIco( QPen item);

/* allowed char on file name image to save */
QString Imagename( const  QString txt );

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

