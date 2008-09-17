#include <QtCore>
#include <QString>
#include <QtDebug>
#include <QApplication>
#include <QXmlQuery>
#include <QtGui>

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
  QString data() { return QString(d->data()); }
  QBuffer *d;
}; 


int main(int argc, char *argv[]) {
    QApplication a( argc, argv );
   
    qDebug() << "### init main void Extract, Transform ";
    QDateTime timer1( QDateTime::currentDateTime() ); 
    const QString localoutfile =  "outresult.html";
    StreamBuf *buf = new StreamBuf();
    QXmlQuery xquery(QXmlQuery::XSLT20);
    xquery.setFocus(QUrl("http://fop-miniscribus.googlecode.com/svn/trunk/doc/Xsltqt5/data.xml"));
    xquery.bindVariable("unixtime", QVariant(timer1.toTime_t()));
    xquery.setQuery(QUrl("http://fop-miniscribus.googlecode.com/svn/trunk/doc/Xsltqt5/style.xsl"));
    xquery.evaluateTo(buf->device());
    qDebug() << "### close file Extract, Transform end ";
    QTextDocument *d = new QTextDocument();
    d->setHtml ( buf->data() );
    delete buf;
    QTextEdit t;
    t.show();
    t.setDocument ( d );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
};



