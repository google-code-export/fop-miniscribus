#include "panel.h"

Panel::Panel(QWidget *parent)
		: QFrame(parent),tievents(0),NotPaperUpdate(true)
{
	setFrameStyle(Sunken | StyledPanel);
	textPanel = new QTextPanel(this);
	textPanel->setObjectName(QString("graphicsView"));
	textPanel->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	//textPanel->setFooterActive(false);

	setMinimumSize(999,508);
	int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
	QSize iconSize(size, size);

	// top menu
	QHBoxLayout *labelLayout = new QHBoxLayout;
	openGlButton = new QToolButton;
	openGlButton->setText(tr("OpenGL"));
	openGlButton->setCheckable(true);

	headerButton = new QToolButton;
	headerButton->setText(tr("Header"));
	headerButton->setCheckable(true);
	headerButton->setChecked(true);

	footerButton = new QToolButton;
	footerButton->setText(tr("Footer"));
	footerButton->setCheckable(true);
	footerButton->setChecked(true);

	labelLayout->addWidget(openGlButton);
	labelLayout->addWidget(headerButton);
	labelLayout->addWidget(footerButton);
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
	topLayout->addWidget(textPanel, 1, 0);
	topLayout->addLayout(zoomSliderLayout, 1, 1);
	setLayout(topLayout);
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
	connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
	connect(textPanel->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
	connect(textPanel->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
	connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
	connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
	///////connect(textPanel, SIGNAL(sceneSwap()), this, SLOT(SceneChange()));
	connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
	connect(headerButton, SIGNAL(toggled(bool)), textPanel, SLOT(setHeaderActive(bool)));
	connect(footerButton, SIGNAL(toggled(bool)), textPanel, SLOT(setFooterActive(bool)));
	connect(PortraitPaper, SIGNAL(currentIndexChanged(int)), this, SLOT(PaperSwap(int)));

	resetView();
	QTimer::singleShot(10, this, SLOT(displayTop()));
#ifndef Q_WS_X11
	openGlButton->setChecked(true);
	toggleOpenGL();
#endif

}

void Panel::PaperSwap(const int index)
{
	if (NotPaperUpdate)
	{
		return;
	}
	const int PaperNr = PortraitPaper->itemData(index).toInt();
	///////qDebug() << "### PaperSwap  " << index;
	QMap<int,PanelPageSize> allpaper = QTextPanelData::instance()->mpages();
	PanelPageSize OtherFormat = allpaper[PaperNr];
	QTextPanelData::instance()->SetPageFormat(OtherFormat);
	//////qDebug() << "### PaperSwap  " << OtherFormat.HName();
	/////PanelPageSize current
	textPanel->swapPaper();
}

void Panel::FillPaperSize()
{
	NotPaperUpdate = true;
	///////////// QMap<int,PanelPageSize> allpaper = ;
	PortraitPaper->clear();
	PanelPageSize activeformat = QTextPanelData::instance()->CurrentPageFormat();  /* user define or other */
	/* to index */
	const QString currentname = activeformat.HName();
	int posi = -1;
	int activeindex = 0;

	QMapIterator<int,PanelPageSize> i(QTextPanelData::instance()->mpages());
	while (i.hasNext())
	{
		i.next();
		posi++;
		PanelPageSize current = i.value();
		if (currentname == current.HName())
		{
			activeindex = posi;
		}
		PortraitPaper->addItem(QIcon(":/img/page.png"),current.HName(),i.key());
	}
	PortraitPaper->setCurrentIndex(activeindex);
	///////PortraitPaper = new QComboBox;
	//////////LandscapePaper = new QComboBox;
	NotPaperUpdate = false;
}


void Panel::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
	textPanel->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
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

	const qreal Page_Width = QTextPanelData::instance()->CurrentPageFormat().G_regt.width();
	zoomSlider->setValue(DefaultStartZoom);
	setupMatrix();
	///////textPanel->ensureVisible(QRectF(0, 0, Page_Width,10));
	resetButton->setEnabled(false);
	////////////////textPanel->fitInView(textPanel->scene->sceneRect(), Qt::KeepAspectRatio);
}



void Panel::setupMatrix()
{

	///////////////////qDebug() << "### setupMatrix " << zoomSlider->value();
	qreal scale = ::pow(2.0, (zoomSlider->value() - DefaultStartZoom) / 50.0);
	QMatrix matrix;
	matrix.scale(scale, scale);
	///////matrix.rotate(10);
	textPanel->setMatrix(matrix);
	setResetButtonEnabled();
	/////////textPanel->displayTop();
}

void Panel::setResetButtonEnabled()
{
	resetButton->setEnabled(true);
}

void Panel::displayTop()
{
	textPanel->displayTop();
}
