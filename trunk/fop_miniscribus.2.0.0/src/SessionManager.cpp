#include "SessionManager.h"

#include "BasicFoConfig.h"


ApiSession* ApiSession::st_ = 0;


ApiSession* ApiSession::instance() {
	if (st_ == 0) {
		st_ = new ApiSession();
  }
	return st_;
}

ApiSession::~ApiSession()
{
  delete this;
  st_ = 0;
}

ApiSession::ApiSession()
: pdev(new QPrinter(QPrinter::ScreenResolution)) 
{ 
  
  FF_db = QFontDatabase();
  SessionID = qrand () % 1000;
  qDebug() << "### session init...."  << SessionID;
  
  history_page_norms.clear();
  
  /* qsetting init to open */
  ///////QCoreApplication::setOrganizationName("CrossKern");
  ////QCoreApplication::setOrganizationDomain("fop.ciz.ch");
  ///////QCoreApplication::setApplicationName("Dummy TextApi");
    
     M_PageSize CDLabel;
     CDLabel.name = "CD-Label (128 x 128mm)";
     CDLabel.G_regt = QRectF(0,0,MM_TO_POINT(128),MM_TO_POINT(128));
     CDLabel.P_rect = QPrinter::Custom;
     CDLabel.RealSize = QSizeF(MM_TO_POINT(220),MM_TO_POINT(220));  //////CDLabel.G_regt.size();
     const qreal margindd = MM_TO_POINT(10);
    FoRegion cdregion;
    cdregion.rpen = QPen(Qt::black,MM_TO_POINT(2),Qt::DashLine);
    cdregion.border = 1;
    cdregion.bog = QColor("tomato");
    
    cdregion.toAll( margindd );
    CDLabel.body = cdregion;
    
    FormatRegister(QT_TR_NOOP("A4 (210 x 297 mm)"), QPrinter::A4);
    FormatRegister(QT_TR_NOOP("A4 (211 x 297 mm)"), QPrinter::A4);
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
    
    
    
    AppendPaper( CDLabel , true );
    
    
    
    current_Page_Format = history_page_norms[1];
    
    
    /////////////qDebug() << "### current format faktor name " << current_Page_Format.faktor() << "-" << current_Page_Format.HName();
    //////////////qDebug() << "### current format margin hash" << current_Page_Format.body << "-" << current_Page_Format.hashmodel();
    
}

void ApiSession::clearKey()
{
    uniqueid.clear();
}

bool ApiSession::validKey( const QString idkei ) const
{
      QMap<QString, int>::const_iterator i = uniqueid.find(idkei);
      int loop = 0;
    
         while (i != uniqueid.end() && i.key() == idkei) {
             ///////////qDebug() << "### unique key found ->"  << i.key();
             ++i;
             loop++;
         }
         if (loop == 0) {
          return true;
         } else {
          return false; 
         }
}


QString ApiSession::validateUnique( const QString idkei )
{
         if (validKey(idkei)) {
            uniqueid.insert(idkei,uniqueid.size() + 1 ); 
            return QString(idkei);
         } else {
              for (int i = 0; i < 100; ++i) {
                    int increase = qrand() % 99;
                    const int gostop = i + increase;
                    QChar letter('A' + (qrand() % 26));
                    QChar letter1('A' + (qrand() % 26));
                    QString place = idkei;
                    place.prepend(letter1);
                    place.append(QString("%1").arg(increase));
                    place.append(letter);
                    if (validKey(place)) {
                     uniqueid.insert(place,uniqueid.size() + 1 ); 
                     return place;                        
                    }
              }
             
            
          //////////////qDebug() << "### unable / unique key  / to handle->"  << idkei;
          return QString(idkei);
            
         }
    
}



qreal ApiSession::maximumPage()
{
    qreal largo = 500;
    
         QMapIterator<int,M_PageSize> i(mpages());
         while (i.hasNext()) {
             i.next();
             M_PageSize current = i.value();
             const QRectF rect = current.pageBoundingRect();
             largo = qMax (rect.width(),largo);
             largo = qMax (rect.height(),largo);
         }
  return largo;
}

bool ApiSession::is_OnChain( M_PageSize e )
{
    bool iifound = false;
    
         QMapIterator<int,M_PageSize> i(mpages());
         while (i.hasNext()) {
             i.next();
             M_PageSize current = i.value();
             if ( e.hashmodel() == current.hashmodel()) {
                 iifound = true;
             }
         }
         
     if (  !iifound )  {
       AppendPaper(e);
     }         
    return iifound;
}


bool ApiSession::coreSave( M_PageSize e )
{
    bool cansave= false;
    
         QMapIterator<int,M_PageSize> i(mpages());
         while (i.hasNext()) {
             i.next();
             M_PageSize current = i.value();
             if ( e.HName() == current.HName()) {
                 cansave= true;
                 const int dove = i.key();
                 history_page_norms[dove]= e;
             }
         }
    return cansave;
}



void ApiSession::FormatRegister( const QString txt , QPrinter::PageSize pp )
{
  
	   M_PageSize Pxx;
       Pxx.Register(txt,pp);
       AppendPaper(Pxx);
       
}

void ApiSession::AppendPaper( M_PageSize cur , bool enableregion )
{
   ////////// qDebug() << "### AppendPaper  "  << enableregion  ;
    cur.area[0].enable = enableregion;
    cur.area[1].enable = enableregion;
    cur.area[2].enable = enableregion;
    cur.area[3].enable = enableregion;
    /*
      FoRegion region_before() const { return area[0]; }
    FoRegion region_after() const { return area[1]; }
    FoRegion region_start() const  { return area[2]; }
    FoRegion region_end()  const { return area[3]; }
    
    */
    history_page_norms.insert(history_page_norms.size() + 1,cur);   
}





M_PageSize ApiSession::FindPagePsize( const QRect paper )
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





