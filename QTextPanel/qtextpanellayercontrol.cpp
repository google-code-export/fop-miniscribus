#include "qtextpanellayercontrol.h"


TextMount::TextMount() : device(0)
{

}

/* only one connect */
ScribePage *TextMount::txtControl() const
{
	if (!device)
	{
		PanelPageSize A4;
		QTextPanelLayerControl *that = const_cast<QTextPanelLayerControl *>(q);
		device = new ScribePage(A4);
		//////////qDebug() << "### init  api txt ...........................................................................";
		connect(device, SIGNAL(q_cursor_newPos()),q, SLOT(cursorWakeUp()));
		connect(device, SIGNAL(q_update(QRect)),q, SLOT(updateArea(QRect)));
		connect(device, SIGNAL(q_visible(QRectF)),q, SLOT(ensureVisible(QRectF)));
		connect(device, SIGNAL(q_update_scene()),q, SLOT(sceneReload()));
	}

	return device;
}

QTextPanelLayerControl::~QTextPanelLayerControl()
{
	//~ qDebug() << "### destroy obj ...";
}



QTextPanelLayerControl::QTextPanelLayerControl(QGraphicsItem *parent)
		: QGraphicsRectItem(QRectF(0,0,100,100),parent),device(new TextMount),
         contextOpen(false),PageRecords(1)
{
	//////qDebug() << "### init....";
	device->q = this;
	header = footer = NULL;

	setAcceptsHoverEvents(true);
	setAcceptDrops(true);

	headerActive = footerActive = false;

	QTextDocument *dummy = new QTextDocument();
	setDocument(dummy,FOP);
	QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable);
	setFlag(QGraphicsItem::ItemIsMovable, false);
	setZValue(0.555555);

	lastRect = device->txtControl()->boundingRect();
	QGraphicsRectItem::setRect(lastRect);
	lastUpdateRequest = lastRect;
	setupHeaderFooter();
}

void QTextPanelLayerControl::setupHeaderFooter()
{
	setHeaderActive(true);
	setFooterActive(true);
}

void QTextPanelLayerControl::setHeaderActive(bool active)
{
	headerActive = active;

	if (active)
	{
		if (header == NULL)
		{
			header = new AbsoluteLayer(this, DIV_HEADER);
			QTextDocument *dummy = new QTextDocument();

			dummy->setHtml("<p>Header and Logo.</p>");
			header->setDocument(dummy, FOP);

			connect(header, SIGNAL(close_main_cursor()),this, SLOT(cursorStopIt()));
			connect(header, SIGNAL(pagesize_swap()),this, SLOT(pageSizeReload()));

			header->UpdatePageFormat();
		}
	}
	else
	{
		if (header)
		{
			GraphicsScene *sc;
			if (sc = qobject_cast<GraphicsScene *>(scene()))
			{
				sc->removeItem(header);
			}

			QTextDocument *dummy = header->document();
			disconnect(header, 0, 0, 0);
			header->deleteLater();
			dummy->deleteLater();

			pageSizeReload();

			header = NULL;
		}
	}

	sceneReload();
}

void QTextPanelLayerControl::setFooterActive(bool active)
{
	footerActive = active;

	if (active)
	{
		if (footer == NULL)
		{
			footer = new AbsoluteLayer(this, DIV_FOOTER);

			QTextDocument *dummy = new QTextDocument();

			dummy->setHtml("<p>Footer Page " + _PAGE_NUMERATION_ + "</p>");

			footer->setDocument(dummy, FOP);

			connect(footer, SIGNAL(close_main_cursor()),this, SLOT(cursorStopIt()));
			connect(footer, SIGNAL(pagesize_swap()),this, SLOT(pageSizeReload()));

			footer->UpdatePageFormat();
		}
	}
	else
	{
		if (footer)
		{
			GraphicsScene *sc;
			if (sc = qobject_cast<GraphicsScene *>(scene()))
			{
				sc->removeItem(footer);
			}

			QTextDocument *dummy = footer->document();
			disconnect(footer, 0, 0, 0);

			footer->deleteLater();
			dummy->deleteLater();

			pageSizeReload();

			footer = NULL;
		}
	}

	sceneReload();
}

void QTextPanelLayerControl::appendLayer()
{
	QTextDocument *dummy = new QTextDocument();
	dummy->setHtml("<p>Your text.</p>");

	QLineF cursorpos = device->txtControl()->ViewBlinkedCursorLine();

	AbsoluteLayer *absolute = new AbsoluteLayer(this,DIV_ABSOLUTE);
	absolute->setPos(cursorpos.p1());
	absolute->setDocument(dummy,FOP);

	connect(absolute, SIGNAL(close_main_cursor()),this, SLOT(cursorStopIt()));
	connect(absolute, SIGNAL(pagesize_swap()),this, SLOT(pageSizeReload()));
}


QTextCursor QTextPanelLayerControl::textCursor()
{
	return device->txtControl()->textCursor();
}

void QTextPanelLayerControl::changePageModel(PanelPageSize e)
{
	device->txtControl()->SwapPageModel(e);

	/* double check :) */
	if (headerActive && header)
	{
		header->UpdatePageFormat();
	}

	if (footerActive && footer)
	{
		footer->UpdatePageFormat();
	}

	sceneReload();
}


QTextDocument *QTextPanelLayerControl::document()
{
	return device->txtControl()->document();
}



void QTextPanelLayerControl::setDocument(const QTextDocument * document , FileHandlerType Type)
{
	device->txtControl()->setDocument(document,Type);
}

void QTextPanelLayerControl::updateArea(const QRect areas)
{
	lastRect = device->txtControl()->boundingRect();
	QGraphicsRectItem::setRect(lastRect);
	////////////qDebug() << "### updateArea " << areas;
	lastUpdateRequest = areas;
	update(areas);
}



void QTextPanelLayerControl::cursorStopIt()
{
	device->txtControl()->setBlinkingCursorEnabled(false);
	sceneReload();
}

void QTextPanelLayerControl::pageSizeReload()
{
	changePageModel(QTextPanelData::instance()->CurrentPageFormat());
}


void QTextPanelLayerControl::cursorWakeUp()
{
	const int PageCount = qBound(1, document()->pageCount(), MaximumPages);

    if (PageRecords != PageCount)
    {
		PageRecords = PageCount;
		sceneReload();
		emit pageCountChange();
    }

    emit autoCursorChange();
}


void QTextPanelLayerControl::sceneReload()
{
	/////////qDebug() << "### sceneReload page count " << document()->pageCount();
	GraphicsScene *sc;
	if (sc = qobject_cast<GraphicsScene *>(scene()))
	{
		sc->clearSelection();
	}
}

void QTextPanelLayerControl::ensureVisible(const QRectF areas)
{
	lastVisibleRequest = areas;
	GraphicsScene *sc;
	if (sc = qobject_cast<GraphicsScene *>(scene()))
	{
		sc->SetVisibleArea(areas);
	}
}

QString QTextPanelLayerControl::pageName()
{
	return device->txtControl()->Model().HName();
}

void QTextPanelLayerControl::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	lastRect = device->txtControl()->boundingRect();
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::lightGray);
	painter->drawRect(lastRect);
	const int pageCount = qBound(1, document()->pageCount(), MaximumPages);

	/* draw white first background */
	for (int x = 0; x < pageCount; ++x)
	{
		painter->save();
		const QRectF pagen =  device->txtControl()->Model().PageExternal(x);
		painter->setBrush(QColor(Qt::white));
		painter->setPen(QPen(Qt::black,0.3));
		painter->drawRect(pagen);
		painter->restore();
		device->txtControl()->DrawPage(x, painter, x);
		painter->save();

		QRectF rightShadow(pagen.right(), pagen.top() + BorderShadow, BorderShadow, pagen.height());
		QRectF bottomShadow(pagen.left() + BorderShadow, pagen.bottom(), pagen.width(), BorderShadow);
		painter->fillRect(rightShadow, Qt::darkGray);
		painter->fillRect(bottomShadow, Qt::darkGray);
		/* small border */
		painter->setBrush(Qt::NoBrush);
		painter->setPen(QPen(Qt::black,0.3));
		painter->drawRect(pagen);
		painter->setPen(Qt::NoPen);

		if (x != 0)
		{
			/* only draws header/footer if they are active */
			if (headerActive && header)
			{
				//////qDebug() << "header" << endl;
				/* draw  header */
				QImage headerPicture = header->LayerImage(x);
				QPointF headerPosition = device->txtControl()->Model().HeaderInitPoints(x);
				painter->drawImage(headerPosition, headerPicture);
			}

			if (footerActive && footer)
			{
				//////qDebug() << "footer" << endl;
				/* draw footer */
				QImage footerPicture = footer->LayerImage(x);
				QPointF footerPosition = device->txtControl()->Model().FooterInitPoints(x);
				painter->drawImage(footerPosition, footerPicture);
			}
		}

		painter->restore();
	}

	/*
	QColor Visiblerecord(Qt::red);
	Visiblerecord.setAlpha(22);
	painter->setBrush(Visiblerecord);
	painter->drawRect(lastUpdateRequest);
	*/



	/*
	 QColor BackHightlight("#a6ffc7");
	BackHightlight.setAlpha(80);

	 painter->setPen( Qt::NoPen );
	 painter->setBrush(BackHightlight);
	painter->drawRect(lastUpdateRequest);



	 QColor Visiblerecord(Qt::red);
	Visiblerecord.setAlpha(45);

	 if (!lastVisibleRequest.isNull()) {
	 painter->setBrush(Visiblerecord);
	painter->drawRect(lastVisibleRequest);
	 }
	 */

}

void QTextPanelLayerControl::focusInEvent(QFocusEvent * event)
{
	//~ qDebug() << "### QTextPanelLayerControl focusInEvent ..." << flags();
	QGraphicsItem::setSelected(true);
    emit pageCountChange();
	scene()->setFocusItem(this,Qt::ShortcutFocusReason);
	device->txtControl()->setBlinkingCursorEnabled(true);
	///////return QGraphicsItem::focusInEvent(event);
}

void QTextPanelLayerControl::focusOutEvent(QFocusEvent * event)
{
	//~ qDebug() << "### QTextPanelLayerControl focusOutEvent ...";
	QGraphicsItem::setSelected(false);
	device->txtControl()->setBlinkingCursorEnabled(false);
	//////return QGraphicsItem::focusOutEvent(event);
}

void QTextPanelLayerControl::inputMethodEvent(QInputMethodEvent * event)
{
	///////////qDebug() << "### inputMethodEvent ...";
	return QGraphicsItem::inputMethodEvent(event);
}


QRectF QTextPanelLayerControl::boundingRect() const
{
	return device->txtControl()->GroupboundingRect();  /* shadow */
}

void QTextPanelLayerControl::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

	if (device->txtControl()->AllowedPosition(event->pos()) && event->button() == Qt::LeftButton)
	{
		/////////// qDebug() << "###  mouseDoubleClickEvent... ";
		if (device->txtControl()->procesevent(event))
		{
            cursorWakeUp();
			return;
		}
	}


	return QGraphicsItem::mouseDoubleClickEvent(event);
}


void QTextPanelLayerControl::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	///////////qDebug() << "### layer mouseMoveEvent a.. " << event->button() ;  /* no button capture !!!! */

	if (device->txtControl()->AllowedPosition(event->pos()))
	{
		//////////// qDebug() << "### layer mouseMoveEvent b .. ";
		if (device->txtControl()->procesevent(event))
		{
            cursorWakeUp();
			return;
		}
	}
	return QGraphicsItem::mouseMoveEvent(event);
}

void QTextPanelLayerControl::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{


	if (device->txtControl()->AllowedPosition(event->pos()) && event->button() == Qt::LeftButton)
	{
		////////////qDebug() << "### layer mouseReleaseEvent.. left .";
		if (device->txtControl()->procesevent(event))
		{
            cursorWakeUp();
			return;
		}
	}

	return QGraphicsItem::mouseReleaseEvent(event);
}

void QTextPanelLayerControl::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	if (device->txtControl()->AllowedPosition(event->pos()) && event->button() == Qt::LeftButton)
	{

		//////////////qDebug() << "### layer mousePressEvent left ...";

		if (device->txtControl()->procesevent(event))
		{
            cursorWakeUp();
			return;
		}
	}


	return QGraphicsItem::mousePressEvent(event);
}

void QTextPanelLayerControl::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	/////// qDebug() << "### hoverEnterEvent...";
	return QGraphicsItem::hoverEnterEvent(event);
}

void QTextPanelLayerControl::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	//////////qDebug() << "### hoverLeaveEvent...";
	return QGraphicsItem::hoverLeaveEvent(event);
}

void QTextPanelLayerControl::keyPressEvent(QKeyEvent * event)
{
	/////////qDebug() << "### keyPressEvent...";
    cursorWakeUp();
	return device->txtControl()->Controller_keyPressEvent(event);
	/////return QGraphicsItem::keyPressEvent(event);
}

void QTextPanelLayerControl::keyReleaseEvent(QKeyEvent * event)
{
	/////////////qDebug() << "### keyReleaseEvent...";
	return device->txtControl()->Controller_keyReleaseEvent(event);
	//////////return QGraphicsItem::keyReleaseEvent(event);
}


bool QTextPanelLayerControl::sceneEvent(QEvent *event)
{
	if (event->type() == QEvent::GraphicsSceneDragEnter ||
	      event->type() == QEvent::GraphicsSceneDrop ||
	      event->type() == QEvent::GraphicsSceneDragLeave ||
	      event->type() == QEvent::GraphicsSceneDragMove)
	{

		device->txtControl()->procesevent(event);
        cursorWakeUp();
		return true;
	}
	/* buggi events */
	///////if (event->type() == QEvent::UngrabMouse ||
	//////event->type() == QEvent::GrabMouse) {
	//////return true;
	//////}
	return QGraphicsItem::sceneEvent(event);
}

/*
void QTextPanelLayerControl::deleteSelected()
{
   device->txtControl()->deleteSelected();
}

void QTextPanelLayerControl::cut()
{
   device->txtControl()->cut();
}
void QTextPanelLayerControl::paste()
{
    device->txtControl()->paste();
}
void QTextPanelLayerControl::copy()
{
    device->txtControl()->copy();
}
void QTextPanelLayerControl::undo()
{
    device->txtControl()->undo();
}
void QTextPanelLayerControl::showhtml()
{
    device->txtControl()->showhtml();
}
void QTextPanelLayerControl::redo()
{
    device->txtControl()->redo();
}
void QTextPanelLayerControl::selectAll()
{
   textCursor().select(QTextCursor::Document);
  sceneReload();
}
void QTextPanelLayerControl::InsertImageonCursor()
{
    device->txtControl()->InsertImageonCursor();
}

*/
