#ifndef ZIPHANDLE_H
#define ZIPHANDLE_H



/* zip libs  ###### start ###################################################################*/
#include "zlib.h"
#include "bestqtidy.h"   /* open xml and check on tidy */
#include <stdio.h>
#include "quazip.h"
#include "quazipfile.h"
#include <QTextCodec>
#include <QtCore> 
#include <QMessageBox>
#ifdef STDC
#  include <string.h>
#  include <stdlib.h>
#endif

#ifdef USE_MMAP
#  include <sys/types.h>
#  include <sys/mman.h>
#  include <sys/stat.h>
#endif

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#ifdef VMS
#  define unlink delete
#  define GZ_SUFFIX "-gz"
#endif
#ifdef RISCOS
#  define unlink remove
#  define GZ_SUFFIX "-gz"
#  define fileno(file) file->__file
#endif
#if defined(__MWERKS__) && __dest_os != __be_os && __dest_os != __win32_os
#  include <unix.h> /* for fileno */
#endif


#ifndef GZ_SUFFIX
#  define GZ_SUFFIX ".gz"
#endif
#define SUFFIX_LEN (sizeof(GZ_SUFFIX)-1)

#define BUFLEN      16384
#define MAX_NAME_LEN 1024

#ifdef MAXSEG_64K
#  define local static
   /* Needed for systems with limitation on stack size. */
#else
#  define local
#endif

#include <iostream>
#include <stdio.h>


using namespace std;

/* simple mkdir */
static inline bool Zmkdir( const QString dirpath )
{
    QDir dira(dirpath);
    if ( dira.mkpath(dirpath) ) {
    return true;
    } else {
    return false;
    }
}



extern inline void gz_compress( FILE   *in , gzFile out ) 
{
    local char buf[BUFLEN];
    int len;
    int err;
    
    for (;;) {
        len = (int)fread(buf, 1, sizeof(buf), in);
        if (ferror(in)) {
            perror("fread");
            exit(1);
        }
        if (len == 0) break;

        if (gzwrite(out, buf, (unsigned)len) != len) {
         return;   
        }
    }
    fclose(in);
    if (gzclose(out) != Z_OK) {
     return;
    }
}


static inline bool  OpenZip( const QString zipfile, const QString outputdir )
{
    ///////////// const QString cartella = QDir::currentPath();
     /////////qDebug() << "### from unzzip function path " <<  QDir::currentPath();
   /////////// qDebug() << "### outputdir " <<  outputdir;
    //////////qDebug() << "### zipfile " <<  zipfile;
 
    QDir sdir(outputdir);
    Zmkdir(sdir.absolutePath()+"/");
 
    char* suk;
    QuaZip zip(zipfile);
    bool extractsuccess = false;
    zip.open(QuaZip::mdUnzip);
    QuaZipFile file(&zip);
       for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile()) {
           file.open(QIODevice::ReadOnly);
           QString name = zip.getCurrentFileName();
                   name.prepend(sdir.absolutePath()+"/");   /* extract to path ....... */
           QFile meminfo(name);
           QFileInfo infofile(meminfo);
           QDir dira(infofile.absolutePath());
           if ( dira.mkpath(infofile.absolutePath()) ) { 
           /* dir is exist*/
           /////////qDebug() << "### name  " << name;     
           /////qDebug() << "### namedir yes  " << infofile.absolutePath();
               
               
               
               QString fileqinfo = zip.getCurrentFileName();
                QByteArray fe = fileqinfo.toAscii();
                suk = fe.data();
                
                //////////////////cout <<  "Processing..... " << suk << "\n";
               
               
               
               if ( meminfo.open(QIODevice::ReadWrite) ) {
               meminfo.write(file.readAll());   /* write */
               meminfo.close();
               extractsuccess = true;
               //////////RegisterImage(name);
               }
           } else {
             file.close();
             return false;               
           }
           file.close(); // do not forget to close!
        }
    zip.close();
    
return extractsuccess;
}




extern inline bool  MakeGzipOneFile( QString filelocation )
{
    local char outfile[MAX_NAME_LEN];
    FILE  *in;
    gzFile out;
    QByteArray cgofile = filelocation.toAscii();
    char* file = cgofile.data();
    QString const busgo = filelocation.append(GZ_SUFFIX);
    strcpy(outfile, file);
    strcat(outfile, GZ_SUFFIX);   /* attach suffix */
    in = fopen(file, "rb");
    
    if (in == NULL) {
     return false;
    }
    out = gzopen(outfile,"wb6");
    if (out == NULL) {
      fclose( in );
      return false;
    }
    gz_compress(in, out);
    
    QFile f( busgo );
    return f.exists(busgo);
}



extern inline QString  OpenGzipOneFile( QString fileName )
{
    QString inside = "";
     gzFile file;
      file = gzopen (fileName.toUtf8().data(), "rb");
      if(!file) {
        QMessageBox::critical(0, "Error",QString("Can't open file %1").arg(fileName));
        return inside;
      }
      QByteArray input;
      char buffer[1024];
      QByteArray inputData;
      while(int readBytes =  gzread (file, buffer, 1024))
       {
        input.append(QByteArray(buffer, readBytes));
      }
      gzclose(file);
      
      inside = QString::fromUtf8(input.data());
      return inside;
}



extern inline QByteArray  OpenGzipFileByte( QString fileName )
{
     gzFile file;
      file = gzopen (fileName.toUtf8().data(), "rb");
      if(!file) {
        QMessageBox::critical(0, "Error",QString("Can't open file %1").arg(fileName));
        return QByteArray();
      }
      QByteArray input;
      char buffer[1024];
      QByteArray inputData;
      while(int readBytes =  gzread (file, buffer, 1024))
       {
        input.append(QByteArray(buffer, readBytes));
      }
      gzclose(file);
      return input;
}





















#endif // ZIPHANDLE_H
