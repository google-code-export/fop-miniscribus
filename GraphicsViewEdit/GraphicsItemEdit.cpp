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
///// isSelected() IsSelectActive
#include "GraphicsItemEdit.h"
#include "mounttextprivate.h"
#include "GraphicsView.h"



TextLayer::TextLayer(const int layer_id , QGraphicsItem *parent , QGraphicsScene *scene )
    : QGraphicsItem(parent,scene),evesum(0),modus(Show),border(1.),currentprintrender(false),
    hi(Metric("30px")),wi(Metric("110px")),bgcolor(QColor(Qt::white)),SwapLockBreak(false),
    bordercolor(QColor(Qt::red)),Rotate(0),check_view_area_time(0),ActionHover(false),
    format(DIV_ABSOLUTE),mount(new TextController)
{
    mount->q = this;
    setAcceptsHoverEvents(true);
    wisub_border = wi + border;
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
/* on print true dont play hover and icon */
void TextLayer::SetPrintModus( bool e )
{
  currentprintrender = e;
  if (e) {
    setSelected(false);
  } else {
    setSelected(true);
  }
  update(boundingRect());
}

bool TextLayer::sceneEvent(QEvent *event)
{
    /* drag here */
    if ( event->type() == QEvent::GraphicsSceneDrop) {
        mount->txtControl()->procesevent(event);
        return true;
    } else if (event->type() == QEvent::GraphicsSceneDragMove ) {
        ///////QGraphicsSceneDragDropEvent *e = static_cast<QGraphicsSceneDragDropEvent *>(event);
        /////////qDebug() << "### muove ";
        
    } else if (event->type() == QEvent::DragEnter ) {
        ////////e = static_cast<QGraphicsSceneDragDropEvent *>(event);
        //////////qDebug() << "### entra ";
    }
    return QGraphicsItem::sceneEvent(event);
}


void TextLayer::setSelected( bool selected ) 
{
    if (id == 0) {
    IsSelectActive = false;
    QGraphicsItem::setSelected(false);
    return;
    } 
    
    IsSelectActive = selected;
    QGraphicsItem::setSelected(selected);
    if (modus == Lock) {
    QApplication::restoreOverrideCursor();
    return;
    }
    
    if (format == DIV_ABSOLUTE) {
    return;
    }
    
    
    if (selected) {
        ///////////qDebug() << "### id  " << id << "+" << editable() << format;
        if (format != DIV_ABSOLUTE && modus != Lock) {
        EditModus(); 
        } else {
        /* move able */          
        return;
        }
    } else {
         if (format != DIV_ABSOLUTE) {
         //////////qDebug() << "### id  " << id << "-";
         RestoreMoveAction();
         }
    }
    QApplication::restoreOverrideCursor();
    update();
}

QList<QAction *> TextLayer::MainActions()
{
  return mount->txtControl()->MainActions();
}


void TextLayer::undo()
{
   mount->txtControl()->undo();
   updatearea( boundingRect() );
}

void TextLayer::redo()
{
   mount->txtControl()->redo();
   updatearea( boundingRect() );
}



TextLayer::~TextLayer()
{
   _doc->clear();
   _doc = 0;
    mount = 0;
}

QRectF TextLayer::viewport_need()
{
    return CurrentCursorboundingRect;
}


void TextLayer::cursor_wake_up()
{
   
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





void TextLayer::setModus( CurrentModus  e)
{
   modus = e;
   update();
}

void TextLayer::cursorPositionChanged( const QTextCursor cur)
{
   //////////update();
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
    if (!IsSelectActive) {
    return;
    }
    Rotater *slider;
    QWidgetAction *widgetslider;
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
        actionSwapEdit->setChecked(canedit);
	      connect(actionSwapEdit, SIGNAL(triggered()),this,SLOT(SwapEdit()));
        RootMenu->addAction(actionSwapEdit);
    
        bool locked = false;
        if (modus == Lock && !canedit) { 
        locked = true;
        }
        
        if (canedit && format == DIV_ABSOLUTE ) {
        slider = new Rotater(Rotate,event->widget());
        widgetslider = new QWidgetAction(event->widget());
        widgetslider->setDefaultWidget(slider);
        RootMenu->addAction(widgetslider); 
        connect(slider, SIGNAL(rotater(int)),this, SLOT(RotateLayer(int)));
            
        
        }
        
        actionSwapLock = new QAction(tr("Lock Unlock Layer"),this);
        actionSwapLock->setIcon( W_modus() == Lock ? QIcon(":/img/encrypted.png") : QIcon(":/img/unlock.png"));
        actionSwapLock->setCheckable(true);
        actionSwapLock->setChecked ( W_modus() == Lock ? true : false );
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
        
        
        
        
        
        a = RootMenu->addAction(tr("Open Layer file"), this, SLOT(OpenFilelayer()));
        a->setIcon(QIcon(":/img/bringtofront.png"));
        
        
        
        
    }
    
    a = RootMenu->addAction(tr("Save as Layer file"), this, SLOT(SaveFilelayer()));
    a->setIcon(QIcon(":/img/bringtofront.png"));
    
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/copy.png")),tr( "Copy full text plain" ), this , SLOT( Copy_Text_Plain() ) );
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/copy.png")),tr( "Copy xhtml text plain" ), this , SLOT( Copy_Html_Plain() ) );
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/copy.png")),tr( "Copy current Layer" ), this , SLOT( copyLayer() ) );
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/alayer_clone.png")),tr( "Clone this Layer" ), this , SLOT( CloneLayer() ) );
    RootMenu->addAction(QIcon(QString::fromUtf8(":/img/button_cancel.png")),tr( "Remove Layer" ), this , SLOT( Removehere() ) );
    RootMenu->exec(event->screenPos());
    
    
   
    /////delete slider;
    slider = 0;
    //////delete widgetslider;
    widgetslider = 0;
    delete RootMenu; 
}



void TextLayer::seTBack()
{
      
    qreal maxi = 0.1;
    if (format != DIV_AUTO ) {
      maxi = 111.1;  
    }
    qreal minimums = 0.1;
    if (format != DIV_AUTO ) {
      minimums = 10.1;  
    }
    
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    qreal backs = qBound(minimums,sc->zmin() - 0.1,maxi);
    setZValue(backs);
    update();
    
}
void TextLayer::seTFront()
{
    qreal maxi = 0.1;
    if (format != DIV_AUTO ) {
      maxi = 111.1;  
    }
    qreal minimums = 0.1;
    if (format != DIV_AUTO ) {
      minimums = 10.1;  
    }
    
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    qreal top = qBound(minimums,sc->zmax() + 0.1,maxi);
    setZValue(top); 
    update();
}







void TextLayer::Removehere()
{
    setData (ObjectNameEditor,id);
    qDebug() << "### TextLayer rem " << id;
    GraphicsScene *sc = qobject_cast<GraphicsScene *>(scene());
    sc->remid(id);
    emit recalcarea();
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
    ////////////QMessageBox::information(0,tr("Layer status"),tr("Unlock first the layer to edit."));
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
    QAction *invoice = qobject_cast<QAction *>(sender());
    if (invoice) {
        if (invoice->isChecked()) {
           RestoreMoveAction();
           modus = Lock;
           setFlag(QGraphicsItem::ItemIsSelectable,true);
           setFlag(QGraphicsItem::ItemIsFocusable,true);  
           update(boundingRect());
           return;
        } else {  
        modus = Show;
        setFlag(QGraphicsItem::ItemIsSelectable,true);
        setFlag(QGraphicsItem::ItemIsFocusable,true);
        RestoreMoveAction();
        update(boundingRect());
        return;
        } 
    } else {   
    RestoreMoveAction();
    modus = Lock;
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);  
    update(boundingRect());
    }
}

void TextLayer::BreakRem()
{
  SwapLockBreak = false;  
}


void TextLayer::RotateLayer( const int ro ) 
{ 
   Rotate = ro;
    ////////////qDebug() << "### RotateLayer " << ro;
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
                ft.setWidth(wi);
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



TextController::TextController()
  : device(0)
{ 

}


/* only one connect */
TextWriter *TextController::txtControl() const
{
    if (!device) {
       TextLayer *that = const_cast<TextLayer *>(q); 
       device = new TextWriter(that);
       connect(device, SIGNAL(q_cursor_newPos() ),q, SLOT(cursor_wake_up()));
       connect(device, SIGNAL(q_update(QRectF) ),q, SLOT(updatearea(QRectF)));
       connect(device, SIGNAL(q_cursor_area(QRectF,qreal) ),q, SLOT(cursor_area(QRectF,qreal)));
    }
    return device;
}


void TextLayer::read()
{
        RichDoc itemdoc = ReadActualItem();
        QDateTime timer1( QDateTime::currentDateTime() );
        history.insert(timer1.toTime_t(),SaveRichDoc(itemdoc));
}









int TextLayer::getXcursor()
{
    QTextBlock block = textCursor().block();
    int pos = textCursor().position() - block.position();
    const QTextLayout *layout = block.layout();
    int x = 0;
    QTextLine line = layout->lineForTextPosition(pos);
    if (line.isValid()) {
        x = line.cursorToX(pos);
    } 
    else {
        x = -1; // delayed init.  Makes movePosition() call setX later on again.
    }
    return x;
}


void TextLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    Q_UNUSED(widget);
    
    if (!mount) {
        qApp->beep();
		    qApp->beep();
    }
    
    if (currentprintrender) {
        //////qDebug() << "### paint by printer xxxxx ";
    }
    painter->setRenderHint(QPainter::TextAntialiasing);
    
    /////QPainter::TextAntialiasing  QPainter::Antialiasing
    
    bool canedit = mount->txtControl()->editable();
    wisub_border = wi + border;
    const qreal minimumHi = Metric("15mm");
    const qreal maximHi = Metric("210mm");
     /* Layer Background draw! */
		const qreal hightlengh =  mount->txtControl()->boundingRect().height();
    if (format != Lock )  {  
        
                if (hightlengh > hi ) {
                  hi = hightlengh;
                }
                
                if (format == DIV_AUTO ) {
                   hi = qMax(minimumHi, hightlengh);
                }
                   if (hi > maximHi ) {
                       AlertSize = true;
                       /////////////////RestoreMoveAction();
                   } else {
                       AlertSize = false;
                   }
                
                
                
                
    } 
    QPen BorderPaint;
    if (border > 0) {
        BorderPaint = QPen(QBrush(bordercolor),border,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin);
    } else {
        BorderPaint = QPen(Qt::NoPen);
    }
    
    mount->txtControl()->paint_doc(painter,option,QBrush(bgcolor),boundingRect(),BorderPaint,currentprintrender);
    
      
      if (modus == Lock && !canedit && !currentprintrender) {  /* writteln modus icon */
				painter->save();
				QPixmap pixbg(":/img/encrypted.png");
				painter->drawPixmap(QPointF(boundingRect().width() - 40,8),pixbg);
				painter->restore();
			}
      
      int hoverborder = 1;
      if (border > 1 && !currentprintrender) {
        hoverborder =  border + 1; 
      }
      
    if (ActionHover && !currentprintrender) {
        painter->save();
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::red),hoverborder,Qt::DotLine,Qt::RoundCap)	);
        painter->drawRect(boundingRect());    ////////  mount->txtControl()->boundingRect() 
        painter->restore();  
    }
    setTransform(ActualMatrixe(Rotate),false);
}


QTransform TextLayer::ActualMatrixe( int r ) 
{
     QMatrix matrix;
     matrix.translate ( boundingRect().center().x() , boundingRect().center().y() );
     matrix.rotate(r);
     matrix.translate ( - boundingRect().center().x() , - boundingRect().center().y() );
     return QTransform(matrix);
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
    
    if (id == 0) {
    IsSelectActive = false;
    QGraphicsItem::setSelected(false);
    setFlag(QGraphicsItem::ItemIsSelectable,false);
    setFlag(QGraphicsItem::ItemIsFocusable,false);
    return;
    }
    
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    update(boundingRect());
    //////////qDebug() << "### reset RestoreMoveAction ";
}


void TextLayer::EditModus() 
{
    if (id == 0) {
    return;
    }
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
         SetDimension(wi,hi);
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
   infoPart.append(QString("\ndocument width = %1p.\n").arg(document()->size().width()));
    ///////document()->adjustSize()  QSizeF size() const   document()->size().width()
    
    
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
    LayerHightChecks();
  
    
    if (!IsSelectActive) {
    return;
    }
    
    if (modus == Lock) {
    return;
    }
    
    bool cursordrag = textCursor().hasSelection();
    
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
}

void TextLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (modus == Lock) {
    return;
    }
    ActionHover = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void TextLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (modus == Lock) {
    return;
    }
    ActionHover = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}




void TextLayer::keyPressEvent( QKeyEvent * event ) 
{
    
    
    if (!IsSelectActive) {
    return;
    }
    
    if (modus == Lock) {
    return;
    }
    if (mount->txtControl()->editable()) {
    mount->txtControl()->procesevent(event);
    LayerHightChecks();
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
    //////QTextFrame  *Tframe = document()->rootFrame();
    ///////QTextFrameFormat ft = Tframe->frameFormat();
    ////////int xBottomMargin =  ToUnit(ft.bottomMargin(),"px");
    //////////int xTopMargin = ToUnit(ft.topMargin(),"px");
    ////////int xRightMargin = ToUnit(ft.rightMargin(),"px");
    //////////////int xLeftMargin = ToUnit(ft.leftMargin(),"px");
    
    QTextFrame  *Tframe = document()->rootFrame();
    QTextFrameFormat ft = Tframe->frameFormat();
    ft.setWidth(wi);
    Tframe->setFrameFormat(ft);
    
    
    ///////////const QString padding = QString("padding:%1px;").arg(ft.padding());
    
    ////////////const QString margin = QString("margin:%1px %2px %3px %4px;").arg(xTopMargin).arg(xRightMargin).arg(xBottomMargin).arg(xLeftMargin); 
    qreal realhightdoc = ToUnit(hi,"px");
    qreal realwidhtdoc = ToUnit(wi,"px");
    #define ALPHAHTML(alpha) ((alpha)/254.99999999)
    if (format == DIV_ABSOLUTE) {
    styles = "position:absolute; overflow:hidden; top:"+QString("%1px").arg(ToUnit(pos().y(),"px"))+"; left:"+QString("%1px").arg(ToUnit(pos().x(),"px"))+"; width:"+QString("%1px").arg(ToUnit(wi,"px"))+"; height:"+QString("%1px").arg(ToUnit(hi,"px"))+"; ";
    } else {
    styles = "min-height:"+QString("%1px").arg(hi)+";"+QString("top:%1px").arg(ToUnit(pos().y(),"px"))+";"+QString("left:%1px").arg(ToUnit(pos().x(),"px"))+";";   
    }
    ////////styles.append(margin);
    /////////styles.append(padding);
    
   
    
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
    styles.append(QString("border-width:%2px;border-style:solid;border-color:%1;").arg(bordercolor.name()).arg(ToUnit(border,"px")));
    }
    
    if (zValue() > 1) {
    styles.append(QString("z-index:%1;").arg(qRound(zValue())));
    }
    
    
    if (modus == Lock ) {
    styles.append(QString("l-lock:1; ") );
    }
    
    
    RichDoc Rdoc;
            Rdoc.Register(document(),mount->txtControl()->imglist(),styles);
    return Rdoc;
}



void TextLayer::insert( RichDoc Rdoc , bool clone )
{
    RestoreMoveAction();
    _doc = Rdoc.todoc();
    setStyle(Rdoc.style.split(";"),clone);
    ///////////mount->txtControl()->RegisterResource(Rdoc.resource);
    update(boundingRect());
    guiwait = Rdoc;
    LayerHightChecks();
}


/*
QGraphicsItem(parent,scene),evesum(0),modus(Show),border(1.),currentprintrender(false),
    hi(Metric("22mm")),wi(Metric("55mm")),bgcolor(QColor(Qt::white)),
    bordercolor(QColor(Qt::red)),Rotate(0),check_view_area_time(0),
    format(DIV_ABSOLUTE),mount(new TextController)

*/


void TextLayer::setStyle( QStringList syle , bool fromclone )
{
    #define ALPHAHTML(alpha) ((alpha)*254.99999999)
    QRectF Srect = scene()->sceneRect();
    if (_doc) {
        
            QTextFrame  *Tframe1 = _doc->rootFrame();
            QTextFrameFormat rootformats1 = Tframe1->frameFormat();
            rootformats1.setWidth(Srect.width()); /* margin padding on doc */
            Tframe1->setFrameFormat(rootformats1);
        
        
    //////_doc->setTextWidth(Srect.width());
   ///////_doc->setPageSize(QSizeF(Srect.width(),Srect.width() / 2));
    mount->txtControl()->setDocument(_doc,this);
    }
    QTextFrame  *Tframe2 = document()->rootFrame();
    QTextFrameFormat rootformats2 = Tframe2->frameFormat();
    
    qreal maxi = 0.1;
    if (format != DIV_AUTO ) {
      maxi = 111.1;  
    }
    qreal minimums = 0.1;
    if (format != DIV_AUTO ) {
      minimums = 10.1;  
    }
    
    bgcolor = QColor(Qt::white);
    bgcolor.setAlpha(0);
    border = 0;
    currentprintrender = false;
    format = DIV_AUTO;  /* default go auto and wi from scene rect */
    wi = Srect.width() - _DEBUGRANGE_WI_;
    
    setPos(QPointF(_DEBUGRANGE_WI_,0));  /* next Y from scene */
    setZValue(minimums);  /* auto default zero */
    QStringList find;
    find << "position" << "top" << "left" << "width" << "min-height" << "degree-rotation" << "opacity" << "height" << "background-color" << "z-index" << "id" << "border-width" << "border-color" << "border-style" << "l-lock";  //////  border-color:#FFFF00; border-width:2px; border-style:solid;
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
    
     if (Metric(incss.value("min-height").toString()) > 10) {
        hi = Metric(incss.value("min-height").toString());
      }
    
    if (!incss.value("left").toString().isEmpty()) {
        setPos(QPointF(Metric(incss.value("left").toString()),Metric(incss.value("top").toString())));
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
            
            qreal Ziindex = qBound(minimums,incss.value("z-index").toDouble(),maxi);
            if (fromclone) {
              setZValue(Ziindex + 1.1);
            } else {
               setZValue(Ziindex); 
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
    
    
    const int lockstatus = incss.value("l-lock").toInt();
    
    
    
    
    wisub_border = wi - (border * 2);
    document()->setTextWidth(wisub_border);
    rootformats2.setWidth(wi);
    Tframe2->setFrameFormat(rootformats2);
    TextboundingRect = mount->txtControl()->boundingRect();
    LayerHightChecks();
    document()->setPageSize(QSizeF(wi,hi));
    modus = Show;
    update();
    
    
    if (lockstatus !=0 ) {
    QTimer::singleShot(600, this, SLOT(SwapLockmodus()));
    }
    
      
}

void TextLayer::LayerHightChecks() 
{
    wisub_border = wi - (border * 2);
    document()->setTextWidth(wisub_border);
    
        qreal txthight = mount->txtControl()->boundingRect().height();
        if (txthight > hi) {
        SetDimension(wi,txthight);
        document()->setTextWidth(wisub_border);   
        QTextFrame  *Tframe = document()->rootFrame();
        QTextFrameFormat ft = Tframe->frameFormat();
        ft.setWidth(wi);
        Tframe->setFrameFormat(ft);  
        document()->adjustSize();
        emit recalcarea();
        }
        wisub_border = wi - (border * 2);
        document()->adjustSize();
}


void TextLayer::updatearea( const QRectF areas )
{
    const qreal limits = boundingRect().width();
    if (areas.width() > limits) {
    return;
    }
    evesum++;
    QRect simprect = areas.toRect();
    //////////qDebug() << "### area " << areas.width() << "x" << areas.height() << "|" <<  evesum;
    /////qDebug() << "### updatearea " << simprect;
    update(areas);
}

/* CurrentCursorboundingRect   viewport to enable to show! */

void TextLayer::cursor_area( const QRectF areas , const qreal ip )
{
    QDateTime timer1( QDateTime::currentDateTime() );
    const uint now = timer1.toTime_t();
    QRect simprect = areas.toRect();
    if (check_view_area_time == 0) {
       check_view_area_time = now;  /* first run */ 
    }
    if ( now > (check_view_area_time + 2) ) { 
    check_view_area_time = now;
            ///////const int Xpos = getXcursor();
            evesum++;
            if (ip > 0 && evesum%3 && areas.y() > 0) {
            CurrentCursorboundingRect = QRectF(ip,areas.y(),300,300);
            ///////qDebug() << "### CurrentCursorboundingRect " << CurrentCursorboundingRect;
            }
            
    /////qDebug() << "### cursor_area " << simprect << "," << ip;
    }
}










