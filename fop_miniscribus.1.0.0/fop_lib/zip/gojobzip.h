#ifndef GOJOBZIP_H
#define GOJOBZIP_H

#include "quazip.h"
#include "quazipfile.h"
#include <QObject>
#include <QEvent>
#include <QCoreApplication>
#include <QApplication>
#include <QtGui>


class PackZip : public QObject
{
    Q_OBJECT
    
public:
PackZip( QObject * parent )
{
 startnow = QDir::currentPath();
}  
~PackZip()
{
    QDir::setCurrent(startnow);
}      
void SetJob( const QString packfolder , const QString zipoutfolder )
{
    
  startdir = packfolder;   /* grab all this file */
  zipdir = zipoutfolder;   /* save zip to this dir */
  startnow = QDir::currentPath();
  Init();
}

private:
void Init()
{
    canmake = false;
    if (!zipdir.contains("/")) {
        ResetDir();
        return;
    }
    if (!zipdir.endsWith("/")) {
        ResetDir();
       return; 
    }
    if (!startdir.contains("/")) {
        ResetDir();
        return;
    }
    if (!startdir.endsWith("/")) {
       ResetDir();
       return; 
    }
    canmake = true;
    QDir dir(startdir);
    char c;
    qreal cento;
    qreal percentuale;
    QString dirname = dir.dirName();
    selfname = dirname + ".zip";
    zipfile = zipdir;
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
      
       ///////qDebug() << "### totalfile " <<  dirandfile.size();
         for (int i = 0; i < dirandfile.size(); ++i) {
             QString filepak = dirandfile.at(i);
             const  QString abse = filepak;
             const  QString goname = filepak.replace(startdir,"");
             
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
             
             qDebug() << "### status " <<  status;
             //////////qDebug() << "### d " <<  goname;
         }
        
          zip.close();
          if(zip.getZipError()!=0) {
          QDir::setCurrent(startnow);
          }
          qDebug() << "### status 100";
          emit Status(100);
          emit ZipEnd(zipfile);
          return;
          /////////////qDebug() << "### emitend " <<  zipfile;
    }
emit ZipError();
}
void ResetDir()
{
    emit ZipError();
    QDir::setCurrent(startnow);
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

