#include <QtCore>
#include <QDebug>
#include <QImage>

#define PNG_USER_MEM_SUPPORTED

typedef unsigned char png_byte;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
////base ref http://www.littlesvr.ca/apng/tutorial/x148.html
/* png lib from firefox 3 is patch to APNG format! */
#include "moz_png/png.h"
void fatalError(char* str)
{
    fprintf(stderr, "Fatal Error: %s\n", str);
    exit(1);
}

void readSetup(FILE** image, char* filename, png_structp* png_ptr_read, png_infop* info_ptr_read);
void writeSetup(FILE** image, png_structp* png_ptr_write, png_infop* info_ptr_write);
void writeSetup2(png_structp png_ptr_write, png_infop info_ptr_write,
                 png_structp png_ptr_read, png_infop info_ptr_read);



int main(int argc, char** argv)
{
    QCoreApplication a( argc, argv );
    
    QStringList imglist = a.arguments();
    
    FILE* originalImage;
    FILE* newImage;
    int count;
    int count2;
    bool globalsSet;
    
    
    png_structp png_ptr_read;
    png_infop info_ptr_read;
    png_structp png_ptr_write;
    png_infop info_ptr_write;
    png_bytep* row_pointers;
    png_uint_32 bytesPerRow;
    
    
    
    if(argc == 1)  {
        fatalError("usage: compose a.png b.png c.png ...\n");
    }
    
    writeSetup(&newImage, &png_ptr_write, &info_ptr_write);
    if(setjmp(png_ptr_write->jmpbuf))
        fatalError("something didn't work while writing");
    
    globalsSet = false;
    for(count = 1; count < argc; count++)
    {
        const QString imgfile = imglist.at(count);
        QImage image(imgfile);
        
        printf("writing frame from %s (%d of %d)\n", argv[count], count, argc - 1);
        
        readSetup(&originalImage, argv[count], &png_ptr_read, &info_ptr_read);
        if(setjmp(png_ptr_read->jmpbuf))
            fatalError("something didn't work while reading");
        
        png_read_info(png_ptr_read, info_ptr_read);
        
        if(!globalsSet)
        {
            qDebug() << "### first frame to normal PNG read format ------------- ";
            writeSetup2(png_ptr_write, info_ptr_write, png_ptr_read, info_ptr_read);
            png_set_acTL(png_ptr_write, info_ptr_write, argc - 1, 0);
            ///////////////png_set_first_frame_is_hidden(png_ptr_write, info_ptr_write, 1);
            png_write_info(png_ptr_write, info_ptr_write);
            
             
              uint height = image.height();
              const uchar* const* jt = image.jumpTable();
              row_pointers=new png_bytep[height];
              uint y;
              for (y=0; y<height; y++) {
                        // PNG lib has const issue with the write image function
                        row_pointers[y]=const_cast<png_byte*>(jt[y]);
              }
            
            
            globalsSet = true;
        }
        qDebug() << "### next frame  APNG format ------------- ";
        png_read_image(png_ptr_read,row_pointers);
        
        //~ if(count == 2)
        //~ {
            //~ png_write_frame_head(png_ptr_write, info_ptr_write, rowPointers, 
                                 //~ info_ptr_read->width,  /* width */
                                 //~ info_ptr_read->height, /* height */
                                 //~ 0,       /* x offset */
                                 //~ 0,       /* y offset */
                                 //~ 1, 1,    /* delay numerator and denominator */
                                 //~ PNG_DISPOSE_OP_PREVIOUS, /* dispose */
                                 //~ PNG_BLEND_OP_OVER        /* blend */
                                 //~ );
        //~ }
        //~ else
        {
            png_write_frame_head(png_ptr_write, info_ptr_write, row_pointers, 
                                 info_ptr_read->width,  /* width */
                                 info_ptr_read->height, /* height */
                                 0,       /* x offset */
                                 0,       /* y offset */
                                 1, 1,    /* delay numerator and denominator */
                                 PNG_DISPOSE_OP_NONE, /* dispose */
                                 PNG_BLEND_OP_SOURCE    /* blend */
                                );
        }
        png_write_image(png_ptr_write, row_pointers);
        png_write_frame_tail(png_ptr_write, info_ptr_write);
        
        fclose(originalImage);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
    }
    
    png_write_end(png_ptr_write, NULL);
    
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    fclose(newImage);
    
    printf("all done\n");
    
    return a.exec();
    
    
    
}


void readSetup(FILE** image, char* filename, png_structp* png_ptr_read, png_infop* info_ptr_read)
{
    int rc;
    png_byte pngSig[8];
    
    *image = fopen(filename, "rb");
    if(image == NULL)
        fatalError("couldn't open original png");
    
    rc = fread(pngSig, 1, 8, *image);
    if(rc != 8)
        fatalError("unable to read signature");
    
    *png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(*png_ptr_read == NULL)
        fatalError("unable to create read struct");
    
    *info_ptr_read = png_create_info_struct(*png_ptr_read);
    if(*info_ptr_read == NULL)
        fatalError("unable to create info struct");
    
    png_init_io(*png_ptr_read, *image);
    png_set_sig_bytes(*png_ptr_read, 8);
}

void writeSetup(FILE** image, png_structp* png_ptr_write, png_infop* info_ptr_write)
{
    *image = fopen("written-apng.png", "wb");
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










