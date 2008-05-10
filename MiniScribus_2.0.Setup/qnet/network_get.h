#ifndef NETWORK_GET_H
#define NETWORK_GET_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QHttp>
#include <QAbstractSocket>
#include <QFileInfo>
#include <QHttp>
#include <QUrl>
#include <QTimer>

#if defined Q_WS_MAC
#define WEBAGENTNAME "Mac WebDavExplorer QT4.2 / PPK_W @ciz.ch"
#endif
#if defined Q_WS_WIN
#define WEBAGENTNAME "Windows WebDavExplorer QT4.2 / PPK_W @ciz.ch"
#endif
#if defined Q_WS_X11
#define WEBAGENTNAME "Unix WebDavExplorer QT4.2  / PPK_W @ciz.ch"
#endif



#include "quazip.h"
#include "quazipfile.h"


/* zip libs  ###### start ###################################################################*/

#include "zlib.h"
#include <stdio.h>
#ifdef STDC
#  include <string.h>
#  include <stdlib.h>
#endif

#ifdef USE_MMAP
#  include <sys/types.h>
#  include <sys/mman.h>
#  include <sys/stat.h>
#endif

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#ifdef VMS
#  define unlink delete
#  define GZ_SUFFIX "-gz"
#endif
#ifdef RISCOS
#  define unlink remove
#  define GZ_SUFFIX "-gz"
#  define fileno(file) file->__file
#endif
#if defined(__MWERKS__) && __dest_os != __be_os && __dest_os != __win32_os
#  include <unix.h> /* for fileno */
#endif


#ifndef GZ_SUFFIX
#  define GZ_SUFFIX ".gz"
#endif
#define SUFFIX_LEN (sizeof(GZ_SUFFIX)-1)

#define BUFLEN      16384
#define MAX_NAME_LEN 1024

#ifdef MAXSEG_64K
#  define local static
   /* Needed for systems with limitation on stack size. */
#else
#  define local
#endif

#include <iostream>
#include <stdio.h>
#include <QPixmap>

class LoadGetImage : public QHttp
{
    Q_OBJECT
//
public: 
     LoadGetImage( int nr , QUrl url_send );
     void Start();
     inline int Get_id() { return cid; }
     inline int Htpp_id() { return Http_id; } 
     inline QPixmap pics() { return resultimage; } 
     int cid;
     int Http_id;
    QHttpRequestHeader header;
    QUrl url;
    QPixmap resultimage;
    signals:
      void take(int);
    public slots:
     void ImageReady( bool error );
};


class Gloader : public QThread
{
    Q_OBJECT
     
public:
  void Setting( QObject *parent , int id , QUrl url_send ); 
protected:
  void run();
  signals:
private:
    int cid;
    QUrl url;
    LoadGetImage *Rhttp;
    QObject* receiver;
};




typedef enum
{  
  GZIP_MODE = 1,
  DEFLATE_MODE,
  QT_MODE,
  NO_COMPRESS
} COMPRESS_MODE;



class PutSender : public QHttp
{
    Q_OBJECT
//
public: PutSender( QUrl urigo , QFileInfo locfile , uint nummer , const QString serialkey )
{
 setHost(urigo.host(), 80);
 serialnummer = nummer;
 filehaving = true;
 BeamUp = new QFile(locfile.absoluteFilePath());   /* upload file */
 if (BeamUp->exists()) {
 BeamUp->open(QIODevice::ReadOnly);
 header.setRequest("PUT",urigo.path(),1,1);
 header.setValue("Host",urigo.host());
 header.setValue("User-Agent", WEBAGENTNAME );
 header.setValue("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
 header.setValue("Accept-Language", "it,de-de;q=0.8,it-it;q=0.6,en-us;q=0.4,en;q=0.2");
 header.setValue("Connection", "keep-alive");
 header.setValue("Cookie",serialkey);
 david = request(header,BeamUp,0);
 connect(this, SIGNAL(done(bool)), this, SLOT(Exitdone(bool)));
 } else {
    filehaving = false; 
 }
}

protected:
    int david;        /* current process id */
    QFile *BeamUp;   /* file to upload */
    uint serialnummer;
    int readyCall;
    bool filehaving;
    QString davoption;
    QHttpRequestHeader header;
private:
signals:
  void LastDone(uint);
  void ErrorOnNummer(uint);
public slots:
void Exitdone( bool e  )
{
    BeamUp->close();
    //////////BeamUp->remove();
    if (QString(readAll().data()).contains("|Zip ON DB OK|")) { 
        emit LastDone(serialnummer);
    } else {
        emit ErrorOnNummer(serialnummer);
    }
    abort();
    deleteLater();
}


};

class PutSending : public QThread
{
    Q_OBJECT
     
public:
  void Setting( QObject *parent , uint id , QUrl url_send ,  QFileInfo zipfile , const QString serialkey )
{
  receiver = parent;
	cid = id;
	url = url_send;
  fix = zipfile;
  seq = serialkey;
	setTerminationEnabled(true);
}
protected:
  void run()
{
     PutSender *Rhttp = new PutSender(url,fix,cid,seq);
     connect(Rhttp, SIGNAL(LastDone(uint)), receiver , SLOT(childFinished(uint)));
     connect(Rhttp, SIGNAL(ErrorOnNummer(uint)), receiver , SLOT(childerror(uint)));
     exec();
}
signals:
private:
    uint cid;
    QUrl url;
    QString seq;
    QFileInfo fix;
    PutSender *Rhttp;
    QObject* receiver;
};













/* zip libs  ###### end ###################################################################*/


class LoadPage : public QHttp
{
    Q_OBJECT
//
public: LoadPage( const QString urigoget , int nr )
{
 CookieList.clear();
 Compressed = false;
 cicle = nr;
 urigoup = QUrl(urigoget);
 doneCall =0;
 readyCall =0;
 Requesturi = urigoget;
 setHost(urigoup.host(),QHttp::ConnectionModeHttp,80);  /* QHttp::ConnectionModeHttps /  QHttp::ConnectionModeHttp */
    
 header.setRequest("GET",urigoget,1,1);
 header.setValue("Host",urigoup.host());
 header.setValue("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; it; rv:1.8.1.5) Gecko/20070713 Firefox/2.0.0.5" );
 header.setValue("Accept", "text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5");
 header.setValue("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
 header.setValue("Accept-Language", "it-it,it;q=0.8,en-us;q=0.5,en;q=0.3");
 header.setValue("Accept-Encoding", "gzip,deflate,qcompress");
 header.setValue("Keep-Alive", "300");
 header.setValue("Connection", "keep-alive");
}

void Start()
{
 ///////////// 
  footcount =1.08;
  david = request(header,0,0);
  connect(this, SIGNAL(done(bool)), this, SLOT(Exitdone(bool)));
  connect(this, SIGNAL(dataReadProgress(int, int)), this, SLOT(ReciverStatus(int, int)));
  connect(this, SIGNAL(readyRead(const QHttpResponseHeader &)), this, SLOT(SaveData(const QHttpResponseHeader &)));
    
    /////////std::cout << "############## start get ->" <<  qPrintable(header.path()) << std::endl; 
    
}
void AppendHeader( const QString nam , const QString val )
{
    ///////////std::cout << "### Send cookie ->" <<  qPrintable(val) << std::endl;
   header.setValue(nam,val); 
}
inline QByteArray Data() { return byteArray; }
/////////////inline int  IdCicle() { return david; }
protected:
    int david;        /* current process id */
    int cicle;
    qreal footcount;
    QUrl urigoup;     /* url path */
    QByteArray byteArray;  /* incomming data */
    int doneCall;
    int readyCall;
    int headercode;
    bool firstpassage;
    bool Compressed;
    QString davoption;
    QString Requesturi;
    QHttpRequestHeader header;
    QStringList CookieList;
    COMPRESS_MODE zipmode;
private:
    
void  DecompressStream()
{
    /*  QByteArray byteArray;   incomming data */
    
    QChar letter('A' + (qrand() % 26));
    /* must not go on file solution gunzip buffer ? */
    const QString tmpfiler = QString("%1/http_%2.gz").arg( QDir::homePath ()).arg(letter);
    
    /*
     QBuffer qtbuffer;
     qtbuffer.open(QBuffer::ReadWrite);
     qtbuffer.write(byteArray);
     qtbuffer.seek(0);
    */
    
    
    
    QFile f( tmpfiler );
      if ( f.open( QIODevice::WriteOnly ) ) {
        f.write(byteArray);
        f.close();
      } else {
      std::cout << "### Unable to work on cache dir!. "  << std::endl;
      return;
      }
      
     gzFile file;
      file = gzopen (tmpfiler.toUtf8().data(),"rb");
      if(!file) {
        std::cout << "### Unable to work on cache dir!. "  << std::endl;
        return;
      }
      QByteArray input;
      char buffer[1024];
      QByteArray inputData;
      while(int readBytes =  gzread (file, buffer, 1024))
       {
        input.append(QByteArray(buffer, readBytes));
      }
      gzclose(file);
      f.remove();
      
      byteArray.clear();
      byteArray.append(input);
}


signals:
  void LastDone(int,QByteArray,QString);
  void SendLog(int);
  void RedirectUrl(QString);
public slots:
void ReciverStatus( int fatto , int tot )
{
    ///////qDebug() << "### ReciverStatus b." << fatto << " t." << tot;
    qreal cento = 100.0;
    int percentuale;
    percentuale = fatto*cento/tot;
    if (percentuale < 1) {
        footcount = footcount + 0.13;
        if (footcount > 99) {
        footcount = 0.01;
        }
        
    percentuale = footcount;        
    }
    ////////////qDebug() << "### percentuale" << percentuale;
    emit SendLog(percentuale);
    ////////std::cout << "Progress: %" << percentuale << "\r";
                             ////////std::fflush ( stdin );    
}
void Exitdone( bool e  )
{
    doneCall++;
    headercode = lastResponse().statusCode();
    if(headercode == 404) {
    QStringList ss;
    QByteArray aa("<error/>");
    emit LastDone(cicle,aa,Requesturi);
    abort();
    deleteLater();
    return;
    }
    QByteArray bytes;
    if (Compressed) {
                 if (zipmode == GZIP_MODE ||  zipmode == DEFLATE_MODE ) {
                 DecompressStream();
                 } else if (zipmode == QT_MODE) {
                     QByteArray input =  qUncompress(byteArray); 
                     byteArray.clear();
                     byteArray.append(input);
                 }
    } else {
    ////////////std::cout <<  qPrintable(QString(byteArray.data())) << std::endl;
    }
    emit LastDone(cicle,byteArray,Requesturi);
    abort();
    deleteLater();
    
}

void SaveData(const QHttpResponseHeader &responseHeader )
{
    readyCall++;
    headercode = lastResponse().statusCode();
    ////////std::cout << "Process IDA:" << david << "------headercode =" << headercode << "----------"  << std::endl;
    
            if(responseHeader.hasKey("location")) {
                   byteArray.clear();
                    const QString redirecturl = responseHeader.value("location");
                    emit RedirectUrl(redirecturl);
                    abort();
                    deleteLater();
                    return;
             }  
    
    ///////std::cout << "Process IDB:" << david << "------headercode =" << headercode << "----------"  << std::endl;
    byteArray.append(readAll());
    
    
    
    //////////////std::cout << "Last http header respond is " << headercode << std::endl; 
    ////////std::cout << "contentLength " << responseHeader.contentLength() << std::endl; 
    /////////std::cout << "contentType " << qPrintable(responseHeader.contentType()) << std::endl;
    QStringList headerin = responseHeader.keys();
    for (int i = 0; i < headerin.size(); ++i) {
    /////////////std::cout << "" << qPrintable(headerin.at(i)) << " ->" << qPrintable(responseHeader.value(headerin.at(i))) << std::endl; 
        
        const QString HeaderValue = responseHeader.value(headerin.at(i));
        
        if (headerin.at(i) == "content-encoding") {
            if (HeaderValue.contains("gzip",Qt::CaseInsensitive)) {
             Compressed = true;
             zipmode = GZIP_MODE;
            } else if (HeaderValue.contains("deflate",Qt::CaseInsensitive)) {
              Compressed = true;
              zipmode = DEFLATE_MODE;
            } else if (HeaderValue.contains("qcompress",Qt::CaseInsensitive)) {
               Compressed = true;
               zipmode = QT_MODE;
            }
        }
        if (headerin.at(i) == "set-cookie") {
            if (!CookieList.contains(HeaderValue,Qt::CaseInsensitive)) {
              CookieList.append( HeaderValue );
            }
        }
        
        
        
        
    }
    ///////////////std::cout << "---------------------------------HEADER---------------------------"  << std::endl;
}

};






#endif // NETWORK_GET_H

