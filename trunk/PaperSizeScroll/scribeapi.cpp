#include "scribeapi.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif

GraphicsView::GraphicsView( QWidget * parent )
	: QGraphicsView( parent ),OnPrintRender(false)
{
	 QApplication::restoreOverrideCursor();
   QPalette p = palette();
   p.setColor(QPalette::Window,Qt::lightGray);
   p.setColor(QPalette::Base,Qt::lightGray);
   setPalette(p);
   scene = new GraphicsScene(rectToScene(),this);
   setCacheMode(CacheBackground);
   setScene(scene);
    
   BASE_TEXT = new TextLayer(0);
   scene->addItem(BASE_TEXT);
   connect(scene, SIGNAL(MakeVisible(QRectF) ), this, SLOT(ViewDisplay(QRectF)));
    
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
    /* start only one page */
    return QRectF(0,0,PAGE_MODEL.G_regt.width(),PAGE_MODEL.G_regt.height() * 4);
}

void GraphicsView::PrintSetup( bool printok )
{
    OnPrintRender = printok;
    update();
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
}

GraphicsView::~GraphicsView()
{
	pageclear();   /* clear all item */
  /* delete session */
  ////SessionManager *sx = SessionManager::instance();
  /////sx->~SessionManager();
}



Panel::Panel( QWidget *parent)
    : QFrame(parent),tievents(0)
{
		setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView(this);
	  graphicsView->setObjectName(QString("graphicsView"));
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);
    
     // top menu 
    QHBoxLayout *labelLayout = new QHBoxLayout;
    openGlButton = new QToolButton;
    openGlButton->setText(tr("OpenGL"));
    openGlButton->setCheckable(true);
    openGlButton->setChecked ( true );
    
    labelLayout->addWidget(openGlButton);
    labelLayout->addStretch();
    
    
    
    

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
    zoomSlider->setMaximum(300);
    zoomSlider->setValue(150);
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
		
		QTimer::singleShot(10, this, SLOT(DisplayTop()));  
    toggleOpenGL();
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
    zoomSlider->setValue(150);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));
    resetButton->setEnabled(false);
}



void Panel::setupMatrix()
{
    qreal scale = ::pow(2.0, (zoomSlider->value() - 150) / 50.0);
    QMatrix matrix;
    matrix.scale(scale, scale);
    ///////matrix.rotate(10);
    graphicsView->setMatrix(matrix);
    setResetButtonEnabled();
	  /////////graphicsView->DisplayTop();
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
