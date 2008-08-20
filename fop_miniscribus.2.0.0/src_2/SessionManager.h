#ifndef ApiSession_H
#define ApiSession_H

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

#include "Config.h"



class ApiSession {
public:
	static ApiSession* instance();
  ~ApiSession();
  void LoadFontDir( const QString path );
  inline QString tra_file() const  { return transfile; }
  void Set_Translate_File( const QString file );
  inline QMap<int,M_PageSize> mpages() { return history_page_norms; }
  inline QStringList  FontList() { return FF_db.families(); }
  M_PageSize FindPagePsize( const QRect paper );
  inline bool Lastsignature() { return true; }  /* is open a miniscribus file true xsl-fo chunk unrelax false */
  inline QPicture page( const int e) const {
  return pagecache[e];
  }
  void AppendPaper( M_PageSize cur );
  inline M_PageSize CurrentPageFormat() { return current_Page_Format; }
	inline void SetPageFormat( M_PageSize e ) { current_Page_Format = e; }
	void SaveMimeTmp();
	bool canmime() { return mime_story.size() >  0 ? true : false; }
	QPrinter *pdev;
  QRectF SceneViewPort;
	int CurrentDocWidth;
	QMap<QString,SPics> ImagePageList;   /* image on all documents cache */
	QMap<int,QString> LastCharOpen;
	QMap<int,QString> LastBlockOpen;
	QMap<QString,QByteArray> SvgList;
  QMap<int,QPicture> pagecache;  /* cache pages images */
	QMap<int,QMimeData*> mime_story;
  QSettings setter;
	int SessionID;
	
  M_PageSize current_Page_Format;
  
private:
  ApiSession();
  static ApiSession* st_;
  void FormatRegister( const QString txt , QPrinter::PageSize pp );
  QString transfile;
  QFontDatabase FF_db;
  QTranslator PPtr;
  QMap<int,M_PageSize> history_page_norms;
  

  
};






#endif // ApiSession_H

