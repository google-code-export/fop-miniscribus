#include "textlayer.h"
#include "mounttextprivate.h"

QT_BEGIN_NAMESPACE


TextLayer::TextLayer(const int layer_id , QGraphicsItem *parent , QGraphicsScene *scene )
    : QGraphicsItem(parent,scene),evesum(0),
    pageNumber(0),moving(false),edit_enable(false),id(layer_id),modus(M_VIEW),IsSelectActive(false),
    hi(330),wi(550),bgcolor(QColor(Qt::yellow)),bordercolor(QColor(Qt::transparent)),e_current(DIV_AUTO)
{
    mount = new MountTextPrivate(this);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    
    QString Stream_0 = "PHAgc3R5bGU9IiBtYXJnaW4tdG9wOjBweDsgbWFyZ2luLWJvdHRvbTowcHg7IG1hcmdpbi1sZWZ0OjBweDsgbWFyZ2luLXJpZ2h0OjBweDsgLXF0LWJsb2NrLWluZGVudDowOyB0ZXh0LWluZGVudDowcHg7IGZvbnQtc2l6ZTo4cHQ7Ij48c3BhbiBzdHlsZT0iIGZvbnQtZmFtaWx5OidMdWNpZGEgQ29uc29sZSc7IGZvbnQtc2l6ZToxNHB0OyBmb250LXdlaWdodDo2MDA7IGNvbG9yOiNmZjU1MDA7Ij5DVFJMK1EgIHF0IDQuNCBRdGV4dGVkaXRvcjwvc3Bhbj4KPC9wPgoKPHAgc3R5bGU9Ii1xdC1wYXJhZ3JhcGgtdHlwZTplbXB0eTsgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjwvcD4KCjxwIHN0eWxlPSIgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjxzcGFuIHN0eWxlPSIgdGV4dC1kZWNvcmF0aW9uOiB1bmRlcmxpbmU7Ij5DVFJMK2RvdWJsZSBjbGljayB0byBpbmxpbmUgZWRpdCBsYXllcjwvc3Bhbj4KPC9wPgoKPHAgc3R5bGU9Ii1xdC1wYXJhZ3JhcGgtdHlwZTplbXB0eTsgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjwvcD4KCjxwIHN0eWxlPSIgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjxzcGFuIHN0eWxlPSIgdGV4dC1kZWNvcmF0aW9uOiB1bmRlcmxpbmU7Ij5Nb3ZlIGxheWVyIG9ubHkgZHJhZy4uLjwvc3Bhbj4KPC9wPgoKPHAgc3R5bGU9Ii1xdC1wYXJhZ3JhcGgtdHlwZTplbXB0eTsgbWFyZ2luLXRvcDowcHg7IG1hcmdpbi1ib3R0b206MHB4OyBtYXJnaW4tbGVmdDowcHg7IG1hcmdpbi1yaWdodDowcHg7IC1xdC1ibG9jay1pbmRlbnQ6MDsgdGV4dC1pbmRlbnQ6MHB4OyBmb250LWZhbWlseTonTHVjaWRhIENvbnNvbGUnOyBmb250LXNpemU6MTRwdDsiPjwvcD4KCjxwIHN0eWxlPSItcXQtcGFyYWdyYXBoLXR5cGU6ZW1wdHk7IG1hcmdpbi10b3A6MHB4OyBtYXJnaW4tYm90dG9tOjBweDsgbWFyZ2luLWxlZnQ6MHB4OyBtYXJnaW4tcmlnaHQ6MHB4OyAtcXQtYmxvY2staW5kZW50OjA7IHRleHQtaW5kZW50OjBweDsgZm9udC1mYW1pbHk6J0x1Y2lkYSBDb25zb2xlJzsgZm9udC1zaXplOjE0cHQ7Ij48L3A+";
    
    QByteArray xcode("");;
    xcode.append(Stream_0);
    QByteArray precode(QByteArray::fromBase64(xcode));    
    mount->setxhtml(precode.data());
    connect(mount, SIGNAL(updateRequest(QRectF) ), this, SLOT(updatearea(QRectF)));
    mount->edit(false);
    evesum = 0;
}


void TextLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    
    bool canedit = mount->editable();
    if (!canedit) {
    return;
    }
    
     ////////textedit->addSeparator();
    
    
    textedit = new QMenu(tr("Base menu"),event->widget());
    QMenu *charsfo = mount->TextMenu(event->widget());
    QMenu *blockfo = mount->BlockMenu(event->widget());
    textedit->addAction(charsfo->menuAction()); 
    textedit->addAction(blockfo->menuAction()); 
    
    actionLayerBackColor = new QAction(tr("Layer Background color"),this);
    actionLayerBackColor->setIcon(createColorIcon(bgcolor));
	  connect(actionLayerBackColor, SIGNAL(triggered()),this,SLOT(LayerBGcolor()));
    textedit->addAction(actionLayerBackColor);
    
    actionLayerMargin = new QAction(tr("Layer margin top,left,right,bottom"),this);
    actionLayerMargin->setIcon(QIcon(QString::fromUtf8(":/img/view_top_bottom.png")));
	  connect(actionLayerMargin, SIGNAL(triggered()),mount,SLOT(SetLayerMargin()));
    textedit->addAction(actionLayerMargin);
    
    
    
    /////
    textedit->exec(event->screenPos());
    delete blockfo;
    
}

void TextLayer::LayerBGcolor()
{
    QRgb col = QColorDialog::getRgba(bgcolor.rgb()); 
    QColor col2 = QColor(col);
    int trans = qAlpha(col); 
    col2.setAlpha(trans);
       if (!col2.isValid()) {
        return;
        } 
    
    bgcolor = col2;
}

void TextLayer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    if (mount->editable()) {
    mount->procesevent(event);  
    }
    QGraphicsItem::mouseMoveEvent(event); 
}

void TextLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    
    if (mount->editable()) {
    mount->procesevent(event);  
    }
    
    QGraphicsItem::mouseReleaseEvent(event); 
}

void TextLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    
    if (mount->editable()) {
    mount->procesevent(event);  
    }
    QGraphicsItem::mousePressEvent(event); 
}

void TextLayer::keyPressEvent( QKeyEvent * event ) 
{
    if (!isSelected()) {
    return;
    }
    
    if (modus == M_LOCK) {
    return;
    }
    
    
    if (mount->editable()) {
    mount->procesevent(event);  
    }
    QGraphicsItem::keyPressEvent(event);
}


void TextLayer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    setSelected(true);   /* scene attension auto select ! */
    }
    if (modus == M_LOCK) {
    return;
    }
    if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier && mount->editable()) {
    RestoreMoveAction(); 
    return;
    }
    if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier  &&  modus == M_VIEW && !mount->editable()) {
    modus == M_WRITTELN;
    mount->edit(true);
    mount->procesevent(event);
    setFlag(QGraphicsItem::ItemIsMovable,false);
    update(boundingRect());
    return;
    }
    if (mount->editable()) {
    mount->procesevent(event); 
    }
    QGraphicsItem::mouseDoubleClickEvent(event);  
}

void TextLayer::RestoreMoveAction() 
{
    modus == M_VIEW;
    QGraphicsItem::setCursor(Qt::ArrowCursor);
    QApplication::restoreOverrideCursor();
    setSelected(true);
    mount->edit(false);
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    update();
    //////////////////qDebug() << "### reset ";
}

void TextLayer::updatearea( const QRectF areas )
{
    /* incomming  from large rect request buggi code!!! */
    if (areas.width() > boundingRect().width()) {
    return;
    }
    evesum++;
    ///////qDebug() << "### area " << areas.width() << "x" << areas.height() << "|" <<  evesum;
    update(areas);
}



void TextLayer::inputMethodEvent(QInputMethodEvent *event)
{
    //////////mount->procesevent(event);
}

void TextLayer::appendtext( const QString txt )
{
   textcursor().movePosition(QTextCursor::End);
   textcursor().beginEditBlock();
   textcursor().insertText(txt);
   textcursor().endEditBlock(); 
}


void TextLayer::cursorPositionChanged( const QTextCursor curs )
{
    document()->setTextWidth(wi);
}


TextLayer::~TextLayer()
{
    delete mount;
}


QTextCursor TextLayer::textcursor()
{
  return QTextCursor(document());
}

QTextDocument *TextLayer::document()
{
  mount->wake_up(5);
  return mount->_d;
}


QRectF TextLayer::boundingRect() const
{
    return QRectF(0,0,wi,hi);
}


int TextLayer::type() const
{
    return Type;
}
void TextLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    Q_UNUSED(widget);
    /////////painter->setPen(Qt::NoPen);
    ///////////painter->setRenderHint(QPainter::Antialiasing, true);
    const QColor fgcolor = option->palette.windowText().color();
    const QColor ebgcolor( // ensure good contrast against fgcolor
        fgcolor.red()   > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue()  > 127 ? 0 : 255);
    
    if (mount) {
        QTextFrame  *Tframe = document()->rootFrame();
        QTextFrameFormat rootformats = Tframe->frameFormat();
        rootformats.setWidth(wi);
        Tframe->setFrameFormat(rootformats);
        document()->setPageSize(QSizeF(wi,hi)); 
        /////////  void paint_doc( QPainter *painter , const QStyleOptionGraphicsItem *option , QBrush BGpage , const QRectF fulllayer ,  bool alternate = false );
        TextboundingRect = mount->boundingRect();
        text_width = TextboundingRect.width();
        text_hight = TextboundingRect.height();
        
        if (e_current == DIV_AUTO && hi < text_hight) {
         hi = text_hight + 5;
        }
        if (e_current == DIV_ABSOLUTE && hi < text_hight) {
        edit_enable = false;
        mount->edit(edit_enable);
        hi = text_hight + 5;
        }
        mount->paint_doc(painter,option,QBrush(bgcolor),boundingRect(),false);
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








QT_END_NAMESPACE











