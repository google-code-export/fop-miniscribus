#include <QtGui>
/////#include "qzipreader_p.h"
//////#include "qzipwriter_p.h"
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class ChildImport : public QXmlStreamReader
{
public:
ChildImport( QIODevice* device = 0 ) 
 :QXmlStreamReader( device )
{
  setNamespaceProcessing(false);
  /* only prefix append if exist */
}
/* copy all subelement from reader to writer out */
void copyDeep( QIODevice* device , QXmlStreamWriter &out  )
{
    setDevice(device);
    while (!atEnd()) {
         readNext();
         if ( isStartElement() ) {
             /* a dom element node is start to read */
             /////qDebug() << "### copydeep element tagname   ---------- " << name().toString();
             /////////qDebug() << "### copydeep element tagname   ---------- " << prefix().toString();
             if (prefix().toString().isEmpty()) {
             out.writeStartElement(name().toString());
             } else {
             out.writeStartElement(prefix().toString()+":"+name().toString()); 
             }
             if (attributes().size() > 0) {
             out.writeAttributes(attributes());
             }      
         } else if (!isWhitespace()) {
             /* element having text or ??? */
             out.writeCharacters(text().toString());
         } else if (isComment()) {
            /* leave not import */
         } else if (isCDATA()) {
            /* take cdata */
             out.writeComment(text().toString());
         }
         
         
   }
   const int erno = (int)error();
   if (erno != 0) {
       qWarning() << "### ChildImport::copyDeep error    ---------- " << error();
   } else {
      out.writeEndElement(); 
   }
   device->close();
}

};


class ReadWriteBuf
{
  public:
  ReadWriteBuf( const QString xml = QString() ) 
  :d(new QBuffer())
  {
  d->open(QIODevice::ReadWrite);
  start();
    if (xml.size() > 0 )
      write(xml);
  }
  bool clear()
  {
   d->write(QByteArray()); 
   start();
   return d->bytesAvailable() == 0 ? true : false;
  }
  qint64 write( const QString dat )
  {
    QByteArray chunk;
               chunk.append(dat); 
    d->write(chunk);
    start();
    return d->bytesAvailable();
  }
  qint64 write( const QByteArray dat )
  {
    d->write(dat); 
    start();
    return d->bytesAvailable();
  }
  void start() { 
    d->seek(0);
  }
  bool LoadFile( const QString file ) { 
    if (clear()) {
     QFile f(file); 
        if (f.exists()) {
          if (f.open(QFile::ReadOnly)) {
           d->write(f.readAll());
           f.close();
           start();
           return true;
          }
        }
    }
    return false;
  }
  bool PutOnFile( const QString file ) { 
      QFile f(file);
      start();
      if (f.open(QFile::WriteOnly)) {
        uint bi = f.write(d->readAll());
        f.close();
        start();
        return bi > 0 ? true : false;
      }
   return false; 
  }

  QIODevice *device() { return d; }
  QByteArray stream() { return d->data(); }
  QString toString() { return QString(d->data()); }
  QBuffer *d;
};



int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QString txtlog;
    ChildImport job(0); 
    
    QString fopNS (QLatin1String("http://www.w3.org/1999/XSL/Format"));
    QString svgNS (QLatin1String("http://www.w3.org/2000/svg"));
    ////QString xlinkNS (QLatin1String("http://www.w3.org/1999/xlink"));
    //////QString cmsNS (QLatin1String("http://www.pulitzer.ch/2007/CMSFormat"));
    
    /* start a new xml to write */
    ReadWriteBuf *Awriter = new  ReadWriteBuf();
    QXmlStreamWriter writer0(Awriter->device());
    
    
    writer0.setCodec(QTextCodec::codecForName("utf-8"));
    writer0.setAutoFormatting(true);
    writer0.setAutoFormattingIndent(2);
    writer0.writeNamespace(fopNS, QString::fromLatin1("fo"));
    writer0.writeStartElement(fopNS, QString::fromLatin1("initroot"));
    
    for (int i = 0; i < 400; ++i)  {   
    writer0.writeStartElement(fopNS, QString::fromLatin1("frame"));   
    writer0.writeAttribute("id",QString("pos_%1").arg(i));
    /* create a temp buffer to import */
             
    ReadWriteBuf *appendTree = new  ReadWriteBuf( 
    QString("<inline xmlns:fo=\"http://www.w3.org/1999/XSL/Format\" "
    "margin-start=\"1cm\"><fo:chars lang=\"en\">data</fo:chars></inline>") );
    job.copyDeep(appendTree->device(),writer0);
     writer0.writeEndElement();
     writer0.writeEndElement();  
     }
    writer0.writeEndDocument();
    txtlog.append(QString(Awriter->stream()));
    QTextEdit t;
    t.setPlainText ( txtlog );
    t.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}

//////#include "main.moc"
