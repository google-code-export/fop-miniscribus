#ifndef SCRIBEMIME_H
#define SCRIBEMIME_H

#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>

#include <stdio.h>
#include <math.h>

#include <QtGui/qtextdocument.h>
#include <QtGui/qtextoption.h>
#include <QtGui/qtextcursor.h>
#include <QtGui/qtextformat.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qmenu.h>
#include <QtCore/qrect.h>
#include <QtGui/qabstracttextdocumentlayout.h>
#include <QtGui/qtextdocumentfragment.h>
#include <QTextLine>
#include <QSvgRenderer>

#include "_XML_Editor.h"
#include <QGraphicsScene>

#include <QHttp>
#include <QThread>

#define _DRAWMODUS_WEB_ 0


#define FOPIMAGEDIR "Pictures/"

static const int InterSpace = 15;  /* distance from page 1 to 2 */
static const int SliderTopHeight = 28;
static const int SliderLeftHeight = 28;
static const int MaximumPages = 99;
static const int BorderShadow = 5;

static const qreal FrameMarginBottomBugDraw = 2.8;

static const int DocumentBugSpace = 15;

static const int ObjectName = 853;
static const int FlashReferenceID = 322;


#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())    /* gs tmp */
             
#define _DUMMY_SPACE_DOCUMENT_ \
             QString("dummy_space_please") 
						 

#define _LINK_COLOR_ \
             QColor("#dc0000")
             
#define _DEFAULT_TXT_COLOR_ \
             QColor("#000088")   /* cursor color palette from text now is cursor color */

#define _DOCUMENT_TXT_COLOR_ \
             QColor("#717171")    /* real txt palette */


/* set your monitor to 72 DPI on go to */
/* set your monitor to 92 DPI on go to */
/*      http://www.unitconversion.org/unit_converter/typography.html            */

#define POINT_TO_CM(cm) ((cm)/28.3465058)  
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)
#define POINT_TO_PI(pi) ((pi)/12)
#define POINT_TO_DD(dd) ((dd)/154.08124)
#define POINT_TO_CC(cc) ((cc)/12.840103)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
#define PI_TO_POINT(pi) ((pi)*12)
#define DD_TO_POINT(dd) ((dd)*154.08124)
#define CC_TO_POINT(cc) ((cc)*12.840103)

/* help functions */
QPixmap BruschChess( qreal chesswidth = 50. );
/* allowed char on file name image to save */
QString Imagename( QString touri );
/* 12mm to point , 12cm , 2inch */
/* metric conversion from and to */
qreal FopInt( const QString datain );
qreal Pointo( qreal unit , const QString unita );
qreal ToPoint( qreal unit , const QString unita );

bool Cache( const QString dirpath );  /* mkdir */
QString PathConvert( QString path );  /* QDir::toNativeSeparators  */
bool fwriteutf8(const QString file , QString xml );
QString ReadFile(const QString fullFileName); /* read utf8 text */
/* read the contenet of a local file as QByteArray*/
QByteArray StreamFromFile(const QString fullFileName);
QString decodeBase64( QString xml );
QString encodeBase64( QString xml );
void OpenDeskBrowser( QUrl loc );  /* open any file or url */

QPixmap RenderPixmapFromSvgByte(  QByteArray streams ); 

QRectF M_PagesizeMake( QPrinter::PageSize psize , bool landscape = true );

QRectF CenterRectSlaveFromMaster( const QRectF Master , QRectF Slave  );

QPixmap ImagefromMime( const QMimeData *mime );


QString getGSLinuxPath( QString apps = QString("gs") );
QString getGSDefaultExeName();
double getGSVersion();
double JavaVersion();
int callGS( const QStringList args );
QPixmap LoadPS( QString fn , const QString arguments_append = QString() );
QPixmap LoadPDF(QString fn, int Page, int w );



class QTextEditMimeData : public QMimeData
{
public:
    inline QTextEditMimeData(const QTextDocumentFragment &aFragment) : fragment(aFragment) {}
    virtual QStringList formats() const;
protected:
    virtual QVariant retrieveData(const QString &mimeType, QVariant::Type type) const;
private:
    void setup() const;
    mutable QTextDocumentFragment fragment;
};






class GraphicsScene : public QGraphicsScene
{
     Q_OBJECT
    
public:
GraphicsScene( QObject * parent = 0 );
GraphicsScene( const QRectF & sceneRect, QObject * parent = 0 );
void clear();
void setSceneRect(  const QRectF & rect );
void clearSelection();
void SetVisibleArea( const QRectF area );
qreal zmax();
qreal zmin();
signals:
    void SelectOn(QGraphicsItem*);
    void MakeVisible(QRectF);
public slots:
protected:
    void dragMoveEvent ( QGraphicsSceneDragDropEvent * e );
    bool WakeUp( const QPointF incomming );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * e );
    void mousePressEvent( QGraphicsSceneMouseEvent * e );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void keyPressEvent(QKeyEvent *e);
    QGraphicsItem *bridge;
    void storno(); /* reset */
};


/*
Page formatting info enum qtprinter compatible 
Margin params on QRectF
css like QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
http://de.selfhtml.org/css/eigenschaften/randabstand.htm#margin
*/




class M_PageSize
{
    public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };
    M_PageSize();
		M_PageSize& operator=( const M_PageSize& d );
		operator QVariant() const {
    return QVariant::fromValue(*this);
    }
		QString HName();
		qreal faktor();
    void Register( QString n , QPrinter::PageSize pp , bool La = false );
		inline qreal hight()
		{
			return G_regt.height() - P_margin.width() - P_margin.x() + 0.5;  ////  -bottom - top 
		}
		qreal width()
		{
			return G_regt.width() - P_margin.height() - P_margin.y() + 0.5; /////////  -lef , - right
		}
		QSizeF internal_size()
		{ 
			return QSizeF(width(),hight());
		} 
		void SetMargin( QRectF rectp )
		{
			P_margin = rectp;
		}
		QRectF BoundingPageSumme( const int summe );      
		
		QRectF MarginArea()   /* page 0 */
		{
			const qreal LargeDoc = G_regt.width() - P_margin.y()  - P_margin.height();
			const qreal InnHightDoc = G_regt.height() - P_margin.x()  - P_margin.width();
			return QRectF(P_margin.height(),P_margin.x(),LargeDoc,InnHightDoc);
		}
		/* swap right - left margin  */
		QRectF MarginSwapArea()   /* page /0\ */
		{
			const qreal LargeDoc = G_regt.width() - P_margin.y()  - P_margin.height();
			const qreal InnHightDoc = G_regt.height() - P_margin.x()  - P_margin.width();
			return QRectF(P_margin.y(),P_margin.x(),LargeDoc,InnHightDoc);
		}
		
		QRectF DocumentInternal( const int index )
		{
			 const qreal fromTopY = internal_size().rheight() * index;
			 return QRectF(QPointF(0,fromTopY),internal_size());
		}
		
		QRectF DocumentExternal( const int index )
		{
			 const qreal fromTopY = G_regt.height() * index;
			 return QRectF(QPointF(0,fromTopY),G_regt.size());
		}
		/* internal rect whitout margin */
		QRectF PageInternal( const int index )
		{
			 const qreal fromTopY = index * G_regt.height();
			 const qreal spacepage = index * InterSpace;
			 return QRectF(QPointF(P_margin.height(),fromTopY + spacepage + P_margin.x()),internal_size());
		}       
		/* external rect document */
		QRectF PageExternal( const int index )
		{
			 const qreal fromTopY = index * G_regt.height();
			 const qreal spacepage = index * InterSpace;
			 return QRectF(QPointF(0,fromTopY + spacepage),G_regt.size());
		}
		QPointF MarginTranslate()
		{
			return QPointF(P_margin.height(),P_margin.x());
		}
		/*  edit modus */
		//////////////void HandleDocument( QTextDocument *doc );
		/* open or set a qtexdocument from this */
    QTextDocument *nulldoc( QString htm );
		/* write fop file margins */
    int M_PageSize::FillFopAttributes( QDomElement e );
		/* form qtexdocument to this margin an papersize */
		void HandlePrint( QTextDocument *doc );
		
		QPixmap PicfromMime( QMimeData *mime );
		
		
		
/////////  MarginPage = QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
QPrinter::PageSize P_rect;
QRectF G_regt;
QRectF P_margin;
QSizeF RealSize;
QString name;
bool landscape;
int modus;  /* 0 = normal , 1 = book , 2 = 2column */
int coolspace;
};

Q_DECLARE_METATYPE(M_PageSize);






class LoadGetImage : public QHttp
{
    Q_OBJECT
//
public: 
     LoadGetImage( int nr , QUrl url_send );
     void Start();
     inline int Get_id() { return cid; }
     inline int Htpp_id() { return Http_id; } 
     inline QPixmap pics() { return resultimage; } 
     int cid;
     int Http_id;
    QHttpRequestHeader header;
    QUrl url;
    QPixmap resultimage;
    signals:
      void take(int);
    public slots:
     void ImageReady( bool error );
};


class Gloader : public QThread
{
    Q_OBJECT
     
public:
  void Setting( QObject *parent , int id , QUrl url_send ); 
protected:
  void run();
  signals:
private:
    int cid;
    QUrl url;
    LoadGetImage *Rhttp;
    QObject* receiver;
};








#endif // SCRIBEMIME_H

