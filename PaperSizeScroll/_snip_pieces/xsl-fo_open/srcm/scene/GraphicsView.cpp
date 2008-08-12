#include <math.h>
#include "GraphicsView.h"



GraphicsView::GraphicsView(  QWidget * parent )
	: QGraphicsView( parent )
{
	 QApplication::restoreOverrideCursor();
	 QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
	  setRenderHint(QPainter::Antialiasing,true);
	  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	  scene = new GraphicsScene(rectToScene(),this);
	  setCacheMode(CacheNone);
	  setScene(scene);
	  ////////////DisplayTop();
	  ///////connect(scene, SIGNAL(SelectOn(QGraphicsItem*)), this, SLOT(ActiveItem(QGraphicsItem*)));
		/////connect(scene, SIGNAL(LayerPaste(bool)), this, SLOT(PasteLayer(bool)));
		//////QTimer::singleShot(5, this, SLOT(DisplayTop()));
}




Panel::Panel(const QString &name, QWidget *parent)
    : QFrame(parent),tievents(0)
{
		setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView;
	
	  graphicsView->setObjectName(QString("view_%1").arg(name));
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

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
    topLayout->addWidget(graphicsView, 0, 0);
    topLayout->addLayout(zoomSliderLayout, 0, 1);
    setLayout(topLayout);
		connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
		connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
		connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
		connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
		connect(graphicsView, SIGNAL(SceneSwap()), this, SLOT(SceneChange()));
		
		
}

void Panel::SceneChange()
{
	tievents++;
	QGraphicsItem *it = graphicsView->Active();
	int active = 0;
	if (it) {
		active = 1;
	}
	///////qDebug() << "### SceneChange->" << active << "," << tievents;
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
	  graphicsView->DisplayTop();
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
	/////graphicsView->DisplayTop();
}






