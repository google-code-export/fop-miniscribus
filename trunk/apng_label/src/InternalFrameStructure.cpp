#include "InternalFrameStructure.h"


void fatalError(char* str)
{
    fprintf(stderr, "Fatal Error: %s\n", str);
    exit(1);
}


FrameWrite::FrameWrite( QMap<int,VIFrame> li , const QString File_APNG_Format ) 
  : file(File_APNG_Format),opening(true),MainFrameCache(false)
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
                APNGwrittelStream myqtchunk(this);
                VIFrame record = i.value();
                QImage imageios = record.exportpic();
                imageios.save(myqtchunk.device(),"PNG",1); // writes image into ba in PNG format
                myqtchunk.start();  /* block to begin pointer */
                qDebug() << "### writing frame from " << framenr << " from " << li.size();
                qDebug() << "### rect " << record.maxframe;
                qDebug() << "### rect "  << imageios.width() << "x" << imageios.height();
             
                PrepareStruct( &png_ptr_read, &info_ptr_read);
                png_set_read_fn(png_ptr_read,&myqtchunk,EncoderReaderCallback);
             
                  if(setjmp(png_ptr_read->jmpbuf))
                     fatalError("something didn't work while reading");
             
                png_read_info(png_ptr_read, info_ptr_read);
                  
                      uint height = imageios.height();
                      const uchar* const* jt = imageios.jumpTable();
                      row_pointers=new png_bytep[height];
                      uint y;
                      for (y=0; y<height; y++) {
                                // PNG lib has const issue with the write image function
                                row_pointers[y]=const_cast<png_byte*>(jt[y]);
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
    
       
           
         
    png_write_end(png_ptr_write, NULL);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    fclose(newImage);
    printf("all done\n");
        
    }
}






void PrepareStruct( png_structp* png_ptr_read, png_infop* info_ptr_read)
{

    *png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(*png_ptr_read == NULL)
        fatalError("unable to create read struct");
    
    *info_ptr_read = png_create_info_struct(*png_ptr_read);
    if(*info_ptr_read == NULL)
        fatalError("unable to create info struct");
    
    //////////png_init_io(*png_ptr_read, *image);
    //////////png_set_sig_bytes(*png_ptr_read, 8);
}

void writeSetup(FILE** image, png_structp* png_ptr_write, png_infop* info_ptr_write)
{
    
    if(image == NULL)
        fatalError("couldn't open original png");
    
    *png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(*png_ptr_write == NULL)
        fatalError("unable to create write struct");
    
    *info_ptr_write = png_create_info_struct(*png_ptr_write);
    if(*info_ptr_write == NULL)
        fatalError("unable to create write struct");
    
    png_init_io(*png_ptr_write, *image);
}

void writeSetup2(png_structp png_ptr_write, png_infop info_ptr_write,
                 png_structp png_ptr_read, png_infop info_ptr_read)
{
    /* IHDR */
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int colour_type;
    int interlace_method;
    int compression_method;
    int filter_method;
    
    /* PLTE */
    png_colorp palette = NULL;
    int palette_size = 0;
    
    /* gAMA */
    double gamma;
    
    /* tRNS */
    png_bytep trans;
    int num_trans;
    png_color_16p trans_values;
    
    /* bKGD */
    png_color_16p background;
    
    png_get_IHDR(png_ptr_read, info_ptr_read, &width, &height,
                 &bit_depth, &colour_type, &interlace_method,
                 &compression_method, &filter_method);
    png_set_IHDR(png_ptr_write, info_ptr_write,  width, height,
                 bit_depth, colour_type, interlace_method,
                 compression_method, filter_method);
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_PLTE))
    {
        png_get_PLTE(png_ptr_read, info_ptr_read, &palette, &palette_size);
        png_set_PLTE(png_ptr_write, info_ptr_write, palette, palette_size);
    }
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_gAMA))
    {
        png_get_gAMA(png_ptr_read, info_ptr_read, &gamma);
        png_set_gAMA(png_ptr_write, info_ptr_write, gamma);
    }
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_tRNS))
    {
        png_get_tRNS(png_ptr_read, info_ptr_read, &trans, &num_trans, &trans_values);
        png_set_tRNS(png_ptr_write, info_ptr_write, trans, num_trans, trans_values);
    }
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_bKGD))
    {
        png_get_bKGD(png_ptr_read, info_ptr_read, &background);
        png_set_bKGD(png_ptr_write, info_ptr_write, background);
    }
}














FrameWrite::~FrameWrite()
{
    
}





/* definition from one qt frame to play */

 VIFrame::VIFrame()
 {
    play = 1000;
    dimg = QByteArray("error"); 
    mode = 5;
    pos = 0;
    bg = QColor(Qt::transparent);
    point = QPoint(0,0);  /* QPoint point */
}
VIFrame& VIFrame::operator=( const VIFrame& d )
{
			dimg = d.dimg;
      play = d.play;
      mode = d.mode;
      pos = d.pos;
	    point = d.point;
      maxframe = d.maxframe;
	    bg = d.bg;
	    option = d.option;
	    zliblevel = d.zliblevel;
	    option = d.option;
	    colortype = d.colortype;
      return *this;
}
/*
VIFrame VIFrame::operator=( const VIFrame d )
{
      dimg = d.dimg;
      play = d.play;
      mode = d.mode;
      pos = d.pos;
	    point = d.point;
      maxframe = d.maxframe;
	    bg = d.bg;
	    option = d.option;
	    zliblevel = d.zliblevel;
	    option = d.option;
	    colortype = d.colortype;
      return *this;
}
*/

/*

QRect maxframe;
QByteArray dimg;    
uint play;
quint32 mode;  
int pos;   
QColor bg; 
QStringList option;  
QPoint point;
int zliblevel;   
int colortype;  

*/

    
QPixmap VIFrame::erno_pix() 
{
    maxframe = QRect(0,0,250,250);
    
    QPixmap pError = QPixmap(maxframe.width(),maxframe.height());
    pError.fill( Qt::red  );
    QPainter pter( &pError );
    pter.setFont( QFont( "Helvetica", 8 ) );
    pter.setBrush( Qt::green );
    pter.drawText( 20, 12 , "Sorry is not APNG image!" );
    return pError;
} 
QPixmap VIFrame::pix() 
{
      if (dimg.size() < 1 || dimg == QByteArray("error")) {
      return erno_pix();
      }
      QPixmap resultimage;
      QByteArray daunq = qUncompress( dimg );
      resultimage.loadFromData( daunq );
          if (resultimage.isNull()) {
            return erno_pix();
          }  
    return resultimage;
} 

QPixmap VIFrame::videopix() 
{
    QPixmap base = pix();
    ///////qDebug() << "### option  ->" << option;
   //////// qDebug() << "### play  ->" << play;
    ////////qDebug() << "### rect  ->" << maxframe;
    ///////////qDebug() << "### rect pi ->" << base.width()<< "," << base.width();
    QPixmap Pvidi = QPixmap(maxframe.width(),maxframe.height());
    Pvidi.fill(bg);
    QPainter p( &Pvidi );
    p.drawPixmap(point,base);
    return Pvidi;
}


QImage VIFrame::theardpix() 
{
    QImage base;
    QByteArray daunq = qUncompress( dimg );
    base.loadFromData( daunq );
    QRectF target(point.x(),point.y(),base.width(),base.height());
    QImage Pvidi(maxframe.width(),maxframe.height(),32);
    QPainter p( &Pvidi );
    p.setBrush(bg);
    p.drawRect(maxframe);
    p.drawImage(target,base);
    return Pvidi.convertDepth(32);
}

QImage VIFrame::exportpic()
{
   QImage alla = videopix().toImage();
   return alla.convertDepth(32);
}

QByteArray VIFrame::stream()
{
   return qUncompress( dimg ); 
}

QImage VIFrame::ipix() 
{
    if (dimg.size() < 1) {
    return erno_pix().toImage();
    }
    QImage resultimage;
    QByteArray daunq = qUncompress( dimg );
    resultimage.loadFromData( daunq );
    if (resultimage.isNull()) {
    return erno_pix().toImage();
    }  
    return resultimage;
}
    
void VIFrame::set_pics( const QByteArray bytes )
{
     dimg = qCompress(bytes,9);
}
void VIFrame::set_pics( const QPixmap * barcode )
{
      if (barcode->isNull()) {
        return;
      }
      QByteArray bytes;
      QBuffer buffer(&bytes);
      buffer.open(QIODevice::WriteOnly);
      barcode->save(&buffer,"PNG");
      dimg = qCompress(bytes,9);
} 

void VIFrame::set_pics( QPixmap barcode )
{
      if (barcode.isNull()) {
        return;
      }
      QByteArray bytes;
      QBuffer buffer(&bytes);
      buffer.open(QIODevice::WriteOnly);
      barcode.save(&buffer,"PNG");
      dimg = qCompress(bytes,9);
} 
