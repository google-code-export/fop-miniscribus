#ifndef QTEXTPANELDATA_H
#define QTEXTPANELDATA_H

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

#include "qtextpanelmime.h"
#include "qtextpanelimage.h"

#define _PARSE_DEBUG_FOP_  1

class QTextPanelData
{
   private:
	   static QTextPanelData *_instance;
	   QString transfile;
	   QFontDatabase FF_db;
	   QTranslator PPtr;
	   QMap<int,PanelPageSize> history_page_norms;
	   QTextPanelData();
	   void FormatRegister(const QString txt , QPrinter::PageSize pp);

   public:
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
	   PanelPageSize current_Page_Format;

	   static QTextPanelData* instance();
	   ~QTextPanelData();
	   void LoadFontDir(const QString path);
	   inline QString tra_file() const {return transfile;}
	   void Set_Translate_File(const QString file);
	   inline QMap<int,PanelPageSize> mpages() {return history_page_norms;}
	   inline QStringList  FontList() {return FF_db.families();}
	   PanelPageSize FindPagePsize(const QRect paper);
	   inline bool Lastsignature() {return true;}   /* is open a miniscribus file true xsl-fo chunk unrelax false */
	   inline QPicture page(const int e) const {return pagecache[e];}
	   void AppendPaper(PanelPageSize cur);
	   inline PanelPageSize CurrentPageFormat() {return current_Page_Format;}
	   inline void SetPageFormat(PanelPageSize e) {current_Page_Format = e;}
	   void SaveMimeTmp();
	   bool canmime() {return mime_story.size() >  0 ? true : false;}
};

#endif // QTEXTPANELDATA_H
