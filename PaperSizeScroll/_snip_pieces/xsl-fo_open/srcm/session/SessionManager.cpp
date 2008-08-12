#include "SessionManager.h"

SessionManager* SessionManager::st_ = 0;


SessionManager* SessionManager::instance() {
	if (st_ == 0) {
		st_ = new SessionManager();
  }
	return st_;
}

SessionManager::SessionManager()
: transfile(0)
{ 
  history_pics.clear();
  FF_db = QFontDatabase();
  LoadFontDir(QDir::currentPath()); 
  
	history_page_norms.clear();
  
    
	  FormatRegister(QT_TR_NOOP("A4 (210 x 297 mm, 8.26 x 11.7 inches)"), QPrinter::A4);
	  FormatRegister(QT_TR_NOOP("A4 (211 x 297 mm, 8.26 x 11.7 inches)"), QPrinter::A4);
	  FormatRegister(QT_TR_NOOP("A0 (841 x 1189 mm)"), QPrinter::A0);
    FormatRegister(QT_TR_NOOP("A1 (594 x 841 mm)"), QPrinter::A1);
    FormatRegister(QT_TR_NOOP("A2 (420 x 594 mm)"), QPrinter::A2);
    FormatRegister(QT_TR_NOOP("A3 (297 x 420 mm)"), QPrinter::A3);
    FormatRegister(QT_TR_NOOP("A5 (148 x 210 mm)"), QPrinter::A5);
    FormatRegister(QT_TR_NOOP("A6 (105 x 148 mm)"), QPrinter::A6);
    FormatRegister(QT_TR_NOOP("A7 (74 x 105 mm)"), QPrinter::A7);
    FormatRegister(QT_TR_NOOP("A8 (52 x 74 mm)"), QPrinter::A8);
    FormatRegister(QT_TR_NOOP("A9 (37 x 52 mm)"), QPrinter::A9);
    FormatRegister(QT_TR_NOOP("B0 (1000 x 1414 mm)"), QPrinter::B0);
    FormatRegister(QT_TR_NOOP("B1 (707 x 1000 mm)"), QPrinter::B1);
    FormatRegister(QT_TR_NOOP("B2 (500 x 707 mm)"), QPrinter::B2);
    FormatRegister(QT_TR_NOOP("B3 (353 x 500 mm)"), QPrinter::B3);
    FormatRegister(QT_TR_NOOP("B4 (250 x 353 mm)"), QPrinter::B4);
    FormatRegister(QT_TR_NOOP("B5 (176 x 250 mm, 6.93 x 9.84 inches)"), QPrinter::B5);
    FormatRegister(QT_TR_NOOP("B6 (125 x 176 mm)"), QPrinter::B6);
    FormatRegister(QT_TR_NOOP("B7 (88 x 125 mm)"), QPrinter::B7);
    FormatRegister(QT_TR_NOOP("B8 (62 x 88 mm)"), QPrinter::B8);
    FormatRegister(QT_TR_NOOP("B9 (44 x 62 mm)"), QPrinter::B9);
    FormatRegister(QT_TR_NOOP("B10 (31 x 44 mm)"), QPrinter::B10);
    FormatRegister(QT_TR_NOOP("C5E (163 x 229 mm)"), QPrinter::C5E);
    FormatRegister(QT_TR_NOOP("DLE (110 x 220 mm)"), QPrinter::DLE);
    FormatRegister(QT_TR_NOOP("Executive (7.5 x 10 inches, 191 x 254 mm)"), QPrinter::Executive);
    FormatRegister(QT_TR_NOOP("Folio (210 x 330 mm)"), QPrinter::Folio);
    FormatRegister(QT_TR_NOOP("Ledger (432 x 279 mm)"), QPrinter::Ledger);
    FormatRegister(QT_TR_NOOP("Legal (8.5 x 14 inches, 216 x 356 mm)"), QPrinter::Legal);
    FormatRegister(QT_TR_NOOP("Letter (8.5 x 11 inches, 216 x 279 mm)"), QPrinter::Letter);
    FormatRegister(QT_TR_NOOP("Tabloid (279 x 432 mm)"), QPrinter::Tabloid);
    FormatRegister(QT_TR_NOOP("US Common #10 Envelope (105 x 241 mm)"), QPrinter::Comm10E);
}


void SessionManager::LoadFontDir( const QString path )
{
  ////  QFontDatabase
  
  
  QDir dir(path);
   if (dir.exists())
   {
      const QFileInfoList list = dir.entryInfoList();
      QFileInfo fi;
      for (int l = 0; l < list.size(); l++)
      {
         fi = list.at(l);
         if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..") {
           
         } else if (fi.isFile())
         {
               QString surr = fi.completeSuffix().toLower();
               if (surr == "ttf") {
                 QByteArray fakefont = FiStream( fi.absoluteFilePath () );
                 int loadet = FF_db.addApplicationFontFromData( fakefont );
                 qDebug() << "### load " << fi.fileName() << " loadet ->" << loadet; 
               }
               ////////////qDebug() << "### read  " << fi.fileName() << " extension ->" << surr; 
         } 
        
      }
   }
}







void SessionManager::FormatRegister( const QString txt , QPrinter::PageSize pp )
{
	   M_PageSize Lxx;
	              Lxx.Register(txt,pp,false);
	   history_page_norms.insert(history_page_norms.size() + 1,Lxx);
	   M_PageSize Pxx;
	              Pxx.Register(txt,pp,true);
	   history_page_norms.insert(history_page_norms.size() + 1,Pxx);
}


M_PageSize SessionManager::FindPagePsize( const QRect paper )
{
    QMapIterator<int,M_PageSize> i(mpages());
         while (i.hasNext()) {
            i.next();
					  M_PageSize xx = i.value();
            if (xx.G_regt.toRect() == paper) {
              return xx;
            }
           }
   return history_page_norms[1];
}


QString SessionManager::DebugAttributes()
{
  QString fox;
  
    QMapIterator<int,QString> i(LastBlockOpen);
    QMapIterator<int,QString> e(LastCharOpen);
  
         while (i.hasNext()) {
            i.next();
					  fox.append(QString("Para %1:\n").arg(i.key()));
            fox.append(i.value());
           }
           
           while (e.hasNext()) {
            e.next();
					  fox.append(QString("Char %1:\n").arg(e.key()));
            fox.append(e.value());
           
           }
           
           
   return fox;
}




void SessionManager::Set_Translate_File( const QString file ) 
{ 
    transfile = file;
	  PPtr.load(file);
	  qApp->installTranslator(&PPtr);
}



