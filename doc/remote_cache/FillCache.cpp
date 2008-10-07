#include "FillCache.h"

#include <zlib.h>

static QString urlName( const QString txt )
{
    QString touri = txt;
    touri = touri.replace(" ","_");
    QStringList piece = touri.split("");
    QString alloweduri = "abcdefghijklmnopqrstuvwxyz1234567890_-ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QStringList alist = alloweduri.split("");
    for (int i = 0; i < piece.size(); ++i)
    {
        QString pin = piece.at(i);
        if (!alist.contains(pin))
        {
            touri.replace(pin,"");
        }
    }
    return touri.trimmed();
}

static qreal  floatMega( const qint64 peso  )
{
    QString humanread;
    double canno = peso / 1024;
    int sale = canno;
    if (peso > 0) {
        if (canno < 1) {
           sale = 1; 
        }
    }
   if (sale < 1025) {
   humanread = QString("0.%1").arg(sale);
   return humanread.toDouble();
   }
   float megad = sale / 1024;
   return megad;
}


static  QString fastmd5( const QString xml )
{
    return urlName(xml);
     ////////QCryptographicHash formats( QCryptographicHash::Md5 );
     /////formats.addData(xml.toUtf8());
     ////////return QString(formats.result().toHex().constData());
}

static  QString fastmd5( const QByteArray xml )
{
     QCryptographicHash formats( QCryptographicHash::Md5 );
     formats.addData(xml);
     return QString(formats.result().toHex().constData());
}




NetCacheSwap::NetCacheSwap( QObject *parent )
: QNetworkAccessManager(parent),summincomming(0)
{
    
diskCache= new QNetworkDiskCache(this);
connect(this, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
            SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
loadSettings();
location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
QDir dd_cache;
    
    if (dd_cache.mkpath ( location )) {
        qDebug() << "### mkdir Remote Cache setting to ok  -> " << location;
    } else {
        qDebug() << "### unable to create cache on   -> " << location;
    }
    

diskCache->setCacheDirectory(location);
setCache(diskCache);
    
    
    
    
    /////start_Get(QUrl("http://fop-miniscribus.googlecode.com/files/scribe_2beta.zip") );
//////////start_Get(QUrl("http://dist.trolltech.com/video/xmldemo/XMLPatterns%20Final.swf") );
//////////start_Get(QUrl("http://code.google.com/p/fop-miniscribus/wiki/QT_Trolltech") );
//////start_Get(QUrl("http://code.google.com/p/fop-miniscribus/wiki/BookMark") );
////////start_Get(QUrl("http://code.google.com/p/fop-miniscribus/wiki/KeyBoard") );
    
}

void NetCacheSwap::start_Get( const QUrl url )
{
    CacheInfo havingram = take_Url(url);
    
    qDebug() << "### > start_Get a   havingram.pending  ->" << havingram.pending;
    qDebug() << "### > start_Get  a  havingram.valid  ->" << havingram.valid;
    
    if (havingram.pending) {
    return;
    }
    if (havingram.valid) {
    emit incomming(url); 
    }
    
    
    qDebug() << "### > start_Get b   havingram.pending  ->" << havingram.pending;
    qDebug() << "### > start_Get  b  havingram.valid  ->" << havingram.valid;
    
    
    CacheInfo newrequest;
    QNetworkRequest need(url);
    need.setRawHeader( "User-Agent", "Mozilla/5.0 (X11; U; Linux i686 (x86_64); en-US; rv:1.9.0.1) Gecko/2008070206 Firefox/3.0.1" );
    need.setRawHeader( "Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7" );
    need.setRawHeader( "Accept-Encoding", "gzip,deflate,qcompress" ); ////////gzip,deflate,qcompress
    need.setRawHeader( "Connection", "keep-alive" );
    if (cookie_host[url.host()].size() > 0) {
     qDebug() << "### > send cookie ->" << cookie_host[url.host()].toAscii();
     need.setRawHeader( "Cookie",cookie_host[url.host()].toAscii()); 
    }
    QNetworkReply *reps = get(need);
    newrequest.pending = true;
    cache_ram.insert(fastmd5(url.toString()),newrequest);
    
    connect(reps, SIGNAL(downloadProgress (qint64,qint64)),SLOT(progress(qint64,qint64)));
    connect(reps, SIGNAL(error(QNetworkReply::NetworkError)),SLOT(url_error(QNetworkReply::NetworkError)));
    connect(reps, SIGNAL(finished()),SLOT(incomming_cache()));
    
}

NetCacheSwap::CacheInfo NetCacheSwap::take_Url( const QUrl url )
{
    const QString cachename = fastmd5(url.toString());
    CacheInfo neturl;
    if (cache_ram[cachename].valid) {
        qDebug() << "### > take valid having  ->" << cachename << " url->" << url.toString();
        return cache_ram[cachename]; 
    } else {
        neturl.pending = false;
        neturl.valid = false;
        qDebug() << "### <> not having having  ->" << cachename << " url->" << url.toString();
    }
    return neturl;
}


void NetCacheSwap::incomming_cache()
{
    /* memo !!! QNetworkReply  is subclass from QIODevice */
    QNetworkReply *current = qobject_cast<QNetworkReply *>(sender());
    if (!current) {
    return;
    }
    summincomming++;
    QMap<QString,QString> m_inhead;
    m_inhead.clear();
    
    CacheInfo dd;
    dd.url = current->url().toString();
    dd.hash = fastmd5(current->url().toString());
    dd.size = current->readBufferSize();
    qDebug() << "### load end by a  ->" << dd.size << " url->" << dd.hash << dd.url;
    QList<QByteArray> inhead = current->rawHeaderList();
    for (int i = 0; i < inhead.size(); ++i) {
        const QString name = QString(inhead.at(i).constData()).toLower();
        const QString value = QString(current->rawHeader(inhead.at(i)).constData());
        m_inhead.insert(name,value);
        ////////////qDebug() << "### in header  ->" << name << " ==  " << value;
    }
    if (m_inhead["content-encoding"] == QLatin1String("gzip")) {
        qDebug() << "### compressed gzip ";
        dd.chunk = deflate( current->readAll() );
        
    } else if (m_inhead["content-encoding"] == QLatin1String("deflate")) {
        qDebug() << "### compressed gzip ";
        dd.chunk = deflate( current->readAll() );
        
    } else if (m_inhead["content-encoding"] == QLatin1String("qcompress")) {
        qDebug() << "### compressed qcompress ";
        dd.chunk = qUncompress( current->readAll() );
    } else {
        /* uncompressed chunk */
        dd.chunk = current->readAll();
    }
    if (dd.size == dd.chunk.size()) {
            dd.valid = true;
            qDebug() << "### valid on size ... ";
    } else {
        if (dd.chunk.size() > 0) {
            dd.size = dd.chunk.size();
            dd.valid = true;
            qDebug() << "### decompress more size " << dd.size;
        }
    }
    
    if (m_inhead["content-type"].size() > 0) {
        dd.mime = m_inhead["content-type"].toLower();
    }
    if (m_inhead["set-cookie"].size() > 0) {
        cookie_host.insert(current->url().host(),m_inhead["set-cookie"]);
    }
    if (dd.valid) {
       cache_ram.insert(dd.hash,dd);
       qDebug() << "### register valid -> " << dd.hash << dd.url;
        if (SAVELocal_as_File == 1) {
         savecurrent(dd.chunk,current->url());
        }
        
       emit incomming(current->url());
    } else {
       qDebug() << "### noooot valid -> " << current->url().toString();
    }
}

void NetCacheSwap::savecurrent( const  QByteArray chunk , const QUrl url )
{
    const QString file = urlName(url.toString());
    QFile f(file);
             if ( f.open(QIODevice::WriteOnly) ) {
                 f.write(chunk);
                 f.close();
             }
}

QByteArray NetCacheSwap::deflate( const  QByteArray chunk )
{
    QChar letter('A' + (qrand() % 26));
    /* must not go on file solution gunzip buffer ?  go cache from net location */
    const QString tmpfiler = QString("%1/http_%2.gz").arg(location).arg(letter);
             QByteArray input;
             QFile file(tmpfiler);
             if ( file.open(QIODevice::WriteOnly) ) {
                 file.write(chunk);
                 file.close();
                 gzFile filegunzip;
                 filegunzip = gzopen(tmpfiler.toUtf8().data(),"rb");
                 if(!filegunzip) {
                 qDebug() << "### Unable to work on tmp file ... ";
                  return QByteArray();
                 }
                  char buffer[1024];
                  while(int readBytes =gzread(filegunzip, buffer, 1024))
                   {
                    input.append(QByteArray(buffer, readBytes));
                  }
                 gzclose(filegunzip);
                 file.remove();
             }
return input;          
}


void NetCacheSwap::url_error(QNetworkReply::NetworkError)
{
    QNetworkReply *current = qobject_cast<QNetworkReply *>(sender());
    if (!current) {
    return;
    }
    qDebug() << "### ernocode  ->" << current->url().toString();
}

void NetCacheSwap::progress(const qint64 in ,const qint64 tot )
{
    QNetworkReply *current = qobject_cast<QNetworkReply *>(sender());
    if (!current) {
    return;
    }
    qreal parts = 100.00;
    if (in > 1 && tot > 0) {
    parts = in*100.00/tot;
    }
    emit inTextUtf(QString("Loading %1\% MB%2").arg(parts).arg(floatMega(in)));
    qDebug() << "### incomming progress -> " << in << tot << " ->" << current->url().toString();
}


void NetCacheSwap::loadSettings()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("proxy"));
    QNetworkProxy proxy;
    if (settings.value(QLatin1String("enabled"), false).toBool()) {
        if (settings.value(QLatin1String("type"), 0).toInt() == 0)
            proxy.setType(QNetworkProxy::Socks5Proxy);
        else
            proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(settings.value(QLatin1String("hostName")).toString());
        proxy.setPort(settings.value(QLatin1String("port"), 1080).toInt());
        proxy.setUser(settings.value(QLatin1String("userName")).toString());
        proxy.setPassword(settings.value(QLatin1String("password")).toString());
    }
    setProxy(proxy);
}

void NetCacheSwap::authenticationRequired(QNetworkReply *reply, QAuthenticator *auth)
{
    
    qDebug() << "### Error need  authentication .....";
}



NetCacheSwap::CacheInfo::CacheInfo(const CacheInfo &d )
{
	operator=(d);
}

NetCacheSwap::CacheInfo& NetCacheSwap::CacheInfo::operator=(const CacheInfo &d)
{
	url = d.url;
	hash = d.hash;
    mime = d.mime;
	size = d.size;
	chunk = d.chunk;
    valid = d.valid;
    pending = d.pending;
	return *this;
}

NetCacheSwap::CacheInfo::CacheInfo()
 : valid(false),url(QLatin1String("error")),
   mime(QLatin1String("unknow")),pending(false)
{
    
}





