#ifndef APNGCONFIG_H
#define APNGCONFIG_H

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
#include "../moz_png/png.h" 


static  QImage GreyScale( QImage income )
{
    
    QImage base =  income.convertToFormat(QImage::Format_RGB32);
    for (int y = 0; y < base.height(); ++y) {
             for (int x = 0; x < base.width(); ++x) {
                 int pixel = base.pixel(x, y);
                 int gray = qGray(pixel);
                 int alpha = qAlpha(pixel);
                 base.setPixel(x, y, qRgba(gray, gray, gray, alpha));
             }
         }
         
    return base;
}




typedef struct {
  unsigned char *buf;
  unsigned char *p;
  long          remain;
} BufferSource;


static void BufferReadImage( png_structp png_ptr, png_bytep data, png_size_t length )
{
  BufferSource *psource = static_cast<BufferSource*>(png_get_io_ptr(png_ptr));
  if ( psource->remain >= length ) {
	memcpy( data, psource->p, length);
	psource->p += length;
	psource->remain -= length;
  }  else {
	png_error( png_ptr, "illegal data read");
  }
}




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













#endif
