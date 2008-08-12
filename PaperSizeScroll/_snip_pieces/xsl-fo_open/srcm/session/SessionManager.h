#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QtGui>
#include <QDebug>
#include <QApplication>
#include <QtGui>
#include <QtCore> 

#include <QHttp>
#include <QAbstractSocket>
#include <QFileInfo>
#include <QHttp>
#include <QUrl>
#include <QTimer>

#include "Basic_Config.h"
#include "GSInterlace.h"
#include "pics_structure.h"
#include "richdoc_structure.h"
#include <QSettings>


#define _PARSE_DEBUG_FOP_  1



static inline QByteArray FiStream( const QString file )
{
    QByteArray stream;
     QFile f(file); 
        if (f.exists()) {
          if (f.open(QFile::ReadOnly)) {
           stream = f.readAll();
           f.close();
           return stream;
          }
        }
   return stream;
}





class SessionManager {
public:
	static SessionManager* instance();
  void LoadFontDir( const QString path );
  inline QString tra_file() const  { return transfile; }
  void Set_Translate_File( const QString file );
	inline QMap<int,M_PageSize> mpages() { return history_page_norms; }
	inline QStringList  FontList() { return FF_db.families(); }
	M_PageSize FindPagePsize( const QRect paper );
	QString DebugAttributes();
  inline bool Lastsignature() { return true; }  /* is open a miniscribus file true xsl-fo chunk unrelax false */
	
	int CurrentDocWidth;
	QMap<int,QString> LastCharOpen;
	QMap<int,QString> LastBlockOpen;
	QMap<QString,QByteArray> SvgList;
	QMap<QString,SPics> current_pics;
  
private:
	SessionManager();
	static SessionManager* st_;
  void FormatRegister( const QString txt , QPrinter::PageSize pp );
  QString transfile;
  QFontDatabase FF_db;
  QTranslator PPtr;
  QMap<QString,SPics> history_pics;  /* name + class */
  QMap<int,M_PageSize> history_page_norms;

  
};



/*  tr macro on no Q_OBJECT class !!!! */
static inline void OpenDesk( QUrl loc )
{
    #if defined Q_WS_MAC
    /* direct to open */
    QProcess *m = new QProcess();
    QStringList macs;
    macs << loc.toString();
    m->startDetached(QString("open") , macs );
    return;
    #endif 
    
#if defined Q_WS_WIN
    QString fullFileName = loc.toString();
    
    if (fullFileName.startsWith("http://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("https://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("ftp://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("news://", Qt::CaseInsensitive) ||
        fullFileName.startsWith("mailto:", Qt::CaseInsensitive) ||
        fullFileName.startsWith("webdav://", Qt::CaseInsensitive)  )
       { 
         
       } else {
         fullFileName.prepend("file:///");     /* or qurl to localfile */
       }
    bool wr = QDesktopServices::openUrl(QUrl(fullFileName));
    if (!wr) {
    QMessageBox::warning(0,QString("Error"),
                QString("Window Unable to open action file or dir  %1").arg(loc.toString()));
    }
return;
#endif
    /* linux */
     bool r = QDesktopServices::openUrl(loc);
    if (!r) {
    QMessageBox::warning(0,QString("Error"),
              QString("Linux Unable to open action file or dir  %1").arg(loc.toString()));
    }
    
}





#endif // SESSIONMANAGER_H

