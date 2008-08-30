#include "apngreader.h"



void Openchunk::run()
{
		png_structp png_ptr_read;
    png_infop info_ptr_read;
    png_structp png_ptr_write;
    png_infop info_ptr_write;
	
    png_uint_32 next_frame_width;
    png_uint_32 next_frame_height;
    png_uint_32 next_frame_x_offset;
    png_uint_32 next_frame_y_offset;
    png_uint_16 next_frame_delay_num;
    png_uint_16 next_frame_delay_den;
    png_byte next_frame_dispose_op;
    png_byte next_frame_blend_op;
    png_color_16p Framebg;
	  framlist.clear();
	
  buffer = new StreamFile();
  buffer->LoadFile(subfile);
	qDebug() << "### start read ->" << subfile;
	if (!buffer->isValid()) {
		alert("unvalid buffer fill",__LINE__);
		exit(0);
	}
	validpng = buffer->device()->peek(4).contains("PNG");
	qDebug() << "### ahed  ->" << validpng;
	frame1.loadFromData(buffer->stream()); /* load first frame */
	if (frame1.isNull()) {
		alert("unable find a valid image",__LINE__);
		return;
	}
	png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr_read == NULL) {
		alert("unable to create read struct",__LINE__);
		return;
	}
	info_ptr_read = png_create_info_struct(png_ptr_read);
	if(info_ptr_read == NULL) {
     alert("unable to create info struct",__LINE__);
		 return;
	}

	png_set_read_fn(png_ptr_read,buffer,EncoderReaderCallback);
	
	if(setjmp(png_ptr_read->jmpbuf)) {
        alert("something didn't work, jump 1",__LINE__);
		    return;
	}
	
	png_read_info(png_ptr_read, info_ptr_read);
	
	if(!png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_acTL)) {
			alert("source image must be animated",__LINE__);
		  return;
	}
	
	  QRect Frect(0,0,info_ptr_read->width,info_ptr_read->height);
    ////////////qDebug() << "### Frame rect from head ->" << Frect;
    QImage master(Frect.width(),Frect.height(),QImage::Format_ARGB32);
    const uint height = master.height();
    png_bytep *row_pointers = new png_bytep[height];
                        for (uint i = 0; i < height; ++i)  {
                          row_pointers[i] = (png_bytep)master.scanLine(i);
                        }
    int validloop = -1;
    for(int i = 0; i < png_get_num_frames(png_ptr_read, info_ptr_read); i++)
    {
        /////////qDebug() << "### frame read  ------------- " << i;
				png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(png_ptr_write == NULL)  {
        alert("unable to create write struct");
        return;
        }
        StreamFile *framer = new StreamFile();
        png_set_write_fn(png_ptr_write,framer,EncoderWriteCallback, NULL);
        info_ptr_write = png_create_info_struct(png_ptr_write);
        if(info_ptr_write == NULL)  {
        alert("unable to create write struct");
        return;
        }
        if(setjmp(png_ptr_write->jmpbuf))  {
            alert("something didn't work, jump 2");
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
				
				
				AFRAMES FrameInfo = OneFrameReader(png_ptr_read, info_ptr_read, png_ptr_write, info_ptr_write,
        next_frame_width, next_frame_height);
				png_write_info(png_ptr_write, info_ptr_write);
				png_read_image(png_ptr_read, row_pointers);
        png_write_image(png_ptr_write, row_pointers);
        png_write_end(png_ptr_write, NULL);
				
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
										
				/* extract frames */
				if ( framer->isValid() && Frect.contains(C_frame_rect) ) {
					 validloop++;
					 int Coalpha = 255;
					 /* prepare image if no background find grab a pixel color! */
					 QImage tmpgd(C_frame_rect.width(),C_frame_rect.height(),QImage::Format_ARGB32);
					 /* compose it files */
					 APNGFRAME one;
					 one.dimg = framer->stream();
					 one.maxframe = Frect;
					 one.pos = validloop;
					 one.point = QPoint(next_frame_x_offset,next_frame_y_offset);
					 one.play = PlayGo;
					 if (!(png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_bKGD))) {
					 tmpgd.loadFromData(one.dimg);
					 QRgb GrepColor = tmpgd.pixel(QPoint(2,2));
					 one.bg = QColor(GrepColor);
					 one.bg.setAlpha(qAlpha(GrepColor));
					 Coalpha = qAlpha(GrepColor);
					 } else {
					 one.bg = FrameInfo.bg; 
					 }
					 
					       QImage tmpe;
					       QImage Pvidi(one.maxframe.width(),one.maxframe.height(),QImage::Format_ARGB32);
					       if (tmpe.loadFromData(one.dimg)) {
								 QPainter p(&Pvidi);
								 p.setRenderHint(QPainter::Antialiasing, true);
								 p.setBrush(one.bg);
								 p.drawRect(Pvidi.rect());
								 p.drawImage(one.point,tmpe);
								 p.end();
								 one.item = Pvidi; 
                 ////qDebug() << "### isNull() in theard .." << one.item.isNull();									 
								 }
					 
					 
					 //////item tmpe
					 //////qDebug() << "### Frame rect ->" << one.point << ",nr." << validloop << "C." << one.bg.name() << "Al." << Coalpha;
					 framlist.insert(validloop,one);
				}
				framer->~StreamFile();
				png_ptr_write = 0;
        info_ptr_write = 0;
		}
	
	
	qDebug() << "### Frame size() ->" << framlist.size();
	//////alert("end of chunks",__LINE__);
	validpng = true;
	buffer->~StreamFile();
	exit(0);
}





FrameReader::~FrameReader()
{
  movie.clear();
	buffer->~StreamFile();
}

FrameReader::FrameReader( QObject *reciver , const QString apngfile , uint defaulttime )
  :qfile(apngfile),default_play_time_ms(defaulttime),ValidApng(false),buffer(new StreamFile())
{ 
    /////qDebug() << "### libpng version  ->" << PNG_LIBPNG_VER;
    movie.clear();
	  parent = reciver;
	  running = false;
	  current = -1;
	  if (apngfile.size() > 0) {
	  OpenFile();
		}
	  
}
void FrameReader::append( QImage recimg , int playtime )
{
	int position = movie.size();
	int fplay = playtime;
	if (fplay < 30) {
		fplay = default_play_time_ms;
	}
	if (!recimg.isNull()) { 
		 QImage record;
		 if (position == 0) {
			   record = recimg;
		 } else {
			   APNGFRAME first = movie[0];
			   record =  recimg.scaled(first.maxframe.width(),
			             first.maxframe.height(),
			             Qt::IgnoreAspectRatio);
		 }
		 buffer->clear();
		 record.save(buffer->device(),"PNG");
		 APNGFRAME one;
			one.dimg = buffer->stream();
			one.maxframe = record.rect();
			one.pos = position;
			one.point = QPoint(0,0);
			one.item = record;
		  one.play = fplay;
		  one.bg = QColor(Qt::black);
		 buffer->clear();
		 ValidApng = true;
		 disconnect(this);
		 movie.insert(position,one);
		 ////////qDebug() << "### append " << position;
	}
	
}

void FrameReader::start()
{
	if (movie.size() < 2) {
    return;
	}
	  QApplication::beep();
	  current = 0;
		running = true;
		ValidApng = true;
	  NextFrame();
}
void FrameReader::OpenFile()
{
	  running = false;
	  running_loop = 0;
	  current = -1;
	  movie.clear();
	  moviereader = new Openchunk();
		moviereader->Setting(qfile);
		moviereader->start(QThread::LowPriority);  
		connect(moviereader,SIGNAL(finished()),this, SLOT(ReadStream()));
}

void FrameReader::ReadStream()
{
	Openchunk *invoice = qobject_cast<Openchunk *>(sender());
  if (!invoice) {
		return;
	}
	if (!invoice->validpng) {
		QMessageBox::warning(0, tr("Error on reader."),tr("Error say:%1").arg(invoice->error()));
	return;
	}
	////////qDebug() << "### valid image...say .." << invoice->error();
	//////qDebug() << "### valid image...say .." << invoice->validpng;
	QMap<int,APNGFRAME> framlist = invoice->list();
	QMap<int,APNGFRAME> readylist;
	readylist.clear();
	if (QThread::currentThread() == qApp->thread()) {
     qDebug() << "### can paint .." << framlist.size();
	   QMapIterator<int,APNGFRAME> i(framlist);
		 int rec = -1;
         while (i.hasNext()) {
                i.next();
					       APNGFRAME one = i.value();
					     if (one.item.isNull()) {
					       QImage tmpe;
					       QImage Pvidi(one.maxframe.width(),one.maxframe.height(),QImage::Format_ARGB32);
					       if (tmpe.loadFromData(one.dimg)) {
								 QPainter p(&Pvidi);
								 p.setRenderHint(QPainter::Antialiasing, true);
								 p.setBrush(one.bg);
								 p.drawRect(one.maxframe);
								 p.drawImage(one.point,tmpe);
								 p.end();
								 one.item = Pvidi;
								 ///////qDebug() << "### isNull() out theard .." << one.item.isNull();
								 readylist.insert(rec++,one);
								 }
							 }
				 }
  }
	if (readylist.size() > 0 ) {
		movie = readylist;
		framlist.clear();
	} else {
		movie = framlist;
	}
	if (movie.size() < 1) {
	buffer->LoadFile(qfile); /* stay on buffer to save  */
	} else {
		current = 0;
		running = true;
		ValidApng = true;
		disconnect(this);
		NextFrame();
	}
}

void FrameReader::NextFrame()
{
    if (!running || movie.size() < 2) {
    return;
    }
		QObject *invoice = qobject_cast<QObject *>(sender());
		if (!invoice) {
		disconnect(this);
		QTimer::singleShot(200, this, SLOT(NextFrame()));
		return;
		}
		const int grab = qBound (0,current,movie.size() - 1);
		APNGFRAME im = movie[grab];
		///////////cout <<  "ID " << grab << "from." << movie.size() << " time " << im.play << " l." << running_loop << "\r";
		fflush ( stdin );
		current++;
		current = qBound (1,current,movie.size());
		if (current == movie.size()) {
		current = 0;
		}
		if (parent) {
		qApp->postEvent(parent,new ImageEvent(im));
		disconnect(this);
		running_loop++;
		QTimer::singleShot(im.play, this, SLOT(NextFrame()));
		}
}





AFRAMES OneFrameReader(png_structp png_ptr_read, png_infop info_ptr_read,
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









