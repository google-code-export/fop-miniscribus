#include <QtGui>

#include "qzipreader_p.h"
#include "qzipwriter_p.h"
#include "Tools_Basic.h"
#include "qtextodfwriter_p.h"
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>


/*                    file name and path , is data */
static inline QMap<QString,QByteArray> unzipstream( const QString file )
{
/*
  #include "qzipreader_p.h"  
*/
      QMap<QString,QByteArray> ooFile;
      QZipReader unzip(file,QIODevice::ReadOnly);
      QList<QZipReader::FileInfo> list = unzip.fileInfoList();
      QZipReader::FileInfo fi;
      if (list.size() > 0 && unzip.exists()) {
                  for (int l = 0; l < list.size(); l++)
                  {
                     fi = list.at(l);
                     if (fi.size > 0) {
                     ooFile.insert(fi.filePath,unzip.fileData(fi.filePath));
                     }
                     qDebug() << "### file  " << fi.filePath << fi.size;
                  }
                  
        unzip.close();    
       }
   return ooFile;
       
     /*  usage 
     QString txtlog;
     QMap<QString,QByteArray>  filist = unzipstream("giallo.odt");
     QMapIterator<QString,QByteArray> i(filist);
         while (i.hasNext()) {
             i.next();
             qDebug() << "### name---------- " << i.key();
             txtlog.append(i.key());
         }   
    QByteArray base = filist["content.xml"];
    if (base.size() > 0) {
        txtlog.append(QString(base));
    }
    */ 
}


/* switch format */
class DocumentWriter
{
public:
    DocumentWriter( const QString &fileName )
     : de(0),cod(QTextCodec::codecForHtml("utf-8"))
    {
       QFile *file = new QFile(fileName);
        if (file->open(QIODevice::WriteOnly)) {
            qDebug() << "### file open  ---------- " << fileName;   
            de = file;     
        } else {
            qDebug() << "### unable to write  ---------- " << fileName; 
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
};






class TextEdit : public QTextEdit
{
     Q_OBJECT
//
public:
TextEdit( QWidget * parent = 0 )
   : QTextEdit(parent),dev(0)
{
    
}

protected:

void contextMenuEvent ( QContextMenuEvent * e )
{
       QMenu *menu = createStandardContextMenu();
       menu->addAction(tr("save odf" ), this , SLOT( saveOOO() ) );
       menu->addAction(tr("save xml" ), this , SLOT( WriteDummy() ) );
       menu->exec(QCursor::pos());
       delete menu;   
}
private:
    QIODevice *dev;
signals:
public slots:
void saveOOO()
{
     QString fileName = QFileDialog::getSaveFileName(0, "Save OOO file","", "*.odt");
      if (fileName.size() > 1) {
          qDebug() << "### name a ---------- " << fileName;
          DocumentWriter one(fileName);
          QTextOdfWriter *owriter = new QTextOdfWriter(document()->clone(),one.device());
          if (!owriter->writeAll()) {
             qDebug() << "### error b  ---------- " << fileName; 
          }
         
      }
}


void WriteDummy()
{
    DocumentWriter one("aa.xml");
    QString fopNS (QLatin1String("http://www.w3.org/1999/XSL/Format"));
    QString svgNS (QLatin1String("http://www.w3.org/2000/svg"));
    QString xlinkNS (QLatin1String("http://www.w3.org/1999/xlink"));
    QString cmsNS (QLatin1String("http://www.pulitzer.ch/2007/CMSFormat"));
    
    QXmlStreamWriter writer(one.device());
    writer.setCodec(QTextCodec::codecForName("utf-8"));
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(2);
    
    qDebug() << "### xml on codec init  ---------- " << writer.codec()->name();
    
    writer.writeNamespace(fopNS, QString::fromLatin1("fo"));
    writer.writeNamespace(svgNS, QString::fromLatin1("svg"));
    writer.writeNamespace(xlinkNS, QString::fromLatin1("uri"));
    writer.writeNamespace(cmsNS, QString::fromLatin1("cms"));
    
    writer.writeStartDocument();
    writer.writeStartElement(fopNS, QString::fromLatin1("root"));
    
    writer.writeStartElement(fopNS, QString::fromLatin1("flow"));
    
     for (int i = 0; i < 2; ++i)  {   
    writer.writeStartElement(fopNS, QString::fromLatin1("block"));
    writer.writeAttribute(fopNS, QString::fromLatin1("padding-top"),QString("10"));
    writer.writeCharacters (QString("Line nummer %1").arg(i) );
    writer.writeStartElement(fopNS, QString::fromLatin1("inline"));    
         
         QXmlStreamReader chidxml(QString("<root><node>data</node></root>"));
         ////////while (!chidxml.atEnd()) {
            if (!chidxml.hasError()) { 
            writer.writeCurrentToken(chidxml);
            }     
         ///////}
    
    writer.writeEndElement();
    writer.writeEndElement();
     }
    ///////writer.writeEndElement();
    ///////writer.writeEndElement();
    ////////writer.writeEndElement();
    ///////////writer.writeEndElement();
     qDebug() << "### xml on codec end  ---------- " << writer.codec()->name();
    writer.writeEndDocument();   
}

};


int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QString txtlog;
    
    
    
    TextEdit t;
    t.setPlainText ( txtlog );
    t.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}


#include "main.moc"














