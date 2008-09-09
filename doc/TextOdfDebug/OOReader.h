#ifndef OOREADER_H
#define OOREADER_H
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include "qzipreader_p.h"
#include "qzipwriter_p.h"
#include <QDomDocument>
#include <QTextTableFormat>

#include "FoColorName.h"

#include <QCryptographicHash>

#define _LINK_COLOR_ \
             QColor("#dc0000")


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
  bool isValid() { return doc.setContent(stream(),false,0,0,0);  }
  QDomDocument Dom() { return doc; }

 

  QIODevice *device() { return d; }
  QByteArray stream() { return d->data(); }
  QString toString() { return QString(d->data()); }
  QDomDocument doc;
  QBuffer *d;
};


class FoRegion
{
public:
  FoRegion();
  FoRegion& operator=( const FoRegion& d );
  operator QVariant() const
  {
    return QVariant::fromValue(*this);
  }
  QString hash() const;
  void styleContainer( QDomElement e , const qreal height = 0 );
  void styleSmallDocument( QTextDocument *doc );
  void styleReadDocument( const QTextDocument *doc );
  void fomax(  FoRegion& m ,  FoRegion& d );
  /* same margin to all */
  void toAll( const qreal unique );
  void marginNoBody();
  QMap<QString,QPen> penstyle();  /* return avaiable pen */
  QColor bg; /* background color */
  QColor bog; /* border color */
  qreal margin_top;
  qreal margin_bottom;
  qreal margin_left;
  qreal margin_right;
  qreal border;
  qreal padding;
  QPen rpen;
  bool enable;
  QByteArray edom;   /* dome element*/
};

Q_DECLARE_METATYPE(FoRegion);


static const int InterSpace = 15;  /* distance from page 1 to 2 */
static const int SliderTopHeight = 28;
static const int SliderLeftHeight = 28;
static const int MaximumPages = 99;
static const int BorderShadow = 5;

static const int PageSizeID = 824;
static const int PageMarginID = 825;
static const int LayerCss2ID = 826;  /* layer css on documents propritety as QString */

static const qreal DEBUgletterspacing = 333;
static const int DEBUgdefaultFontPointSize = 55;


QMap<QString,QByteArray> unzipstream( const QString file );

class ChildImport : public QXmlStreamReader
{
public:
ChildImport::ChildImport( QIODevice* device = 0 );
/* copy all subelement from reader to writer out */
void ChildImport::copyDeep( QIODevice* device , QXmlStreamWriter &out  );
};

class ReadWriteBuf;
class OOReader : public QObject
{
     Q_OBJECT
//
public:
    
   enum WIDGEDEST {
    otextedit,
    ographicview,
    otextbrowser,
    opanel,
    ounknowwidged = 404
  };
  
 OOReader( const QString file = QString() , WIDGEDEST e = otextbrowser , QObject* parent = 0 );
 void openFile( const QString file );
 void openStreams( QMap<QString,QByteArray> list ); /* unzipped file */
 QTextDocument *document() { return Qdoc->clone(); }  /* only body */
 QMap<QString,QTextDocument*> option() { return lateral; } /* header footer fo region */
 

 enum STYLETYPE {
    opara = 100, /* paragraph - block */
    ochar = 200, /* span fragment */
    otable = 300, /* table body footer */
    ocell = 400, /* table cell */
    oframe = 500,  /* inline frame */
    oabsoluteframe = 600, /* floating absolute*/
    obodypage = 1000,  /* body size */
    obefore = 2000, /* header */
    oafter = 3000, /* footer*/
    ostart = 4000,  /* left area */
    oend = 5000,  /* right area*/
    ounknow = 10000
  };
  


  
struct StyleInfo
    {
        StyleInfo();
        StyleInfo(const StyleInfo &d );
        StyleInfo &operator=(const StyleInfo &d);
        QString name;
        uint position;
        QTextFormat of;
        QTextCharFormat ofchar;
        OOReader::STYLETYPE type;
        QByteArray chunk;
        bool valid;
        bool filled;
    };




protected:
    int maxStyleCount;
    int sumOfBlocks;
    uint charsRead;
    WIDGEDEST construct_modus;
    int styleCurrentCount;
    QMap<int,QTextDocument*> layerlist; /* absolute layer having y,x  */
    QMap<QString,QTextDocument*> lateral; /* header footer fo region */
    QMap<QString,StyleInfo> css2; /* all oo style name */
    QMap<QString,QByteArray> filist;  /* all file from zip oo */
    QTextDocument *Qdoc;
    FopColor *FoCol;
    QDomDocument bodyStarter;
    bool DocInitContruct;
    bool QTWRITTELN;
    qreal FontMaxPoint;
    qreal FontMinPoint;
    int imageCurrentCount;
private:
    void styleNameSetup(  const QByteArray chunk   , const QString label   );
    void convertStyleNameRoot( const QDomElement &element );
    bool convertBody( const QDomElement &element );
    bool convertTable( QTextCursor &cur , const QDomElement e  , const int processing );
    bool convertCellTable( const QDomElement e  , QTextTableCell  &cell  ,
                                QTextCursor &cur  , const QString tablenamecss , const int processing );
    bool convertBlock( QTextCursor &cur , QDomElement e  , const int processing );
    bool convertList( QTextCursor &cur , QDomElement e  , const int processing , int level = 1);
    bool convertFragment( QTextCursor &cur , const QDomElement e , QTextCharFormat parent = QTextCharFormat() ,  bool HandleSpace = false );
    void insertTextLine( QTextCursor &cur , QStringList line , QTextCharFormat parent =  QTextCharFormat() ,  bool HandleSpace = false  );

    QTextFrameFormat FrameFormat( const QString name );  /* format from css name class */

   /* formats fill */
   void TextBlockFormatPaint( const QString name , const QDomElement e );
   void TextCharFormatPaint( const QString name , const QDomElement e );
   void TextListFormatPaint( const QString name , const QDomElement e );
   void FrameImageFormatPaint( const QString name , const QDomElement e );  /* image / frame */
   /* table column cell row format container + unknow */
   void UnknowFormatPaint( const QString name , const QString style_name  ,  const QDomElement e );

   QTextBlockFormat TextBlockFormFromDom( const QDomElement e , QTextBlockFormat pf =  QTextBlockFormat() );
   QTextCharFormat TextCharFormFromDom( const QDomElement e , QTextCharFormat pf = QTextCharFormat()  );
   Qt::Alignment TagAlignElement(const QDomElement e );

signals:
    void statusRead(int,int);
    void setError(QString);
    void ready();

public slots:
    void reset(); /* abort or new file next */
private slots:
    void ReadBody();

};
//
#endif // OOREADER_H

