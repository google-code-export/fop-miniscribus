#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "loadconfig.h"
#include <QHttp>

#include "quazip.h"
#include "quazipfile.h"

//
class Communicator : public QHttp
{
     Q_OBJECT
//
public:
Communicator( QMap<QString,QString> onheader , 
              QUrl requesturl , 
              const QString stype, 
              const QString  method , 
              QObject * parent = 0 );
void Start( QIODevice * data = 0, QIODevice * to = 0 );
inline QString headerin() { return headerresponder; }
inline QMap<QString,QString> inmap() { return HeaderIncome; }
protected:
int currentID;
QUrl url;
QByteArray byteArray_in;  /* incomming data */
int doneCall;
int readyCall;
int headercode;
bool firstpassage;
bool Compressed;
QString davoption;
QString headerresponder;
QString Requesturi;
QHttpRequestHeader header;
QStringList CookieList;
QByteArray byteArray_out;
QMap<QString,QString> HeaderIncome;
private:
signals:
   void LastDone(int);
public slots:
     void Exitdone( bool e  );
     void SaveData(const QHttpResponseHeader &responseHeader );
     void ReciverStatus( int fatto , int tot );

};


//
#endif // COMMUNICATOR_H










