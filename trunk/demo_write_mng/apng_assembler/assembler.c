/* assmbler.c */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "libpng-1.2.23/png.h"

void readSetup(FILE** image, char* filename, png_structp* png_ptr_read, png_infop* info_ptr_read);
void writeSetup(const char* filename, FILE** image, png_structp* png_ptr_write, 
                png_infop* info_ptr_write);
void writeSetup2(png_structp png_ptr_write, png_infop info_ptr_write,
                 png_structp png_ptr_read, png_infop info_ptr_read);

void fatalError(char* str, int code)
{
    fprintf(stderr, "Fatal Error: %s\n", str);
    exit(code);
}

int main(int argc, char** argv)
{
    FILE* originalImage;
    FILE* newImage;
    int count;
    int count2;
    bool globalsSet;
    int delay;
    int skipFirstFrame;
    
    png_structp png_ptr_read;
    png_infop info_ptr_read;
    png_structp png_ptr_write;
    png_infop info_ptr_write;
    
    png_bytepp rowPointers;
    png_uint_32 bytesPerRow;
    
    if(argc == 1)
        fatalError("usage: assembler outfile.png time 1|0 frame1.png ...\n", 1);
    
    if( sscanf(argv[2], "%d", &delay) != 1 )
        fatalError("argument for 'time' not a number", 12);
    
    if( (sscanf(argv[3], "%d", &skipFirstFrame) != 1) || 
        (skipFirstFrame != 1 && skipFirstFrame != 0) )
        fatalError("argument for 'hide first frame' not 1 or 0", 13);
    
    writeSetup(argv[1], &newImage, &png_ptr_write, &info_ptr_write);
    if(setjmp(png_ptr_write->jmpbuf))
        fatalError("something didn't work while writing", 2);
    
    globalsSet = false;
    for(count = 4; count < argc; count++)
    {
        printf("writing frame from %s (%d of %d)\n", argv[count], count - 3, argc - 4);
        
        readSetup(&originalImage, argv[count], &png_ptr_read, &info_ptr_read);
        
        if(setjmp(png_ptr_read->jmpbuf))
            fatalError("something didn't work while reading", 3);
        
        png_read_info(png_ptr_read, info_ptr_read);
        
        if(!globalsSet)
        {
            writeSetup2(png_ptr_write, info_ptr_write, png_ptr_read, info_ptr_read);
            png_set_acTL(png_ptr_write, info_ptr_write, argc - 4, 0);
            if(skipFirstFrame)
                png_set_first_frame_is_hidden(png_ptr_write, info_ptr_write, 1);
            png_write_info(png_ptr_write, info_ptr_write);
            
            rowPointers = png_malloc(png_ptr_read, sizeof(png_bytep) * info_ptr_read->height);
            bytesPerRow = png_get_rowbytes(png_ptr_read, info_ptr_read);
            for(count2 = 0; count2 < info_ptr_read->height; count2++)
                rowPointers[count2] = png_malloc(png_ptr_read, bytesPerRow);
            
            globalsSet = true;
        }
        
        png_read_image(png_ptr_read, rowPointers);
        
        png_write_frame_head(png_ptr_write, info_ptr_write, rowPointers, 
                             info_ptr_read->width,  /* width */
                             info_ptr_read->height, /* height */
                             0,       /* x offset */
                             0,       /* y offset */
                             delay, 1000,    /* delay numerator and denominator */
                             PNG_DISPOSE_OP_NONE, /* dispose */
                             PNG_BLEND_OP_SOURCE    /* blend */
                            );
        png_write_image(png_ptr_write, rowPointers);
        png_write_frame_tail(png_ptr_write, info_ptr_write);
        
        fclose(originalImage);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
    }
    
    png_write_end(png_ptr_write, NULL);
    
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    fclose(newImage);
    
    printf("all done\n");
    
    return 0;
}

void readSetup(FILE** image, char* filename, png_structp* png_ptr_read, png_infop* info_ptr_read)
{
    int rc;
    png_byte pngSig[8];
    
    *image = fopen(filename, "rb");
    if(*image == NULL)
        fatalError("couldn't open original png", 4);
    
    rc = fread(pngSig, 1, 8, *image);
    if(rc != 8)
        fatalError("unable to read signature", 5);
    
    rc = png_check_sig(pngSig, 8);
    if(rc == 0)
        fatalError("invalid png signature", 6);
    
    *png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(*png_ptr_read == NULL)
        fatalError("unable to create read struct", 7);
    
    *info_ptr_read = png_create_info_struct(*png_ptr_read);
    if(*info_ptr_read == NULL)
        fatalError("unable to create info struct", 8);
    
    png_init_io(*png_ptr_read, *image);
    png_set_sig_bytes(*png_ptr_read, 8);
}

void writeSetup(const char* filename, FILE** image, png_structp* png_ptr_write, 
                png_infop* info_ptr_write)
{
    *image = fopen(filename, "wb");
    if(image == NULL)
        fatalError("couldn't open file for writing", 9);
    
    *png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(*png_ptr_write == NULL)
        fatalError("unable to create write struct", 10);
    
    *info_ptr_write = png_create_info_struct(*png_ptr_write);
    if(*info_ptr_write == NULL)
        fatalError("unable to create write struct", 11);
    
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
