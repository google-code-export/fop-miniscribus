#include "Paper_Text_Edit.h"
#include "Fo_Reader.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif


#include "Config.h"

using namespace ApacheFop;

GraphicsView::GraphicsView( QWidget * parent )
	: QGraphicsView( parent ),OnPrintRender(false)
{
	 QApplication::restoreOverrideCursor();
   QPalette p = palette();
   p.setColor(QPalette::Window,Qt::lightGray);
   p.setColor(QPalette::Base,Qt::lightGray);
   setPalette(p);
    
   BASE_TEXT = new TextLayer(0);
   Fo_Reader * fops = new Fo_Reader("a.fo");
    
   scene = new GraphicsScene(rectToScene(),this);
   setCacheMode(CacheBackground);
   setScene(scene);
    
   
   const QTextDocument *fopdoc = fops->document()->clone();
   fops->deleteLater(); 
   BASE_TEXT->setDocument(fopdoc);
   scene->addItem(BASE_TEXT);
   connect(scene, SIGNAL(MakeVisible(QRectF) ), this, SLOT(ViewDisplay(QRectF)));
   connect(BASE_TEXT, SIGNAL(PageCountChange() ), this, SLOT(ForceResize()));
   QTimer::singleShot(400, this, SLOT(ForceResize())); 
    
}

void GraphicsView::ForceResize()
{
    scene->setSceneRect( rectToScene());
    emit NewPageFormatin();
}


void GraphicsView::resizeEvent(QResizeEvent *event)
{
    scene->setSceneRect( rectToScene());
    QGraphicsView::resizeEvent(event);
 
    ///////ApiSession *sx = ApiSession::instance();
    /////const qreal Page_Width = sx->CurrentPageFormat().G_regt.width();
    ////////ensureVisible(QRectF(0, 0, Page_Width, Page_Width / 2));
    ///////////fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
   
}


void GraphicsView::ViewDisplay( const QRectF area )
{
    ////////qDebug() << "### GraphicsView::ViewDisplay -------- " << area;
    /* if scale to big return */
    QMatrix matx = matrix();
    qreal HHscaled = matx.m11();
    if (HHscaled > 1.4333333) {
    return;
    }
    QGraphicsView::ensureVisible(area);
}

QRectF GraphicsView::rectToScene()
{
    ApiSession *sx = ApiSession::instance();
    M_PageSize PAGE_MODEL = sx->CurrentPageFormat();
    const QTextDocument *bdoc = BASE_TEXT->document()->clone();
    const int PageSumm = qBound (1,bdoc->pageCount(),MaximumPages);
    if (PageSumm == MaximumPages) {
        /* save your chunk please ???? */
        QMessageBox::warning(this, tr("Alert on %1").arg(_APPLICATIONS_NAME_),
                           tr("You are try to cover maximum page %1!").arg(PageSumm));
    }
    qDebug() << "### PageSumm request by view -------- " << PageSumm;
    const qreal fromTopY = PageSumm * PAGE_MODEL.G_regt.height();
    const qreal spacepage = (PageSumm - 1) * InterSpace;
    return QRectF(0,0,PAGE_MODEL.G_regt.width(),fromTopY + spacepage + _BOTTOM_VIEW_SPACE_RESERVE_);
}

void GraphicsView::PrintSetup( bool printok )
{
    OnPrintRender = printok;
    update();
}

void GraphicsView::SwapPaper()
{
   const QRectF rectscene = rectToScene();
   scene->setSceneRect( rectscene );
    
    ApiSession *sx = ApiSession::instance();
    M_PageSize PAGE_MODEL = sx->CurrentPageFormat();
    
    
   BASE_TEXT->SwapPageModel(PAGE_MODEL);
}



void GraphicsView::keyPressEvent(QKeyEvent *e )
{
  QGraphicsView::keyPressEvent(e);
}


QRectF GraphicsView::boundingRect()
{
	return scene->sceneRect();
}

void GraphicsView::DisplayTop()
{
	horizontalScrollBar()->setValue(0); 
	verticalScrollBar()->setValue(0);
}

void GraphicsView::pageclear()
{
	  scene->clear();  /*  remove all item */
    BASE_TEXT = new TextLayer(0);
    scene->addItem(BASE_TEXT);
}

GraphicsView::~GraphicsView()
{
	pageclear();   /* clear all item */
  /* delete session */
  ////SessionManager *sx = SessionManager::instance();
  /////sx->~SessionManager();
}



Panel::Panel( QWidget *parent)
    : QFrame(parent),tievents(0),NotPaperUpdate(true)
{
		setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView(this);
	  graphicsView->setObjectName(QString("graphicsView"));
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setMinimumSize(999,508);
    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);
    
     // top menu 
    QHBoxLayout *labelLayout = new QHBoxLayout;
    openGlButton = new QToolButton;
    openGlButton->setText(tr("OpenGL"));
    openGlButton->setCheckable(true);
    
    
    labelLayout->addWidget(openGlButton);
    labelLayout->addStretch();
    
    PortraitPaper = new QComboBox;
    /////LandscapePaper = new QComboBox;
    labelLayout->addWidget(PortraitPaper);
    FillPaperSize();
    

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/img/viewzoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/img/viewzoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(400);
    zoomSlider->setValue(DefaultStartZoom);
    zoomSlider->setTickPosition(QSlider::TicksRight);
		
		resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);
		
		
		// Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    zoomSliderLayout->addWidget(resetButton);
		
		QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    setLayout(topLayout);
		connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
		connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
		connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
		connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
		///////connect(graphicsView, SIGNAL(SceneSwap()), this, SLOT(SceneChange()));
    connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
    connect(PortraitPaper, SIGNAL(currentIndexChanged(int)), this, SLOT(PaperSwap(int)));
    connect(graphicsView, SIGNAL(NewPageFormatin()),this, SLOT(FillPaperSize()));
    
    
    
   resetView();
   QTimer::singleShot(10, this, SLOT(DisplayTop()));  
   #ifndef OPENGLINITYES
   openGlButton->setChecked ( true );
   toggleOpenGL();
   #endif
    
    
}

void Panel::PaperSwap( const int index )
{
   if (NotPaperUpdate) {
    return;
   }
   const int PaperNr = PortraitPaper->itemData(index).toInt();
   ///////qDebug() << "### PaperSwap  " << index;
   ApiSession *sx = ApiSession::instance();
   QMap<int,M_PageSize> allpaper = sx->mpages();
   M_PageSize OtherFormat = allpaper[PaperNr];
   sx->SetPageFormat(OtherFormat);
   //////qDebug() << "### PaperSwap  " << OtherFormat.HName();
   /////M_PageSize current
   graphicsView->SwapPaper();
}

void Panel::FillPaperSize()
{
   NotPaperUpdate = true;
   ApiSession *sx = ApiSession::instance();
  ///////////// QMap<int,M_PageSize> allpaper = ;
   PortraitPaper->clear();
   M_PageSize activeformat = sx->CurrentPageFormat();  /* user define or other */
   /* to index */
   const QString currentname = activeformat.HName();
   int posi = -1;
   int activeindex = 0;
 
     QMapIterator<int,M_PageSize> i(sx->mpages());
         while (i.hasNext()) {
             i.next();
             posi++;
             M_PageSize current = i.value();
             if (currentname == current.HName()) {
              activeindex = posi;
             }
             PortraitPaper->addItem(QIcon(":/img/page.png"),current.HName(),i.key());
         }
    PortraitPaper->setCurrentIndex ( activeindex );
    ///////PortraitPaper = new QComboBox;
   //////////LandscapePaper = new QComboBox;
   NotPaperUpdate = false;
   graphicsView->SwapPaper();
}


void Panel::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}


void Panel::SceneChange()
{
	  tievents++;
}

void Panel::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void Panel::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void Panel::resetView()
{
 
    ApiSession *sx = ApiSession::instance();
	  const qreal Page_Width = sx->CurrentPageFormat().G_regt.width();
    zoomSlider->setValue(DefaultStartZoom);
    setupMatrix();
    ///////graphicsView->ensureVisible(QRectF(0, 0, Page_Width,10));
    resetButton->setEnabled(false);
    ////////////////graphicsView->fitInView(graphicsView->scene->sceneRect(), Qt::KeepAspectRatio);
}



void Panel::setupMatrix()
{
 
    ///////////////////qDebug() << "### setupMatrix " << zoomSlider->value();
    qreal scale = ::pow(2.0, (zoomSlider->value() - DefaultStartZoom) / 50.0);
    QMatrix matrix;
    matrix.scale(scale, scale);
    ///////matrix.rotate(10);
    graphicsView->setMatrix(matrix);
    setResetButtonEnabled();
    graphicsView->ForceResize();
}

void Panel::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}


GraphicsView *Panel::view() const
{
	return graphicsView;
}


void Panel::DisplayTop()
{
	graphicsView->DisplayTop();
}
