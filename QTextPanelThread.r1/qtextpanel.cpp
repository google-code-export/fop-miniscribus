#include "qtextpanel.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif

#include "qtextpaintmashine.h"


QTextPanel::QTextPanel(QWidget * parent)
		: QGraphicsView(parent),
        onPrintRender(false),BASE_TEXT(new QTextPanelLayerControl(0))
{
    /* BASE_TEXT must start at first to play the QRect from scene*/
	QApplication::restoreOverrideCursor();
	QPalette p = palette();
	p.setColor(QPalette::Window,Qt::lightGray);
	p.setColor(QPalette::Base,Qt::lightGray);
	setPalette(p);
	scene = new GraphicsScene(rectToScene(),this);    /* QRect from page tot BASE_TEXT */
	setCacheMode(CacheBackground);
	setScene(scene);
    pageClear();  /* clear all item and reinit */
}

void QTextPanel::pageClear()
{
	scene->clear();  /*  remove all item */
    BASE_TEXT = new QTextPanelLayerControl(0);
    scene->addItem(BASE_TEXT);
	connect(scene, SIGNAL(MakeVisible(QRectF)), this, SLOT(viewDisplay(QRectF)));
    connect(BASE_TEXT, SIGNAL(pageCountChange() ), this, SLOT(forceResize()));
    connect(BASE_TEXT, SIGNAL(autoCursorChange() ), this, SLOT(cursorChange()));
    
    /* load document and recalculate the first time */
    QTimer::singleShot(22, this, SLOT(forceResize()));
}

void QTextPanel::newPageInit()
{
    pageClear();
    emit sceneSwap();
}

void QTextPanel::cursorChange()
{
    emit sceneSwap();
}



QTextCursor QTextPanel::textCursor()
{
	return BASE_TEXT->textCursor();
}


QTextDocument *QTextPanel::document() const
{
	return BASE_TEXT->document();
}


/* remake qrect scene */
void QTextPanel::forceResize()
{
    scene->setSceneRect( rectToScene());
    emit newPageFormatin();
    matrixExchange();
}



void QTextPanel::resizeEvent(QResizeEvent *event)
{
	scene->setSceneRect( rectToScene());
    QGraphicsView::resizeEvent(event);
    matrixExchange();
}

void QTextPanel::setMatrix ( const QMatrix & matri, bool combine )
{
    QGraphicsView::setMatrix(matri,combine);
}


void QTextPanel::matrixExchange()
{
    const QRect viewr = viewport()->rect();
    QPolygonF realpoli = mapToScene ( viewr );
    BASE_TEXT->paintManager(matrix(),realpoli.boundingRect(),onPrintRender);
}


void QTextPanel::viewDisplay(const QRectF area)
{
	/* if scale to big return */
	QMatrix matx = matrix();
	qreal HHscaled = matx.m11();
	if (HHscaled > 1.4333333)
	{
		return;
	}
	QGraphicsView::ensureVisible(area);
    matrixExchange();
    emit sceneSwap();
}

QRectF QTextPanel::rectToScene()
{
	PanelPageSize PAGE_MODEL = QTextPanelData::instance()->CurrentPageFormat();
    const QTextDocument *bdoc = BASE_TEXT->document()->clone();
    const int PageSumm = qBound (1,bdoc->pageCount(),MaximumPages);
    /*
    if (PageSumm == MaximumPages) {
        QMessageBox::warning(this, tr("Alert.........."),
                           tr("You are try to cover maximum page %1!").arg(PageSumm));
    }
    */
    const qreal fromTopY = PageSumm * PAGE_MODEL.G_regt.height();
    const qreal spacepage = (PageSumm - 1) * InterSpace;
    return QRectF(0,0,PAGE_MODEL.G_regt.width(),fromTopY + spacepage + _BOTTOM_VIEW_SPACE_RESERVE_);
}

void QTextPanel::viewPortchange()
{
	matrixExchange();
}


void QTextPanel::printSetup( bool printok )
{
	onPrintRender = printok;
	
}

void QTextPanel::swapPaper()
{
	PanelPageSize PAGE_MODEL = QTextPanelData::instance()->CurrentPageFormat();
	BASE_TEXT->changePageModel(PAGE_MODEL);
    forceResize();  /* at end page total */
}



void QTextPanel::keyPressEvent(QKeyEvent *e)
{
	QGraphicsView::keyPressEvent(e);
}


QRectF QTextPanel::boundingRect()
{
	return scene->sceneRect();
}

void QTextPanel::displayTop()
{
	horizontalScrollBar()->setValue(0);
	verticalScrollBar()->setValue(0);
}

QTextPanel::~QTextPanel()
{
	pageClear();
}

void QTextPanel::insert( RichDoc e )
{
    BASE_TEXT->setDocument(e.todoc()->clone(),FOP);
    forceResize();
}

void QTextPanel::stressTestPaint()
{
    
    PanelPageSize PAGE_MODEL = QTextPanelData::instance()->CurrentPageFormat();
    MountPage ddPage;
    ddPage.model = PAGE_MODEL;
    ComposeDoc *djob = new ComposeDoc(this,ddPage);
    djob->start (QThread::HighPriority);
    ////////qRegisterMetaType<RichDoc>("RichDoc");
    ////////connect(djob, SIGNAL(generator(RichDoc)), this, SLOT(insert(RichDoc)));
}











