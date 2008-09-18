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
  //////////QXmlStreamReader Xml() { return doc; }
  QIODevice *device() { return d; }
  ///////bool isValid() { return doc.hasError() == true ? false : true;  }
  QByteArray stream() { return d->data(); }
  QBuffer *d;
};






class DocumentWriter
{
public:
    
    typedef enum
    {
            DOC_NOT_KNOW = 404,
            DOC_XHTML = 100,
            DOC_ODT = 200,
            DOC_FOP = 300,
            DOC_RTF = 400,
            DOC_XML = 500,
            DOC_PAGE  = 600  /* binary */
    } WDOCTYPE;

    DocumentWriter( const QString &fileName )
     : de(0),cod(QTextCodec::codecForHtml("utf-8")),type(DOC_FOP)
    {
       QFileInfo fi(fileName);
       const QString ext = fi.completeSuffix().toLower();
       if (ext == "xml") {
         type = DOC_XML;  
       } else if (ext == "html") {
          type = DOC_XHTML;   
       } else if (ext == "odt") {
          type = DOC_ODT;   
       }
       
       QFile *file = new QFile(fileName);
        if (file->open(QIODevice::WriteOnly)) {
            qDebug() << "### DocumentWriter file open  ---------- " << fileName;   
            de = file;   
        }
        
     ft = cod->name();
    }
    ~DocumentWriter() {
      de->close();  
    }
    
    QIODevice *device() { return de; }
    QByteArray format() { return ft; }
    QTextCodec *codec() { return cod; }
    QByteArray ft;
    QIODevice *de;
    QTextCodec *cod;
    WDOCTYPE type;
};


int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QString txtlog;
    
    QString fopNS (QLatin1String("http://www.w3.org/1999/XSL/Format"));
    QString svgNS (QLatin1String("http://www.w3.org/2000/svg"));
    QString xlinkNS (QLatin1String("http://www.w3.org/1999/xlink"));
    QString cmsNS (QLatin1String("http://www.pulitzer.ch/2007/CMSFormat"));
    
    
    ReadWriteBuf *Awriter = new  ReadWriteBuf();
    QXmlStreamWriter writer0(Awriter->device());
    writer0.setCodec(QTextCodec::codecForName("utf-8"));
    writer0.setAutoFormatting(true);
    writer0.setAutoFormattingIndent(2);
    writer0.writeNamespace(fopNS, QString::fromLatin1("fo"));
    writer0.writeStartElement(fopNS, QString::fromLatin1("initroot"));
    
    ReadWriteBuf *buf = new  ReadWriteBuf( QString("<rooteternal xmlns:fo=\"http://www.w3.org/1999/XSL/Format\"><fo:node margin=\"10\">data</fo:node></rooteternal>") );
    
    ChildImport job(buf->device()); 
                               job.copyDeep(buf->device(),writer0);
    
    qDebug() << "### main  errorString---------- " << buf->stream();  /////.size()
    
    writer0.writeEndDocument();
    qDebug() << "### summ all string ---------- " << Awriter->stream();  /////.size()
    
    
    
     DocumentWriter one("dummytest.xml");
   
    
    QXmlStreamWriter writer(one.device());
    writer.setCodec(QTextCodec::codecForName("utf-8"));
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(2);
    
    qDebug() << "### main xml on codec init  ---------- " << writer.codec()->name();
    
    writer.writeNamespace(fopNS, QString::fromLatin1("fo"));
    writer.writeNamespace(svgNS, QString::fromLatin1("svg"));
    writer.writeNamespace(xlinkNS, QString::fromLatin1("uri"));
    writer.writeNamespace(cmsNS, QString::fromLatin1("cms"));
    
    writer.writeStartDocument();
    writer.writeStartElement(fopNS, QString::fromLatin1("root"));
    
    writer.writeStartElement(fopNS, QString::fromLatin1("flow"));
    
     //////for (int i = 0; i < 5; ++i)  {   
    int i = 77;
    writer.writeStartElement(fopNS, QString::fromLatin1("block"));
    writer.writeAttribute(fopNS, QString::fromLatin1("padding-top"),QString("10"));
    writer.writeCharacters (QString("Line nummer %1").arg(i) );
    writer.writeStartElement(fopNS, QString::fromLatin1("inline"));    
         /* append sub node to last elements */
         
                 ///delete buf;            
         ///////}
    
    writer.writeEndElement();
    writer.writeEndElement();
     ///////}
    ///////writer.writeEndElement();
    ///////writer.writeEndElement();
    ////////writer.writeEndElement();
    ///////////writer.writeEndElement();
     qDebug() << "### main xml on codec end  ---------- " << writer.codec()->name();
    writer.writeEndDocument();
    QTextEdit t;
    t.setPlainText ( txtlog );
    t.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}

//////#include "main.moc"
