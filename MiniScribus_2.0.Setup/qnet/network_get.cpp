#include "network_get.h"



LoadGetImage::LoadGetImage( int nr , QUrl url_send  )
	: QHttp(url_send.host(),QHttp::ConnectionModeHttp ,80)
{
     url = url_send;
     cid = nr;
     setHost(url_send.host() , 80);
}

void LoadGetImage::Start()
{
	     const QString METHOD =  "GET";
       const QString agent = QString("Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");
			 header.setRequest(METHOD,url.path(),1,1);
			 header.setValue("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");
			 header.setValue("Host",url.host());
			 header.setValue("User-Agent",agent);
			 connect(this, SIGNAL(done(bool)), this , SLOT(ImageReady(bool)));
       Http_id = request(header,0,0);
}

void LoadGetImage::ImageReady( bool error )
{
    if (!error) {
        resultimage.loadFromData(readAll());
                if (!resultimage.isNull()) {
                    emit take(cid);
                }
        
     
    }
}



void Gloader::Setting( QObject *parent , int id , QUrl url_send )
{
	receiver = parent;
	cid = id;
	url = url_send;
	setTerminationEnabled(true);
}

void Gloader::run()
{
	 Rhttp = new LoadGetImage(cid,url);
	 connect(Rhttp, SIGNAL(take(int)), receiver , SLOT(in_image(int)));
	 Rhttp->Start();
	 exec();
}



