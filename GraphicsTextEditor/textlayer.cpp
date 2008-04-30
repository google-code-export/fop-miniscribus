#include "textlayer.h"
#include "mounttextprivate.h"

QT_BEGIN_NAMESPACE


TextLayer::TextLayer(const int layer_id , QGraphicsItem *parent , QGraphicsScene *scene )
    : QGraphicsItem(parent,scene),evesum(0),modus(Show),border(0.),
    hi(330),wi(550),bgcolor(QColor(Qt::yellow)),
    bordercolor(QColor(Qt::transparent)),
    format(DIV_ABSOLUTE),mount(new TextController)
{
    mount->q = this;
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    setSelected(true);
    QString Stream_0 = "PHAgc3R5bGU9IiBtYXJnaW4tdG9wOjBweDsgbWFyZ2luLWJvdHRvbTowcHg7IG1hcmdpbi1sZWZ0OjBweDsgbWFyZ2luLXJpZ2h0OjBweDsgLXF0LWJsb2NrLWluZGVudDowOyB0ZXh0LWluZGVudDowcHg7IGZvbnQtc2l6ZTo4cHQ7Ij48c3BhbiBzdHlsZT0iIGZvbnQtZmFtaWx5OidMdWNpZGEgQ29uc29sZSc7IGZvbnQtc2l6ZToxNHB0OyBmb250LXdlaWdodDo2MDA7IGNvbG9yOiNmZjU1MDA7Ij5DVFJMK1EgIHF0IDQuNCBRdGV4dGVkaXRvcjwvc3Bhbj4KPC9wPgoKPHAgc3R5bGU9Ii1xdC1wYXJhZ3JhcGgtdHlwZTplbXB0eTsgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjwvcD4KCjxwIHN0eWxlPSIgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjxzcGFuIHN0eWxlPSIgdGV4dC1kZWNvcmF0aW9uOiB1bmRlcmxpbmU7Ij5DVFJMK2RvdWJsZSBjbGljayB0byBpbmxpbmUgZWRpdCBsYXllcjwvc3Bhbj4KPC9wPgoKPHAgc3R5bGU9Ii1xdC1wYXJhZ3JhcGgtdHlwZTplbXB0eTsgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjwvcD4KCjxwIHN0eWxlPSIgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjxzcGFuIHN0eWxlPSIgdGV4dC1kZWNvcmF0aW9uOiB1bmRlcmxpbmU7Ij5Nb3ZlIGxheWVyIG9ubHkgZHJhZy4uLjwvc3Bhbj4KPC9wPgoKPHAgc3R5bGU9Ii1xdC1wYXJhZ3JhcGgtdHlwZTplbXB0eTsgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjwvcD4KCjxwIHN0eWxlPSItcXQtcGFyYWdyYXBoLXR5cGU6ZW1wdHk7IG1hcmdpbi10b3A6MHB4OyBtYXJnaW4tYm90dG9tOjBweDsgbWFyZ2luLWxlZnQ6MHB4OyBtYXJnaW4tcmlnaHQ6MHB4OyAtcXQtYmxvY2staW5kZW50OjA7IHRleHQtaW5kZW50OjBweDsgZm9udC1mYW1pbHk6J0x1Y2lkYSBDb25zb2xlJzsgZm9udC1zaXplOjE0cHQ7Ij48L3A+";
    QByteArray xcode("");;
    xcode.append(Stream_0);
    QByteArray precode(QByteArray::fromBase64(xcode));    
    _doc = new QTextDocument();  
    _doc->setHtml(QString(precode.data()));
        QTextFrame  *Tframe = _doc->rootFrame();
        QTextFrameFormat rootformats = Tframe->frameFormat();
        rootformats.setWidth(wi);
        rootformats.setBorder(1);
        rootformats.setBorderBrush(QBrush(bgcolor));
        Tframe->setFrameFormat(rootformats);
        _doc->setPageSize(QSizeF(wi,hi)); 
        DLayout = _doc->documentLayout();
    qDebug() << "### TextLayer init  html in size ->" << _doc->toHtml().size();
    init();
    qDebug() << "### TextLayer init  html in size ->" << mount->txtControl()->document()->toHtml().size();
}
void TextLayer::init()
{
    history.clear();
    read();
}


void TextLayer::cursor_wake_up()
{
    qDebug() << "### cursor_wake_up ";
}


TextController::TextController()
  : device(0)
{ 

}
TextWriter *TextController::txtControl() const
{
    if (!device) {
       TextLayer *that = const_cast<TextLayer *>(q); 
       device = new TextWriter(q->_doc,that);
       connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursor_wake_up()));
       connect(device, SIGNAL(q_update(QRectF) ),q, SLOT(updatearea(QRectF)));
    }
    return device;
}


void TextLayer::read()
{
        /* big undo action */
        dd = new LogData;
        dd->bgcolor = bgcolor;
        dd->border = border;
        dd->bordercolor = bordercolor;
        dd->hi = hi;
        dd->wi = wi;
        dd->DFormat = format;
        dd->doc = _doc;
        QDateTime timer1( QDateTime::currentDateTime() );
        history.insert(timer1.toTime_t(),dd);
}




void TextLayer::updatearea( const QRectF areas )
{
    if (areas.width() > boundingRect().width()) {
    return;
    }
    evesum++;
    qDebug() << "### area " << areas.width() << "x" << areas.height() << "|" <<  evesum;
    update(areas);
}

TextLayer::~TextLayer()
{
    delete _doc;
}


void TextLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    Q_UNUSED(widget);
    if (mount) {
    mount->txtControl()->paint_doc(painter,option,QBrush(Qt::green),boundingRect(),false);
    }
}


bool TextLayer::sceneEvent(QEvent *event)
{
    evesum++;
    setSelected(true);
    return QGraphicsItem::sceneEvent(event);
}

bool TextLayer::contains(const QPointF &point) const
{
    return boundingRect().contains(point);
}






/* proprieta oggetto */
int TextLayer::type() const
{
    return Type;
}

LAYERTYPE TextLayer::Ltype() const
{
    return format;
}

void TextLayer::setSelected( bool selected ) 
{
    if (!selected) {
      modus = Show;
    }
  IsSelectActive = selected;
  QGraphicsItem::setSelected(selected);
  update();
}

QRectF TextLayer::boundingRect() const
{
    return QRectF(0,0,wi,hi);
}

/* proprieta oggetto QRectF docrect = DLayout->frameBoundingRect(_doc->rootFrame()) */
void TextLayer::RestoreMoveAction() 
{
    modus == Show;
    QGraphicsItem::setCursor(Qt::ArrowCursor);
    QApplication::restoreOverrideCursor();
    setSelected(true);
    mount->txtControl()->edit(false);
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    update();
    //////////////////qDebug() << "### reset ";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/*  events  enum CurrentModus{ Show, Edit, Move , Lock }; */

////////////////////////////////////////////////////////////////////////////////////////////////////
void TextLayer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    setSelected(true);   /* scene attension auto select ! */
    }
    if (modus == Lock) {
    return;
    }
    if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier &&
        mount->txtControl()->editable()) {
    RestoreMoveAction(); 
    return;
    }
    if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier  &&  modus == Show && 
        !mount->txtControl()->editable()) {
    modus == Edit;
    mount->txtControl()->edit(true);
    mount->txtControl()->procesevent(event);
    setFlag(QGraphicsItem::ItemIsMovable,false);
    update(boundingRect());
    return;
    }
    if (mount->txtControl()->editable()) {
    mount->txtControl()->procesevent(event);
    }
    QGraphicsItem::mouseDoubleClickEvent(event);  
}

void TextLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    if (mount->txtControl()->editable()) {
    mount->txtControl()->procesevent(event);
    }
    QGraphicsItem::mouseMoveEvent(event); 
}

void TextLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    
    if (mount->txtControl()->editable()) {
    mount->txtControl()->procesevent(event);
    }
    
    QGraphicsItem::mouseReleaseEvent(event); 
}

void TextLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    
    if (mount->txtControl()->editable()) {
    mount->txtControl()->procesevent(event);
    }
    QGraphicsItem::mousePressEvent(event); 
}

void TextLayer::keyPressEvent( QKeyEvent * event ) 
{
    if (!isSelected()) {
    return;
    }
    
    if (modus == Lock) {
    return;
    }
    if (mount->txtControl()->editable()) {
    mount->txtControl()->procesevent(event);
    }
    QGraphicsItem::keyPressEvent(event);
}








QT_END_NAMESPACE











