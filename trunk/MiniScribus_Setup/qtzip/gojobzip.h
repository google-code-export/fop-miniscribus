#ifndef GOJOBZIP_H
#define GOJOBZIP_H

#include "quazip.h"
#include "quazipfile.h"
#include <QObject>
#include <QEvent>
#include <QCoreApplication>
#include <QApplication>
#include <QtGui>

/* simple mkdir */
static inline bool Mkdir( const QString dirpath )
{
    QDir dira(dirpath);
    if ( dira.mkpath(dirpath) ) {
    return true;
    } else {
    return false;
    }
}




class PackZip : public QObject
{
    Q_OBJECT
    
public:
PackZip( QObject * parent = 0  )
{
 startnow = QDir::currentPath();
}  
~PackZip()
{
    QDir::setCurrent(startnow);
}      
void Setup( const QString packfolder , const QString zipoutfolder )
{
  //////qDebug() << "### packfolder 1 " << packfolder;
  startdir = packfolder;   /* grab all this file */
  zipdir = zipoutfolder;   /* save zip to this dir */
  startnow = QDir::currentPath();
  Init();
}
QString result()
{
   return zipfile;
}
private:
void Init()
{
    canmake = false;
    QDir odir(zipdir);
    QDir sdir(startdir);
    if (!sdir.exists(startdir)) {
       ResetDir();
       return; 
    }
    QDir scopy(startdir);
    scopy.cdUp();
    zipfile.clear();
    startdir = sdir.absolutePath()+"/";
    //////qDebug() << "### packfolder 2 " << startdir;
    canmake = true;
    QDir dir(startdir);
    char c;
    qreal cento;
    qreal percentuale;
    QString dirname = dir.dirName();
    selfname = dirname + ".zip";
    Mkdir( odir.absolutePath() );
    
    QDir::setCurrent(startdir);
    
    zipfile = odir.absolutePath()+"/";
    zipfile.append(dirname);
    zipfile.append(".zip");
    
    QFile zcheck(zipfile);
     if (zcheck.exists()) {
      zcheck.remove();
     }
    
    dirandfile.clear();
    IndexDir(startdir);
    if (dirandfile.size() > 0) {
        
      QuaZip zip(zipfile);
      if(!zip.open(QuaZip::mdCreate)) {
      ResetDir();
      return;
      }
      
      QuaZipFile outFile(&zip);
      
      int totalf = dirandfile.size();
      
       //////qDebug() << "### totalfile " <<  dirandfile.size();
      
         for (int i = 0; i < dirandfile.size(); ++i) {
             QString filepak = dirandfile.at(i);
             const  QString abse = filepak;
             const  QString goname = filepak.replace(scopy.absolutePath()+"/","");
             
             QFile inFile(abse);
                   inFile.setFileName(abse);
             
             if(!inFile.open(QIODevice::ReadOnly)) {
              ResetDir();
              return;
             }
             
              if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(goname, inFile.fileName()))) {
              ResetDir();
              return;
              }
              
               while(inFile.getChar(&c)&&outFile.putChar(c));
                if(outFile.getZipError()!=UNZ_OK) {
                  ResetDir();
                  return;
                }
                
                outFile.close();
                if(outFile.getZipError()!=UNZ_OK) {
                ResetDir();
                return;
                }
                inFile.close();
                cento = 100;
                percentuale = i*cento/totalf;
                int status = percentuale;
                emit Status(status);
             
             ///////qDebug() << "### status " <<  status;
             //////////qDebug() << "### d " <<  goname;
         }
        
          zip.close();
          if(zip.getZipError()!=0) {
          QDir::setCurrent(startnow);
          }
          /////qDebug() << "### status 100";
          emit Status(100);
          emit ZipEnd(zipfile);
          ///////qDebug() << "### emitend " <<  zipfile;
          return;
    }
emit ZipError();
}
void ResetDir()
{
    emit ZipError();
    QDir::setCurrent(startnow);
    qDebug() << "### ResetDir to " << startnow;
}
void IndexDir(const QString d)
{
   QDir dir(d);
   if (dir.exists())
   {
      const QFileInfoList list = dir.entryInfoList();
      QFileInfo fi;
      for (int l = 0; l < list.size(); l++)
      {
         fi = list.at(l);
         if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..") {
            IndexDir(fi.absoluteFilePath());
         } else if (fi.isFile() && fi.fileName() != selfname ) {
            dirandfile.append(fi.absoluteFilePath());
         } 
        
      }
   }
}
    QString startnow;
    QStringList dirandfile;
    QString startdir;
    QString zipdir;
    QString zipfile;
    QString selfname;
    bool canmake;
signals:
    void ZipEnd( QString fileresult );
    void ZipError();
    void Status(int o );
public slots:

};







//
#endif // GOJOBZIP_H

