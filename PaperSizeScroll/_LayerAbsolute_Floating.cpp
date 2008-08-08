#include "_LayerAbsolute_Floating.h"

AbsoluteLayer::~AbsoluteLayer()
{
  qDebug() << "### destroy obj ...";
}



AbsoluteLayer::AbsoluteLayer(QGraphicsItem *parent )
    : QGraphicsRectItem(parent),dev(new AbsText),Rotate(0)
{
    dev->q = this;
    qDebug() << "### init....";
    setAcceptsHoverEvents(true);
    setAcceptDrops(true);
    QGraphicsItem::setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setFlag(QGraphicsItem::ItemIsMovable,false);
    setZValue (333.555555);
    setRect(QRectF(0,0,200,23));
    Angle_1 = new FWButton(this,Qt::green,tr("Move Layer"));
    Angle_1->allow(true);
    connect( Angle_1, SIGNAL(dragging(const QPointF&)), this, SLOT(slotModpos_1(const QPointF&)));
    Angle_4 = new FWButton(this,Qt::green,tr("Resize layer"));
    Angle_4->allow(true);
    connect( Angle_4, SIGNAL(dragging(const QPointF&)), this, SLOT(slotResize_1(const QPointF&)));
    Angle_4->setPos(boundingRect().bottomRight());

    Angle_2 = new FWButton(this,Qt::green,tr("Rotate layer"));
    Angle_2->allow(true);
    connect( Angle_2, SIGNAL(dragging(const QPointF&)), this, SLOT(slotRotate_1(const QPointF&)));
    Angle_2->setPos(boundingRect().topRight());
    lastUpdateRequest = rect();  

    QTextDocument *dummy = new QTextDocument();
    dummy->setHtml ( "<p>Floating Layer1...andando a passeggio di cose preio...</p>" ); /////  ReadFile("a.html")
    setDocument(dummy,FOP);
    dev->txtControl()->SetRect ( rect() );
}


void AbsoluteLayer::setDocument( const QTextDocument * doc , FileHandlerType Type )
{
    dev->txtControl()->setDocument(doc,Type);
    dev->txtControl()->SetRect ( rect() );
    
}

QTextCursor AbsoluteLayer::textCursor() 
{
  return dev->txtControl()->textCursor();
}

QTextDocument *AbsoluteLayer::document()
{
  return dev->txtControl()->document();
}


void AbsoluteLayer::updatearea( const QRect areas )
{
    lastUpdateRequest = areas; 
    qDebug() << "### updatearea " << areas;
   
    const QRectF txtrect = dev->txtControl()->boundingRect();
    if (rect().height() < txtrect.height()) {
    setRect(QRectF(0,0,rect().width(),txtrect.height()));
    Angle_4->setPos(boundingRect().bottomRight());
    Angle_2->setPos(boundingRect().topRight());
    }
    update(areas);
}

void AbsoluteLayer::slotRotate_1( const QPointF posi )
{
    QPointF newPos = mapFromScene(posi);
    QPointF refPos = Angle_2->pos();
    if (newPos == refPos) {
        return;
    }
    QLineF newangles(boundingRect().topLeft(),newPos);
    newangles.setLength ( boundingRect().width() );
    Rotate = 360 - newangles.angle();
    Angle_4->setPos(boundingRect().bottomRight());
    Angle_2->setPos(boundingRect().topRight());
    dev->txtControl()->SetRect ( rect() );
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

void AbsoluteLayer::slotModpos_1( const QPointF posi )
{
    QPointF newPos = mapFromScene(posi);
    QPointF refPos = Angle_1->pos();
    if (newPos == refPos) {
        return;
    }
    QTransform trap = transform();
    QRectF truebound = trap.mapRect ( absoluteRect() );
    QPointF buttonrelative = mapFromItem( Angle_1 ,  posi );
    setPos(buttonrelative - LineTops().p1());
    setToolTip(QString("X = %1 / Y= %2").arg(buttonrelative.x()).arg(buttonrelative.y()));
}

void AbsoluteLayer::slotResize_1( const QPointF posi )
{
    QPointF newPos = mapFromScene(posi);
    QPointF refPos = Angle_4->pos();
    if (newPos == refPos) {
        return;
    }
    QPointF buttonrelative = mapFromItem( Angle_4 ,  posi ) ;
    QPointF tl = Diagonal().p1();
    const qreal largo = qAbs(pos().rx() +  boundingRect().width()  - buttonrelative.x()  - tl.x() );
    const qreal alto = qAbs(pos().ry() +  boundingRect().height()  - buttonrelative.y() - tl.y());
    setRect(QRectF(0,0,qBound(MinimumWhidhLayer,largo,MAXLargoTmp),qBound(MinimumHightLayer,alto,MAXLargoTmp)));
    setToolTip(QString("X = %1 / Y= %2").arg(buttonrelative.x()).arg(buttonrelative.y()));
    Angle_4->setPos(boundingRect().bottomRight());
    Angle_2->setPos(boundingRect().topRight());
    dev->txtControl()->SetRect ( rect() );

}

void AbsoluteLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::TextAntialiasing);
    dev->txtControl()->SetRect ( rect() );
    const qreal liwid = 5.0;
    const qreal limid = liwid / 2;
    dev->txtControl()->paint(painter,option,widget);
    painter->setPen( QPen( Qt::green ,liwid));
    //////painter->drawLine(QLineF(limid,limid,limid,rect().height() - limid));



    painter->setPen( QPen(Qt::black ,1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect());

    painter->setPen(Qt::NoPen);
    QColor Visiblerecord(Qt::red);
		Visiblerecord.setAlpha(22);
    painter->setBrush(Visiblerecord);
    painter->drawRect(lastUpdateRequest);







    QMatrix matrix;
    matrix.translate ( boundingRect().center().x() , boundingRect().center().y() );
    matrix.rotate(Rotate);
    matrix.translate ( - boundingRect().center().x() , - boundingRect().center().y() );

    setTransform(QTransform(matrix),false);
}

void AbsoluteLayer::focusInEvent ( QFocusEvent * event ) 
{
    qDebug() << "### AbsoluteLayer focusInEvent ..." << flags();
    QGraphicsItem::setSelected(true);
    scene()->setFocusItem(this,Qt::ShortcutFocusReason);
    dev->txtControl()->setBlinkingCursorEnabled(true);
    emit close_main_cursor();
    return QGraphicsItem::focusInEvent(event);
}

void AbsoluteLayer::focusOutEvent ( QFocusEvent * event ) 
{
    qDebug() << "### AbsoluteLayer focusOutEvent ...";
    QGraphicsItem::setSelected(false);
    dev->txtControl()->setBlinkingCursorEnabled(false);
    return QGraphicsItem::focusOutEvent(event);
}

void AbsoluteLayer::inputMethodEvent ( QInputMethodEvent * event )  
{
    qDebug() << "### inputMethodEvent ...";
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
     qDebug() << "### mouseDoubleClickEvent...";
     return QGraphicsItem::mouseDoubleClickEvent(event);
}


void AbsoluteLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "### mouseMoveEvent.. ";
    return QGraphicsItem::mouseMoveEvent(event);
    
}

void AbsoluteLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
     

  if (dev->txtControl()->procesevent(event)) {
  return;
  }


   qDebug() << "### mouseReleaseEvent...";
  return QGraphicsItem::mouseReleaseEvent(event);
}

void AbsoluteLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

       if (dev->txtControl()->procesevent(event)) {
        return;
       }
     
    qDebug() << "### mousePressEvent...";
    return QGraphicsItem::mousePressEvent(event);
}

void AbsoluteLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "### hoverEnterEvent...";
    return QGraphicsItem::hoverEnterEvent(event);
}

void AbsoluteLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "### hoverLeaveEvent...";
    dev->txtControl()->setBlinkingCursorEnabled(false);
    return QGraphicsItem::hoverLeaveEvent(event);
}

void AbsoluteLayer::keyPressEvent( QKeyEvent * event ) 
{
   return dev->txtControl()->Controller_keyPressEvent(event);
   qDebug() << "### keyPressEvent...";
    return QGraphicsItem::keyPressEvent(event);
}

void AbsoluteLayer::keyReleaseEvent ( QKeyEvent * event )
{
    return dev->txtControl()->Controller_keyReleaseEvent(event);
    qDebug() << "### keyReleaseEvent...";
    return QGraphicsItem::keyReleaseEvent(event);
}


bool AbsoluteLayer::sceneEvent(QEvent *event)
{
  /////////qDebug() << "### sceneEvent.... + type   " << event->type();
  return QGraphicsItem::sceneEvent(event);
}

void AbsoluteLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   qDebug() << "### contextMenuEvent....";
}



FWButton::FWButton(QGraphicsItem * parent, const QBrush & brush , const QString msg )
    : QGraphicsItem(parent)
    , m_brush(brush),permission(false)
{
    setAcceptsHoverEvents(true);
    txt = msg;
    setToolTip(txt);
}

void FWButton::allow( bool e )
{
    permission = e;
    if (e) {
        QGraphicsItem::setCursor(Qt::SizeAllCursor);
    } else {
        QGraphicsItem::setCursor(Qt::ForbiddenCursor);
    }
}
QRectF FWButton::boundingRect() const
{
    return QRectF(-8, -8, 16, 16);
}

void FWButton::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
    if (permission) {
            if (option->state & QStyle::State_MouseOver) {
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






AbsText::AbsText()
  : device(0)
{ 

}

/* only one connect */
LayerText *AbsText::txtControl() const
{
    if (!device) {
       AbsoluteLayer *that = const_cast<AbsoluteLayer *>(q); 
       device = new LayerText();
       ////////connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursor_wake_up()));
       connect(device, SIGNAL(q_update(QRect) ),q, SLOT(updatearea(QRect)));
       ///////connect(device, SIGNAL(q_visible(QRectF) ),q, SLOT(ensureVisible(QRectF)));
       ///////connect(device, SIGNAL(q_update_scene()),q, SLOT(SceneReload()));
    }
    return device;
}




