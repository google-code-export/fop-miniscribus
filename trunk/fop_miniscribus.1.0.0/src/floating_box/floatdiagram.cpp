#include "floatdiagram.h"


FloatDiagram::FloatDiagram( int idx , QMenu *Diss , QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent, scene)
{
d = new QVimedit();
d->hide();
setAcceptsHoverEvents(true);
itpararent = parent;
AlphaColor = 255;
scenes = scene;
PaintLockStaus = true;
setFocus ( Qt::OtherFocusReason ); 
id = idx;
setProperty ("_ID_layer",QVariant(idx));
PMenu = Diss;
BGColor = QColor ( Qt::white );
BorderDicks = 0.0;
MarginColor = QColor ( BGColor );
wi = 100;
hi = 20;
BoxRotate = 0;
ZIndex = qrand() % 256;
setZValue(qBound(10.01,ZIndex,10000.01));
setToolTip(DefaultMessage);
modus = M_MOVE_ALL;  
baseram = QApplication::clipboard();     
}

qreal FloatDiagram::zValue() const  
{
   return ZIndex;  
}

void  FloatDiagram::setZValue( const qreal zindex ) 
{
    ZIndex = zindex;
    DefaultMessage = QString("ID=%1 Zindex=%2").arg(id).arg(ZIndex);
    QGraphicsItem::setZValue(zindex);
}


/* clone a floating elements */
FloatDiagram *FloatDiagram::clone() const
{
    FloatDiagram *fdoc = new FloatDiagram(id+1000,PMenu,itpararent,scenes);
    fdoc->setDocument( document()->clone() );
    fdoc->SetBGColor( BGColor );
    fdoc->SetupRoq( BoxRotate );
    fdoc->SetBorder(MarginColor,BorderDicks);
    fdoc->SetDimension(wi,hi);
    fdoc->SetModus(modus);
    fdoc->SetResource( ImageContainer );  
    fdoc->setPos(QPointF(pos().x() - 33,pos().y() - 33));
    return fdoc;
}


void FloatDiagram::SetMarginColor() {
    bool ok;
     QRgb col = QColorDialog::getRgba(BGColor.rgba(),&ok);
     if (!ok) {
        return; 
     }
     MarginColor = QColor( col );
     MarginColor.setAlpha(255);
     
     if (BorderDicks < 1) {
         BorderDicks =1;
     }
     
     
     update();
     emit Rebuild();
    
}



void FloatDiagram::FlyFonts()
{
  bool ok;
  QFont fontyour = QFontDialog::getFont(&ok,font(),0);
  if (ok) {
      PaintAllFonts( fontyour );
  }
}

void FloatDiagram::PaintAllFonts( QFont f )
{
      d->setDocument ( document()->clone() );
      d->selectAll();
      QTextCursor c = d->textCursor();
      QTextCharFormat  format = c.charFormat();
      format.setFont ( f );
      c.setCharFormat(format);
      setDocument ( d->document() );
}

QString FloatDiagram::toDivHtml( QFileInfo f )
{
    #define ALPHAHTML(alpha) ((alpha)/254.99999999)
    QString masterpath = f.absolutePath()+"/"+FOPIMAGEDIR;
    QString styles = "position:absolute; top:"+QString("%1px").arg(pos().y())+"; left:"+QString("%1px").arg(pos().x())+"; width:"+QString("%1px").arg(wi)+"; height:"+QString("%1px").arg(hi)+"; ";
    if (AlphaColor < 44) {
    styles.append(QString("background-color:transparent; ") );
    } else {
    styles.append(QString("background-color:%1; ").arg( BGColor.name() ) );  
    }
    
    int alFaCol = qBound(0,AlphaColor,256);
    qreal percentos = ALPHAHTML( alFaCol );
    if (percentos < 1 && AlphaColor > 44) {
    QString Tnetto = QString("%1").arg(percentos, 0, 'f', 4);
    styles.append(QString("opacity:%1; filter: alpha(opacity=%1); -moz-opacity:%1; ").arg( Tnetto) );
    }
    
  
    
    if (BorderDicks > 0 ) {
    styles.append(QString("border:%2px solid %1; ").arg(MarginColor.name()).arg(BorderDicks));
    }
    styles.append(QString("z-index:%1;").arg(QString("%1").arg(zValue()).replace(".","")));
    
    QDomDocument doc;
    QDomElement blox = doc.createElement("div");
                blox.setAttribute ("id",QString("layer_%1").arg(id));
                blox.setAttribute ("style",styles);
    doc.appendChild(blox);
    
   QString errorStr;
    int errorLine, errorColumn;
    QDomDocument exdoc;
    if (!exdoc.setContent(document()->toHtml(),false, &errorStr, &errorLine, &errorColumn)) {
        return doc.toString(5);
    }
    QDomElement root = exdoc.documentElement();
    QDomNode child;
    QDomElement e = root.firstChildElement("body");
    ImageFindertoHtml(e,masterpath);
    
    
    child = e.firstChild();
       while ( !child.isNull() ) {
           if ( child.isElement() ) {
            blox.appendChild(doc.importNode(child,true).toElement());
           } else if (child.isText()) {
             blox.appendChild(doc.createTextNode(child.toText().data()));
           }
         child = child.nextSibling();            
       }
    
    return doc.toString(5);
}

void FloatDiagram::ImageFindertoHtml( QDomElement e , const QString imagepath  )
{
    QString saveimageto = FOPIMAGEDIR;
    
    if (e.tagName().toLower() == "img") {
    saveimageto.append(SaveHtmlImage(e.attribute("src"),imagepath));
    e.setAttribute("src",saveimageto);
    return;  
    }
    
   QDomNode child = e.firstChild();
        while ( !child.isNull() ) {
                if ( child.isElement() ) {
                    QDomElement el = child.toElement(); 
                    if (el.tagName().toLower() == "img") {
                        saveimageto.append(SaveHtmlImage(el.attribute("src"),imagepath));
                        el.setAttribute("src",saveimageto);
                    } else {
                      ImageFindertoHtml(el,imagepath);  
                    }
                }    
            child = child.nextSibling();        
        }
}

QString  FloatDiagram::SaveHtmlImage( const QString urlimage , const QString path  ) 
{
      QString finame;
    
      QDir dira(path);
      dira.mkpath(path);
      QMapIterator<QString,QVariant> i(ImageContainer);
         while (i.hasNext()) {
             i.next();
             if ( i.key() == urlimage ) {
                 
                 finame = FilenameAllow( i.key() + ".png");
                 
                 if (urlimage.startsWith("/svg/",Qt::CaseInsensitive)) {
                     QPixmap trassvg = RenderPixmapFromSvgByte(  i.value().toByteArray() ); 
                     trassvg.save(path + finame,"PNG",100);
                     return finame;                   
                 } else {
                     QPixmap allformat;
                     allformat.loadFromData( i.value().toByteArray() );
                     allformat.save(path + finame,"PNG",100);
                     return finame;
                 }
                 
                 
                                          
                                         
             }
         } 
         
         return QString();
 
}







/* fill the xml fop document */
QDomElement FloatDiagram::GetBaseLayerElement( QDomDocument docwritteln )
{
    QTextFrameFormat rootformat = document()->rootFrame()->frameFormat();
    QDomElement flo = docwritteln.createElement("fo:block-container");
    flo.setAttribute ("absolute-position","absolute");
    flo.setAttribute ("width",QString("%1pt").arg(wi));
    flo.setAttribute ("id",QString("layer_%1").arg(id));
    flo.setAttribute ("height",QString("%1pt").arg(hi));
    if (AlphaColor == 0) {
    AlphaColor = 1;
    }
    int alFaCol = qBound(0,AlphaColor,256);
    if (alFaCol > 1) {
    flo.setAttribute ("opacity",alFaCol);
    flo.setAttribute ("background-color",BGColor.name());
    } else {
    flo.setAttribute ("opacity","1");
    }
    
    flo.setAttribute ("top",QString("%1pt").arg(pos().y()));
    flo.setAttribute ("left",QString("%1pt").arg(pos().x()));
    if (BoxRotate >0) {
     flo.setAttribute ("rotate",BoxRotate);
    }
    ////////////flo.setAttribute ("padding-end",QString("%1pt").arg(rootformat.padding()));
    ///////////flo.setAttribute ("padding-start",QString("%1pt").arg(rootformat.padding()));
    ///////////flo.setAttribute ("padding-after",QString("%1pt").arg(rootformat.padding()));
    /////////////flo.setAttribute ("padding-before",QString("%1pt").arg(rootformat.padding()));
    flo.setAttribute ("margin-right",QString("%1pt").arg(rootformat.rightMargin()));
    flo.setAttribute ("margin-top",QString("%1pt").arg(rootformat.topMargin()));
    flo.setAttribute ("margin-bottom",QString("%1pt").arg(rootformat.bottomMargin()));
    flo.setAttribute ("margin-left",QString("%1pt").arg(rootformat.leftMargin()));
    if (BorderDicks >0) {
    flo.setAttribute ("border-width",QString("%1pt").arg(BorderDicks));
    flo.setAttribute ("border-start-color",MarginColor.name());
    }
    if (modus == M_LOCK) {
    flo.setAttribute ("lock","1");
    }
    flo.setAttribute ("z-index",zValue());
    flo.setAttribute ("imgcount",ImageContainer.size());
    return flo;
}

QRectF FloatDiagram::boundingRect() const
{
   QRectF now = QGraphicsTextItem::boundingRect();  /* actual setting hi -  wi */
return QGraphicsTextItem::boundingRect().adjusted(0,0, wi - now.width(), hi - now.height());
}
/* height from layer on keyPressEvent  */

int FloatDiagram::DocumentHighgtActual() 
{
    int HightLayerSumms = 0;
    QAbstractTextDocumentLayout *Layout = document()->documentLayout();
     QTextFrame  *Tframe = document()->rootFrame();
        QTextFrame::iterator it;
        for (it = Tframe->begin(); !(it.atEnd()); ++it) {
         QTextBlock para = it.currentBlock();
             if (para.isValid()) {
                HightLayerSumms += Layout->blockBoundingRect(para).height();
             }
        }
        
    return HightLayerSumms;
}


void FloatDiagram::LayerHightChecks() 
{
        int txthight = DocumentHighgtActual();
        if (txthight > hi) {
        SetDimension(wi,txthight);
        }
}


void FloatDiagram::EditAdvanceModus() {
    if (modus == M_LOCK || modus == M_ADV_EDITOR ) {
    QMessageBox::warning(0, tr("Floating Box Message"),tr("This Item is Lock,  Unlock to edit!"));
    return;
    }
     modus = M_ADV_EDITOR;
     editTXT =  new WymsingTxt(boundingRect(),BGColor,MarginColor,BorderDicks,BoxRotate,document()->clone(),ImageContainer,this);
     editTXT->XxAxys->setValue(BoxRotate);
     editTXT->SetInternalLink(TargetInternalLink);
     connect(editTXT, SIGNAL(RetocValues(QRectF,qreal,QColor,QColor,QTextDocument*,TypImageContainer)), this, SLOT(InValues(QRectF,qreal,QColor,QColor,QTextDocument*,TypImageContainer)));
     connect(editTXT, SIGNAL(TransnRotation(int)), this, SLOT(SPTransForm(int)));
     int ready = editTXT->exec();
     modus = M_EDITOR;
}

void  FloatDiagram::InValues( QRectF mbox ,qreal margin ,QColor bg , QColor margincolor , QTextDocument * d , TypImageContainer block )  {
    BGColor = bg; 
    MarginColor = margincolor;
    AlphaColor = bg.alpha();
    BorderDicks = margin;
    setDocument(d);
    SetDimension(  mbox.width() , mbox.height() );
    SetResource(block);
    update();
}
void  FloatDiagram::SPTransForm( int rot ) 
{
        BoxRotate = rot;
        update();
        emit Rebuild();
 }

/* Paste text */
void FloatDiagram::Tpaste() {
    const QMimeData *md = QApplication::clipboard()->mimeData();
    if (md->hasText()) {
        textCursor().insertText(md->text());
    } else if (md->hasHtml()) {
        QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(md->html());
         textCursor().insertFragment(fragment);
    }
}

/*  text select */
void FloatDiagram::TselectAll() {
   textCursor().select(QTextCursor::Document);
    
    //////QGraphicsTextItem::setFont(fontyour);
}

/* copy text */
void FloatDiagram::Tcopy() {
    baseram->setText ( textCursor().selectedText() , QClipboard::Clipboard ); 
}

void FloatDiagram::RestoreMoveAction() {
    modus = M_EDITOR;
    setTextInteractionFlags(Qt::NoTextInteraction);
    QApplication::restoreOverrideCursor();
    setSelected(true);
}

void FloatDiagram::SetNewBGColor() {
    bool ok;
     QRgb col = QColorDialog::getRgba(BGColor.rgba(),&ok);
     if (!ok) {
        return; 
     }
     AlphaColor =  qAlpha ( col );
     BGColor = QColor( col );
     BGColor.setAlpha(AlphaColor);
     update();
     emit Rebuild();
    
}




/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/

void FloatDiagram::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    QPointF Gisose( event->scenePos() - pos() ); 
    if (modus == M_LOCK || modus == M_MOVE_ALL ) {
    return;
    }
    if (modus == M_WRITTELN ) {
    RestoreMoveAction();
    return;
    }
    if ( event->modifiers() == Qt::NoModifier  &&  modus == M_EDITOR ) {
    modus = M_WRITTELN;
    /////////std::cout << "mouseDoubleClickEvent  entra in writteln 55 " << std::endl;
    ////////////std::cout << "mouseDoubleClickEvent a " << id  << " m->" << modus <<  std::endl;
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    mouseEvent->setAccepted(true);
    mouseEvent->setPos(event->pos());
    mouseEvent->setScenePos(event->scenePos());
    mouseEvent->setScreenPos(event->screenPos());
    mouseEvent->setButtonDownPos(Qt::LeftButton,event->buttonDownPos(Qt::LeftButton));
    mouseEvent->setButtonDownScreenPos(Qt::LeftButton,event->buttonDownScreenPos(Qt::LeftButton));
    mouseEvent->setButtonDownScenePos(Qt::LeftButton,event->buttonDownScenePos(Qt::LeftButton));
    mouseEvent->setWidget(event->widget());
    QGraphicsTextItem::mousePressEvent(mouseEvent);
    delete mouseEvent;
    update();
    QApplication::restoreOverrideCursor();
    return;
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void FloatDiagram::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    
    if (modus == M_LOCK | modus == M_WRITTELN ) {
    return;
    }
   //////////// std::cout << "mouseMoveEvent a " << id  << " m->" << modus <<  std::endl;
    
    
    
     if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier && modus == M_MOVE_ALL ) {
         bool ChangeXY = false;
         /////////////QApplication::setOverrideCursor(QCursor(Qt::SizeFDiagCursor));
         setToolTip(tr("Normal Move action - CTRL + click to change size."));
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
         
         if (ChangeXY) {
         event->accept();
         update();
         emit Rebuild();
         }
     }
     
     
    if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::NoModifier && modus == M_MOVE_ALL ) {
        setPos(event->scenePos() - event->lastPos());
        event->accept();
        emit Rebuild();
        return;
     }
     
     if (!event->isAccepted()) {
      QGraphicsItem::mouseMoveEvent(event);
     } 
}



void FloatDiagram::keyPressEvent ( QKeyEvent * event ) 
{
    if (!isSelected()) {
    return;
    }
    
    if (modus == M_LOCK) {
    return;
    }
    LayerHightChecks();
    QGraphicsTextItem::keyPressEvent(event);
}


void FloatDiagram::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QApplication::restoreOverrideCursor();
    if (!isSelected()) {
    return;
    }
    
    if (modus == M_LOCK || modus == M_WRITTELN ) {
    return;
    }
    
    if (modus != M_WRITTELN || modus != M_ADV_EDITOR ) {
    RestoreMoveAction();
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}


void FloatDiagram::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
    return;
    }
    
    if (modus == M_LOCK) {
    //////////setSelected(false);
    return;
    }
    if (modus != M_WRITTELN) {
    modus = M_MOVE_ALL;
    }
    update();
    ///////////////////std::cout << "mousePressEvent"  << id  << " m->" << modus <<  std::endl;
    QGraphicsItem::mousePressEvent(event);
}

/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/
/* ############################### mouse action ###############################################*/



void FloatDiagram::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (modus == M_LOCK || modus == M_WRITTELN ) {
    return;
    }
    modus = M_HOVER;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void FloatDiagram::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (modus == M_LOCK || modus == M_WRITTELN ) {
    return;
    }
    modus = M_EDITOR;
    QGraphicsItem::hoverLeaveEvent(event);
}

void  FloatDiagram::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) 
{
    scene()->clearSelection();
    QApplication::restoreOverrideCursor();
    setSelected(true);
    bool ihavemake = false;
    
    if (modus == M_WRITTELN) {
        ihavemake = true;
    Aundo = new QAction(tr("Undo"),this);
    Aundo->setIcon(QIcon(QString::fromUtf8(":/img/editundo.png")));
    Aundo->setShortcut(QKeySequence::Undo);
    connect(Aundo, SIGNAL(triggered()), document(),  SLOT(undo()));
        
    Aredo = new QAction(tr("Redo"),this);
    Aredo->setIcon(QIcon(QString::fromUtf8(":/img/editredo.png")));
    Aredo->setShortcut(QKeySequence::Redo);
    connect(Aredo, SIGNAL(triggered()),document(),  SLOT(redo()));
    
    Apaste = new QAction(tr("Paste"),this);
    Apaste->setIcon(QIcon(QString::fromUtf8(":/img/paste.png")));
    Apaste->setShortcut(QKeySequence::Paste);
    connect(Apaste, SIGNAL(triggered()), this,  SLOT(Tpaste()));
    
    Acopy = new QAction(tr("Copy"),this);
    Acopy->setIcon(QIcon(QString::fromUtf8(":/img/copy.png")));
    Acopy->setShortcut(QKeySequence::Copy);
    connect(Acopy, SIGNAL(triggered()), this,  SLOT(Tcopy()));
    
    
    Aselall = new QAction(tr("Select All"),this);
    //////Aselall->setIcon(QIcon(QString::fromUtf8(":/img/textpointer.png")));
    Aselall->setShortcut(QKeySequence::SelectAll);
    connect(Aselall, SIGNAL(triggered()), this,  SLOT(TselectAll()));
    
    Asetmyfont = new QAction(tr("Set Font (all text from layer ID:%1)").arg(id),this);
    Asetmyfont->setIcon(QIcon(QString::fromUtf8(":/img/textpointer.png")));
    connect(Asetmyfont, SIGNAL(triggered()), this,  SLOT(FlyFonts()));
    
    
     QPixmap pixbg(22, 22);
     pixbg.fill(BGColor);
     
       QPixmap pixma(22, 22);
     pixma.fill(MarginColor);
    
    Abgcolor = new QAction(tr("BackGround Color"),this);
    Abgcolor->setIcon(pixbg);
    connect(Abgcolor, SIGNAL(triggered()), this,  SLOT(SetNewBGColor()));
    
    AMacolor = new QAction(tr("Border Color"),this);
    AMacolor->setIcon(pixma);
    connect(AMacolor, SIGNAL(triggered()), this,  SLOT(SetMarginColor()));
    
    PMenu->addAction(Aundo);
    PMenu->addAction(Aredo);
    PMenu->addAction(Apaste);
    PMenu->addAction(Acopy);
    PMenu->addAction(Aselall);
    PMenu->addAction(Asetmyfont);
    PMenu->addAction(Abgcolor);
    PMenu->addAction(AMacolor);
    }
    
    PMenu->exec(event->screenPos());
    if (ihavemake) {
    PMenu->removeAction ( Aundo );
    PMenu->removeAction(Aredo);
    PMenu->removeAction(Apaste);
    PMenu->removeAction(Acopy);
    PMenu->removeAction(Aselall); 
    PMenu->removeAction(Asetmyfont);
    PMenu->removeAction(Abgcolor);
    PMenu->removeAction(AMacolor);
    }
}





/*

   //////////qDebug() << "dragPosition:" << id <<  " event->scenePos->" << event->scenePos();
        ///////////qDebug() << "dragPosition:" << id <<  " event->pos->" << event->pos();
        //////////qDebug() << "dragPosition:" << id <<  " event->pos->" << event->lastPos();

*/








void FloatDiagram::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    /* QColor ground; */
    const QPointF obsect = QPointF(wi/2,hi/2);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(QBrush(BGColor));
    
    ////////////////////std::cout << "paint a " << id  << " m->" << modus <<  std::endl;
    
    
    QPen pen;
    if (modus == M_HOVER) {
            if (PaintLockStaus) {
            pen.setStyle( Qt::DotLine );
            pen.setWidth( 2 );
            pen.setColor( Qt::red ); 
            painter->setPen( pen);
            painter->drawRect(boundingRect());
            }
    } else if (modus == M_MOVE_ALL) {
        if (PaintLockStaus) {
        pen.setStyle( Qt::DotLine );
        pen.setWidth( 2 );
        pen.setColor( Qt::red ); 
        painter->setPen( pen);
        /////////painter->setBrush(QBrush(QApplication::palette().midlight().color()));
        painter->drawRect(boundingRect());
        }
    }
            if (BorderDicks > 0) {
            pen.setStyle( Qt::SolidLine );
            pen.setWidth( BorderDicks );
            pen.setColor(MarginColor);
            painter->setPen(pen);
            } else {
            painter->setPen(Qt::NoPen);
            }
            painter->setBrush(QBrush(BGColor));
            painter->drawRect(boundingRect());
    
    
    if (PaintLockStaus) {
        if (modus == M_LOCK) {
        painter->drawPixmap(QPointF (wi - 18,3),QPixmap(":/img/lock_0.png"));
        }
        if (modus == M_WRITTELN) {
        painter->drawPixmap(QPointF (wi - 18,3),QPixmap(":/img/_writteln.png"));
        }
    }
    
    setTransform(ActualMatrixe(BoxRotate),false);
    
    if (BorderDicks < 1) {
    painter->setPen(Qt::NoPen);
    }
    setToolTip(DefaultMessage);
    QGraphicsTextItem::paint(painter,option,widget);
}





QTransform FloatDiagram::ActualMatrixe( int r ) 
{
     QMatrix matrix;
     matrix.translate ( boundingRect().center().x() , boundingRect().center().y() );
     matrix.rotate(r);
     matrix.translate ( - boundingRect().center().x() , - boundingRect().center().y() );
     return QTransform(matrix);
}
















