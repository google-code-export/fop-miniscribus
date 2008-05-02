#include "textlayer.h"
#include "mounttextprivate.h"

QT_BEGIN_NAMESPACE


TextLayer::TextLayer(const int layer_id , QGraphicsItem *parent , QGraphicsScene *scene )
    : QGraphicsItem(parent,scene),evesum(0),modus(Show),border(1.),
    hi(330),wi(550),bgcolor(QColor(Qt::yellow)),
    bordercolor(QColor(Qt::red)),
    format(DIV_ABSOLUTE),mount(new TextController)
{
    mount->q = this;
    history.clear();
    id = layer_id;
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
        rootformats.setBorder(0);
        Tframe->setFrameFormat(rootformats);
        _doc->setPageSize(QSizeF(wi,hi)); 
        DLayout = _doc->documentLayout();
    mount->txtControl()->document()->toHtml().size();  /* connect all */
    
    //////////qDebug() << "### TextLayer init ...........  html in size ->" << _doc->toHtml().size();
    setZValue(0.99999);
    setDocument(_doc);
    RestoreMoveAction();
    init();
    //////////////qDebug() << "### TextLayer init  html in size ->" << mount->txtControl()->document()->toHtml().size();
}


void TextLayer::cursorPositionChanged( const QTextCursor cur)
{
    
   ///////////////qDebug() << "### cursorPositionChanged ->" << document()->toHtml().size();
   update();
    
}

void TextLayer::SaveFilelayer()
{
    QString file = QFileDialog::getSaveFileName(0, tr("Save Layer File"),QString(setter.value("LastDir").toString()),tr("Layer File (*.layer)"));
    if (file.size() > 0) {
        setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
        
        RichDoc asidoc = ReadActualItem();
         if (!file.endsWith(".layer")) {
            file.append(".layer");
         }
        const QString stream = SaveRichDoc(asidoc);
        fwriteutf8(file,stream);
    }
}


void TextLayer::OpenFilelayer()
{
    QString file = QFileDialog::getOpenFileName(0, tr( "Choose Layer File..." ), QString(setter.value("LastDir").toString()) ,tr("Layer File (*.layer)"));
    
    if (file.size() > 0) {
        setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
        
        QString inside;
        QFile f(file); 
                if (f.open(QFile::ReadOnly | QFile::Text)) {
                    inside = QString::fromUtf8(f.readAll());
                    f.close();
                }
        RichDoc dddoc = OpenRichDoc(inside); 
        insert(dddoc);       
    }
}


void TextLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    bool canedit = mount->txtControl()->editable();
    if (!isSelected()) {
    return;
    }
    if (modus == Lock) {
    return;
    }
    bool activeedit = canedit ?  true : false;
    
    
    
    QAction *a;
    RootMenu = mount->txtControl()->StandardMenu(event->widget());
    
        actionSwapEdit = new QAction(tr("Edit modus"),this);
        actionSwapEdit->setIcon(QIcon(":/img/icon.png"));
        actionSwapEdit->setCheckable(true);
        actionSwapEdit->setChecked(activeedit);
	      connect(actionSwapEdit, SIGNAL(triggered()),this,SLOT(SwapEdit()));
        RootMenu->addAction(actionSwapEdit);
    
    a = RootMenu->addAction(tr("Register commit layer now"), this, SLOT(CommitLayer()));
    a->setIcon(QIcon(":/img/html_div.png"));
    if (canedit) {
    QMenu *DocRevision = new QMenu(tr("Revision History"),event->widget());
    DocRevision->setIcon(QIcon(":/img/html_div.png"));
        int longs = 0;
                 QMapIterator<uint,QString> i(history);
                 while (i.hasNext()) {
                     i.next();
                     if (longs < 101) {
                     QDateTime from;
                              from.setTime_t(i.key()); 
                     QAction *revisior = new QAction(tr("Set revision from %1 - id %2").arg(from.toString()).arg(i.key()),this);
                     revisior->setIcon(QIcon(":/img/html_div.png"));
                     revisior->setData(i.key());
	                   connect(revisior, SIGNAL(triggered()),this,SLOT(InsertRevision()));
                     DocRevision->addAction(revisior);
                     }
                     longs++;
                 } 
        
        
        if (longs > 0) {
         RootMenu->addAction(DocRevision->menuAction()); 
        }
        
        a = RootMenu->addAction(tr("Layer background color"), this, SLOT(SetNewBGColor()));
        a->setIcon(createColorIcon(bgcolor));
        a = RootMenu->addAction(tr("Layer border color"), this, SLOT(SetNewBorderColor()));
        a->setIcon(createColorIcon(bordercolor));
        a = RootMenu->addAction(tr("Layer Border widht"), this, SLOT(Borderwidht()));
        a->setIcon(QIcon(":/img/configure.png"));
        
        
        
        a = RootMenu->addAction(tr("Save as Layer file"), this, SLOT(SaveFilelayer()));
        a->setIcon(QIcon(":/img/bringtofront.png"));
        
        a = RootMenu->addAction(tr("Open Layer file"), this, SLOT(OpenFilelayer()));
        a->setIcon(QIcon(":/img/bringtofront.png"));
        
        
        
        
    }
    RootMenu->exec(event->screenPos());
    delete RootMenu;
    
}

void TextLayer::SwapEdit()
{
    bool canedit = mount->txtControl()->editable();
    if (canedit) {
        RestoreMoveAction();
    } else {
        EditModus();
    }
    
}
void TextLayer::Borderwidht()
{
    QTextFrame  *Tframe = document()->rootFrame();
    QTextFrameFormat ft = Tframe->frameFormat();
    
    int maxborder = mount->txtControl()->boundingRect().width() / 10;
    bool ok;
    int LargeSet = QInputDialog::getInteger(0, tr("Layer border Width"),
                                      tr("Point Length:"),border,0,maxborder,1,&ok);
    if (ok) {
        border = LargeSet;
        qreal maxi0 = qMin (ft.topMargin(),ft.bottomMargin());
        qreal maxi1 = qMin (ft.rightMargin(),ft.leftMargin());
        qreal maxi = qMin (maxi0,maxi1);
        if ( border > maxi) {
                ft.setBottomMargin(border);
                ft.setTopMargin(border);
                ft.setRightMargin(border);
                ft.setLeftMargin(border);
                ft.setPadding(0);
                Tframe->setFrameFormat(ft);
        }
        
        /*
        int xBottomMargin =  ToUnit(ft.bottomMargin(),"mm");
    int xTopMargin = ToUnit(ft.topMargin(),"mm");
    int xRightMargin = ToUnit(ft.rightMargin(),"mm");
    int xLeftMargin = ToUnit(ft.leftMargin(),"mm");
        
        */
        
        update();
    }
}

void TextLayer::InsertRevision()
{
    
    read();
    QAction *invoice = qobject_cast<QAction *>(sender());
    const uint index = invoice->data().toUInt();
    qDebug() << "### history id  ->" << index;
    RichDoc olditem = OpenRichDoc(history[index]);
    insert(olditem);
    EditModus();
    update();
}

void TextLayer::insert( RichDoc Rdoc )
{
    _doc = Rdoc.todoc();
    setStyle(Rdoc.style.split(";"),false);
    mount->txtControl()->setDocument(_doc,this);
    mount->txtControl()->RegisterResource(Rdoc.resource);
}


void TextLayer::CommitLayer()
{
    read();
    ///////////qDebug() << "### history.size ->" << history.size();
}


void TextLayer::setDocument ( QTextDocument * document )
{
    mount->txtControl()->setDocument(document,this);
}

QTextDocument *TextLayer::document()
{
    return mount->txtControl()->document();
}
QTextCursor TextLayer::textCursor()
{
  return mount->txtControl()->textCursor();
}

void TextLayer::init()
{
    read();
}
void TextLayer::cursor_wake_up()
{
    ///////////qDebug() << "### cursor_wake_up ";
    update();
}


TextController::TextController()
  : device(0)
{ 

}
TextWriter *TextController::txtControl() const
{
    if (!device) {
       TextLayer *that = const_cast<TextLayer *>(q); 
       device = new TextWriter(that);
       connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursor_wake_up()));
       connect(device, SIGNAL(q_update(QRectF) ),q, SLOT(updatearea(QRectF)));
    }
    return device;
}


void TextLayer::read()
{
        RichDoc itemdoc = ReadActualItem();
        QDateTime timer1( QDateTime::currentDateTime() );
        history.insert(timer1.toTime_t(),SaveRichDoc(itemdoc));
}




void TextLayer::updatearea( const QRectF areas )
{
    if (areas.width() > boundingRect().width()) {
    return;
    }
    evesum++;
    //////////////qDebug() << "### area " << areas.width() << "x" << areas.height() << "|" <<  evesum;
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
    
     /* Layer Background draw! */
		qreal hightlengh =  mount->txtControl()->boundingRect().height();
    if (format != Lock )  {  
                if (hightlengh > hi ) {
                  hi = hightlengh + 5;
                }
                if (format == DIV_AUTO ) {
                   hi = qBound(Metric("18mm"), hightlengh + 10,Metric("297mm"));
                    
                   if (hi > Metric("297mm") ) {
                       RestoreMoveAction();
                   }
                }
                
    } 
    
    
    QPen BorderPaint;
    if (border > 0) {
        BorderPaint = QPen(QBrush(bordercolor),border);
    } else {
        BorderPaint = QPen(Qt::NoPen);
    }
    
     if (mount) {
    mount->txtControl()->paint_doc(painter,option,QBrush(bgcolor),boundingRect(),BorderPaint,false);
    } else {
        qApp->beep();
		    qApp->beep();
    }
    
      
      if (modus == Lock ) {  /* writteln modus icon */
				painter->save();
				QPixmap pixbg(":/img/encrypted.png");
				painter->drawPixmap(QPointF(boundingRect().width() - 40,8),pixbg);
				painter->restore();
			}
      
   
    
    
    
    //////QGraphicsItem::paint(painter,option,widget);
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


void TextLayer::EditModus() 
{
    modus == Edit;
    mount->txtControl()->edit(true);
    setFlag(QGraphicsItem::ItemIsMovable,false);
    update(boundingRect());
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
    EditModus();
    mount->txtControl()->procesevent(event);
    EditModus();
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
    
    if (modus == Lock) {
    return;
    }
    
    if (modus == Move) {
        /* move from stop xy */
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
    
    if (modus == Lock) {
    return;
    }
    
    if (modus == Move) {
    modus = Show;
    RestoreMoveAction();
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
    
    if (modus == Lock) {
    return;
    }
    
    if (format == DIV_ABSOLUTE && event->buttons() == Qt::LeftButton && 
        event->modifiers() == Qt::ControlModifier  &&  
        modus == Show && !mount->txtControl()->editable()) {
        modus = Move;  
        /* start to move item size hi x wi  */
        return;
    }
    mount->txtControl()->procesevent(event);
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


void TextLayer::timerEvent(QTimerEvent *event)
{

    
}

void TextLayer::setZValue(qreal index )
{
    QGraphicsItem::setZValue(index);
    
}


void TextLayer::SetNewBGColor() 
{
     bool ok;
     QRgb col = QColorDialog::getRgba(bgcolor.rgba(),&ok);
     if (!ok) {
        return; 
     }
     int AlphaColor =  qAlpha ( col );
     bgcolor = QColor( col );
     bgcolor.setAlpha(AlphaColor);
     update();
}


void TextLayer::SetNewBorderColor() 
{
     bool ok;
     QRgb col = QColorDialog::getRgba(bordercolor.rgba(),&ok);
     if (!ok) {
        return; 
     }
     int AlphaColor =  qAlpha ( col );
     bordercolor = QColor( col );
     bordercolor.setAlpha(AlphaColor);
     if (border < 1) {
       border = 1;  
     }
     update();
}

RichDoc TextLayer::ReadActualItem()
{
    QString styles;
    QTextFrame  *Tframe = document()->rootFrame();
    QTextFrameFormat ft = Tframe->frameFormat();
    int xBottomMargin =  ToUnit(ft.bottomMargin(),"mm");
    int xTopMargin = ToUnit(ft.topMargin(),"mm");
    int xRightMargin = ToUnit(ft.rightMargin(),"mm");
    int xLeftMargin = ToUnit(ft.leftMargin(),"mm");
    const QString margin = QString("padding:%1mm %2mm %3mm %4mm;").arg(xTopMargin).arg(xRightMargin).arg(xBottomMargin).arg(xLeftMargin); 
    qreal realhightdoc = ToUnit(hi,"mm");
    qreal realwidhtdoc = ToUnit(wi,"mm");
    #define ALPHAHTML(alpha) ((alpha)/254.99999999)
    if (format != DIV_AUTO) {
    styles = "position:absolute; overflow:hidden; top:"+QString("%1mm").arg(ToUnit(pos().y(),"mm"))+"; left:"+QString("%1mm").arg(ToUnit(pos().x(),"mm"))+"; width:"+QString("%1mm").arg(ToUnit(wi,"mm"))+"; height:"+QString("%1mm").arg(ToUnit(hi,"mm"))+"; ";
    } else {
    styles = "min-height:"+QString("%1mm").arg(hi)+";";   
    }
    styles.append(margin);
    int alphacolor = qBound(0,bgcolor.alpha(),255);
    qreal percentos = ALPHAHTML( alphacolor );
    
    if (alphacolor <= 1) {
    styles.append(QString("background-color:transparent; ") );
    } else {
    styles.append(QString("background-color:%1; ").arg( bgcolor.name() ) );  
    }
    
    if (border > 0 ) {
    styles.append(QString("background-color:%1; ").arg( bgcolor.name() ) );  
    }
    
    
    
    
    if (percentos < 1 && alphacolor > 1) {
    QString Tnetto = QString("%1").arg(percentos, 0, 'f', 4);  /* CSS 2 alpha */
    QString IETnetto = QString("%1").arg(percentos * 100, 0, 'f', 4);  /* IE alpha */
    styles.append(QString("opacity:%1;filter:alpha(opacity=%2);-moz-opacity:%1;").arg(Tnetto).arg(IETnetto));
    }
    
    if (border > 0 ) {
    styles.append(QString("border-width:%2mm;border-style:solid;border-color:%1;").arg(bordercolor.name()).arg(ToUnit(border,"mm")));
    }
    
    if (zValue() > 1) {
    styles.append(QString("z-index:%1;").arg(qRound(zValue())));
    }
    
    RichDoc Rdoc;
            Rdoc.Register(document(),mount->txtControl()->imglist(),styles);
    return Rdoc;
}

void TextLayer::setStyle( QStringList syle , bool fromclone )
{
    #define ALPHAHTML(alpha) ((alpha)*254.99999999)
    QStringList find;
    find << "position" << "top" << "left" << "width" << "opacity" << "height" << "background-color" << "z-index" << "id" << "border-width" << "border-color" << "border-style";  //////  border-color:#FFFF00; border-width:2px; border-style:solid;
    QMap<QString,QVariant> incss; 
    for (int o = 0; o < find.size(); ++o)  {
         incss.insert(find.at(o),QString("0"));
    }
    /* fill default null values */
    /////qDebug() << "### syle " << syle;
    for (int i = 0; i < syle.size(); ++i)  {
        QString values = syle.at(i).trimmed();
        QStringList css = values.split(":");
        if (css.size() == 2) {
        incss.insert(css.at(0),css.at(1));
        }
    }
   if (incss.value("position").toString() == "absolute") {
         format = DIV_ABSOLUTE;
         if (fromclone) {  
         setPos(QPointF(Metric(incss.value("left").toString())+5,Metric(incss.value("top").toString()) + 5)); 
         } else {
         setPos(QPointF(Metric(incss.value("left").toString()),Metric(incss.value("top").toString())));
         }
           
           
      if ( Metric(incss.value("width").toString()) > 10 ) {
        wi = Metric(incss.value("width").toString());
            QTextFrame  *Tframe = document()->rootFrame();
            QTextFrameFormat rootformats = Tframe->frameFormat();
            rootformats.setWidth(wi); 
            Tframe->setFrameFormat(rootformats);
      }
    
      if (Metric(incss.value("height").toString()) > 10) {
        hi = Metric(incss.value("height").toString());
      }
    
        if (incss.value("z-index").toInt() > 1) {
            if (fromclone) {
              setZValue(incss.value("z-index").toDouble() + 1);
            } else {
               setZValue(incss.value("z-index").toDouble()); 
            }
         }
    }
    
    if (incss.value("background-color").toString() !="0") {
       bgcolor = QColor(incss.value("background-color").toString());
          if (incss.value("opacity").toDouble() > 0) {
              int alFaCol = qBound(0.00,ALPHAHTML(incss.value("opacity").toDouble()),255.00);
              bgcolor.setAlpha(alFaCol);
          }
          if (incss.value("background-color").toString() == "transparent") {
            bgcolor.setAlpha(0);
          }
          
          
      }
      
    if (incss.value("border-width").toString() !="0") {
    bordercolor = QColor(incss.value("border-color").toString());
    border = Metric(incss.value("border-width").toString()); 
    }
    
    modus = Show;
    update();
      
}


QT_END_NAMESPACE











