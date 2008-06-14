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




PreviewDialog::PreviewDialog(QGraphicsScene *sceneprint)
    : QDialog(0),scene(sceneprint),
                print(new QPrinter(QPrinter::HighResolution)),
                Rscene(sceneprint->sceneRect()),printonpdf(false)
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
        print->setOutputFormat(QPrinter::PdfFormat);
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
     onepagescene_HI = Rscene.width() / faktor_print;
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
        qreal InitOnYtop = 0;
        if (pageIndex != 0) {
          InitOnYtop = rectScenePiece.height() * pageIndex;
        } 
        QRectF pagepiece(0,InitOnYtop,rectScenePiece.width(),rectScenePiece.height());
        if (isSelected(pageIndex)) {
            
            if (!firstPage) {
                print->newPage();
            }
            
            progressBar->setValue(++printed);
            scene->render(&painter,print->pageRect(),pagepiece,Qt::IgnoreAspectRatio);
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
    
    QPrinter::Orientation co = QPrinter::Orientation(paperOrientationCombo->itemData(paperOrientationCombo->currentIndex()).toInt());
    bool portrait = co == QPrinter::Portrait ? true : false;
    
    
    const qreal wit = qMin(pixmap.width(),pixmap.height());
	  const qreal hei = qMax(pixmap.width(),pixmap.height());
    const qreal smallpart = qMax(wit,hei) / faktor_print;
    const qreal space = (qMax(wit,hei) - smallpart) / 2;
    pixmap.fill(Qt::lightGray);
    QPainter painter;
    painter.begin(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF pagepiece;
    QRectF devicerect;
    qreal InitOnYtop = 0;
    if (index != 0) {
          InitOnYtop = rectScenePiece.height() * index;
    }        
    
    if (portrait) {
    painter.fillRect(QRectF(space,0,smallpart,qMax(wit,hei)),QBrush(Qt::white));  
    pagepiece = QRectF(0,InitOnYtop,rectScenePiece.width(),rectScenePiece.height());
    devicerect = QRectF(space,0,smallpart,qMax(wit,hei));
    painter.fillRect(devicerect,QBrush(Qt::white));
    scene->render(&painter,devicerect,pagepiece,Qt::KeepAspectRatio);   
    } else {
    pagepiece = QRectF(0,InitOnYtop,rectScenePiece.width(),rectScenePiece.height()); 
    devicerect = QRectF(0,space,qMax(wit,hei),smallpart);
    painter.fillRect(devicerect,QBrush(Qt::white));
    scene->render(&painter,devicerect,pagepiece,Qt::KeepAspectRatio);
    }
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
    //////QPixmap pixmap(previewLabel->size());
    //////paintPreview(pixmap, pageList->indexOfTopLevelItem(pageList->currentItem()));
    //////previewLabel->setPixmap(pixmap);
    //////previewLabel->update();
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

















