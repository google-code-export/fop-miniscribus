/*******************************************************************************
 * class TextLayer
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/

#include "GraphicsItemEdit.h"
#include "mounttextprivate.h"
#include "GraphicsView.h"



TextLayer::TextLayer(const int layer_id , QGraphicsItem *parent , QGraphicsScene *scene )
    : QGraphicsItem(parent,scene),evesum(0),modus(Show),border(1.),currentprintrender(false),
    hi(Metric("22mm")),wi(Metric("55mm")),bgcolor(QColor(Qt::white)),
    bordercolor(QColor(Qt::red)),Rotate(0),
    format(DIV_ABSOLUTE),mount(new TextController)
{
    mount->q = this;
    history.clear();
    id = layer_id;
    setAcceptDrops(true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    setFlag(QGraphicsItem::ItemIsMovable,false);
    setSelected(false);
    _doc = new QTextDocument();  
    _doc->setHtml(tr("<p>Write your text<p>"));
        QTextFrame  *Tframe = _doc->rootFrame();
        QTextFrameFormat rootformats = Tframe->frameFormat();
        rootformats.setWidth(wi);
        rootformats.setBorder(0);
        Tframe->setFrameFormat(rootformats);
        _doc->setPageSize(QSizeF(wi,hi)); 
        DLayout = _doc->documentLayout();
    setDocument(_doc);
    mount->txtControl()->document()->toHtml().size();  /* connect all */
    setZValue(1.99999);
    RestoreMoveAction();
    init();
}

QList<QAction *> TextLayer::MainActions()
{
   return mount->txtControl()->MainActions();
    
}

qreal TextLayer::pointnext()
{
  LayerHightChecks();
	if (Ltype() != DIV_ABSOLUTE) {
		return hi;
	} else {
		return 0;
	}
  update();
}


bool TextLayer::editable()
{
    return mount->txtControl()->editable();
}


void TextLayer::LayerHightChecks() 
{
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    QRectF Srect = sc->sceneRect();
    if (Ltype() == DIV_AUTO || Ltype() == DIV_HEADER || Ltype() == DIV_FOOTER) {
    wi = Srect.width();
    }
    
    if (Ltype() != DIV_ABSOLUTE) {
        qreal txthight = mount->txtControl()->boundingRect().height() + 5;
        if (txthight > hi) {
        SetDimension(wi,txthight);
        }
    }  
        
        QTextFrame  *Tframe = document()->rootFrame();
        QTextFrameFormat rootformats = Tframe->frameFormat();
        rootformats.setWidth(wi);
        Tframe->setFrameFormat(rootformats);
        document()->setPageSize(QSizeF(wi,hi));
}


void TextLayer::setModus( CurrentModus  e)
{
   modus = e;
   update();
}

void TextLayer::cursorPositionChanged( const QTextCursor cur)
{
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
    Rotater *slider;
    /////GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    /* zvalue up and down   zmax()   zmin() */
    bool activeedit = canedit ?  true : false;
    
    
    
    QAction *a;
    RootMenu = mount->txtControl()->StandardMenu(event->widget());
    if (format == DIV_ABSOLUTE && modus != Lock) {
         a = RootMenu->addAction(tr("Send Front"), this, SLOT(seTFront()));
         a->setIcon(QIcon(":/img/sendtoback.png"));
         a = RootMenu->addAction(tr("Send Back"), this, SLOT(seTBack()));
         a->setIcon(QIcon(":/img/bringtofront.png")); 
        
    }
    
    
    
        actionSwapEdit = new QAction(tr("Edit modus"),this);
        actionSwapEdit->setIcon(QIcon(":/img/icon.png"));
        actionSwapEdit->setCheckable(true);
        actionSwapEdit->setChecked(activeedit);
	      connect(actionSwapEdit, SIGNAL(triggered()),this,SLOT(SwapEdit()));
        RootMenu->addAction(actionSwapEdit);
    
        bool locked = false;
        if (modus == Lock && !canedit) { 
        locked = true;
        }
        
        if (canedit && format == DIV_ABSOLUTE ) {
        slider = new Rotater(Rotate,event->widget());
        QWidgetAction *widgetslider = new QWidgetAction(event->widget());
        widgetslider->setDefaultWidget(slider);
        RootMenu->addAction(widgetslider); 
        connect(slider, SIGNAL(rotater(int)),this, SLOT(RotateLayer(int)));
            
        
        }
        
        actionSwapLock = new QAction(tr("Lock Unlock Layer"),this);
        if (locked) {
        actionSwapLock->setIcon(QIcon(":/img/encrypted.png"));
        } else {
        actionSwapLock->setIcon(QIcon(":/img/unlock.png"));   
        }
        
        actionSwapLock->setCheckable(true);
        actionSwapLock->setChecked(locked);
        connect(actionSwapLock, SIGNAL(triggered()),this,SLOT(SwapLockmodus()));
        RootMenu->addAction(actionSwapLock);
        
    
    
    
    
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
    
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/copy.png")),tr( "Copy full text plain" ), this , SLOT( Copy_Text_Plain() ) );
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/copy.png")),tr( "Copy xhtml text plain" ), this , SLOT( Copy_Html_Plain() ) );
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/copy.png")),tr( "Copy current Layer" ), this , SLOT( copyLayer() ) );
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/alayer_clone.png")),tr( "Clone this Layer" ), this , SLOT( CloneLayer() ) );
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/button_cancel.png")),tr( "Remove Layer" ), this , SLOT( Removehere() ) );
    RootMenu->exec(event->screenPos());
    
    
    if (canedit && format == DIV_ABSOLUTE ) {
     slider->disconnect(this);
     slider->deleteLater(); 
    }
    
    
    delete RootMenu;
    
    
    
}



void TextLayer::seTBack()
{
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    qreal backs = qBound(0.01,sc->zmin() - 1,1000.00);
    setZValue(backs);
    update();
    
}
void TextLayer::seTFront()
{
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    qreal top = qBound(0.01,sc->zmax() - 1,1000.00);
    setZValue(top); 
    update();
}







void TextLayer::Removehere()
{
   emit remid(id);
}

void TextLayer::CloneLayer()
{
   emit clonehere(); 
}

void TextLayer::Copy_Html_Plain()
{
    const QString txt = document()->clone()->toHtml("utf-8");
    QClipboard  *baseram = QApplication::clipboard();
    QMimeData *data = new QMimeData;
    data->setText(txt);
    baseram->setMimeData(data,QClipboard::Clipboard);
}

void TextLayer::Copy_Text_Plain()
{
    const QString txt = document()->clone()->toPlainText();
    QClipboard  *baseram = QApplication::clipboard();
    QMimeData *data = new QMimeData;
    data->setText(txt);
    baseram->setMimeData(data,QClipboard::Clipboard);
}

void  TextLayer::copyLayer()
{
    RichDoc dd = ReadActualItem();
    QString stream = SaveRichDoc(dd); /* QDataStream zu toBase64 fuer sql*/
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-layerrichdoc",stream.toUtf8());
    QClipboard *ramclip = QApplication::clipboard();
    ramclip->setMimeData(mimeData,QClipboard::Clipboard);
}


void TextLayer::SwapEdit()
{
    if (modus == Lock) {
    QMessageBox::information(0,tr("Layer status"),tr("Unlock first the layer to edit."));
    return;
    }
    
    bool canedit = mount->txtControl()->editable();
    if (canedit) {
        mount->txtControl()->edit(false);
        RestoreMoveAction();
    } else {
        mount->txtControl()->edit(true);
        EditModus();
    }
    
}

void TextLayer::SwapLockmodus()
{
    QApplication::restoreOverrideCursor();
    
    if (modus == Lock) {
        modus = Show;
        mount->txtControl()->edit(false);
         if (format == DIV_ABSOLUTE) {
         ///////
         }
        setFlag(QGraphicsItem::ItemIsSelectable,true);
        setFlag(QGraphicsItem::ItemIsFocusable,true);
        RestoreMoveAction();
        return;
    } else {
        modus = Lock;
        mount->txtControl()->edit(false);
        setFlag(QGraphicsItem::ItemIsSelectable,true);
        setFlag(QGraphicsItem::ItemIsFocusable,true);
    }
    
    update(boundingRect());
}


void TextLayer::RotateLayer( const int ro ) 
{ 
   Rotate = ro;
    qDebug() << "### RotateLayer " << ro;
   update();
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
    ////qDebug() << "### history id  ->" << index;
    RichDoc olditem = OpenRichDoc(history[index]);
    insert(olditem);
    EditModus();
    update();
}

void TextLayer::insert( RichDoc Rdoc )
{
    RestoreMoveAction();
    
    _doc = Rdoc.todoc();
    setStyle(Rdoc.style.split(";"),false);
    mount->txtControl()->setDocument(_doc,this);
    mount->txtControl()->RegisterResource(Rdoc.resource);
    LayerHightChecks();
    update(boundingRect());
    guiwait = Rdoc;
    LayerHightChecks();
}

void TextLayer::E_Reload()
{
    ///////mount->txtControl()->RegisterResource(guiwait.resource);
    LayerHightChecks();
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
    if (format != DIV_ABSOLUTE) {
    setFlag(QGraphicsItem::ItemIsMovable,false);
    emit recalcarea();
    }
    
    const qreal limits = boundingRect().width() + 20;
    if (areas.width() > limits) {
    return;
    }
    evesum++;
    //////////qDebug() << "### area " << areas.width() << "x" << areas.height() << "|" <<  evesum;
    ///////////qDebug() << "### area top left " << areas.topLeft();
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
    bool canedit = mount->txtControl()->editable();
    
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
    mount->txtControl()->paint_doc(painter,option,QBrush(bgcolor),boundingRect(),BorderPaint,currentprintrender);
    } else {
        qApp->beep();
		    qApp->beep();
    }
    
      
      if (modus == Lock && !canedit && !currentprintrender) {  /* writteln modus icon */
				painter->save();
				QPixmap pixbg(":/img/encrypted.png");
				painter->drawPixmap(QPointF(boundingRect().width() - 40,8),pixbg);
				painter->restore();
			}
      
   
    setTransform(ActualMatrixe(Rotate),false);
    
    
   ///////QGraphicsItem::paint(painter,option,widget);
}


QTransform TextLayer::ActualMatrixe( int r ) 
{
     QMatrix matrix;
     matrix.translate ( boundingRect().center().x() , boundingRect().center().y() );
     matrix.rotate(r);
     matrix.translate ( - boundingRect().center().x() , - boundingRect().center().y() );
     return QTransform(matrix);
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

int TextLayer::Ltype() const
{
    return format;
}

void TextLayer::setSelected( bool selected ) 
{
    if (!selected) {
         if (format != DIV_ABSOLUTE) {
         RestoreMoveAction();
         }
         
    } else {
        if (format != DIV_ABSOLUTE && modus != Lock) {
        EditModus(); 
        } else {
            if ( modus == Lock) {
            setFlag(QGraphicsItem::ItemIsMovable,false); 
            }
        }
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
    mount->txtControl()->edit(false);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    update(boundingRect());
    //////////////////qDebug() << "### reset ";
}


void TextLayer::EditModus() 
{
    modus == Edit;
    mount->txtControl()->edit(true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    update(boundingRect());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/*  events  enum CurrentModus{ Show, Edit, Move , Lock }; */

////////////////////////////////////////////////////////////////////////////////////////////////////
void TextLayer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (!IsSelectActive) {
    return;
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
    if (!IsSelectActive) {
    return;
    }
    
    if (modus == Lock) {
    return;
    }
    if (format == DIV_ABSOLUTE) {
    if (modus == Move ) {
        bool ChangeXY;
        QGraphicsItem::setCursor(Qt::SizeAllCursor);
        /* move from stop xy */
        //////qDebug() << " move layer status ...............";
         qreal WWxi = event->pos().x()  - event->lastPos().x();
         qreal HHyi = event->pos().y()  - event->lastPos().y();
         qreal diff = qMax ( HHyi , WWxi );
         if (diff > 0) {
         SetDimension( wi + WWxi , hi + HHyi );
         ChangeXY = true;
         } else {
             if (wi > 100 && hi > 20) {
                 if (WWxi < 0) {
                 SetDimension( wi - 1 ,hi);
                 } else if (HHyi < 0) {
                 SetDimension( wi ,hi - 1);
                 }
                 ChangeXY = true;
             }
         }
   
    } else if ( modus == MoveAll ) { 
        if (!mount->txtControl()->editable()) {
         QGraphicsItem::setCursor(Qt::ClosedHandCursor);  
         setPos(event->scenePos() - event->lastPos());
         event->accept();
        }
    }
   }
    
    ShowInfos();
    
    if (mount->txtControl()->editable()) {
    mount->txtControl()->procesevent(event);
    }
    
    
    QGraphicsItem::mouseMoveEvent(event); 
}

void TextLayer::ShowInfos()
{
   
   QString infoPart = QString("Zindex %1 type %2 ID = %3").arg(zValue()).arg(Ltype()).arg(id);
   setToolTip(QString("Layer %1mm x %2mm X=%4 mm Y=%3 mm %5")
                                .arg(ToUnit(wi,"mm"))
                                .arg(ToUnit(hi,"mm"))
                                .arg(ToUnit(pos().y(),"mm"))
                                .arg(ToUnit(pos().x(),"mm")).arg(infoPart)); 
    
}

void TextLayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::setCursor(Qt::ArrowCursor);
    QApplication::restoreOverrideCursor();
    
    if (!IsSelectActive) {
    return;
    }
    
    if (modus == Lock) {
    return;
    }
    
    if (modus == Move || modus == MoveAll ) {
    modus = Show;
    RestoreMoveAction();
    return;
    }
    
    if (mount->txtControl()->editable()) {
    mount->txtControl()->procesevent(event);
    }
    ShowInfos();
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
        mount->txtControl()->procesevent(event);            
        /* start to move item size hi x wi  */
        return;
    }
    
    if (format == DIV_ABSOLUTE && event->buttons() == Qt::LeftButton && 
        event->modifiers() == Qt::NoModifier  &&  
        modus == Show && !mount->txtControl()->editable()) {
        modus = MoveAll; 
        mount->txtControl()->procesevent(event);            
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
    
    if (Rotate > 0 ) {
    styles.append(QString("degree-rotation:%1; ").arg( Rotate ) );  
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
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    QRectF Srect = sc->sceneRect();
    #define ALPHAHTML(alpha) ((alpha)*254.99999999)
    format = DIV_AUTO;  /* default go auto and wi from scene rect */
    wi = Srect.width();
    setPos(QPointF(0,0));  /* next Y from scene */
    setZValue(0.);  /* auto default zero */
    QStringList find;
    find << "position" << "top" << "left" << "width" << "degree-rotation" << "opacity" << "height" << "background-color" << "z-index" << "id" << "border-width" << "border-color" << "border-style";  //////  border-color:#FFFF00; border-width:2px; border-style:solid;
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
    
    if (incss.value("degree-rotation").toInt() !=0) {
       Rotate =  incss.value("degree-rotation").toInt();
    }
    
    modus = Show;
    update();
      
}














