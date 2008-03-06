#include "communicator.h"




Communicator::Communicator( QMap<QString,QString> onheader , 
              QUrl requesturl , 
              const QString stype, 
              const QString method , 
              QObject * parent )
	: QHttp(requesturl.host(),requesturl.port(), parent )
{
	   url = QUrl(requesturl);
	   /////const QString & path = 
	   header.setRequest(method,requesturl.path(),1,1);
	   QMapIterator <QString,QString> d(onheader);
                 while (d.hasNext()) {
                       d.next();
									     header.setValue(d.key(),d.value());
									     qDebug() << "### set header Communicator r " << d.key() << "|" << d.value();
                 }
}

void Communicator::Start( QIODevice * data , QIODevice * to )
{
	currentID = request(header,data,to);
  connect(this, SIGNAL(done(bool)), this, SLOT(Exitdone(bool)));
  connect(this, SIGNAL(dataReadProgress(int, int)), this, SLOT(ReciverStatus(int, int)));
  connect(this, SIGNAL(readyRead(const QHttpResponseHeader &)), this, SLOT(SaveData(const QHttpResponseHeader &)));
	connect(this, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(SaveData(const QHttpResponseHeader &)));
	
	
}

void Communicator::Exitdone( bool e  )
{
    headercode = lastResponse().statusCode();
    if(headercode == 404) {
    emit LastDone(headercode);
    abort();
    deleteLater();
    return;
    }
    emit LastDone(headercode);
    abort();
    deleteLater();
}

void Communicator::SaveData(const QHttpResponseHeader &responseHeader )
{
    headercode = lastResponse().statusCode();
    QStringList headerin = responseHeader.keys();
	  headerresponder =""; //// headerin.join("\n");
	  qDebug() << "### headerin " << headerin;
	  
	  HeaderIncome.clear();
    for (int i = 0; i < headerin.size(); ++i) {
			const QString HeaderValue = responseHeader.value(headerin.at(i)); 
			 //////qDebug() << "### HeaderValue " << HeaderValue;
			  HeaderIncome.insert(headerin.at(i),HeaderValue);
			  headerresponder.append(QString("%1=%2\n").arg(headerin.at(i)).arg(HeaderValue));
    }

}


void Communicator::ReciverStatus( int fatto , int tot )
{
    qDebug() << "### ReciverStatus b." << fatto << " t." << tot;
}


























