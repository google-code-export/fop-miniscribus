#ifndef FILLCACHE_H
#define FILLCACHE_H
#include <QtCore>
#include <QObject>
#include <QCryptographicHash>


/*


try to get IE proxy settings from
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsCurrentVersion\Internet Settings
parameters:
ProxyEnable
ProxyOverride
ProxyServer 


*/


#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>



static const int SAVELocal_as_File = 0;






class NetCacheSwap : public QNetworkAccessManager
{
     Q_OBJECT
//
public:
    

 NetCacheSwap( QObject *parent = 0 );
 void start_Get( const QUrl url );
 QByteArray deflate( const  QByteArray chunk ); 

  struct CacheInfo
    {
        CacheInfo();
        CacheInfo(const CacheInfo &d );
        CacheInfo &operator=(const CacheInfo &d);
        QString url;
        QString mime;
        QString hash;
        qint64 size;
        QByteArray chunk;
        bool valid;
        bool pending;
    };
    
  CacheInfo take_Url( const QUrl url );
    
protected:
 void loadSettings();
 uint summincomming;
 QMap<QString,CacheInfo> cache_ram;
 /* host && cokie values */
 QMap<QString,QString> cookie_host;
 void savecurrent( const  QByteArray chunk , const QUrl url );
private:
    QNetworkDiskCache *diskCache;
    QString location;
    
signals:
     void incomming(QUrl);
     void inTextUtf(QString);

public slots:
    
private slots:
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *auth);
    void progress(const qint64 in ,const qint64 tot );
    void url_error(QNetworkReply::NetworkError);
    void incomming_cache();

};





//
#endif // FILLCACHE_H

