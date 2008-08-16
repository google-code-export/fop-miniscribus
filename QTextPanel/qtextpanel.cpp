#include "qtextpanel.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif

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
    /* load document and recalculate the first time */
    QTimer::singleShot(22, this, SLOT(forceResize()));
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
}



void QTextPanel::resizeEvent(QResizeEvent *event)
{
	scene->setSceneRect( rectToScene());
    QGraphicsView::resizeEvent(event);
}


void QTextPanel::viewDisplay(const QRectF area)
{
	/* if scale to big return */
	QMatrix matx = matrix();
	qreal HHscaled = matx.m11();
    
    qDebug() << "### Matrix m11 ->" << HHscaled;

	if (HHscaled > 1.4333333)
	{
		return;
	}

	QGraphicsView::ensureVisible(area);
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

void QTextPanel::printSetup(bool printok)
{
	onPrintRender = printok;
	update();
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



void QTextPanel::stressTestPaint()
{
    /////////pageClear();  /* remove all if exist text */
    
    
    QTextDocument *playdoc = new QTextDocument();
    QTextCursor c(playdoc);
    c.setPosition(0,QTextCursor::MoveAnchor);
    
    int loop = -1;
    c.beginEditBlock();
    
    QStringList colorNames = QColor::colorNames();
    foreach (QString name, colorNames) {
        loop++;
        
        if (loop != 0) { 
        c.insertBlock();
        }
        
        ////////qDebug() << "### name ->" << name;
        
        QPixmap e = createColorMaps( name );
        playdoc->addResource(QTextDocument::ImageResource,QUrl(name),e);
        QTextImageFormat format;
        format.setName( name );
        format.setHeight ( e.height() );
        format.setWidth ( e.width() );
        format.setToolTip(name);
        c.insertImage( format );
        c.insertText(QString(QChar::LineSeparator));   /* br */
        QTextBlockFormat bbformat = c.blockFormat();
        bbformat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);
        c.setBlockFormat(bbformat);
        
        for (int i = 0; i < 15; ++i)  {
        c.insertText(name+ "   .");   
        }
        c.endEditBlock();
        c.atBlockEnd();
        
    }
    
    setHeaderActive(false);
	setFooterActive(true);  /* to display page nr */
    BASE_TEXT->setDocument(playdoc->clone(),FOP);
    /* update all chunk */
    forceResize();
}











