#include "qtextpanel.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif

QTextPanel::QTextPanel(QWidget * parent)
		: QGraphicsView(parent),onPrintRender(false)
{
	QApplication::restoreOverrideCursor();

	QPalette p = palette();
	p.setColor(QPalette::Window,Qt::lightGray);
	p.setColor(QPalette::Base,Qt::lightGray);
	setPalette(p);

	scene = new GraphicsScene(rectToScene(),this);
	setCacheMode(CacheBackground);
	setScene(scene);

	BASE_TEXT = new QTextPanelLayerControl(0);
	scene->addItem(BASE_TEXT);
	connect(scene, SIGNAL(MakeVisible(QRectF)), this, SLOT(viewDisplay(QRectF)));

}


void QTextPanel::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);

	const qreal Page_Width = QTextPanelData::instance()->CurrentPageFormat().G_regt.width();
	////////ensureVisible(QRectF(0, 0, Page_Width, Page_Width / 2));
	///////////fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

}


void QTextPanel::viewDisplay(const QRectF area)
{
	////////qDebug() << "### QTextPanel::viewDisplay -------- " << area;
	/* if scale to big return */
	QMatrix matx = matrix();
	qreal HHscaled = matx.m11();

	if (HHscaled > 1.4333333)
	{
		return;
	}

	QGraphicsView::ensureVisible(area);
}

QRectF QTextPanel::rectToScene()
{
	PanelPageSize PAGE_MODEL = QTextPanelData::instance()->CurrentPageFormat();

	/* start only one page */
	return QRectF(0,0,PAGE_MODEL.G_regt.width(),PAGE_MODEL.G_regt.height() * 8);
}

void QTextPanel::printSetup(bool printok)
{
	onPrintRender = printok;
	update();
}

void QTextPanel::swapPaper()
{
	const QRectF rectscene = rectToScene();
	PanelPageSize PAGE_MODEL = QTextPanelData::instance()->CurrentPageFormat();
	scene->setSceneRect(rectscene);
	BASE_TEXT->changePageModel(PAGE_MODEL);
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

void QTextPanel::pageClear()
{
	scene->clear();  /*  remove all item */
	BASE_TEXT = new QTextPanelLayerControl(0);
	scene->addItem(BASE_TEXT);
}

QTextPanel::~QTextPanel()
{
	pageClear();   /* clear all item */
	/* delete session */
	////SessionManager *sx = SessionManager::instance();
	/////sx->~SessionManager();
}
