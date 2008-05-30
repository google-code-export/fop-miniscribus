#ifndef ANIMATIONSDEVICE_H
#define ANIMATIONSDEVICE_H
#include <QtCore>
#include <QDebug>
#include <QApplication>
#include <Qt3Support>
#include <QImage>
#include <QPixmap>
#include <QtGui>
#include <QBuffer>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <QTemporaryFile>


typedef struct {
	int alpha;
	QColor bg, fg, mg;
  int colortype;
	int compression;
}  AFRAMES;


/*
type png_struct_def = record

  jmpbuf: jmp_buf;

  error_fn: png_error_ptr;

  warning_fn: png_error_ptr;

  error_ptr: png_voidp;

  write_data_fn: png_rw_ptr;

  read_data_fn: png_rw_ptr;

  io_ptr: png_voidp;

  read_user_transform_fn: png_user_transform_ptr;

  write_user_transform_fn: png_user_transform_ptr;

  user_transform_ptr: png_voidp;

  user_transform_depth: png_byte;

  user_transform_channels: png_byte;

  mode: png_uint_32;

  flags: png_uint_32;

  transformations: png_uint_32;

  zstream: z_stream;

  zbuf: png_bytep;

  zbuf_size: png_size_t;

  zlib_level: LongInt;

  zlib_method: LongInt;

  zlib_window_bits: LongInt;

  zlib_mem_level: LongInt;

  zlib_strategy: LongInt;

  width: png_uint_32;

  height: png_uint_32;

  num_rows: png_uint_32;

  usr_width: png_uint_32;

  rowbytes: png_uint_32;

  irowbytes: png_uint_32;

  iwidth: png_uint_32;

  row_number: png_uint_32;

  prev_row: png_bytep;

  row_buf: png_bytep;

  sub_row: png_bytep;

  up_row: png_bytep;

  avg_row: png_bytep;

  paeth_row: png_bytep;

  row_info: png_row_info;

  idat_size: png_uint_32;

  crc: png_uint_32;

  palette: png_colorp;

  num_palette: png_uint_16;

  num_trans: png_uint_16;

  chunk_name: array ... of png_byte;

  compression: png_byte;

  filter: png_byte;

  interlaced: png_byte;

  pass: png_byte;

  do_filter: png_byte;

  color_type: png_byte;

  bit_depth: png_byte;

  usr_bit_depth: png_byte;

  pixel_depth: png_byte;

  channels: png_byte;

  usr_channels: png_byte;

  sig_bytes: png_byte;

  filler: png_uint_16;

  background_gamma_type: png_byte;

  background_gamma: double;

  background: png_color_16;

  background_1: png_color_16;

  output_flush_fn: png_flush_ptr;

  flush_dist: png_uint_32;

  flush_rows: png_uint_32;

  gamma_shift: LongInt;

  gamma: double;

  screen_gamma: double;

  gamma_table: png_bytep;

  gamma_from_1: png_bytep;

  gamma_to_1: png_bytep;

  gamma_16_table: png_uint_16pp;

  gamma_16_from_1: png_uint_16pp;

  gamma_16_to_1: png_uint_16pp;

  sig_bit: png_color_8;

  shift: png_color_8;

  trans: png_bytep;

  trans_values: png_color_16;

  read_row_fn: png_read_status_ptr;

  write_row_fn: png_write_status_ptr;

  info_fn: png_progressive_info_ptr;

  row_fn: png_progressive_row_ptr;

  end_fn: png_progressive_end_ptr;

  save_buffer_ptr: png_bytep;

  save_buffer: png_bytep;

  current_buffer_ptr: png_bytep;

  current_buffer: png_bytep;

  push_length: png_uint_32;

  skip_length: png_uint_32;

  save_buffer_size: png_size_t;

  save_buffer_max: png_size_t;

  buffer_size: png_size_t;

  current_buffer_size: png_size_t;

  process_mode: LongInt;

  cur_palette: LongInt;

  current_text_size: png_size_t;

  current_text_left: png_size_t;

  current_text: png_charp;

  current_text_ptr: png_charp;

  palette_lookup: png_bytep;

  dither_index: png_bytep;

  hist: png_uint_16p;

  heuristic_method: png_byte;

  num_prev_filters: png_byte;

  prev_filters: png_bytep;

  filter_weights: png_uint_16p;

  inv_filter_weights: png_uint_16p;

  filter_costs: png_uint_16p;

  inv_filter_costs: png_uint_16p;

  time_buffer: png_charp;

  free_me: png_uint_32;

  user_chunk_ptr: png_voidp;

  read_user_chunk_fn: png_user_chunk_ptr;

  num_chunk_list: LongInt;

  chunk_list: png_bytep;

  rgb_to_gray_status: png_byte;

  rgb_to_gray_red_coeff: png_uint_16;

  rgb_to_gray_green_coeff: png_uint_16;

  rgb_to_gray_blue_coeff: png_uint_16;

  empty_plte_permitted: png_byte;

  int_gamma: png_fixed_point;

end;

*/

////base ref http://www.littlesvr.ca/apng/tutorial/x148.html
/* png lib from firefox 3 is patch to APNG format! */
#include "../moz_png/png.h"

#define PNG_USER_MEM_SUPPORTED


void readSetup(const QString file, png_structp* png_ptr_read, png_infop* info_ptr_read);
AFRAMES writeSetup2(png_structp png_ptr_read, png_infop info_ptr_read,
                 png_structp png_ptr_write, png_infop info_ptr_write,
                 png_uint_32 width, png_uint_32 height);
void writeSetup(FILE* image, png_structp* png_ptr_write, png_infop* info_ptr_write);

extern inline bool FillFile( const QString file  , const QByteArray stream )
{
  bool xx = false;
  
          QFile fxi(file); 
          if (fxi.open(QIODevice::WriteOnly)) {
             fxi.write(stream);
             fxi.close();
             xx = true;
          }
          
  return xx;
}

extern inline QString  BiteorMega( int peso  )
{
    QString humanread;
    qreal  faktor = 1024.00;

    qreal bytesizefile = peso / faktor;
    int kilo = 0;
    
    if (bytesizefile > 0) {
        if (bytesizefile < 1) {
           kilo = 1; 
        } else {
           kilo = bytesizefile;
        }
    }
    
   if (kilo < 1025) {
   humanread = QString("Kb.%1").arg(kilo);
   return humanread;
   }
   
   qreal megad = bytesizefile / faktor;
   
   if (megad < 1025) {
   humanread = QString("MB.%1").arg(megad, 0, 'f', 2);
   return humanread;
   } else {
            qreal gigab = megad / faktor;
            humanread = QString("GB.%1").arg(gigab, 0, 'f', 2);
       
       
   }
   
return humanread;
}


static inline void OpenDesk( QUrl loc )
{
    #if defined Q_WS_MAC
    QProcess *m = new QProcess();
    QStringList macs;
    macs << loc.toString();  /* oeffnet der default browser */
    m->startDetached(QString("open") , macs );
    ///////std::cout << "OpenDesktop [" << qPrintable(macs.join(" ")) << "]" << std::endl;
    return;
    #endif 
    
#if defined Q_WS_WIN
    
    QString fullFileName = loc.toString();
    
    if (fullFileName.startsWith("http://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("https://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("ftp://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("news://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("mailto:", Qt::CaseInsensitive) ||
        fullFileName.startsWith("webdav://", Qt::CaseInsensitive)  )
       { 
         /* all ok */
       } else {
         fullFileName.prepend("file:///");  
       }
    bool wr = QDesktopServices::openUrl(QUrl(fullFileName));
    if (!wr) {
    QMessageBox::warning(0, qApp->tr("Error"),qApp->tr("Window Unable to open action file or dir  %1").arg(loc.toString()));
    }
return;
#endif
    
    /* linux */

     bool r = QDesktopServices::openUrl(loc);
    if (!r) {
    QMessageBox::warning(0, qApp->tr("Error"),qApp->tr("Linux Unable to open action file or dir  %1").arg(loc.toString()));
    }
    
}



class APngDevice {
  public:
  APngDevice()
    :newImage(new QBuffer()) { }
  bool canread()
  {
    return img.loadFromData(newImage->data());
  }
  QBuffer* device() 
  { 
   return newImage; 
  }
  ~APngDevice() 
  {
   newImage->close(); 
   newImage->deleteLater();
  }
  QImage img;
  QBuffer *newImage;
};


class VIFrame
{
    public:
    enum { MAGICNUMBER = 0xFFAAAFAA, VERSION = 2 };
    VIFrame();
    VIFrame& operator=( const VIFrame& d );
    operator QVariant() const 
    {
    return QVariant::fromValue(*this);
    } 
    QPixmap videopix(); /* pixmap to display frame */
    QPixmap erno_pix();
    QPixmap pix();
    QImage ipix();
    QByteArray stream();
    void set_pics( const QByteArray bytes ); 
    void set_pics( QPixmap barcode ); 
    void set_pics( const QPixmap * barcode );
    
QRect maxframe;
QByteArray dimg;
uint play;
quint32 mode; 
int pos;   
QColor bg; 
QPoint point;    
    
};

/* APNG frame data & time to play */
Q_DECLARE_METATYPE(VIFrame); 

inline QDebug operator<<(QDebug debug, VIFrame& udoc)
{
  debug.nospace() << "VIFrame(size."
    << BiteorMega(udoc.dimg.size()) << ",time()"
    << udoc.play << ",mode()"
    << udoc.mode << ",rect()"
    << udoc.maxframe << ")";
  return debug.space();
} 


class FrameIterator {
  public:
    FrameIterator( const QString File_APNG_Format );
    FrameIterator();
    ~FrameIterator();
    void ReadStream();
    bool ValidApng;
    QRect Frect;
    uint default_play_time_ms;
    QString qfile;
    qreal secondT;
    inline QMap<int,VIFrame> frames() { return movie; }
    inline double playtime() { return secondT; }
    inline int framecount() { return movie.size(); }
    QMap<int,VIFrame> movie;
};



class PMovie : public QLabel
{
     Q_OBJECT
//
public:
  PMovie( QWidget *parent = 0 );
  QMenu *MovieMenu();
protected:
  void contextMenuEvent( QContextMenuEvent * e );
  FrameIterator *movie;
  QMap<int,VIFrame> playmovie;
  int current;
  bool running;
private:
signals:
public slots:
void OpenDemo();
void Stop();
void restart();
void Play();
void OpenFile();
void ExportFrames();
void LaunchFile( const QString file );
void ComposeFrame();
private slots:
void NextFrame();

};







#endif // ANIMATIONSDEVICE_H
