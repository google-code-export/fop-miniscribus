#include "qtextpanellayer.h"

AbsoluteLayer::~AbsoluteLayer()
{
	//~ qDebug() << "### destroy obj ...";
}



AbsoluteLayer::AbsoluteLayer(QGraphicsItem *parent , LAYERTYPE layermodus)
		: QGraphicsRectItem(parent),dev(new AbsText),Rotate(0),OnMoveRects(false),id(10)
{
	dev->q = this;
	layermods = layermodus;
	Background_Color = QColor(0,0,0,44);
	Border_Color = QColor(Qt::white);
	Border_Color_t = QColor(Qt::white);
	Border_Color_b = QColor(Qt::white);
	Border_Color_l = QColor(Qt::white);
	Border_Color_r = QColor(Qt::white);

	_border_top = 0.;
	_border_bottom = 0.;
	_border_left = 0.;
	_border_right = 0.;

	/////////qDebug() << "### init....";
	////const qreal widhtinit = QTextPanelData::instance()->CurrentPageFormat().G_regt.width() - ( FooterHeaderPadding * 2 );
	//////const qreal hightinit = QTextPanelData::instance()->CurrentPageFormat().P_margin.x() - ( FooterHeaderPadding * 2 );
	setAcceptsHoverEvents(true);
	setAcceptDrops(true);
	QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable);
	setFlag(QGraphicsItem::ItemIsMovable,false);

	if (layermodus == DIV_HEADER | layermodus == DIV_FOOTER)
	{
		setZValue(1.5);
		if (layermodus == DIV_HEADER)
		{
			setPos(QTextPanelData::instance()->CurrentPageFormat().HeaderInitPoints(0));
			setRect(QTextPanelData::instance()->CurrentPageFormat().HeaderBoundingrect());
		}
		if (layermodus == DIV_FOOTER)
		{
			setPos(QTextPanelData::instance()->CurrentPageFormat().FooterInitPoints(0));
			setRect(QTextPanelData::instance()->CurrentPageFormat().FooterBoundingrect());
		}
	}
	else
	{
		setZValue(2.5);
		setRect(QRectF(0,0,200,23));
	}

	Angle_1 = new FWButton(this,Qt::green,tr("Move Layer"));
	Angle_2 = new FWButton(this,Qt::green,tr("Rotate layer"));
	Angle_4 = new FWButton(this,Qt::green,tr("Resize layer"));

	if (layermodus == DIV_ABSOLUTE)
	{
		Angle_1->allow(true);
		Angle_4->allow(true);
		Angle_2->allow(true);
		connect(Angle_1, SIGNAL(dragging(const QPointF&)), this, SLOT(slotModpos_1(const QPointF&)));
		connect(Angle_4, SIGNAL(dragging(const QPointF&)), this, SLOT(slotResize_1(const QPointF&)));
		connect(Angle_2, SIGNAL(dragging(const QPointF&)), this, SLOT(slotRotate_1(const QPointF&)));
		connect(Angle_1, SIGNAL(operate(bool)), this, SLOT(MoveActions(bool)));
		connect(Angle_2, SIGNAL(operate(bool)), this, SLOT(MoveActions(bool)));
		connect(Angle_4, SIGNAL(operate(bool)), this, SLOT(MoveActions(bool)));
	}

	lastUpdateRequest = rect();
	QTextDocument *dummy = new QTextDocument();
	dummy->setHtml("<p></p>");
	setDocument(dummy,FOP);
	dev->txtControl()->SetRect(rect());
	UpdateDots();
}


void AbsoluteLayer::setDocument(const QTextDocument *doc, FileHandlerType Type)
{
	dev->txtControl()->setDocument(doc, Type);
	dev->txtControl()->SetRect(rect());
	if (layermods == DIV_HEADER | layermods == DIV_FOOTER)
	{
		document()->setMaximumBlockCount(FooterHeaderMaxBlocks);
	}
}

QTextCursor AbsoluteLayer::textCursor()
{
	return dev->txtControl()->textCursor();
}

QTextDocument *AbsoluteLayer::document()
{
	return dev->txtControl()->document();
}


void AbsoluteLayer::UpdatePageFormat()
{
	if (layermods == DIV_HEADER | layermods == DIV_FOOTER)
	{
		if (layermods == DIV_HEADER)
		{
			setPos(QTextPanelData::instance()->CurrentPageFormat().HeaderInitPoints(0));
			setRect(QTextPanelData::instance()->CurrentPageFormat().HeaderBoundingrect());
		}
		if (layermods == DIV_FOOTER)
		{
			setPos(QTextPanelData::instance()->CurrentPageFormat().FooterInitPoints(0));
			setRect(QTextPanelData::instance()->CurrentPageFormat().FooterBoundingrect());
		}

	}

	updateArea(boundingRect().toRect());
	UpdateDots();
}

void AbsoluteLayer::UpdateDots()
{
	Angle_1->setPos(boundingRect().topLeft());
	Angle_4->setPos(boundingRect().bottomRight());
	Angle_2->setPos(boundingRect().topRight());

}


void AbsoluteLayer::updateArea(const QRect areas)
{
	lastUpdateRequest = areas;
	////////qDebug() << "### updateArea " << areas;
	const qreal anspace = FooterHeaderPadding * 2;
	const QRectF txtrect = dev->txtControl()->boundingRect();

	if (rect().height() < txtrect.height())
	{
		setRect(QRectF(0,0,rect().width(),txtrect.height()));
	}

	if (layermods == DIV_HEADER | layermods == DIV_FOOTER)
	{
		if (txtrect.height() != rect().height())
		{
			setRect(QRectF(0,0,rect().width(),txtrect.height()));
			const QRectF mold = QTextPanelData::instance()->CurrentPageFormat().P_margin;
			//////QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
			if (layermods == DIV_HEADER)
			{
				QTextPanelData::instance()->current_Page_Format.SetMargin(QRectF(txtrect.height() + anspace ,mold.y(),mold.width(),mold.height()));
			}
			if (layermods == DIV_FOOTER)
			{
				QTextPanelData::instance()->current_Page_Format.SetMargin(QRectF(mold.x(),mold.y(),txtrect.height() + anspace ,mold.height()));
				setPos(QTextPanelData::instance()->CurrentPageFormat().FooterInitPoints(0));
			}

			emit pagesize_swap();
		}
	}

	UpdateDots();
	update(areas);
}

void AbsoluteLayer::slotRotate_1(const QPointF posi)
{
	QPointF newPos = mapFromScene(posi);
	QPointF refPos = Angle_2->pos();
	if (newPos == refPos)
	{
		return;
	}
	QLineF newangles(boundingRect().topLeft(),newPos);
	newangles.setLength(boundingRect().width());
	Rotate = 360 - newangles.angle();
	if (Rotate > 359.5 | Rotate < 0.9)
	{
		Rotate = 0;
	}
	//////////qDebug() << "### Rotate " << Rotate;


	dev->txtControl()->SetRect(rect());
	UpdateDots();
	update();
}


QLineF AbsoluteLayer::LineTops()
{
	return transform().map(QLineF(0,0,rect().width(),0));
}

QLineF AbsoluteLayer::Diagonal()
{
	return transform().map(QLineF(QPointF(0,0),boundingRect().bottomRight()));
}


////////////////QPointF ( qreal x, qreal y )

void AbsoluteLayer::slotModpos_1(const QPointF posi)
{
	QPointF newPos = mapFromScene(posi);
	QPointF refPos = Angle_1->pos();
	if (newPos == refPos)
	{
		return;
	}
	QTransform trap = transform();
	QRectF truebound = trap.mapRect(absoluteRect());
	QPointF buttonrelative = mapFromItem(Angle_1 ,  posi);
	setPos(buttonrelative - LineTops().p1());
	UpdateDots();
}

void AbsoluteLayer::slotResize_1(const QPointF posi)
{
	QPointF newPos = mapFromScene(posi);
	QPointF refPos = Angle_4->pos();
	if (newPos == refPos)
	{
		return;
	}
	qreal Altominimo = MinimumHightLayer;
	/////if (device->txtControl()->boundingRect().height() < Altominimo) {
	Altominimo = dev->txtControl()->boundingRect().height();
	///////}


	QPointF buttonrelative = mapFromItem(Angle_4 ,  posi) ;
	QPointF tl = Diagonal().p1();
	const qreal largo = qAbs(pos().rx() +  boundingRect().width()  - buttonrelative.x()  - tl.x());
	const qreal alto = qAbs(pos().ry() +  boundingRect().height()  - buttonrelative.y() - tl.y());
	setRect(QRectF(0,0,qBound(MinimumWhidhLayer,largo,MAXLargoTmp),qBound(Altominimo,alto,MAXLargoTmp)));
	UpdateDots();
	dev->txtControl()->SetRect(rect());

}

void AbsoluteLayer::MoveActions(bool e)
{
	OnMoveRects = e;
	//////qDebug() << "### MoveActions " << e;
	UpdateDots();
	if (!e)
	{
		///////device->txtControl()->setBlinkingCursorEnabled(true);
		update(boundingRect());
	}
	else
	{
		dev->txtControl()->SetRect(rect());
	}
	ShowInfos();
}


void AbsoluteLayer::ShowInfos()
{

	QString infoPart = QString("Zindex %1  ID = %2 Rotate =%3 ").arg(zValue()).arg(id).arg(Rotate);
	infoPart.append(QString("\n[Document width = %1pt]\n").arg(document()->size().width()));
	setToolTip(QString("Layer %1pt x %2pt X=%4 pt Y=%3 pt %5")
	           .arg(toUnit(rect().width(),"pt"))
	           .arg(toUnit(rect().height(),"pt"))
	           .arg(toUnit(pos().y(),"pt"))
	           .arg(toUnit(pos().x(),"pt")).arg(infoPart));

}

QPicture AbsoluteLayer::LayerImage(const int pageNumber)
{
	QPicture img;
	img.setBoundingRect(rect().toRect());
	int pagefollow = pageNumber + 1;
	QTextDocument * doc = document()->clone();

	for (QTextBlock srcBlock = document()->firstBlock(), dstBlock = doc->firstBlock();
	      srcBlock.isValid() && dstBlock.isValid();
	      srcBlock = srcBlock.next(), dstBlock = dstBlock.next())
	{
		dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
	}

	QTextCursor cursor(doc);
	cursor.setPosition(0,QTextCursor::MoveAnchor);

	/* search  _PAGE_NUMERATION_   #Page#  and replace nr. */
	QTextCursor cursor2 = doc->find(_PAGE_NUMERATION_, cursor, QTextDocument::FindWholeWords);
	if (!cursor2.isNull())
	{
		if (cursor2.hasSelection())
		{
			QTextCharFormat format = cursor2.charFormat();
			QString remtxt = cursor2.selectedText();
			for (int i = 0; i < remtxt.size(); ++i)
			{
				cursor2.deleteChar();
			}
			cursor2.insertText(QString("%1").arg(pagefollow),format);
			cursor2.clearSelection();
		}
	}


	QTextFrame  *Tframe = doc->rootFrame();
	const QRectF stxt = doc->documentLayout()->frameBoundingRect(Tframe);

	QPainter painter;
	painter.begin(&img);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setBrush(Background_Color);
	painter.setPen(Qt::NoPen);
	painter.drawRect(rect());

	QAbstractTextDocumentLayout::PaintContext CTX;
	painter.setClipRect(stxt);
	CTX.cursorPosition = -1;
	CTX.clip = stxt;
	doc->documentLayout()->draw(&painter,CTX);
	painter.end();

	return img;
}



void AbsoluteLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::TextAntialiasing);
	painter->setBrush(Background_Color);
	painter->setPen(Qt::NoPen);
	painter->drawRect(rect());
	const qreal hi = rect().height();
	const qreal wi = rect().width();
	QRectF pagen = rect();

	dev->txtControl()->SetRect(rect());
	dev->txtControl()->paint(painter,option,widget);

	const qreal border = 0.5;
	const QRectF  BoRect(-border,-border,wi + (border * 2) , hi + (border * 2));

	if (layermods == DIV_ABSOLUTE)
	{
		if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus)  | OnMoveRects)
		{
			painter->setPen(QPen(Qt::black ,0.5));
			painter->setBrush(Qt::NoBrush);
			painter->drawRect(BoRect);
		}
	}
	else
	{
		if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus)  | OnMoveRects)
		{
			painter->setPen(QPen(Qt::lightGray ,0.5));   /////Qt::DotLine
			painter->setBrush(Qt::NoBrush);
			painter->drawRect(BoRect);
		}

	}


	if (OnMoveRects)
	{
		painter->setOpacity(0.8);
		const qreal fullliwid = 1.5;
		const qreal limid = fullliwid / 2;
		//////painter->setPen( QPen( Qt::green ,fullliwid));

		QRectF rightShadow(pagen.right(), pagen.top() + fullliwid + limid, fullliwid + limid , pagen.height());
		QRectF bottomShadow(pagen.left() + fullliwid + limid, pagen.bottom(), pagen.width(), fullliwid + limid);
		painter->fillRect(rightShadow, Qt::green);
		painter->fillRect(bottomShadow, Qt::green);


		painter->setOpacity(1.0);
	}


	/*
	painter->setPen(Qt::NoPen);
	QColor Visiblerecord(Qt::red);
	Visiblerecord.setAlpha(22);
	painter->setBrush(Visiblerecord);
	painter->drawRect(lastUpdateRequest);
	*/

	QMatrix matrix;
	matrix.translate(boundingRect().center().x() , boundingRect().center().y());
	matrix.rotate(Rotate);
	matrix.translate(- boundingRect().center().x() , - boundingRect().center().y());

	setTransform(QTransform(matrix),false);
}

void AbsoluteLayer::focusInEvent(QFocusEvent * event)
{
	/////////qDebug() << "### AbsoluteLayer focusInEvent ..." << flags();
	QGraphicsItem::setSelected(true);
	scene()->setFocusItem(this,Qt::ShortcutFocusReason);
	dev->txtControl()->setBlinkingCursorEnabled(true);
	emit close_main_cursor();
	return QGraphicsItem::focusInEvent(event);
}

void AbsoluteLayer::focusOutEvent(QFocusEvent * event)
{
	///////////qDebug() << "### AbsoluteLayer focusOutEvent ...";
	QGraphicsItem::setSelected(false);
	dev->txtControl()->setBlinkingCursorEnabled(false);
	return QGraphicsItem::focusOutEvent(event);
}

void AbsoluteLayer::inputMethodEvent(QInputMethodEvent * event)
{
	//~ qDebug() << "### inputMethodEvent ...";
	return QGraphicsItem::inputMethodEvent(event);
}


QRectF AbsoluteLayer::boundingRect() const
{
	return rect();
}

QRectF AbsoluteLayer::absoluteRect()
{
	return QRectF(pos(),boundingRect().size());
}





void AbsoluteLayer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

	if (event->button() != Qt::LeftButton)
	{
		return;
	}

	if (dev->txtControl()->procesevent(event))
	{
		return;
	}

	//~ qDebug() << "### mouseDoubleClickEvent...";
	return QGraphicsItem::mouseDoubleClickEvent(event);
}


void AbsoluteLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{


	if (dev->txtControl()->procesevent(event))
	{
		return;
	}

	//~ qDebug() << "### mouseMoveEvent.. ";
	return QGraphicsItem::mouseMoveEvent(event);

}

void AbsoluteLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

	if (event->button() != Qt::LeftButton)
	{
		return;
	}

	if (dev->txtControl()->procesevent(event))
	{
		return;
	}


	//~ qDebug() << "### mouseReleaseEvent...";
	return QGraphicsItem::mouseReleaseEvent(event);
}

void AbsoluteLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!dev->txtControl()->Edit_On())
	{
		dev->txtControl()->setBlinkingCursorEnabled(true);
		emit close_main_cursor();
	}

	if (event->button() != Qt::LeftButton)
	{
		return;
	}

	if (dev->txtControl()->procesevent(event))
	{
		return;
	}


	//~ qDebug() << "### mousePressEvent...";
	return QGraphicsItem::mousePressEvent(event);
}

void AbsoluteLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	//~ qDebug() << "### hoverEnterEvent...";
	return QGraphicsItem::hoverEnterEvent(event);
}

void AbsoluteLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	//~ qDebug() << "### hoverLeaveEvent...";
	dev->txtControl()->setBlinkingCursorEnabled(false);
	return QGraphicsItem::hoverLeaveEvent(event);
}

void AbsoluteLayer::keyPressEvent(QKeyEvent * event)
{

	if (layermods == DIV_ABSOLUTE)
	{
		if ((event->modifiers() & Qt::AltModifier) && event->key() == Qt::Key_Up)
		{
			seTFront();
		}
		if ((event->modifiers() & Qt::AltModifier) && event->key() == Qt::Key_Down)
		{
			seTBack();
		}
	}

	return dev->txtControl()->Controller_keyPressEvent(event);
	//~ qDebug() << "### keyPressEvent...";
	return QGraphicsItem::keyPressEvent(event);
}

void AbsoluteLayer::keyReleaseEvent(QKeyEvent * event)
{
	return dev->txtControl()->Controller_keyReleaseEvent(event);
	//~ qDebug() << "### keyReleaseEvent...";
	return QGraphicsItem::keyReleaseEvent(event);
}

bool AbsoluteLayer::sceneEvent(QEvent *event)
{
	/////////qDebug() << "### sceneEvent.... + type   " << event->type();
	return QGraphicsItem::sceneEvent(event);
}

void AbsoluteLayer::BackGroundColor()
{
	QRgb col = QColorDialog::getRgba(Background_Color.rgba());
	QColor col2 = QColor(col);
	int trans = qAlpha(col);
	col2.setAlpha(trans);
	if (!col2.isValid())
	{
		return;
	}
	Background_Color = col2;

	QTextFrame  *Tframe = document()->rootFrame();
	QTextFrameFormat Ftf = Tframe->frameFormat();
	Ftf.setBackground(QBrush(Qt::transparent));
	Tframe->setFrameFormat(Ftf);
	update();
}




void AbsoluteLayer::seTBack()
{
	if (layermods != DIV_ABSOLUTE)
	{
		return;
	}
	qreal maxi = 999.9;
	qreal minimums = 1.5;

	GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
	qreal backs = qBound(minimums,sc->zmin() - 0.1,maxi);
	setZValue(backs);

	//~ qDebug() << "### seTBack" << backs;

	update();
	emit pagesize_swap();

}


void AbsoluteLayer::seTFront()
{
	if (layermods != DIV_ABSOLUTE)
	{
		return;
	}

	qreal maxi = 999.9;
	qreal minimums = 1.5;

	GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
	qreal top = qBound(minimums,sc->zmax() + 0.1,maxi);
	top++;
	setZValue(top);

	//~ qDebug() << "### seTFront " << top;

	update();
	emit pagesize_swap();
}



void AbsoluteLayer::RotateLayer(const int ro)
{
	Rotate = ro;
	////////////qDebug() << "### RotateLayer " << ro;
	update();
}















FWButton::FWButton(QGraphicsItem * parent, const QBrush & brush , const QString msg)
		: QGraphicsItem(parent)
		, m_brush(brush),permission(false)
{
	setAcceptsHoverEvents(true);
	txt = msg;
	QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable);
	setToolTip(txt);
}

void FWButton::allow(bool e)
{
	permission = e;
	if (e)
	{
		QGraphicsItem::setCursor(Qt::SizeAllCursor);
	}
	else
	{
		QGraphicsItem::setCursor(Qt::ForbiddenCursor);
	}
}
QRectF FWButton::boundingRect() const
{
	return QRectF(-8, -8, 16, 16);
}

void FWButton::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	if (permission)
	{
		if (option->state & QStyle::State_MouseOver)
		{
			painter->setBrush(m_brush);
			painter->setPen(Qt::white);
			painter->setOpacity(0.8);
			painter->drawEllipse(boundingRect());
			painter->setOpacity(1.0);
		}
		setToolTip(txt);
	}
}

void FWButton::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	event->accept();
	emit operate(true);
	m_startPos = event->scenePos();
}

void FWButton::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (m_startPos.isNull())
		return;
	event->accept();
	emit dragging(event->scenePos());
}

void FWButton::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	event->accept();
	m_startPos = QPointF();
}

void FWButton::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	event->accept();
	emit reset();
}


void FWButton::focusInEvent(QFocusEvent * event)
{
	emit operate(true);
}

void FWButton::focusOutEvent(QFocusEvent * event)
{
	emit operate(false);
}



void FWButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	emit operate(true);
}

void FWButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	emit operate(false);
}



AbsText::AbsText() : device(0)
{

}

/* only one connect */
LayerText *AbsText::txtControl() const
{
	if (!device)
	{
		AbsoluteLayer *that = const_cast<AbsoluteLayer *>(q);
		device = new LayerText();
		////////connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursorWakeUp()));
		connect(device, SIGNAL(q_update(QRect)),q, SLOT(updateArea(QRect)));
		///////connect(device, SIGNAL(q_visible(QRectF) ),q, SLOT(ensureVisible(QRectF)));
		///////connect(device, SIGNAL(q_update_scene()),q, SLOT(sceneReload()));
	}
	return device;
}




