#include "animationdevice.h"


static 
void rec_png_warning(png_structp /*png_ptr*/, png_const_charp message)
{
    qWarning("ANIMATIONSDEVICE libpng warning: %s", message);
}


FrameIterator::~FrameIterator()
{
  movie.clear();
}

FrameIterator::FrameIterator()
 :qfile(0),default_play_time_ms(0),ValidApng(false) 
{
    
}

FrameIterator::FrameIterator( const QString File_APNG_Format )
  :qfile(File_APNG_Format),default_play_time_ms(1000),ValidApng(false) 
{ 
    qDebug() << "### libpng version  ->" << PNG_LIBPNG_VER;
    movie.clear();
    ReadStream();
    if (ValidApng) { 
        uint timelengh = 0;
         QMapIterator<int,VIFrame> i(movie);
         while (i.hasNext()) {
             i.next();
             VIFrame record = i.value();
             timelengh +=record.play;
             ///////qDebug() << i.key() << ": " << record;
         }
         secondT = timelengh / 1000.;
    qDebug() << "### total frame ->" << framecount() << " PlayTime->" << playtime();
        
    } else {
      qWarning("Not a valid APNG file");  
    }
}


void FrameIterator::ReadStream()
{
    png_structp png_ptr_read;
    png_infop info_ptr_read;
    int count;
    png_structp png_ptr_write;
    png_infop info_ptr_write;
    FILE* newImage;
    char filename[512];
    png_uint_32 bytesPerRow;
    png_uint_32 next_frame_width;
    png_uint_32 next_frame_height;
    png_uint_32 next_frame_x_offset;
    png_uint_32 next_frame_y_offset;
    png_uint_16 next_frame_delay_num;
    png_uint_16 next_frame_delay_den;
    png_byte next_frame_dispose_op;
    png_byte next_frame_blend_op;
    png_color_16p Framebg;
  
  
    readSetup(qfile, &png_ptr_read, &info_ptr_read);
    if(setjmp(png_ptr_read->jmpbuf)) {
        qWarning("something didn't work, jump 1");
        return;
    }
    png_read_info(png_ptr_read, info_ptr_read);
    
    if(!png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_acTL)) {
        qWarning("source image must be animated");
        return;
    }
    
    Frect = QRect(0,0,info_ptr_read->width,info_ptr_read->height);
    //////qDebug() << "### Frame rect ->" << Frect;
    QImage master(Frect.width(),Frect.height(),32);
    const uchar* const* jt = master.jumpTable();
    png_bytep* row_pointers;
    row_pointers=new png_bytep[Frect.height()];
    uint y;
    for (y=0; y<Frect.height(); y++) {
        row_pointers[y]=const_cast<png_byte*>(jt[y]);
    }
    
        
    for(int i = 0; i < png_get_num_frames(png_ptr_read, info_ptr_read); i++)
    {
        /////qDebug() << "### frame read  ------------- " << i;
        QTemporaryFile tmpfile;
        tmpfile.setAutoRemove ( true );
        if (tmpfile.open()) {
        /////////qDebug() << "### file.fileName() " << tmpfile.fileName();
        ///////sprintf(filename, "extracted-%02d.png",i);
        newImage = fopen(qPrintable(tmpfile.fileName()), "wb");
        if(newImage == NULL)  {
            qWarning("couldn't create png for writing");
            return;
        }
        writeSetup(newImage, &png_ptr_write, &info_ptr_write);
        if(setjmp(png_ptr_write->jmpbuf))  {
            qWarning("something didn't work, jump 2");
            return;
        }
        png_read_frame_head(png_ptr_read, info_ptr_read);
        
        if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_fcTL))
        {
            png_get_next_frame_fcTL(png_ptr_read, info_ptr_read,
                                    &next_frame_width, &next_frame_height,
                                    &next_frame_x_offset, &next_frame_y_offset,
                                    &next_frame_delay_num, &next_frame_delay_den,
                                    &next_frame_dispose_op, &next_frame_blend_op);
        }
        else
        {
            /* the first frame doesn't have an fcTL so it's expected to be hidden, 
            * but we'll extract it anyway  next_frame_x_offset , next_frame_y_offset */ 
            next_frame_width = png_get_image_width(png_ptr_read, info_ptr_read);
            next_frame_height = png_get_image_height(png_ptr_read, info_ptr_read);
        }
        QRect C_frame_rect(0,0,next_frame_width,next_frame_height);
        if ( Frect.contains(C_frame_rect) )  { 
           ///////qDebug() << "### read in ok   "; 
        }
        /* ##################### alert speed file!##############################################*/
        AFRAMES FrameInfo = writeSetup2(png_ptr_read, info_ptr_read, png_ptr_write, info_ptr_write,
        next_frame_width, next_frame_height);
        /* ##################### alert speed file!##############################################*/
        
        png_write_info(png_ptr_write, info_ptr_write);
        png_read_image(png_ptr_read, row_pointers);
        png_write_image(png_ptr_write, row_pointers);
        png_write_end(png_ptr_write, NULL);
        
        if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_bKGD))
        {
        png_get_bKGD(png_ptr_read, info_ptr_read, &Framebg);
        }
        
        ////png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        /* ##################### alert speed file!##############################################*/
        fclose(newImage);
        /* ##################### alert speed file!##############################################*/
        /////////////   ### Framebg   1474 39928 34
        ////QPixmap framimg(tmpfile.fileName());
        /* direct to QByteArray qpixmap or qimage to long time .... */
        QFile f(tmpfile.fileName()); 
        if (f.exists()) {
                if (f.open(QIODevice::ReadOnly)) {
                    VIFrame Ftoc;
                    //////qDebug() << "### Framebg  " << FrameInfo.bg;
                    Ftoc.bg = FrameInfo.bg;
                    if ( next_frame_x_offset !=0 || next_frame_y_offset !=0) {
                        Ftoc.point = QPoint(next_frame_x_offset,next_frame_y_offset); 
                    }
                    
                    Ftoc.set_pics( f.readAll() );
                    const uint timeframe = qMax (next_frame_delay_num,next_frame_delay_den);
                    if (timeframe > 1) {
                    Ftoc.play = timeframe;
                    } else {
                    Ftoc.play = default_play_time_ms;
                    }
                    Ftoc.maxframe = Frect;
                    movie.insert(i,Ftoc);
                    f.close();
                }  
        }
        png_ptr_write = 0;
        info_ptr_write = 0;
        ////qDebug() << "### next_frame_delay_num  " << next_frame_delay_num;
        //////qDebug() << "### next_frame_delay_den  " << next_frame_delay_den;
       }
    }
    
  
  ValidApng = true;
}










/* definition from one qt frame to play */

 VIFrame::VIFrame()
 {
    play = 1000;
    dimg = QByteArray("error"); 
    mode = 5;
    pos = 0;
    bg = QColor(Qt::black);
    point = QPoint(0,0);  /* QPoint point */
}
VIFrame& VIFrame::operator=( const VIFrame& d )
{
      dimg = d.dimg;
      play = d.play;
      mode = d.mode;
      pos = d.pos;
      maxframe = d.maxframe;
      return *this;
}
    
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
    if (maxframe.width() == base.width() && maxframe.height() == base.width()) {
    /* equal */
    return base;
    }
    /////maxframe
    QPixmap Pvidi = QPixmap(maxframe.width(),maxframe.height());
    Pvidi.fill(bg);
    QPainter p( &Pvidi );
    p.drawPixmap(point,base);
    return Pvidi;
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




void readSetup( const QString file , png_structp* png_ptr_read, png_infop* info_ptr_read)
{
    FILE* image;
    int rc;
    png_byte pngSig[8];
    
    image = fopen(qPrintable(file), "rb");
    if(image == NULL)
        qWarning("couldn't open original png");
    
    rc = fread(pngSig, 1, 8, image);
    if(rc != 8)
        qWarning("unable to read signature");
    
    ////rc = png_check_sig(pngSig, 8);
    /////if(rc == 0)
        /////qWarning("invalid png signature");
    
    *png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(*png_ptr_read == NULL)
        qWarning("unable to create read struct");
    
    *info_ptr_read = png_create_info_struct(*png_ptr_read);
    if(*info_ptr_read == NULL) {
        qWarning("unable to create info struct");
        return;
    }
    
    png_init_io(*png_ptr_read, image);
    png_set_sig_bytes(*png_ptr_read, 8);
}

AFRAMES writeSetup2(png_structp png_ptr_read, png_infop info_ptr_read,
                 png_structp png_ptr_write, png_infop info_ptr_write,
                 png_uint_32 width, png_uint_32 height)
{
    /* IHDR */
    /* struct data need from background */
    AFRAMES pframe;
    png_uint_32 garbage;
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
    
    png_get_IHDR(png_ptr_read, info_ptr_read, &garbage, &garbage,
                 &bit_depth, &colour_type, &interlace_method,
                 &compression_method, &filter_method);
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height,
                 bit_depth, colour_type, interlace_method,
                 compression_method, filter_method);
                 
                 
    
    
    int r = 0, g = 0, b = 0;
    int bpp = bit_depth;
    
    switch (colour_type) {
                case PNG_COLOR_TYPE_GRAY      :           break;
                case PNG_COLOR_TYPE_RGB       : bpp *= 3; break;
                case PNG_COLOR_TYPE_PALETTE   :           break;
                case PNG_COLOR_TYPE_GRAY_ALPHA: bpp *= 2; break;
                case PNG_COLOR_TYPE_RGB_ALPHA : bpp *= 4; break;
                default: 
                return pframe;
    }

    
                 
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_PLTE))
    {
        /////////qDebug() << "### PNG_INFO_PLTE";
        
        png_get_PLTE(png_ptr_read, info_ptr_read, &palette, &palette_size);
        png_set_PLTE(png_ptr_write, info_ptr_write, palette, palette_size);
    }
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_gAMA))
    {
        /////////qDebug() << "### PNG_INFO_gAMA";
        
        png_get_gAMA(png_ptr_read, info_ptr_read, &gamma);
        png_set_gAMA(png_ptr_write, info_ptr_write, gamma);
    }
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_tRNS))
    {
        /////////qDebug() << "### PNG_INFO_tRNS";
        
        png_get_tRNS(png_ptr_read, info_ptr_read, &trans, &num_trans, &trans_values);
        png_set_tRNS(png_ptr_write, info_ptr_write, trans, num_trans, trans_values);
    }
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_bKGD))
    {
        ///////////qDebug() << "### PNG_INFO_bKGD";
        
        png_get_bKGD(png_ptr_read, info_ptr_read, &background);
        png_set_bKGD(png_ptr_write, info_ptr_write, background);
        
          png_color_16p bkgd = background;

        
        int r = 0, g = 0, b = 0;
        
                if(colour_type == PNG_COLOR_TYPE_RGB || colour_type == PNG_COLOR_TYPE_RGB_ALPHA) {
                r = bkgd->red;
                g = bkgd->green;
                b = bkgd->blue;
                }
                if(colour_type == PNG_COLOR_TYPE_GRAY || colour_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
                r = g = b = bkgd->gray;
                }
                if(colour_type == PNG_COLOR_TYPE_PALETTE) {
                r = palette[bkgd->index].red;
                g = palette[bkgd->index].green;
                b = palette[bkgd->index].blue;
                }
                    // scale down to 8 bit color since QColor doesn't support 16 bit colors
                if(bit_depth > 8) {
                r >>= 8;
                g >>= 8;
                b >>= 8;
                }
                
                //////qDebug() << "### color  " << r << g << b;
                
                pframe.bg = QColor(r,g,b);
        
    } else {
      pframe.bg = QColor(Qt::black);  
    }
    
    pframe.compression = compression_method;
    pframe.colortype = colour_type;
    return pframe;
}



void writeSetup(FILE* image, png_structp* png_ptr_write, png_infop* info_ptr_write)
{
    *png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(*png_ptr_write == NULL)  {
        qWarning("unable to create write struct");
        return;
    }
    
    *info_ptr_write = png_create_info_struct(*png_ptr_write);
    if(*info_ptr_write == NULL)  {
        qWarning("unable to create write struct");
        return;
    }
    
    png_init_io(*png_ptr_write, image);
}




PMovie::PMovie( QWidget* parent )
	: QLabel( parent ),current(0),running(false)
{
	  setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    resize(400, 400);
}

void PMovie::Stop()
{
  if (running) {
      running = false;
  }
}

void PMovie::restart()
{
    Stop();
    Play();
}

void PMovie::Play()
{
    if (running) {
      Stop();  
    }
    ///////current = 0;
    if (playmovie.size() < 1) {
       OpenFile(); 
    } else {
       running = true;
       NextFrame(); 
    } 
}

void PMovie::OpenFile()
{
    QString file = QFileDialog::getOpenFileName(0, tr( "Choose File to play" ),"",tr("APNG Animated Image  (*.png *.apng)"));
    if (file.size() > 0) {
    LaunchFile(file);
    }
}

void PMovie::LaunchFile( const QString file )
{
    if (running) {
      Stop();  
    }
    playmovie.clear();
    if (movie) {
    movie = NULL;
    } 
    current = 0;
    movie = new FrameIterator(file);
    playmovie = movie->frames();
    ///////qDebug() << "### movie->framecount()  ->" << movie->framecount();
    
    VIFrame record0 = playmovie[current];
    QPixmap ptt = record0.pix();
    setMinimumSize(record0.maxframe.size());
    setScaledContents(false);
    ///QRect maxframe;
    /////qDebug() << "### ptt.isNull()  ->" << ptt.isNull();
    /////qDebug() << "### time  ->" << record0.play;
    //////return;
    if (playmovie.size() > 0) {
    running = true;
    QTimer::singleShot(100, this, SLOT(NextFrame())); 
    } else {
    Stop();
    } 
}

void PMovie::OpenDemo()
{
    QFile f(QString::fromUtf8(":/img/Kubus.png")); 
    QByteArray demostrem;
    if (f.open(QIODevice::ReadOnly)) {
     demostrem = f.readAll();
     f.close();
    }
    QTemporaryFile tmpfile;
    if (tmpfile.open()) {
        if ( FillFile(tmpfile.fileName(),demostrem) ) {
            LaunchFile(tmpfile.fileName());
        }
    }  
}


void PMovie::NextFrame()
{
    if (!running) {
    return;
    }
    if ( (current + 1) > movie->framecount() ) {
        current = 0;
    }
    ///////////qDebug() << "### play  ->" << current;
    /////if (playmovie[current]) {
    VIFrame record = playmovie[current];
     ///////////////qDebug() << "### rect  ->" << record.maxframe;
    running = true;
    setPixmap ( record.videopix() );
    setScaledContents(false);    
    current++;
    QTimer::singleShot(record.play, this, SLOT(NextFrame())); 
    /////}
}



void PMovie::ExportFrames()
{
    Stop();
    
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select dir to save frames.."),"",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks); 
    if (dir.size() > 0) {
     QMapIterator<int,VIFrame> i(playmovie);
         while (i.hasNext()) {
             i.next();
             const int NrF = i.key();
             VIFrame record = i.value();
             QString cfile = QString("%1/%2_export.png").arg(dir).arg(NrF);     
             FillFile(cfile,  record.stream() );
         }

     }
}

void PMovie::ComposeFrame()
{
  OpenDesk( QUrl("https://addons.mozilla.org/en-US/firefox/addon/5519"));
}

QMenu *PMovie::MovieMenu()
{
    QFileIconProvider  sysicon;
    
    QMenu *Me = new QMenu(tr("Table"),this);
    QAction *a;
    a = Me->addAction(tr("Open APNG Movie"), this, SLOT(OpenFile()));
    a->setIcon(QIcon(QString::fromUtf8(":/img/open.png")));
    
    a = Me->addAction(tr("Stop "), this, SLOT(Stop()));
    a->setIcon(QIcon(QString::fromUtf8(":/img/stop.png")));
    
    a = Me->addAction(tr("Restart "), this, SLOT(restart()));
    a->setIcon(QIcon(QString::fromUtf8(":/img/check.png")));
    
    a = Me->addAction(tr("Play "), this, SLOT(Play()));
    a->setIcon(QIcon(QString::fromUtf8(":/img/play.png")));
    
    a = Me->addAction(tr("Play Demo APNG "), this, SLOT(OpenDemo()));
    a->setIcon(QIcon(QString::fromUtf8(":/img/Kubus.png")));
    
    a = Me->addAction(tr("Export any video frame to single png image"), this, SLOT(ExportFrames()));
    a->setIcon(sysicon.icon(QFileIconProvider::Folder));
    
    a = Me->addAction(tr("Compose image APNG..."), this, SLOT(ComposeFrame()));
    
    
    
    return Me;
}


void PMovie::contextMenuEvent( QContextMenuEvent * e )
{
    QMenu *Fmenu = MovieMenu();
    Fmenu->exec(QCursor::pos());
    delete Fmenu;
}


















