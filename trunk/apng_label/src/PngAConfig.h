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

#ifdef OPCAMENABLE

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>

static QImage IplImageToQImage(const IplImage  *iplImage, uchar **data , bool mirroRimage = true )
{
  uchar *qImageBuffer = NULL;
  int    width        = iplImage->width;

  /*
   * Note here that OpenCV image is stored so that each lined is 32-bits aligned thus
   * explaining the necessity to "skip" the few last bytes of each line of OpenCV image buffer.
   */
  int widthStep = iplImage->widthStep;
  int height    = iplImage->height;

  switch (iplImage->depth)
  {
    case IPL_DEPTH_8U:
      if (iplImage->nChannels == 1)
      {
        /* OpenCV image is stored with one byte grey pixel. We convert it to an 8 bit depth QImage. */
        qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
        uchar *QImagePtr = qImageBuffer;
        const uchar *iplImagePtr = (const uchar *)iplImage->imageData;
        for (int y = 0; y < height; ++y)
        {
          // Copy line by line
          memcpy(QImagePtr, iplImagePtr, width);
          QImagePtr += width;
          iplImagePtr += widthStep;
        }
      }
      else if (iplImage->nChannels == 3)
      {
        /* OpenCV image is stored with 3 byte color pixels (3 channels). We convert it to a 32 bit depth QImage. */
        qImageBuffer = (uchar *)malloc(width * height * 4 * sizeof(uchar));
        uchar *QImagePtr = qImageBuffer;
        const uchar *iplImagePtr = (const uchar *)iplImage->imageData;

        for (int y = 0; y < height; ++y)
        {
          for (int x = 0; x < width; ++x)
          {
            // We cannot help but copy manually.
            QImagePtr[0] = iplImagePtr[0];
            QImagePtr[1] = iplImagePtr[1];
            QImagePtr[2] = iplImagePtr[2];
            QImagePtr[3] = 0;

            QImagePtr += 4;
            iplImagePtr += 3;
          }
          iplImagePtr += widthStep - 3 * width;
        }
      }
      else
        qDebug("IplImageToQImage: image format is not supported : depth=8U and %d channels\n", iplImage->nChannels);

      break;

    case IPL_DEPTH_16U:
      if (iplImage->nChannels == 1)
      {
        /* OpenCV image is stored with 2 bytes grey pixel. We convert it to an 8 bit depth QImage. */
        qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
        uchar *QImagePtr = qImageBuffer;
        const uint16_t *iplImagePtr = (const uint16_t *)iplImage->imageData;

        for (int y = 0; y < height; ++y)
        {
          for (int x = 0; x < width; ++x)
            *QImagePtr++ = ((*iplImagePtr++) >> 8); // We take only the highest part of the 16 bit value. It is similar to dividing by 256.
          iplImagePtr += widthStep / sizeof(uint16_t) - width;
        }
      }
      else
        qDebug("IplImageToQImage: image format is not supported : depth=16U and %d channels\n", iplImage->nChannels);

      break;

    case IPL_DEPTH_32F:
      if (iplImage->nChannels == 1)
      {
        /* OpenCV image is stored with float (4 bytes) grey pixel. We convert it to an 8 bit depth QImage. */
        qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
        uchar *QImagePtr = qImageBuffer;
        const float *iplImagePtr = (const float *)iplImage->imageData;

        for (int y = 0; y < height; ++y)
        {
          for (int x = 0; x < width; ++x)
            *QImagePtr++ = (uchar)(255 * ((*iplImagePtr++)));
          iplImagePtr += widthStep / sizeof(float) - width;
        }
      }
      else
        qDebug("IplImageToQImage: image format is not supported : depth=32F and %d channels\n", iplImage->nChannels);

      break;

    case IPL_DEPTH_64F:
      if (iplImage->nChannels == 1)
      {
        /* OpenCV image is stored with double (8 bytes) grey pixel. We convert it to an 8 bit depth QImage. */
        qImageBuffer = (uchar *) malloc(width * height * sizeof(uchar));
        uchar *QImagePtr = qImageBuffer;
        const double *iplImagePtr = (const double *) iplImage->imageData;

        for (int y = 0; y < height; ++y)
        {
          for (int x = 0; x < width; ++x)
            *QImagePtr++ = (uchar)(255 * ((*iplImagePtr++)));
          iplImagePtr += widthStep / sizeof(double) - width;
        }
      }
      else
        qDebug("IplImageToQImage: image format is not supported : depth=64F and %d channels\n", iplImage->nChannels);

      break;

    default:
      qDebug("IplImageToQImage: image format is not supported : depth=%d and %d channels\n", iplImage->depth, iplImage->nChannels);
  }

  QImage *qImage;
  if (iplImage->nChannels == 1)
  {
    QVector<QRgb> colorTable;
    for (int i = 0; i < 256; i++)
      colorTable.push_back(qRgb(i, i, i));

    qImage = new QImage(qImageBuffer, width, height, QImage::Format_Indexed8);
    qImage->setColorTable(colorTable);
  }
  else
    qImage = new QImage(qImageBuffer, width, height, QImage::Format_RGB32);
    QImage gd0 = qImage->mirrored(false,mirroRimage);
    *data = qImageBuffer;
    QColor textColor = Qt::black;
    QColor fillrectcolor = Qt::red;
    QColor shapepicture = Qt::white;
    QImage gd = gd0.scaledToWidth(350);
    QDateTime now = QDateTime::currentDateTime ();
    QString selectionText = now.toString("dd.MM.yyyy hh:mm:ss");
    QPainter p(&gd);
    p.setRenderHint(QPainter::Antialiasing, true);
  
    QFontMetrics fm( qApp->font() );
    int stringWidth = fm.width(selectionText);
    int stringHeight = fm.ascent();
    const int sx = gd.width() - stringWidth - 5;
    QPen pen;
    pen.setStyle( Qt::SolidLine );
    pen.setWidth( 2 );
    pen.setColor( textColor );
    p.setPen( pen);
    p.drawText(QPointF(sx - 1 ,gd.height() - 2 - stringHeight - 1),selectionText);
    pen.setColor( fillrectcolor );
    p.setPen( pen);
    p.drawText(QPointF(sx,gd.height() - 2 - stringHeight),selectionText);
  
  return gd;
}



#endif




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
