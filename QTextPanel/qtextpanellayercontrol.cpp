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
         contextOpen(false),PageRecords(1),currentActivePager(1),currentCursorPager(1),printProcessRun(false),lastViewMatrixM11(1.00000)
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
    lastViewPortRect = QRectF();
    /*  focus first line  page */
    setFocus();
    //////
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    mouseEvent->setAccepted(true);
    mouseEvent->setPos(QPointF(1,1));
    mouseEvent->setScenePos(QPointF(1,1));
    //////mouseEvent->setScreenPos(QCursor::pos());
    //////////mouseEvent->setButtonDownPos(QCursor::pos());
    ///////////mouseEvent->setWidget(0);
    mousePressEvent(mouseEvent);
    delete mouseEvent;
    device->txtControl()->CursorMovetoPosition(QPointF(1,1));
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
            header->stopCursorBlink();
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
            footer->stopCursorBlink();
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

QStringList QTextPanelLayerControl::toolTipInfoCurrent()
{
    const int pageCount = qBound(1, document()->pageCount(), MaximumPages);
    QStringList dd;
    ///////dd.append(pageName());
    dd.append(QString("Tot.%1").arg(pageCount));
    dd.append(QString("ViewPage.%1").arg(currentActivePager));   /* current page on scroll not cursor */
    dd.append(QString("CursorPage.%1").arg(currentCursorPager));   /* current page on scroll not cursor */
    return dd;
}

void QTextPanelLayerControl::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	lastRect = device->txtControl()->boundingRect();
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::lightGray);
	painter->drawRect(lastRect);
	const int pageCount = qBound(1, document()->pageCount(), MaximumPages);
    
    if (pageCount >= MaximumPages) {
      ///////return;  /* performance from maximum pages */
    }
    

	/* draw white first background */
	for (int x = 0; x < pageCount; ++x)
	{
		painter->save();
		const QRectF pagen =  device->txtControl()->Model().PageExternal(x);
        const QLineF cursorLiner2 = device->txtControl()->ViewBlinkedCursorLine();
        if (pagen.contains(cursorLiner2.p1())) {
             currentCursorPager = x + 1;
        }
        if ( !lastViewPortRect.isNull () ) {
            if (pagen.contains(lastViewPortRect.center())) {
            currentActivePager = x + 1;
            }
        }
		painter->setBrush(QColor(Qt::white));
		painter->setPen(QPen(Qt::black,0.3));
		painter->drawRect(pagen);
		painter->restore();
		device->txtControl()->DrawPage(x,painter);
		painter->save();

        if ( !device->txtControl()->PlayCursorMode ) {
		QRectF rightShadow(pagen.right(), pagen.top() + BorderShadow, BorderShadow, pagen.height());
		QRectF bottomShadow(pagen.left() + BorderShadow, pagen.bottom(), pagen.width(), BorderShadow);
		painter->fillRect(rightShadow, Qt::darkGray);
		painter->fillRect(bottomShadow, Qt::darkGray);
        }
        
        
		/* small border */
		painter->setBrush(Qt::NoBrush);
		painter->setPen(QPen(Qt::black,0.3));
		painter->drawRect(pagen);
		painter->setPen(Qt::NoPen);

		if (x != 0 && !lastViewPortRect.isNull()  )
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
    
    if ( device->txtControl()->PlayCursorMode ) {

    if ( !lastViewPortRect.isNull () ) {
    QColor Visiblerecord(Qt::red);
	Visiblerecord.setAlpha(14);
    painter->setPen(QPen(Qt::darkMagenta,2));
	painter->setBrush(Visiblerecord);
	painter->drawRect(lastViewPortRect);
    }
    
    }
    

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



void QTextPanelLayerControl::cursorStopIt()
{
	device->txtControl()->setBlinkingCursorEnabled(false);
	sceneReload();
}


void QTextPanelLayerControl::paintManager(const QMatrix viewmat , const QRectF viewrect , bool printmodus )
{
    printProcessRun = printmodus;
    const QRectF allpagerect = boundingRect();
    QPolygonF viewareap = mapFromScene (viewrect);
    const QRectF vieportrect = allpagerect.intersected ( viewareap.boundingRect() );
    if ( lastViewPortRect.height() == vieportrect.height()  && 
         lastViewPortRect.bottom() == vieportrect.bottom() && 
         lastViewPortRect.top() == vieportrect.top()) {
    return;
    }
    lastViewPortRect = vieportrect;
    
    
    
    /* cursor blink live */
    if (!device->txtControl()->Edit_On()) {
    /* cursor sleep not having long repaint */
    return;
    }
    
    const qreal HHscaled = viewmat.m11();
    if (lastViewMatrixM11 == HHscaled) {
    /* not news!!! view port rect is saved !! */
    return;
    }
    
    lastViewMatrixM11 = HHscaled;
    bool blinkenable = false;
    
    if (printmodus) {
    device->txtControl()->setBlinkingCursorEnabled(blinkenable); 
    /* reactivate on focus int event */
    }
    /* unable to paint cursor to small view ?*/
    if (HHscaled < 0.522222222222) {
    device->txtControl()->setBlinkingCursorEnabled(blinkenable);   
    }
    qDebug() << "### QTextPanelLayerControl::paintManager  ->" << HHscaled <<  "-" << vieportrect;
    if (footer && !device->txtControl()->Edit_On())  {
      /* check if blink && deactivate */
       footer->stopCursorBlink();
    }
    
    if (header && !device->txtControl()->Edit_On()) {
     /* check if blink && deactivate */
        header->stopCursorBlink();
    }
    
    
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
