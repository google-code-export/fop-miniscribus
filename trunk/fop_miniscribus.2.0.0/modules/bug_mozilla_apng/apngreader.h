#ifndef APNGREADER_H
#define APNGREADER_H
#include <iostream.h>
#include <QtCore>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QtGui>
#include <QBuffer>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define PNG_USER_MEM_SUPPORTED
#define PNG_WRITE_bKGD_SUPPORTED
////base ref http://www.littlesvr.ca/apng/tutorial/x148.html
/* png lib from firefox 3 is patch to APNG format! */
#include "png.h" 


typedef struct {
  QColor bg; /* background color */
  QRect maxframe; /* the max width() x height() from all frame */
  QPoint point; /* y,x draw image */
  QImage item;
  int pos;  /* position on chain 0,1,2,3 */
  uint play; /*  uint millisecond time/ 1000=1sec.  display */
  QByteArray dimg; /* imagedata original png stream not Qimage or so */
} APNGFRAME;


/* */

typedef struct {
	int alpha;
	QColor bg, fg, mg;
  int colortype;
	int compression;
  int bytedep;
  png_color_16p pngBG;
  bool foundcolor;
}  AFRAMES;


/* image sender event  */
class ImageEvent : public QEvent
{
	//QEvent::User==1000
public:
    ImageEvent( APNGFRAME gd )
	: QEvent( QEvent::User ),img(gd)
    {  }
 QImage frame() const { return img.item; }
 QByteArray psave() const { return img.dimg; }
 int time() const { return img.play; }
 QSize size() const { return img.maxframe.size(); }
private:
    APNGFRAME img;
};


/* device to write or read from QThread or not */

class StreamFile
{
  public:
  StreamFile() 
  :d(new QBuffer())
  {
  d->open(QIODevice::ReadWrite);
  start();
  }
  ~StreamFile()
  {
   d->close();
  }
  bool clear()
  {
    d->write(QByteArray()); 
    return d->bytesAvailable() == 0 ? true : false;
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
      if (f.open(QFile::WriteOnly)) {
        uint bi = f.write(d->readAll());
        start();
        return bi > 0 ? true : false;
      }
   return false; 
  }
  QBuffer *device() { return d; }
  bool isValid() { return img.loadFromData(stream()); }
  QByteArray stream() { return d->data(); }
  QImage img;
  QBuffer *d;
}; 

// Called by libpng to flush its internal buffer to ours.
static void EncoderWriteCallback(png_structp png, png_bytep data,
                                 png_size_t size) {
  StreamFile *state = static_cast<StreamFile*>(png_get_io_ptr(png));
  Q_ASSERT(state->device());
  const uint grabytes = state->device()->write((char*)data,size);
}

// Called by libpng to read QImage on buffer .
static void EncoderReaderCallback(png_structp png, png_bytep data, 
                                   png_size_t size)
{
  StreamFile *state = static_cast<StreamFile*>(png_get_io_ptr(png));
  Q_ASSERT(state->device());
  const uint grabytes = state->device()->read((char*)data,size);
}

AFRAMES OneFrameReader(png_structp png_ptr_read, png_infop info_ptr_read,
                 png_structp png_ptr_write, png_infop info_ptr_write,
                 png_uint_32 width, png_uint_32 height);



class Openchunk : public QThread
{
    Q_OBJECT
     
public:
  inline void Setting( const QString file  ) {
    subfile = file;
    validpng = false;
    setTerminationEnabled(true);
  }
  bool validpng;
  QString error() { return lastmessage; }
  QMap<int,APNGFRAME> list() { return framlist; }
  int valid() { return framlist.size(); }
  QImage first() { return frame1; } /* if not apng return a image ...*/
protected:
  void alert( QString msg , int line = 0 ) {
    qWarning() << msg << " OnLine: " << line;
    lastmessage = msg;
    exit(0);
  }
  void run();
  signals:
  private:
  QImage frame1;
  QString subfile;
  QString lastmessage;
  StreamFile *buffer;
  QMap<int,APNGFRAME> framlist;
};


class FrameReader : public QObject
  {
    
    Q_OBJECT
    
  public:
    FrameReader( QObject *reciver , const QString apngfile = QString(), uint defaulttime = 1000 );
    ~FrameReader();
    void append( QImage recimg , int playtime = -1 );
    void OpenFile();
    void start();
    bool ValidApng;
    private:
    QRect Frect;
    uint default_play_time_ms;
    QString qfile;
    qreal secondT;
    inline QMap<int,APNGFRAME> frames() { return movie; }
    inline double playtime() { return secondT; }
    inline int framecount() { return movie.size(); }
    QMap<int,APNGFRAME> movie;
    QObject *parent;
    Openchunk *moviereader;
    StreamFile *buffer;
    bool running;
    uint running_loop;
    int current;
    private slots:
    void ReadStream();
    void NextFrame();
    
    
};


class Sample : public QWidget
{
    Q_OBJECT
//
public:
 Sample( QWidget * parent  = 0 )
{
FrameReader *xdev = new FrameReader(this,QString::fromUtf8(":/image/Kubus.png") /* filename */ );
/////xdev->append(QImage("1.png"));  
//////xdev->append(QImage("2.png"));
/////xdev->start();
}
protected:
void paintEvent(QPaintEvent *e) 
{
		QPainter p(this);
    if (!Fimg.isNull()) {
    p.drawImage(QPointF(0,0),Fimg);
    }
}
bool event ( QEvent * e )
{
   if ( e->type() == QEvent::User ) {
				    ImageEvent *ep = static_cast<ImageEvent *>(e);
            if (ep) {
             Fimg = ep->frame();
             setMinimumSize(ep->size());  /* 1/2*/
             setMaximumSize(ep->size());
             update();
            }
						return true;
		}
  return QWidget::event(e);   
}
private:
  QImage Fimg;
  signals:
  public slots:
};




//
#endif // APNGREADER_H

