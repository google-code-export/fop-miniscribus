/*******************************************************************************
 * class GraphicsScene
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

#include "GraphicsScene.h"

PreviewLabel::PreviewLabel(QWidget *parent)
    : QWidget(parent)
{
}

void PreviewLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), QColor(224,224,224));
    /* center pix */
    QPointF dd = rect().center();
    QPointF ee(  dd.x() - (panello.width() / 2  ),0);
		painter.drawPixmap(ee,panello);
    painter.end();
}

void PreviewLabel::setPixmap(const QPixmap &pixmap)
{
   panello = pixmap;
}


//
/*  Save file as graphicsscene.cpp  */
/*  incomming class name GraphicsScene */
//
GraphicsScene::GraphicsScene( QObject * parent )
	: QGraphicsScene( parent )
{
	
}
GraphicsScene::GraphicsScene(  const QRectF & sceneRect, QObject * parent )
	: QGraphicsScene( sceneRect,  parent )
{
	setSceneRect( sceneRect );
}
GraphicsScene::GraphicsScene( qreal x, qreal y, qreal width, qreal height, QObject * parent )
	: QGraphicsScene(x,y,width,height,parent )
{
	setSceneRect(x,y,width,height);
}

void GraphicsScene::reload()
{
  update();
}
QGraphicsItem *GraphicsScene::ItemTop( const QPointF incomming ) 
{
       QList<QGraphicsItem *>  listing = QGraphicsScene::items(incomming);
       qreal thebest = 0;  /* zindex on front */
       for (int o=0;o<listing.size();o++) {
           thebest = qMax(listing[o]->zValue(),thebest);
       }
       for (int e=0;e<listing.size();e++) {
           if (listing[e]->zValue() == thebest) {
             return listing[e];
           }
       }
       emit nullitem();
       
return NULL;
}

/* filter only item a top Zindex /  zValue */
bool GraphicsScene::WakeUp( const QPointF incomming ) 
{
       QList<QGraphicsItem *>  listing = QGraphicsScene::items(incomming);
       qreal thebest = 0;  /* zindex on front */
       for (int o=0;o<listing.size();o++) {
           listing[o]->setSelected(false); 
           listing[o]->setFlag(QGraphicsItem::ItemIsSelectable,false);
           thebest = qMax(listing[o]->zValue(),thebest);
       }
       ///////qDebug() << "### thebest->" << thebest;
       /* activate item at top z-index zValue  /  not the down not visible!!!! */
       for (int e=0;e<listing.size();e++) {
           if (listing[e]->zValue() == thebest) {
             listing[e]->setFlag(QGraphicsItem::ItemIsSelectable,true);
             listing[e]->setSelected(true); 
             emit SelectOn(listing[e],thebest);
             return true;
           }
       }
       emit nullitem();
return false;
}

void GraphicsScene::TopFocus()
{
   QGraphicsItem *xx = QGraphicsScene::itemAt(1,1);
   QGraphicsScene::setFocusItem(xx);
   
}

void GraphicsScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void GraphicsScene::storno()
{
    emit nullitem();
    
}

void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    }
    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))  {
    event->setAccepted ( true ) ;
    clearSelection();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}



void GraphicsScene::clearSelection()
{
    QGraphicsScene::clearSelection();
    
}

QList<QGraphicsItem *> GraphicsScene::l_items()
{
    QList<QGraphicsItem *> li;
    li.clear();
    QList<QGraphicsItem *> listing = QGraphicsScene::items();
    for (int o=0;o<listing.size();o++) {
        if (listing[o]->data(ObjectNameEditor).toInt() > 0 ) {
            li.append(listing[o]);
        }
    }
    return li;
}

void GraphicsScene::remid( const int id )
{
    clearSelection();
    qDebug() << "### GraphicsScene rem " << id;
    
    QList<QGraphicsItem *> listing = QGraphicsScene::items();
    for (int o=0;o<listing.size();o++) {
        if (listing[o]->data(ObjectNameEditor).toInt() == id) {
            QGraphicsScene::removeItem(listing[o]);  
            
        }
    }
    reload();
    storno();
}

void GraphicsScene::clear()
{
    allclear();
}


void GraphicsScene::allclear()
{
    clearSelection();
    QList<QGraphicsItem *> listing = QGraphicsScene::items();
    for (int o=0;o<listing.size();o++) {
    QGraphicsScene::removeItem(listing[o]);  
    }
    storno();
}
qreal GraphicsScene::zmax()
{
 qreal mValue = 0.00;
 QList<QGraphicsItem *> items = QGraphicsScene::items();
     for (int o=0;o<items.size();o++) {
       mValue = qMax(items[o]->zValue(),mValue);
    }
 return mValue;
}

qreal GraphicsScene::zmin()
{
 qreal mValue = zmax();
 QList<QGraphicsItem *> items = QGraphicsScene::items();
     for (int o=0;o<items.size();o++) {
       mValue = qMin(items[o]->zValue(),mValue);
    }
 return mValue;
}


void GraphicsScene::setSceneRect( qreal x, qreal y, qreal w, qreal h ) 
{
    Area = QRectF(x,y,w,h);
    QGraphicsScene::setSceneRect(x,y,w,h);
}

void GraphicsScene::setSceneRect(  const QRectF & rect ) 
{
    Area = rect;
    QGraphicsScene::setSceneRect(rect);
}
#if QT_VERSION >= 0x040400
QGraphicsProxyWidget *GraphicsScene::addWidget(QWidget * widget, Qt::WindowFlags wFlags )
{
    return QGraphicsScene::addWidget(widget,wFlags);
}
#endif




void GraphicsScene::printPage(int index, int percentual , QPainter &painter, QPrinter * printer)
{
  
    QPixmap pixmap(painter.viewport().width(),painter.viewport().height());   /* virtual paper */
    bool portrait = printer->orientation() == QPrinter::Portrait ? true : false;
    
    const qreal wit = qMin(printer->pageRect().width(),printer->paperRect().width());
	  const qreal hei = qMin(printer->pageRect().height(),printer->paperRect().height());
    const qreal faktor_print = qMax(wit,hei) / qMin(wit,hei);
    
    const QRect Paper_Rect = printer->pageRect();   
    qreal onepagescene_HI = sceneRect().width() * faktor_print;
    if (!portrait) {
    onepagescene_HI = sceneRect().width() / faktor_print;
    } 
    
    QRectF rectScenePiece = QRectF (0.,0.,sceneRect().width(),onepagescene_HI); /* first slice from scene */
    
    const qreal page = sceneRect().height() / onepagescene_HI;   /* page need */
	  int PageSumm = page;
    
	    if (page > PageSumm) {
			 PageSumm++;   /* float to next int */
			}
      
    if ( index > PageSumm ) {
    return;   /* not having this page */
    }
    
    qreal InitOnYtop = 0;
    
    if (index != 0) {
          InitOnYtop = rectScenePiece.height() * index;
    }
    QRect pagepiece = QRect(0,InitOnYtop,rectScenePiece.width(),rectScenePiece.height());
    //////////qDebug() << "### page  " << index << "," << percentual << "," << printer->pageRect();
    const qreal smallpart = qMax(pixmap.width(),pixmap.height()) / faktor_print;
    
    QRectF AFormatPaper;
    /* Paper dimension from printer Faktor run */
    if (portrait) {
    AFormatPaper = QRect(0,0,smallpart,qMax(pixmap.width(),pixmap.height()));
    } else {
    AFormatPaper = QRect(0,0,qMax(pixmap.width(),pixmap.height()),smallpart);    
    }
    
    
    QRectF ZoomFaktorPage = Reduce(AFormatPaper,percentual);   /* zoom rect */
    QRect WhitePaper = CenterRectSlaveFromMaster(pixmap.rect(),AFormatPaper).toRect(); 
    QRect RenderPage = CenterRectSlaveFromMaster(pixmap.rect(),ZoomFaktorPage).toRect(); 
    
    painter.fillRect(painter.viewport(),QBrush(Qt::lightGray));  /* device to cover */
    painter.fillRect(WhitePaper,QBrush(Qt::white));   /* paper */
    /////////painter.fillRect(RenderPage,QBrush(Qt::red));   /* page result */
    render(&painter,RenderPage,pagepiece,Qt::KeepAspectRatio);
}




PreviewDialog::PreviewDialog(GraphicsScene *sceneprint)
    : QDialog(0),scene(sceneprint),
                print(new QPrinter(QPrinter::HighResolution)),
                Rscene(sceneprint->sceneRect()),printonpdf(false),DefaultPrintBorderPercentual(0)
{
    setupUi(this);
    setWindowFlags ( Qt::Window );

    pdfButton = new QPushButton(tr("Pdf File Print"));
    buttonBox->addButton(pdfButton, QDialogButtonBox::ActionRole);
    connect(pdfButton, SIGNAL(clicked()), SLOT(pdfsetter()));

    currentPage = 0;
    pageCount = 0;
    pageList->setIconSize(QSize(SmallPreviewLength, SmallPreviewLength));
    pageList->header()->hide();
    previewLabel = new PreviewLabel;
    previewArea->setWidget(previewLabel);
    setupComboBoxes();
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("&Print"));
    ////////////pageList->clear();
    RenderFaktor();
   
}

void PreviewDialog::RenderFaktor()
{
  print->setFullPage(true);
  ////////pageList->clear();
	print->setPageSize(QPrinter::PageSize(paperSizeCombo->itemData(paperSizeCombo->currentIndex()).toInt()));
  print->setOrientation(QPrinter::Orientation(paperOrientationCombo->itemData(paperOrientationCombo->currentIndex()).toInt()));
  const qreal wit = qMin(print->pageRect().width(),print->paperRect().width());  /* printer border ? */
	const qreal hei = qMin(print->pageRect().height(),print->paperRect().height());
  QPrinter::Orientation co = QPrinter::Orientation(paperOrientationCombo->itemData(paperOrientationCombo->currentIndex()).toInt());
  bool portrait = co == QPrinter::Portrait ? true : false;
  faktor_print = qMax(wit,hei) / qMin(wit,hei);
  Paper_Rect = print->pageRect();   
  qreal onepagescene_HI = Rscene.width() * faktor_print;
  if (!portrait) {
     onepagescene_HI = Rscene.width() / faktor_print;
  } 
  
	
	const qreal page = scene->sceneRect().height() / onepagescene_HI;   /* page need */
	int passage = 0;
      passage = page;
	    if (page > passage) {
			 passage++;
			}
  pageCount = passage;
  rectScenePiece = QRectF (0.,0.,Rscene.width(),onepagescene_HI); /* first slice from scene */
  /////////qDebug() << "### pageCount;  " << pageCount;
	///////////qDebug() << "### faktor;  " << faktor_print;
	////////////qDebug() << "### Paper_Rect;  " << Paper_Rect;
  /////////qDebug() << "### rectScenePiece;  " << rectScenePiece;
  ////////qDebug() << "### portrait;  " << portrait;
  setNumberOfPages(passage);
      
  QTimer::singleShot(0, this, SLOT(addPage()));
}


void PreviewDialog::setNumberOfPages(int count)
{
    pageCount = count;
    currentPage = 0;
    //////pageList->clear();
    const int size_max = ActualMaxPreview();
    QPixmap pixbig(size_max,size_max);
    paintPreview(pixbig,currentPage);
    previewLabel->setPixmap(pixbig);
    previewLabel->update();
    update();
    QTimer::singleShot(10, this, SLOT(addPage()));
}

void PreviewDialog::pdfsetter()
{
    printonpdf = true;
    accept();
}

void PreviewDialog::accept()
{
    int markedPages = 0;
    int pageIndex;
    for (pageIndex = 0; pageIndex < pageList->topLevelItemCount(); ++pageIndex) {
        if (pageList->topLevelItem(pageIndex)->checkState(0) == Qt::Checked)
            ++markedPages;
    }
    // Print all pages that have yet to be previewed.
    markedPages += pageCount - pageIndex;
    qDebug() << "### printsumme;  " << markedPages;
    if (markedPages < 1) {
     return;   
    }
    QPrinter::Orientation co = QPrinter::Orientation(paperOrientationCombo->itemData(paperOrientationCombo->currentIndex()).toInt());
    print->setPageSize(QPrinter::PageSize(paperSizeCombo->itemData(paperSizeCombo->currentIndex()).toInt()));
    print->setOrientation(co);
    print->setOutputFormat(QPrinter::NativeFormat);
    
    if (!printonpdf) {
        QPrintDialog dialog(print, this);
        if (dialog.exec() != QDialog::Accepted) {
            return;
        }
    } else {
       QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",QString(setter.value("LastDir").toString()), "*.pdf");
       if (fileName.size() > 1) {
        setter.setValue("LastDir",fileName.left(fileName.lastIndexOf("/"))+"/");
        QString doname = QInputDialog::getText(this, tr("PDF Document Title"),tr("Document title:"), QLineEdit::Normal,"MiniScribus Page Layer");
        print->setOutputFormat(QPrinter::PdfFormat);
        print->setCreator ( "MiniScribus Layer model by www.crosskern.com" );
        print->setDocName ( doname );
        print->setOutputFileName(fileName); 
       } else {
        return;
       }
        
        
    }
    
    
    
    
    
    print->setFullPage(true);
    bool portrait = print->orientation() == QPrinter::Portrait ? true : false;
    const qreal wit = qMin(print->pageRect().width(),print->paperRect().width());  /* printer border ? */
    const qreal hei = qMin(print->pageRect().height(),print->paperRect().height());
    faktor_print = qMax(wit,hei) / qMin(wit,hei);
    
    qreal onepagescene_HI = Rscene.width() * faktor_print;
    if (!portrait) {
     onepagescene_HI = ( Rscene.width() / faktor_print) - HeaderAndFooterHightSpace;
    } 
    rectScenePiece = QRectF (0.,0.,Rscene.width(),onepagescene_HI); /* first slice from scene */
    canceled = false;
    int printed = 0;
    
    progressBar->setMaximum(markedPages);
    progressBar->setEnabled(true);
    progressBar->setTextVisible(true);
    /* print loop */
    
    QPainter painter;
    painter.begin(print);
    canceled = false;
    bool firstPage = true;
    for (pageIndex = 0; pageIndex < pageCount; ++pageIndex) {
        qApp->processEvents();
        if (canceled) {
            break;
        }
        if (isSelected(pageIndex)) {
            
            if (!firstPage) {
                print->newPage();
            }
            progressBar->setValue(++printed);
            scene->printPage(pageIndex,DefaultPrintBorderPercentual,painter,print);
            firstPage = false;
        }
    }
    painter.end();
    /* print loop */
    progressBar->setTextVisible(false);
    progressBar->setEnabled(false);
    printonpdf = false;
    QDialog::accept();
    
}



void PreviewDialog::paintPreview(QPixmap &pixmap, int index )
{
    
    if (index < 0 || index >= pageCount) {
    return;
    }
    
    pixmap.fill(Qt::lightGray);
    QPainter painter;
    painter.begin(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->printPage(index,DefaultPrintBorderPercentual,painter,print);
    painter.end();
}


void PreviewDialog::addPage()
{
    if (currentPage >= pageCount) {
        return;
    }
    
    if (currentPage == 0) { 
    pageList->clear();
    }
    
    for (int i = 0; i < pageCount; ++i)  {
        
        ///////////qDebug() << "### draw page  " << i << "," << pageCount;
        
        QTreeWidgetItem *item = new QTreeWidgetItem(pageList);
        item->setCheckState(0, Qt::Checked);
        paintItem(item,i);
        
    }
    
    if (pageList->indexOfTopLevelItem(pageList->currentItem()) < 0)  {
        pageList->setCurrentItem(pageList->topLevelItem(0));
    }
    qApp->processEvents();
}


void PreviewDialog::paintItem(QTreeWidgetItem *item, int index)
{
    QPixmap pixmap(SmallPreviewLength, SmallPreviewLength);
    paintPreview(pixmap,index);
    item->setIcon(0,QIcon(pixmap));
    item->setToolTip (0,tr("Page %1").arg(index + 1));
}


int PreviewDialog::ActualMaxPreview()
{
    return qMax(qMin(previewArea->width() - previewArea->verticalScrollBar()->width(),
                         previewArea->height() - previewArea->horizontalScrollBar()->height()),
                    int(LargePreviewLength));
}



void PreviewDialog::on_pageList_currentItemChanged()
{
    const int index = pageList->indexOfTopLevelItem(pageList->currentItem());
    if (index < 0) {
        return;
    }
    const int size_max = ActualMaxPreview();
    QPixmap pixbig(size_max,size_max);
    paintPreview(pixbig,index);
    previewLabel->setPixmap(pixbig);
    previewLabel->update();
    update();
}



void PreviewDialog::on_scales_valueChanged(int index)
{
    DefaultPrintBorderPercentual = index;
    RenderFaktor();
}


void PreviewDialog::on_paperSizeCombo_activated(int index)
{
    RenderFaktor();
}

void PreviewDialog::on_paperOrientationCombo_activated(int index)
{
    RenderFaktor();
}









void PreviewDialog::resizeEvent(QResizeEvent *)
{
    int size = qMax(qMax(previewArea->width() - previewArea->verticalScrollBar()->width(),int(LargePreviewLength))
                ,qMax(previewArea->height(),int(LargePreviewLength)));
    previewLabel->resize(size, size);
    RenderFaktor();
}


void PreviewDialog::setupComboBoxes()
{
    paperSizeCombo->addItem(tr("A0 (841 x 1189 mm)"), QPrinter::A0);
    paperSizeCombo->addItem(tr("A1 (594 x 841 mm)"), QPrinter::A1);
    paperSizeCombo->addItem(tr("A2 (420 x 594 mm)"), QPrinter::A2);
    paperSizeCombo->addItem(tr("A3 (297 x 420 mm)"), QPrinter::A3);
    paperSizeCombo->addItem(tr("A4 (210 x 297 mm, 8.26 x 11.7 inches)"), QPrinter::A4);
    paperSizeCombo->addItem(tr("A5 (148 x 210 mm)"), QPrinter::A5);
    paperSizeCombo->addItem(tr("A6 (105 x 148 mm)"), QPrinter::A6);
    paperSizeCombo->addItem(tr("A7 (74 x 105 mm)"), QPrinter::A7);
    paperSizeCombo->addItem(tr("A8 (52 x 74 mm)"), QPrinter::A8);
    paperSizeCombo->addItem(tr("A9 (37 x 52 mm)"), QPrinter::A9);
    paperSizeCombo->addItem(tr("B0 (1000 x 1414 mm)"), QPrinter::B0);
    paperSizeCombo->addItem(tr("B1 (707 x 1000 mm)"), QPrinter::B1);
    paperSizeCombo->addItem(tr("B2 (500 x 707 mm)"), QPrinter::B2);
    paperSizeCombo->addItem(tr("B3 (353 x 500 mm)"), QPrinter::B3);
    paperSizeCombo->addItem(tr("B4 (250 x 353 mm)"), QPrinter::B4);
    paperSizeCombo->addItem(tr("B5 (176 x 250 mm, 6.93 x 9.84 inches)"), QPrinter::B5);
    paperSizeCombo->addItem(tr("B6 (125 x 176 mm)"), QPrinter::B6);
    paperSizeCombo->addItem(tr("B7 (88 x 125 mm)"), QPrinter::B7);
    paperSizeCombo->addItem(tr("B8 (62 x 88 mm)"), QPrinter::B8);
    paperSizeCombo->addItem(tr("B9 (44 x 62 mm)"), QPrinter::B9);
    paperSizeCombo->addItem(tr("B10 (31 x 44 mm)"), QPrinter::B10);
    paperSizeCombo->addItem(tr("C5E (163 x 229 mm)"), QPrinter::C5E);
    paperSizeCombo->addItem(tr("DLE (110 x 220 mm)"), QPrinter::DLE);
    paperSizeCombo->addItem(tr("Executive (7.5 x 10 inches, 191 x 254 mm)"), QPrinter::Executive);
    paperSizeCombo->addItem(tr("Folio (210 x 330 mm)"), QPrinter::Folio);
    paperSizeCombo->addItem(tr("Ledger (432 x 279 mm)"), QPrinter::Ledger);
    paperSizeCombo->addItem(tr("Legal (8.5 x 14 inches, 216 x 356 mm)"), QPrinter::Legal);
    paperSizeCombo->addItem(tr("Letter (8.5 x 11 inches, 216 x 279 mm)"), QPrinter::Letter);
    paperSizeCombo->addItem(tr("Tabloid (279 x 432 mm)"), QPrinter::Tabloid);
    paperSizeCombo->addItem(tr("US Common #10 Envelope (105 x 241 mm)"), QPrinter::Comm10E);
    paperSizeCombo->setCurrentIndex(paperSizeCombo->findData(QPrinter::A4));
    paperOrientationCombo->addItem(tr("Portrait"), QPrinter::Portrait);
    paperOrientationCombo->addItem(tr("Landscape"), QPrinter::Landscape);
}


bool PreviewDialog::isSelected(int index)
{
    if (index >= 0 && index < pageList->topLevelItemCount())
        return (pageList->topLevelItem(index)->checkState(0) == Qt::Checked);
    else
        return false;
}



void PreviewDialog::reject()
{
    canceled = true;
    QDialog::reject();
}






GMarginScene::GMarginScene(qreal left , qreal right , QGraphicsScene *scene)
  : QGraphicsItem(0,scene),units("mm"),
    dimfontsize(8),
    Cursor_1_X(left),Cursor_2_X(scene->sceneRect().width() - right),
    actual_x(left),MaximumCollisionAllowed(200)
{
  setAcceptDrops(false);
  setFlag(QGraphicsItem::ItemIsSelectable,true);
  setFlag(QGraphicsItem::ItemIsFocusable,true);
  setFlag(QGraphicsItem::ItemIsMovable,false);
  hi = 26;
  wi = scene->sceneRect().width();
  ColText = Qt::black;
  cursorimg = CacheCursorSlider();
    
}


QRectF GMarginScene::boundingRect() const
{
    return QRectF(0,0,wi,hi);
}

QPainterPath GMarginScene::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(0,0,wi+20,hi+20));
    return path;
}


bool GMarginScene::sceneEvent(QEvent *e)
{
    if (e->type() == QEvent::GraphicsSceneMousePress ||
         e->type() == QEvent::GraphicsSceneMouseMove ||
         e->type() == QEvent::GraphicsSceneMouseRelease) {
        QGraphicsSceneMouseEvent *ke0 = static_cast<QGraphicsSceneMouseEvent *>(e);
        HandleMove(ke0->pos());
        e->setAccepted(true);
    } else if (e->type() == QEvent::GraphicsSceneMouseDoubleClick ) {
        QGraphicsSceneMouseEvent *ke = static_cast<QGraphicsSceneMouseEvent *>(e);
        
        if (ke->pos().x() < (boundingRect().width() / 2)) {
            Cursor_1_X = qBound(1.0,ke->pos().rx(),Cursor_2_X - MaximumCollisionAllowed - 1.0);
        } else {
            Cursor_2_X = qBound(Cursor_1_X + MaximumCollisionAllowed + 1.0,ke->pos().rx(),boundingRect().width()- 1.0);
        }
        HandleMove(ke->pos());
        e->setAccepted(true);
    }
    
    
    //////qDebug() << "### e->type() " << e->type();
    
    return QGraphicsItem::sceneEvent(e);
}



void GMarginScene::HandleMove(  QPointF point )
{
    AreaCursor_1 = QRectF(Cursor_1_X - 5,0,10,boundingRect().height());
    AreaCursor_2 = QRectF(Cursor_2_X - 5,0,10,boundingRect().height());
    if (AreaCursor_2.contains(point) || AreaCursor_1.contains(point)) {
       lastMove = point.x();
       HandleMouse(point); 
    } else {
         /* move left or right */
         int Pointer_X = point.x();
        
          if (Pointer_X < (boundingRect().width() / 2)) {
              /* move 1*/
              int copy1 = Cursor_1_X;
              if (Pointer_X < copy1) {
                HandleMouse(QPointF(copy1 - 1,2)); 
              } else {
                HandleMouse(QPointF(copy1 + 1,2)); 
              }
              
          } else {
               /* move 2 */
              int copy2 = Cursor_2_X;
              if (Pointer_X < copy2) {
                HandleMouse(QPointF(copy2 - 1,2)); 
              } else {
                HandleMouse(QPointF(copy2 + 1,2)); 
              }
          }
         

    }
}

void GMarginScene::HandleMouse(  QPointF point )
{
    qreal Pointer_X = point.rx();
    if (actual_x == Pointer_X) {
    return;
    }
    if (AreaCursor_1.contains(point)) {
        Cursor_1_X = qBound(1.,Pointer_X,Cursor_2_X - MaximumCollisionAllowed - 1.0);
        update();
    }
    if (AreaCursor_2.contains(point)) {
        Cursor_2_X = qBound(Cursor_1_X + MaximumCollisionAllowed + 1.0,Pointer_X,boundingRect().width()- 1.0);
        update();
    }
    ///////qDebug() << "### cursor pair " << Cursor_1_X << "," << Cursor_2_X;
    emit CursorMove(Cursor_1_X,Cursor_2_X);
    actual_x = point.rx();
}



void GMarginScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    Q_UNUSED(widget);
    wi = scene()->sceneRect().width();
    const QRectF onlyPP = boundingRect();
    ///////painter->setWindow(0,0,wi,hi);
    const qreal scaleMesure = Pointo(wi,units);
    const qreal lineseparator_0 = FopInt(QString("10%1").arg(units));
    const qreal lineseparator_1 = lineseparator_0 / 10;
    painter->setBrush(QColor("#ece4c7"));
    painter->drawRect(boundingRect());
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::darkGray,1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(0,1,wi,1);
    const uint linessumme_0 = wi / lineseparator_0;
    const uint linessumme_1 = wi / lineseparator_1;
    int PressUnit = -1;
    painter->setPen(QPen(ColText,0.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (int i = 0; i<=linessumme_1; i++)
    {
        PressUnit++;
        
        QString strnu = QString("%1").arg(i);
        const int LeftPointer1 = i * lineseparator_1 - 0.4;
        if (PressUnit > 0 && PressUnit < 5 || PressUnit > 5 && PressUnit < 10) {
        painter->drawLine(LeftPointer1,5,LeftPointer1,6);  
        } else if (PressUnit == 5) {
        painter->drawLine(LeftPointer1,5,LeftPointer1,8);   
        }
        
        if (PressUnit == 9) {
          PressUnit = -1;
        }
    }
    painter->setPen(QPen(ColText,1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QFont valFont("Arial", dimfontsize, QFont::Normal);
    painter->setFont(valFont);
    for (int i = 0; i<=linessumme_0; i++)
    {
        const int LeftPointer0 = i * lineseparator_0 - 0.5;
        painter->drawLine(LeftPointer0,5,LeftPointer0,10);
        if (i > 0) {
        QString val = QString("%1").arg(i);
        QRectF valR(LeftPointer0 - (lineseparator_0 / 2),11,lineseparator_0,dimfontsize + 2);
        painter->drawText(valR, Qt::AlignCenter , val);
        }
    }
    
    painter->setPen(QPen(Qt::darkGray,1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(0,21,wi,21);
    
    ///////painter->setBrush(Qt::red);
    
    
    ///////QRectF cursor(Cursor_1_X,12,5,18);
    ///////painter->setPen(QPen(Qt::red,1.5));
    //////////painter->drawLine(Cursor_1_X, 1,Cursor_1_X, 21);
    ///////painter->drawLine(Cursor_2_X, 1,Cursor_2_X, 21);
    
    //////painter->setPen(QPen(Qt::darkRed,1));
    /////////QRectF rectangle_0(Cursor_1_X - 5,18,10,15); 
    ///////QRectF rectangle_1(Cursor_1_X - 5,-10,10,15); 
    
    ////////QRectF brectangle_0(Cursor_2_X - 5,18,10,15); 
    ///////////QRectF brectangle_1(Cursor_2_X - 5,-10,10,15); 
    
    /////QRectF ( qreal x, qreal y, qreal width, qreal height )
    
    qreal P_2 = Cursor_2_X - 4.5;
    qreal P_1 = qAbs(Cursor_1_X - 4.5);
    
    if ( (Cursor_2_X + cursorimg.width()) > wi) {
    int diffe2 = boundingRect().width() - Cursor_2_X + 5;
    painter->drawPixmap(P_2,0.,cursorimg.copy(QRect(0,0,diffe2,26)));   
    } else {
    painter->drawPixmap(P_2,0.,cursorimg);   
    }
    
    qreal Extreme_Left = qBound(0.,Cursor_1_X - 5.1,wi / 2);
    
    ///////qDebug() << "### Cursor_1_X " << Cursor_1_X;
    ////////qDebug() << "### Extreme_Left " << Extreme_Left;
    
    
    if (Extreme_Left < 6 ) {
    int displayX = qBound (0.,5. - Extreme_Left + 1,5.);
    painter->drawPixmap(Extreme_Left,0.,cursorimg.copy(QRect(displayX,0,10,26)));  //// -+ /// 
    ////////painter->drawPixmap(Extreme_Left,0.,cursorimg);        
    } else {
    painter->drawPixmap(P_1,0.,cursorimg);
    }
    
    
    
    painter->setPen(Qt::NoPen);
    
}


GMarginScene::~GMarginScene()
{
    
}










