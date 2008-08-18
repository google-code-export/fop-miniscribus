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

#include "XML_Editor.h"
#include "Tools_Basic.h"
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


static const int TextFloatObjectName = 20;


static const int FooterHeaderPadding = 2;
static const int FooterHeaderMaxBlocks = 3;

#define _PAGE_NUMERATION_ \
             QString("#Page#")



#define _BG_CHAR_ALPHA_  425
#define _TXT_CHAR_ALPHA_  424


#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())    /* gs tmp */

#define _DUMMY_SPACE_DOCUMENT_ \
             QString("dummy_space_please")


#define _LINK_COLOR_ \
             QColor("#dc0000")

#define _INTERNAL_LINK_COLOR_ \
             QColor(Qt::darkMagenta)

#define _DEFAULT_TXT_COLOR_ \
             QColor("#000088")   /* cursor color palette from text now is cursor color */

#define _DOCUMENT_TXT_COLOR_ \
             QColor("#717171")    /* real txt palette */


#define _MASTERLINK_ \
             QString("http://code.google.com/p/fop-miniscribus/")    /* real txt palette */


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
QString Imagename( const  QString txt );
/* 12mm to point , 12cm , 2inch */
/* metric conversion from and to */
qreal FopInt( const QString datain );
qreal Pointo( qreal unit , const QString unita );
qreal ToUnit( qreal unit , const QString unita );
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

QIcon createColorToolButtonIcon(const QString &imageFile,QColor color);
QIcon createColorIcon( QColor color );


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


class FoRegion
{
public:
  FoRegion() {
    name = "region-body"; /* e.tagName().toLower().right(2) */
    margin_top = CM_TO_POINT(2.5);
    margin_bottom = CM_TO_POINT(2.5);
    margin_left = CM_TO_POINT(1.5);
    margin_right = CM_TO_POINT(1.5);
    bg = QColor(Qt::white);
    }
    FoRegion& operator=( const FoRegion& d )
    {
    name = d.name;
    bg = d.bg;
    margin_top = d.margin_top;
    margin_bottom = d.margin_bottom;
    margin_left = d.margin_left;
    margin_right = d.margin_right;
    edom = d.edom;
    return *this;
   }
   operator QVariant() const
   {
    return QVariant::fromValue(*this);
   }
   void toAll( const qreal unique )
   {
    margin_top = unique;
    margin_bottom = unique;
    margin_left = unique;
    margin_right = unique;
   }
   
   
  QColor bg; /* background color */
  QString name;
  qreal margin_top;
  qreal margin_bottom;
  qreal margin_left;
  qreal margin_right;
  QByteArray edom;   /* dome element*/
};


Q_DECLARE_METATYPE(FoRegion);


inline QDebug operator<<(QDebug debug, FoRegion& udoc)
{
    debug.nospace() << "FoRegion(name."
    << udoc.name << ",top()"
    << udoc.margin_top << ",bottom()"
    << udoc.margin_bottom << ",right()"
    << udoc.margin_right << ",left()"
    << udoc.margin_left << ")";
    return debug.space();
}



////////const qreal TopMargin = Pointo(P_margin.x(),"mm");
//////const qreal RightMargin = Pointo(P_margin.y(),"mm");
//////const qreal BottomMargin = Pointo(P_margin.width(),"mm");
/////const qreal LeftMargin = Pointo(P_margin.height(),"mm");


class M_PageSize
{
public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 2 };
    M_PageSize();
    M_PageSize& operator=( const M_PageSize& d );
    operator QVariant() const
    {
        return QVariant::fromValue(*this);
    }
    inline QRectF HeaderBoundingrect()
    {
        return  QRectF(0,0,width(),body.margin_top - ( FooterHeaderPadding * 2 ));
    }

    inline QPointF HeaderInitPoints( const int index = 0 )
    {
        const qreal fromTopY = index * G_regt.height();
        const qreal spacepage = index * InterSpace;
        return QPointF(body.margin_left,fromTopY + spacepage + FooterHeaderPadding);
    }

    inline QPointF FooterInitPoints( const int index = 0 )
    {
        const qreal fromTopY = index * G_regt.height();
        const qreal Ytop = G_regt.height() - body.margin_bottom;
        if (index ==  0)
        {
            return QPointF(body.margin_left,Ytop + FooterHeaderPadding);
        }
        const qreal spacepage = index * InterSpace;
        return QPointF(body.margin_left,fromTopY + spacepage + Ytop + FooterHeaderPadding);
    }


    inline QRectF FooterBoundingrect()
    {
        return  QRectF(0,0,width(),body.margin_bottom - ( FooterHeaderPadding * 2 ));
    }

    QString HName();
    qreal faktor();
    void Register( QString n , QPrinter::PageSize pp , bool La = false );
    inline qreal hight()
    {
        return G_regt.height() - body.margin_bottom - body.margin_top + 0.5;  ////  -bottom - top
    }
    qreal width()
    {
        return G_regt.width() - body.margin_left - body.margin_right + 0.5; /////////  -lef , - right
    }
    QSizeF internal_size()
    {
        return QSizeF(width(),hight());
    }
    void SetMargin( FoRegion e )
    {
        body = e;
    }
    QRectF BoundingPageSumme( const int summe );

    QRectF MarginArea()   /* page 0 */
    {
        const qreal LargeDoc = G_regt.width() - body.margin_right  - body.margin_left;
        const qreal InnHightDoc = G_regt.height() - body.margin_top  - body.margin_bottom;
        return QRectF(body.margin_left,body.margin_top,LargeDoc,InnHightDoc);
    }
    /* swap right - left margin  */
    QRectF MarginSwapArea()   /* page /0\ */
    {
        const qreal LargeDoc = G_regt.width() - body.margin_right  - body.margin_left;
        const qreal InnHightDoc = G_regt.height() - body.margin_top  - body.margin_bottom;
        return QRectF(body.margin_right,body.margin_top,LargeDoc,InnHightDoc);
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
        return QRectF(QPointF(body.margin_left,fromTopY + spacepage + body.margin_top),internal_size());
    }
    /* external rect document */
    QRectF PageExternal( const int index )
    {
        const qreal fromTopY = index * G_regt.height();
        const qreal spacepage = index * InterSpace;
        return QRectF(QPointF(0,fromTopY + spacepage),G_regt.size());
    }
    
    QRectF PrintArea()
    {
        return QRectF(QPointF(body.margin_left,body.margin_top),QSizeF(width(),G_regt.height() * MaximumPages));
    }
    
    
    
    
    QPointF MarginTranslate()
    {
        return QPointF(body.margin_left,body.margin_top);
    }
    void ReportPage( QDomElement e )
    {
		const qreal TopMargin = Pointo(body.margin_top,"mm");
		const qreal RightMargin = Pointo(body.margin_right,"mm");
		const qreal BottomMargin = Pointo(body.margin_bottom,"mm");
		const qreal LeftMargin = Pointo(body.margin_left,"mm");
		e.setAttribute ("margin-top",QString("%1mm").arg(TopMargin));
    e.setAttribute ("margin-bottom",QString("%1mm").arg(BottomMargin));
    e.setAttribute ("margin-left",QString("%1mm").arg(LeftMargin));
    e.setAttribute ("margin-right",QString("%1mm").arg(RightMargin));
    e.setAttribute ("page-width",QString("%1mm").arg(Pointo(G_regt.width(),"mm")));
    e.setAttribute ("page-height",QString("%1mm").arg(Pointo(G_regt.height(),"mm")));
    }
    /*  edit modus */
    //////////////void HandleDocument( QTextDocument *doc );
    /* open or set a qtexdocument from this */
    QTextDocument *nulldoc( QString htm );
    /* write fop file margins */
    int FillFopAttributes( QDomElement e );
    /* form qtexdocument to this margin an papersize */
    void HandlePrint( QTextDocument *doc );

    ////////QPixmap PicfromMime( QMimeData *mime );
    QPrinter::PageSize P_rect;
    QRectF G_regt;
    FoRegion body;
    QSizeF RealSize;
    QString name;
    bool landscape;
    bool AllowHeaderFooter;
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
    inline int Get_id()
    {
        return cid;
    }
    inline int Htpp_id()
    {
        return Http_id;
    }
    inline QPixmap pics()
    {
        return resultimage;
    }
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




class FrameStyler : public QWidget
{
    Q_OBJECT
//
public:
    FrameStyler( QTextFrame *base , QWidget *parent  )
            : QWidget( parent ),frame(base)
    {
        QTextFrameFormat Ftf = frame->frameFormat();
        QTextDocument *dd = base->document();
        /////QTextFrame  *RootFrame = dd->rootFrame();
        qreal largoss = dd->pageSize().rwidth();
        QTextLength langeframles = Ftf.width();
        if (langeframles.type() == QTextLength::FixedLength)
        {
            largoss = langeframles.rawValue();
        }



        gridLayout = new QGridLayout(this);
        hboxLayout = new QHBoxLayout();
        vboxLayout = new QVBoxLayout();
        label = new QLabel(tr("Border style:"),this);
        vboxLayout->addWidget(label);
        label_3 = new QLabel(tr("Border color:"),this);
        vboxLayout->addWidget(label_3);
        label_2 = new QLabel(tr("Float:"),this);
        vboxLayout->addWidget(label_2);
        vboxLayout->addWidget(new QLabel(tr("Border width:"),this));
        vboxLayout->addWidget(new QLabel(tr("Frame width:"),this));
        vboxLayout->addWidget(new QLabel(tr("Padding:"),this));



        hboxLayout->addLayout(vboxLayout);
        vboxLayout1 = new QVBoxLayout();
        comboBox = new QComboBox(this); ///// style
        comboBox->addItem(tr("Border none"),QTextFrameFormat::BorderStyle_None);
        comboBox->addItem(tr("Border Dotted"),QTextFrameFormat::BorderStyle_Dotted);
        comboBox->addItem(tr("Border Double"),QTextFrameFormat::BorderStyle_Double);
        comboBox->addItem(tr("Border Solid"),QTextFrameFormat::BorderStyle_Solid);
        comboBox->addItem(tr("Border Dashed"),QTextFrameFormat::BorderStyle_Dashed);
        comboBox->addItem(tr("Border DotDash"),QTextFrameFormat::BorderStyle_DotDash);
        comboBox->addItem(tr("Border DotDotDash"),QTextFrameFormat::BorderStyle_DotDotDash);
        comboBox->addItem(tr("Border Groove"),QTextFrameFormat::BorderStyle_Groove);
        comboBox->addItem(tr("Border Ridge"),QTextFrameFormat::BorderStyle_Ridge);
        comboBox->addItem(tr("Border Inset"),QTextFrameFormat::BorderStyle_Inset);
        comboBox->addItem(tr("Border Outset"),QTextFrameFormat::BorderStyle_Outset);
        comboBox->setCurrentIndex(comboBox->findData(Ftf.borderStyle()));


        QPixmap pix(22, 22);
        vboxLayout1->addWidget(comboBox);
        comboBox_3 = new QComboBox(this); //// color
        QStringList colorNames = QColor::colorNames();
        foreach (QString name, colorNames)
        {
            pix.fill(QColor(name));
            comboBox_3->addItem(pix,QString("color %1").arg(name),QColor(name));
        }
        comboBox_3->setCurrentIndex(comboBox_3->findData(Ftf.borderBrush().color()));

        vboxLayout1->addWidget(comboBox_3);
        comboBox_2 = new QComboBox(this);   /// float
        comboBox_2->addItem(tr("Float inline"),QTextFrameFormat::InFlow);
        comboBox_2->addItem(tr("Float left"),QTextFrameFormat::FloatLeft);
        comboBox_2->addItem(tr("Float right"),QTextFrameFormat::FloatRight);

        comboBox_2->setCurrentIndex(comboBox->findData(Ftf.position()));

        vboxLayout1->addWidget(comboBox_2);
        /* border widht */
        borderwi = new QSpinBox(this);
        borderwi->setMaximum(20);
        borderwi->setValue(Ftf.border());
        vboxLayout1->addWidget(borderwi);

        framewi = new QSpinBox(this);
        framewi->setMaximum(dd->idealWidth());
        framewi->setValue(largoss);
        vboxLayout1->addWidget(framewi);

        paddingwi = new QSpinBox(this);
        paddingwi->setMaximum(50);
        paddingwi->setValue(Ftf.padding());
        vboxLayout1->addWidget(paddingwi);


        hboxLayout->addLayout(vboxLayout1);
        gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);
        QObject::connect(comboBox_3, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(comboBox_2, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(borderwi, SIGNAL(valueChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(paddingwi, SIGNAL(valueChanged(int)),this, SLOT(FormatUpdate()));
        QObject::connect(framewi, SIGNAL(valueChanged(int)),this, SLOT(FormatUpdate()));






    }
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QVBoxLayout *vboxLayout1;
    QComboBox *comboBox;
    QComboBox *comboBox_3;
    QComboBox *comboBox_2;
    QSpinBox *borderwi;
    QSpinBox *paddingwi;
    QSpinBox *framewi;

private:
    QTextFrame *frame;
signals:
public slots:

    void FormatUpdate()
    {
        QTextFrameFormat Ftf = frame->frameFormat();
        Ftf.setWidth ( framewi->value() );
        Ftf.setPosition(QTextFrameFormat::Position(comboBox_2->itemData(comboBox_2->currentIndex()).toInt()));
        Ftf.setBorderStyle(QTextFrameFormat::BorderStyle(comboBox->itemData(comboBox->currentIndex()).toInt()));
        Ftf.setBorderBrush(QBrush(comboBox_3->itemData(comboBox_3->currentIndex()).value<QColor>()));
        Ftf.setBorder(borderwi->value());
        Ftf.setPadding(paddingwi->value());
        frame->setFrameFormat(Ftf);
    }

};








class Rotater : public QWidget
{
    Q_OBJECT
//
public:
    Rotater( int rotaten , QWidget *parent  )
    {
        gridLayout = new QGridLayout(this);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label = new QLabel(tr("Rotate �:"),this);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 25));
        hboxLayout->addWidget(label);
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        hboxLayout->addItem(spacerItem);
        lcdNumber = new QLCDNumber(this);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setMaximumSize(QSize(16777215, 25));
        lcdNumber->setAutoFillBackground(false);
        lcdNumber->setFrameShadow(QFrame::Sunken);
        lcdNumber->display(rotaten);
        hboxLayout->addWidget(lcdNumber);
        gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);

        dial = new QDial(this);
        dial->setObjectName(QString::fromUtf8("dial"));
        dial->setFocusPolicy(Qt::NoFocus);
        dial->setContextMenuPolicy(Qt::NoContextMenu);
        dial->setNotchesVisible(true);
        dial->setMaximum(360);
        dial->setValue(rotaten);
        gridLayout->addWidget(dial, 1, 0, 1, 1);
        connect(dial, SIGNAL(dialMoved(int)),this, SLOT(NewValue(int)));

    }

    ~Rotater()
    {
        dial->disconnect(this);
    }


    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QSpacerItem *spacerItem;
    QLCDNumber *lcdNumber;
    QDial *dial;
private:
signals:
    void rotater(int);
public slots:

    void NewValue( const int x )
    {
        lcdNumber->display(x);
        emit rotater(x);
    }



};



#endif // SCRIBEMIME_H

