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
#include "PageFormatGroup.h"
#include "BasicFoConfig.h"
#include "Image_Page_Struct_Mime.h"


class ApiSession {
public:
	static ApiSession* instance();
  ~ApiSession();


  inline QMap<int,M_PageSize> mpages() { return history_page_norms; }
  bool coreSave( M_PageSize e );   /* to restore original from dlg pageformat */
   M_PageSize FindPagePsize( const QRect paper );
  void AppendPaper( M_PageSize cur , bool enableregion = false );
  bool is_OnChain( M_PageSize e );
  inline QStringList  FontList() { return FF_db.families(); }
  inline M_PageSize CurrentPageFormat() { return current_Page_Format; }
  inline void SetPageFormat( M_PageSize e ) { current_Page_Format = e; }
  inline QMap<QString, int> uniqueID() const { return uniqueid; }
  QString validateUnique( const QString idkei );
  bool validKey( const QString idkei ) const;
  void clearKey();
  
  qreal maximumPage();
    QPrinter *pdev;
    QRectF SceneViewPort;
    int CurrentDocWidth;
    QMap<int,QString> LastCharOpen;
    QMap<QString,QByteArray> saveImageFile;
    QMap<int,QString> LastBlockOpen;
    QMap<QString,QByteArray> SvgList;
    QMap<int,QPicture> pagecache;  /* cache pages images */
    QMap<int,QMimeData*> mime_story;
    QMap<QString,SPics> ImagePageList;   /* image on all documents cache */
    QSettings setter;
    int SessionID;
    M_PageSize current_Page_Format;
    QByteArray bokMarkDom;
    
  
private:
  ApiSession();
  QMap<QString, int> uniqueid;
  static ApiSession* st_;
  void FormatRegister( const QString txt , QPrinter::PageSize pp );
  ///////QString transfile;
  QFontDatabase FF_db;
  ////////QTranslator PPtr;
  QMap<int,M_PageSize> history_page_norms;
  

  
};






#endif // ApiSession_H

