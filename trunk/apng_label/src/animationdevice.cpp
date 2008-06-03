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

/* read file qfile and extract any frame */
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
    qDebug() << "### Frame rect ->" << Frect;
    QImage master(Frect.width(),Frect.height(),QImage::Format_ARGB32);
    const uint height = master.height();
    png_bytep *row_pointers = new png_bytep[height];
    
                        for (uint i = 0; i < height; ++i)  {
                          row_pointers[i] = (png_bytep)master.scanLine(i);
                        }
    
  
    
        
    for(int i = 0; i < png_get_num_frames(png_ptr_read, info_ptr_read); i++)
    {
        qDebug() << "### frame read  ------------- " << i;
        // Set our callback for libpng to give us the data.
        png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(png_ptr_write == NULL)  {
        qWarning("unable to create write struct");
        return;
        }
        
        APNGwrittelStream myqtchunk(this);
        png_set_write_fn(png_ptr_write,&myqtchunk,EncoderWriteCallback, NULL);
        
    
        info_ptr_write = png_create_info_struct(png_ptr_write);
        if(info_ptr_write == NULL)  {
        qWarning("unable to create write struct");
        return;
        }
        
        
        //////writeSetup(NULL,&png_ptr_write, &info_ptr_write);
        /* ##################### alert speed file!##############################################*/
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
        
     
        
        AFRAMES FrameInfo = writeSetup2(png_ptr_read, info_ptr_read, png_ptr_write, info_ptr_write,
        next_frame_width, next_frame_height);
        png_write_info(png_ptr_write, info_ptr_write);
        
        if(png_get_valid(png_ptr_read,info_ptr_read, PNG_INFO_bKGD)) {
            
            /* grab fake background color if having */
         qDebug() << "### bg color found  ------------- " << i;
            
        } else {
            qDebug() << "### no bg color found  ------------- " << i;
        }
        
        
        
        
        
        png_read_image(png_ptr_read, row_pointers);
        png_write_image(png_ptr_write, row_pointers);
        png_write_end(png_ptr_write, NULL);
        
        if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_bKGD))
        {
        png_get_bKGD(png_ptr_read, info_ptr_read, &Framebg);
        }
        
        ////png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        /* ##################### alert speed file!##############################################*/
        /////fclose(newImage);
        
        qDebug() << "stream innnnnn ...." << myqtchunk.isValid() << ".... " << myqtchunk.stream().size();
        
        if ( myqtchunk.isValid() ) {
                    VIFrame Ftoc;
                    Ftoc.pos = i;
                   
                    
                    Ftoc.point = QPoint(next_frame_x_offset,next_frame_y_offset);
                    int Blend = next_frame_dispose_op;
                    int NextBlend = next_frame_blend_op;
                    QStringList Foption;
                    Foption << QString("%1").arg(Blend) 
                            << QString("%1").arg(NextBlend) 
                            << QString("%1").arg(next_frame_delay_num) 
                            << QString("%1").arg(next_frame_delay_den) 
                            << "-end-";
                    //// next_frame_delay_num /  next_frame_delay_den                   
                    
                    Ftoc.option = Foption;   ////// next_frame_delay_num, &next_frame_delay_den
                    
                    /*
                    The `delay_num` and `delay_den` parameters together specify a fraction 
                    indicating the time to display the current frame, in seconds. 
                    If the denominator is 0, it is to be treated as if it were 100 
                    (that is, `delay_num` then specifies 1/100ths of a second). 
                    If the the value of the numerator is 0 the decoder should render
                    the next frame as quickly as possible, though viewers may impose a 
                    reasonable lower bound.

                    Frame timings should be independent of the time required for decoding 
                    and display of each frame, so that animations will run at the same rate 
                    regardless of the performance of the decoder implementation. 
                    */
                    float Fraction = (float)next_frame_delay_num /  (float)next_frame_delay_den + 0.00;
                    ////////qDebug() << "### Fraction  " << Fraction;
                    int PlayGo;
                    if (Fraction < 0.001 ) {
                        PlayGo = 100;
                    } else if (Fraction < 1.010 && Fraction > 0.9) {
                        PlayGo = 1000;
                    } else {
                        PlayGo = Fraction * 1000;
                    }
                    Ftoc.set_pics( myqtchunk.stream() );
                    
                    QImage tmpgd = Ftoc.ipix();
                    myqtchunk.clear();
                   
                    
                    /////QRgb* rgb = ((QRgb*)tmpgd.scanLine(1));
                    ///////(0.2125*qRed(*rgb) + 0.7154*qGreen(*rgb) + 0.0721*qBlue(*rgb));
                    //////QColor backimage(qRed(*rgb),qGreen(*rgb),qBlue(*rgb),qAlpha(*rgb));
                    /* greb background color of frame .... */
                    QRgb GrepColor = tmpgd.pixel(QPoint(2,2));
                    if (FrameInfo.foundcolor) {
                     Ftoc.bg = FrameInfo.bg;   
                    } else { 
                     Ftoc.bg = QColor(GrepColor);
                     Ftoc.bg.setAlpha(qAlpha(GrepColor));
                    }
                    
                    
                    /* minimum play time ms not accept < 10 */
                    const uint timeframe = qMax (9,PlayGo);
                    Ftoc.play = timeframe;
                    Ftoc.maxframe = Frect;
                    Ftoc.colortype = FrameInfo.colortype;
                    ///////////qDebug() << "### play time MS  " << Ftoc.play;
                    movie.insert(i,Ftoc);
        }
        png_ptr_write = 0;
        info_ptr_write = 0;
        
       
    }
    
  delete [] row_pointers;
  ValidApng = true;
}














void readSetup( const QString file , png_structp* png_ptr_read, png_infop* info_ptr_read)
{
    FILE* image;
    int rc;
    png_byte pngSig[8];
    
    image = fopen(QFile::encodeName(file), "rb");
    ////image = stdout;
    
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
    
    pframe.colortype = colour_type;
    pframe.bytedep = bpp;
    pframe.pngBG = trans_values;
    
    /////////qDebug() << "### color type  " << colour_type << bpp << num_trans;
    
                 
    
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
        //////////qDebug() << "### PNG_INFO_tRNS";
        
        png_get_tRNS(png_ptr_read, info_ptr_read, &trans, &num_trans, &trans_values);
        png_set_tRNS(png_ptr_write, info_ptr_write, trans, num_trans, trans_values);
    }
    
    if(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_bKGD))
    {
        ////////qDebug() << "### PNG_INFO_bKGD";
        
        png_get_bKGD(png_ptr_read, info_ptr_read, &background);
        png_set_bKGD(png_ptr_write, info_ptr_write, background);
        png_color_16p bkgd = background;
        pframe.pngBG = background;
        
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
                
                ////////qDebug() << "### color  " << r << g << b;
                
                pframe.bg = QColor(r,g,b);
                pframe.foundcolor = true; 
        
    } else {
        
        ////qDebug() << "### no BGGGG color";
      pframe.foundcolor = false; 
      pframe.bg = QColor(Qt::transparent);  
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
	: QLabel( parent ),current(0),
          running(false),
          capturescreen(false),
          foundCamera(false)
{
	  setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    resize(400, 400);
    
    #ifdef OPCAMENABLE
    capture = cvCaptureFromCAM( CV_CAP_ANY );
    if (capture) {
    foundCamera = true; 
    cvReleaseCapture(&capture);
    }
    #endif
}

void PMovie::StartCam()
{
    #ifdef OPCAMENABLE
    capture = cvCaptureFromCAM( CV_CAP_ANY );
    IplImage* imgPtr  = cvQueryFrame( capture );
    int format = imgPtr->nChannels * imgPtr->depth;
    if( format != 24 )  {
    qDebug() << "### format error!!  ->" << format;
    return;
    }
    QImage *imgs = IplImageToQImage(imgPtr,(uchar**)imgPtr->imageData);
    camimage = imgs->mirrored(false,true);
    setPixmap ( QPixmap::fromImage(camimage,Qt::AutoColor) );
    setScaledContents(false);
    capturescreen = true;
    CatScreen();
    #endif
}



void PMovie::CatScreen()
{
    QPixmap smallooooooo;
    if (running || !capturescreen ) {
    return;
    }
    if (camimage.isNull()) {
    QDesktopWidget *desk = qApp->desktop();
    QPixmap small0 = QPixmap::grabWindow(desk->screen()->winId());
    smallooooooo = small0.scaledToWidth(1000);
	  ///////QImage small =  smallooooooo.toImage();
    //////GreyScale( small0.scaledToWidth(1000).toImage() );  QPixmap::fromImage ()
    } else if (!camimage.isNull()) {
    #ifdef OPCAMENABLE
    if (capture) {
    IplImage* imgPtr  = cvQueryFrame( capture );
    QImage *imgs = IplImageToQImage(imgPtr,(uchar**)imgPtr->imageData);
    camimage = imgs->mirrored(false,true);
    }
    #endif
    smallooooooo = QPixmap::fromImage(camimage,Qt::AutoColor);
    } else {
      return;
    }
    setPixmap ( smallooooooo );
    setScaledContents(false);
    VIFrame Ftoc;
    Ftoc.pos = playmovie.size();            
    Ftoc.point = QPoint(0,0);
    Ftoc.set_pics( smallooooooo );
    Ftoc.play = 500;
    Ftoc.bg = QColor(Qt::black);
    Ftoc.maxframe = QRect(0,0,smallooooooo.width(),smallooooooo.height());
    if (!camimage.isNull()) {
    setMinimumSize(smallooooooo.width(),smallooooooo.height());
    } else {
    setMinimumSize(50,50);   
    }
    if (capturescreen) {
    setWindowTitle(QString("Record screen modus frame nr.%1").arg(Ftoc.pos + 1));
    playmovie.insert(Ftoc.pos,Ftoc);
    QTimer::singleShot(Ftoc.play, this, SLOT(CatScreen())); 
    } else {
    setWindowTitle(QString("APNG Label (use ContextMenu to play file)")); 
    }
	 ////////delete &small; 
}



void PMovie::Stop()
{
  if (running) {
      running = false;
  }
  if (capturescreen) {
     capturescreen = false;
    #ifdef OPCAMENABLE
    if (capture) {
    cvReleaseCapture(&capture);
    camimage = QImage();
    }
    #endif 
    SaveAsExport();
  }
  setWindowTitle(QString("APNG Label (use ContextMenu to play file)"));
}

void PMovie::restart()
{
    Stop();
    Play();
}

void PMovie::Play()
{
    capturescreen = false;
    
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
    capturescreen = false;
    if (!running) {
    return;
    }
    if ( (current + 1) > movie->framecount() ) {
        current = 0;
    }
    VIFrame record = playmovie[current];
    if (record.mode == 404) {
        /* no background found on frame!!!! pos!! */
        setBackgroundRole(QPalette::Text);
    } else {
       setBackgroundRole(QPalette::Dark); 
    }
    
    setMinimumSize(record.maxframe.size());

    
    setWindowTitle(QString("Play frame nr.%1 / modus %2").arg(record.pos + 1).arg(record.mode));
    running = true;
    setPixmap ( record.videopix() );
    setScaledContents(false);    
    current++;
    QTimer::singleShot(record.play, this, SLOT(NextFrame())); 
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

void PMovie::SaveAsExport()
{
    Stop();
    running = false;
    
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "",
                                tr("Movie Images (*.png)"));
    
    if (fileName.size() > 0) {
        
        dlg = new QProgressDialog(this,Qt::Popup);
        dlg->setLabelText (tr("Read && Write Frames..."));
        dlg->setCancelButton(0);
        dlg->setMinimumDuration(1000);
        
        
        FrameTrade *grephttp = new FrameTrade();
				grephttp->Setting(this,playmovie,fileName);
				grephttp->start(QThread::LowPriority);  
        connect(grephttp,SIGNAL(finished()),dlg, SLOT(deleteLater()));
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
    
    if (playmovie.size() > 1) {
    a = Me->addAction(tr("APNG Save As export animation.."), this, SLOT(SaveAsExport()));
    }
    
    a = Me->addAction(tr("Capture screen"), this, SLOT(startCapure()));
    
    if (foundCamera) {
    a = Me->addAction(tr("Capture from Cam"), this, SLOT(StartCam()));
    }
    
    return Me;
}


void PMovie::contextMenuEvent( QContextMenuEvent * e )
{
    QMenu *Fmenu = MovieMenu();
    Fmenu->exec(QCursor::pos());
    delete Fmenu;
}

void PMovie::status(uint state , uint tot )
{
    qreal cento = 100.0;
    qreal percentuale = state*cento/tot;
    //////qDebug() << "### percentuale " << percentuale;
    dlg->setRange(0,tot);
    dlg->setValue(state);
    qApp->processEvents();
}



void PMovie::startCapure()
{
     playmovie.clear();
     capturescreen = true;
     CatScreen();
}

bool PMovie::event ( QEvent * e )
{
    qApp->processEvents();
    
   if ( e->type() == QEvent::User ) {
				    PngEventBuffer *ep = static_cast<PngEventBuffer *>(e);
            ///////////qDebug() << "### event " << ep->pos() << "," << ep->sum();
            status(ep->pos(),ep->sum());
          
						return true;
		}
  QLabel::event(e);   /* cath all not needed */    
}
















