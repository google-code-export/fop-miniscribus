#ifndef INTERNALFRAMESTRUCTURE_H
#define INTERNALFRAMESTRUCTURE_H

#include <QtCore>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QtGui>
#include <QBuffer>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "PngAConfig.h"


class APNGwrittelStream
{
  
  public:
  APNGwrittelStream(QObject *o) :d(new QBuffer()),creator(o),xin(0)
  {
  d->open(QIODevice::ReadWrite);
  start();
  }
  ~APNGwrittelStream()
  {
   d->close();
   ////deleteLater();
  }
  bool clear()
  {
    d->write(QByteArray()); 
    return d->bytesAvailable() == 0 ? true : false;
  }
  void start() { 
    d->seek(0);
    xin = 0;
  }
  uint take( const uint i )
  {
    xin +=i;
    return xin;
  }
  uint target()
  {
    return qAbs(d->bytesAvailable() - xin);
  }
  QBuffer *device() { return d; }
  bool isValid() { return img.loadFromData(stream()); }
  QByteArray stream() { return d->data(); }
  QImage img;
  uint xin;
  QBuffer *d;
  QObject *creator;
}; 


/* read and write progress  */
class PngEventBuffer : public QEvent
{
	//QEvent::User==1000
    
public:
    PngEventBuffer(int status, int total )
	: QEvent( QEvent::User ),s(status),t(total)
    {  }
 int pos() const { return s; }
 int sum() const { return t; }

private:
    int s;
    int t;
};


// Called by libpng to flush its internal buffer to ours.
static void EncoderWriteCallback(png_structp png, png_bytep data,
                                 png_size_t size) {
  APNGwrittelStream *state = static_cast<APNGwrittelStream*>(png_get_io_ptr(png));
  Q_ASSERT(state->device());
  const uint grabytes = state->device()->write((char*)data,size);                              
  const uint s1 = state->take(grabytes);
  const uint s2 = state->target();
  qApp->postEvent(state->creator,new PngEventBuffer( qMin(s2,s1) , qMax(s2,s1) ));

}

// Called by libpng to read QImage on buffer .
static void EncoderReaderCallback(png_structp png, png_bytep data, 
                                   png_size_t size)
{
  APNGwrittelStream *state = static_cast<APNGwrittelStream*>(png_get_io_ptr(png));
  Q_ASSERT(state->device());
  const uint grabytes = state->device()->read((char*)data,size);                              
  const uint s1 = state->take(grabytes);
  const uint s2 = state->target();
  qApp->postEvent(state->creator,new PngEventBuffer( qMin(s2,s1) , qMax(s2,s1) ));
}

static void ng_warning(png_structp /*png_ptr*/, png_const_charp message)
{
    qWarning("APNG warning: %s", message);
}






void PrepareStruct( png_structp* png_ptr_read, png_infop* info_ptr_read);
void writeSetup(FILE** image, png_structp* png_ptr_write, png_infop* info_ptr_write);
void writeSetup2(png_structp png_ptr_write, png_infop info_ptr_write,
                 png_structp png_ptr_read, png_infop info_ptr_read);


void fatalError(char* str);



typedef struct {
	int alpha;
	QColor bg, fg, mg;
  int colortype;
	int compression;
  int bytedep;
  png_color_16p pngBG;
  bool foundcolor;
}  AFRAMES;


class VIFrame
{
    public:
    /* 31.06.2008 */
    enum { MAGICNUMBER = 0xFFAAAFAA, VERSION = 3 };
    VIFrame();
    VIFrame& operator=( const VIFrame& d );
    //////VIFrame operator=( const VIFrame d );
    operator QVariant() const 
    {
    return QVariant::fromValue(*this);
    } 
    QImage exportpic();
    QPixmap videopix(); /* pixmap to display frame */
    QPixmap erno_pix();
    QPixmap pix();  /* pixmap clone from original from read note original is dimg */
    QImage ipix();
    QByteArray stream();
    QImage theardpix();
    void set_pics( const QByteArray bytes ); 
    void set_pics( QPixmap barcode ); 
    void set_pics( const QPixmap * barcode );
    void set_pics( QImage barcode );
    
QRect maxframe;
QByteArray dimg;    /* png stream original from libpng! loading show QPixmap VIFrame::pix()  &&  videopix(); */
uint play;
quint32 mode;  /* default 5= progressive encoding to web  | mode 6 = faster save no qpos y,x */
int pos;   /* frame sequence */
QColor bg; 
QStringList option;   /* reserve to other option! */
QPoint point;
int zliblevel;   
int colortype;  /* type from libpng */  
int dispose;  
/* ##############################################################################*/
/* any time is possibel to read on dimg original  png option!!!!! 
  QPixmap or QImage dont touch dimg play it as const 
  
  typedef struct {
	QRect Maxframe;  die erste frame oder die groeste rect
	int zliblevel;
  QByteArray dimg;  original png stream
  QPoint Position;  x,y im Maxframe
  uint play;  millisekunde play
  QColor BackgroundColor;
  QStringList option;   gamma version 
  int pos;   frame nr
  bool foundcolor;  je nach version png lib 
}  AFRAMES;
  
  
  
*/    
    
/*
   http://wiki.mozilla.org/APNG_Specification
   value
   0           APNG_DISPOSE_OP_NONE
   1           APNG_DISPOSE_OP_BACKGROUND
   2           APNG_DISPOSE_OP_PREVIOUS 
*/
/* ##############################################################################*/


};



inline QDebug operator<<(QDebug debug, VIFrame& udoc)
{
  debug.nospace() << "VIFrame(size."
    << BiteorMega(udoc.dimg.size()) << ",time()"
    << udoc.play << ",mode()"
    << udoc.mode << ",rect()"
    << udoc.maxframe << ")";
  return debug.space();
} 

inline QDataStream& operator<<(QDataStream& out, const VIFrame& d)
{
      out << d.dimg;
      out << d.play;
      out << d.mode;
      out << d.pos;
	    out << d.point;
      out << d.maxframe;
	    out << d.bg;
	    out << d.option;
	    out << d.zliblevel;
	    out << d.option;
	    out << d.colortype;
      return out;
}

inline QDataStream& operator>>(QDataStream& in, VIFrame& d )
{
      in >> d.dimg;
      in >> d.play;
      in >> d.mode;
      in >> d.pos;
	    in >> d.point;
      in >> d.maxframe;
	    in >> d.bg;
	    in >> d.option;
	    in >> d.zliblevel;
	    in >> d.option;
	    in >> d.colortype;
    return in;
} 

/* drag & drop stream one or more */
/* APNG frame data & time to play */
Q_DECLARE_METATYPE(VIFrame); 

////////qRegisterMetaTypeStreamOperators<VIFrame>("VIFrame");

/*
QSettings settings;
Player player;
settings.setValue("Main/FirstFrame", VIFrame);

QSettings settings;
VIFrame player = value("Main/FirstFrame").value<VIFrame>();

*/


inline QByteArray SaveFrameGroup( QMap<int,VIFrame> li  )
{
   if (li.size() < 1) {
     return QByteArray(); 
   }
  QByteArray bytes;
  QBuffer buffer(&bytes);
  if (!buffer.open(QIODevice::WriteOnly)) {
    return QByteArray();
  }
  QDataStream ds(&buffer);
  /* place header */
  ds.setVersion(QDataStream::Qt_4_2);
  ds << (quint32)VIFrame::MAGICNUMBER;
  ds << (quint32)VIFrame::VERSION;
  /* place header */
  
         QMapIterator<int,VIFrame> i(li);
         while (i.hasNext()) {
             i.next();
             VIFrame record = i.value();
             ds << record;
         }      
  buffer.close();
  return bytes;
} 

inline QMap<int,VIFrame> OpenFrameGroup( const QByteArray _stream   )
{
  QMap<int,VIFrame> li;
  QByteArray _Schunk = _stream;
  quint32 magic, version; 
  QBuffer buffer(&_Schunk);
  if (!buffer.open(QIODevice::ReadOnly)) {
    return li;
  }
  QDataStream ds(&buffer);
  /* place header */
  ds.setVersion(QDataStream::Qt_4_2);
  ds >> magic;
  if ( (quint32)VIFrame::MAGICNUMBER != magic ) {
    qDebug() << "######## VIFrame::MAGICNUMBER not ok  ";
    buffer.close();
    return li;
  }
  ds >> version;
  if ( (quint32)VIFrame::VERSION != version ) {
    qDebug() << "######## VIFrame::VERSION not ok  ";
    buffer.close();
    return li;
  }
  VIFrame appoint;
  /* place header */
    while (!ds.atEnd()) {
         ds >> appoint;
         li.insert(appoint.pos,appoint);
    }
  buffer.close();
  return li;
} 



class FrameWrite : public QObject
{
     Q_OBJECT
  
public:
   FrameWrite( QMap<int,VIFrame> li , const QString File_APNG_Format );
   ~FrameWrite();
private:
  bool opening;
  QString file;
  bool MainFrameCache;
  int FrameLoop;
  int FrameSumme;
  QRect FrameArea;

};





class FrameTrade : public QThread
{
    Q_OBJECT
     
public:
  void Setting( QObject *parent , QMap<int,VIFrame> list , const  QString filedest )
  {
    li = list;
    file = filedest;
    setTerminationEnabled(true);
    receiver = parent;
  }
protected:
  void run()
{
  
    FILE* newImage;
    bool globalsSet = false;
    png_structp png_ptr_read;
    png_infop info_ptr_read;
    png_structp png_ptr_write;
    png_infop info_ptr_write;
    png_bytep* row_pointers;
    png_uint_32 bytesPerRow;
    int framenr = -1;
    
    if (li.size() > 0) {
    newImage = fopen(QFile::encodeName(file), "wb"); 
    writeSetup(&newImage, &png_ptr_write, &info_ptr_write);
    if(setjmp(png_ptr_write->jmpbuf))
        fatalError("something didn't work while writing");
    
        QMapIterator<int,VIFrame> i(li);
         while (i.hasNext()) {
                i.next();
                framenr++;
                QImage imageios;
                APNGwrittelStream myqtchunk(receiver);
                VIFrame record = i.value();
                if (record.mode !=6) {
                imageios= record.theardpix();
                imageios.save(myqtchunk.device(),"PNG",0); // writes image into ba in PNG format
                } else {
                /* not having qpos y,x to write special mesure */
                myqtchunk.device()->write(record.stream());
                imageios = QImage(record.maxframe.width(),record.maxframe.height(),QImage::Format_ARGB32);
                }
                myqtchunk.start();  /* block to begin pointer */
                ////////////qDebug() << "### writing frame from " << framenr << " from " << li.size();
                //////////qDebug() << "### rect " << record.maxframe;
                /////////////qDebug() << "### rect "  << imageios.width() << "x" << imageios.height();
             
                PrepareStruct( &png_ptr_read, &info_ptr_read);
                png_set_read_fn(png_ptr_read,&myqtchunk,EncoderReaderCallback);
             
                  if(setjmp(png_ptr_read->jmpbuf))
                     fatalError("something didn't work while reading");
             
                      png_read_info(png_ptr_read, info_ptr_read);
                      const uint height = imageios.height();
                  
                        png_bytep *row_pointers = new png_bytep[height];
                        for (uint i = 0; i < height; ++i)  {
                          row_pointers[i] = (png_bytep)imageios.scanLine(i);
                        }
                        
             
               if(!globalsSet)
                {
                    qDebug() << "### first frame header ";
                    writeSetup2(png_ptr_write, info_ptr_write, png_ptr_read, info_ptr_read);
                    png_set_acTL(png_ptr_write, info_ptr_write, li.size(), 0);
                    png_write_info(png_ptr_write, info_ptr_write);
                    
                      
                    
                    globalsSet = true;
                }
                
                png_read_image(png_ptr_read,row_pointers);
                /* all other frame ..... */
                
                 png_write_frame_head(png_ptr_write, info_ptr_write, row_pointers, 
                                 imageios.width(),  /* width */
                                 imageios.height(), /* height */
                                 0,       /* x offset */
                                 0,       /* y offset */
                                 record.play, 1000,    /* delay numerator and denominator */
                                 PNG_DISPOSE_OP_NONE, /* dispose */
                                 0    /* blend */
                                );
             
             
                png_write_image(png_ptr_write, row_pointers);
                png_write_frame_tail(png_ptr_write, info_ptr_write);
                png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
                myqtchunk.~APNGwrittelStream();
         }
    
       
           
    delete [] row_pointers;
    png_write_end(png_ptr_write, NULL);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    fclose(newImage);
    printf("all done\n");
        
    }
  exit(0);
}

  signals:
private:
    QMap<int,VIFrame> li;
    QString file;
    QObject* receiver;
};







#endif // INTERNALFRAMESTRUCTURE_H

