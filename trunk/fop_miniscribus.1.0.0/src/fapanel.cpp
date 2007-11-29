#include "fapanel.h"

/* ################################ view ####################################################*/
/* ################################ view ####################################################*/
/* ################################ view ####################################################*/
/* ################################ view ####################################################*/
/* ################################ view ####################################################*/
/* ################################ view ####################################################*/
/* ################################ view ####################################################*/


/*

    QRectF marginrr(FopInt("10mm"),FopInt("6mm"),FopInt("22cm"),FopInt("22cm"));
    
    QDesktopWidget *desk = QApplication::desktop();
    QRect winsize = desk->screenGeometry();
    PageWidgedsBG = QPixmap(winsize.width(),winsize.width());
    PageWidgedsBG.fill(QApplication::palette().midlight().color());
*/
    
FAPanel::FAPanel( QWidget * parent )
    :QGraphicsView( parent )
{
    LASTIDTOUCH = -1;
    BigParent = parent; 
    MenuFromParent = new QMenu();
    CurrentPrinterPage = 0;
    scene = new GraphicsScene(this);
    scene->setSceneRect(0,BEGINNATOP,50,50);  /* initial value */
    setScene(scene); 
    connect(scene, SIGNAL(SelectOn(QGraphicsItem*,qreal)), this, SLOT(WorksOn(QGraphicsItem*,qreal)));
    chessgrid = BruschChess(FopInt("1cm"));
    PageSumm = 1;
    margin = QRectF(2,2,2,2);  /* Printer pro format margin QRectF margin; */
    
    QRectF usermargin = setter.value("MyPageMargin").toRectF();
    if (usermargin.isValid()) {
    margin = setter.value("MyPageMargin").toRectF();
    }
    TargetInternalLink.clear();
    //////FillData();  /* set up printer format */
}

void FAPanel::NewDocumentSet() 
{
      for (int e=0;e<items.size();e++) {
                   items[e]->deleteLater();
                   items.removeAt ( e ); 
       }
       items.clear();
       PageSumm = 1;
       scene = new GraphicsScene(this);
       scene->setSceneRect(0,BEGINNATOP,50,50);  /* initial value */
       setScene(scene);
       connect(scene, SIGNAL(SelectOn(QGraphicsItem*,qreal)), this, SLOT(WorksOn(QGraphicsItem*,qreal)));
       UpdatePageSumms();
}




void FAPanel::ExtractImages() 
{
     QString newnameimage;
     QString dir;
    
    
      if (items.size() < 1) {
                QMessageBox infoset;
                infoset.setWindowTitle(QObject::tr("Sorry..") );
                infoset.setText ( tr("No layer found, to extract image!"));
                infoset.exec();  
      return;
      }
    
     
      dir = QFileDialog::getExistingDirectory(this, tr("Save file on dir.."),
                                                 setter.value("LastDir").toString(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks); 
      if (dir.size() < 1) {
      return;
      }
      
      dir.append("/");
      
      setter.setValue("LastDir",dir);
    
       for (int e=0;e<items.size();e++) {
                 TypImageContainer  imliste =   items[e]->GetResource();
                  QMapIterator<QString,QVariant> i(imliste);   /* element from layer */
                         while (i.hasNext()) {
                              i.next();
                              QByteArray imagestream = i.value().toByteArray();
                             
                               if (i.key().startsWith("/svg/",Qt::CaseInsensitive)) {
                               /* difference only dwo way vector and other as png! */
                               newnameimage = FilenameAllow( i.key() + ".svg");
                               } else {
                               newnameimage = FilenameAllow( i.key() + ".png");
                               }
                                          QFile f(dir + newnameimage);
                                          if ( f.open( QIODevice::WriteOnly ) ) 
                                          {
                                            f.write(imagestream);
                                            f.close();
                                          }
                         } 
        }
        
    
    
    
}









void FAPanel::WorksOn(QGraphicsItem * item , qreal zindex )
{
        if (CurrentActive = qgraphicsitem_cast<FloatDiagram *>(item)) {
            int id = CurrentActive->Getid();
            CurrentActive->SetINTarget( TargetInternalLink );  /* update internal href document avaiable links */
            qDebug() << "Real view in  thebest  ->" << zindex << " nr." << id;
            emit PaintButton(CurrentActive);
            LASTIDTOUCH = id;
            QTimer::singleShot(50, this, SLOT(SceneRasterClear()));
        } else {
            CurrentResetNow();
        }
}

void FAPanel::CurrentResetNow() 
{
CurrentActive = new FloatDiagram(0,new QMenu(),0,new GraphicsScene());
}

void FAPanel::SetMenuExpandet( QMenu *SuperiorMenu )
{
    MenuFromParent = SuperiorMenu;
}

void FAPanel::SceneRasterClear()
{
    scene->clearSelection();
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
        for (int e=0;e<items.size();e++) {
          if (items[e]->Getid() != CurrentActive->Getid()) {
              items[e]->setSelected(false);
              ///////items[e]->setVisible(false);
          } else {
              items[e]->setVisible(true);
          }
        }
        
        
        CurrentActive->setSelected(true); 
        
}


 /* prepare backround paint */
void FAPanel::MakePageformat() 
{
    setCacheMode(CacheBackground);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
}


/* work layout leave */
void FAPanel::PrePareToprintPaint() 
{
      for (int e=0;e<items.size();e++) {
          items[e]->PaintStausLayer(false);
          items[e]->setSelected(false); 
          items[e]->setFlag(QGraphicsItem::ItemIsSelectable,false);
       }
       
       ReDrawAll();
}

/* work layout reset */
void FAPanel::ResetFromToprintPaint() 
{
      for (int e=0;e<items.size();e++) {
          items[e]->PaintStausLayer(true);
       }
}

/* save all layer to a file */
void FAPanel::SaveOn( const QString savefile ) 
{
    fopSaveXmlfile = new Fop_Handler(savefile,false,db,BigParent);
    
    QDomDocument XMLDomdocfop = fopSaveXmlfile->GetStructure(PrinterPageRectFix,margin,QColor(Qt::white),db->page(CurrentPrinterPage)->name());
    /* Take a root xml dom document to fill from layer !! */
    for (int e=0;e<items.size();e++) {
          fopSaveXmlfile->ImageBlockAppend( items[e]->GetResource() );
       }
     /* now take normal data and color border top left ecc... */
    for (int i=0;i<items.size();i++) {
        fopSaveXmlfile->AppendLayer(items[i]->GetBaseLayerElement(XMLDomdocfop),items[i]->document());
    }
       fopSaveXmlfile->Writtelnend();
    
    
    
    fopSaveXmlfile->~Fop_Handler();   /* remove from cached resource .... */
}



/* all file remote or locale are store to load here */
void FAPanel::PaintPage( Fop_Handler * onOpen , QString topdffile ) 
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    fopOpenXmlfile = onOpen;
    int printerformating = 0;   /* a4*/
    if (!fopOpenXmlfile->PaintPrinterName().isEmpty()) {
        QList<PageDimensionName*> pages =  db->Listing();
        for (int i=0;i<pages.size();i++) {
            if (pages[i]->name() == fopOpenXmlfile->PaintPrinterName()) {
             printerformating = i;   
            }
        }
    }
    SwapPaperFormat(printerformating);    /* page format from page */  
    QList<Fop_Layer*> Litem = fopOpenXmlfile->GetLayerList();
    
    int LayersHightSum = 0;
    
        for (int i=0;i<Litem.size();i++) {
              /* Paint the item inside scene */
              int gradesInit = Litem[i]->GetRotate();
              const int DeegresRotation = qBound(0,gradesInit,359);
            
               QRectF LiRectF = Litem[i]->GetRectLayer();
               PageSumm = NeedetPageSumm(LiRectF.y());
            
            
              FloatDiagram *ioq = new FloatDiagram(i+FLOATLAYER_INCREMENT,MenuFromParent,0,scene);
                       ioq->setDocument ( Litem[i]->Qdoc() );
                       ioq->SetBGColor( Litem[i]->Bgcolor() );
                       if (Litem[i]->Border() > 0) {
                       ioq->SetBorder( Litem[i]->Bocolor() , Litem[i]->Border() );
                       }
                       if (DeegresRotation > 0) {
                       ioq->SetupRoq(DeegresRotation);
                       }
                       if (Litem[i]->TargetName().size() > 0) {
                       TargetInternalLink.append(Litem[i]->TargetName());
                       }
                       
                       
                       ioq->SetDimension(LiRectF.width(),LiRectF.height());
                       ioq->setPos(QPointF(LiRectF.x(),LiRectF.y()));
                       if (Litem[i]->GetZindex() !=0) {
                       ioq->setZValue(qBound(10.01,Litem[i]->GetZindex(),10000.01));
                       } else {
                       ioq->setZValue(11);
                       }
                       ioq->SetResource(Litem[i]->GetResourceBlock());
                        /////////////////connect(ioq, SIGNAL(Rebuild()),this, SLOT(ReDrawAll()));
                       if (Litem[i]->GetLock()) {
                       ioq->SetModus( FloatDiagram::M_LOCK );
                       }
                       LayersHightSum +=ioq->DocumentHighgtActual();
                       items.append(ioq);
        
        
       }
       
    
       
       const qreal docshi = LayersHightSum;
        if (NeedetPageSumm(docshi) !=PageSumm) {
          PageSumm = NeedetPageSumm(docshi);
        }
        
        UpdatePageSumms();
        
        
    emit SetPagePrintIndex(printerformating);
    emit TotalPage(PageSumm);
     QApplication::restoreOverrideCursor();
        
        if (topdffile.size() > 0) {
            Current_file_from_Main_to_PDF = topdffile;
            /* wait to rotate all elements */
            QTimer::singleShot(1350, this, SLOT(PrintPdfCurrentMainFile())); 
        }
     QApplication::restoreOverrideCursor();
}


void FAPanel::PrintPdfCurrentMainFile() 
{
    
    if (Current_file_from_Main_to_PDF.size() < 1 ) {
        return;
    }
    QString topdffile = Current_file_from_Main_to_PDF;
    
    
            QFile fix( topdffile );
            if (fix.exists()) {
                fix.remove();
            }
            qDebug() << "### panel   " << topdffile;
            
                        PrePareToprintPaint();
                        MakePrintChoise(QPrinter::PdfFormat,QPrinter::Portrait,topdffile); 
                        QApplication::restoreOverrideCursor();
                        QFileInfo pddf(topdffile);
                            if (fix.exists()) {
                                /* show pdf if exist! */
                                OpenDesktop(QUrl(pddf.absoluteFilePath()));
                            }

     ///////////////////Current_file_from_Main_to_PDF = "";                            
}




void FAPanel::AppendCurrenttoClone() 
{
    
    if (items.size() < 1) {
    return;
    }
    
    
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    FloatDiagram *created = CurrentActive->clone();
    const qreal increment = 3.14;
    
    
    TargetInternalLink.append(QString("layer_%1").arg(created->Getid()));
    
    created->setZValue(MostUp() + increment);
    created->setSelected(true);
    /////////////connect(created, SIGNAL(Rebuild()),this, SLOT(ReDrawAll()));
    items.append(created);
    PageSumm = NeedetPageSumm(created->pos().y());
    UpdatePageSumms();
    emit TotalPage(PageSumm);
}

void FAPanel::AppendLayerInside() 
{
    int nummerlayer = items.size()+FLOATLAYER_INCREMENT;
    FloatDiagram *ioapp = new FloatDiagram(nummerlayer,MenuFromParent,0,scene);
    ioapp->SetBGColor( QColor(Qt::white) );
    
    TargetInternalLink.append(QString("layer_%1").arg(nummerlayer));
    
    QString insertText = QString(_DEFAULT_TEXT_);
    for (int o=0;o<4;o++) {
        insertText +="\n";
    }
    ioapp->setPlainText(insertText);
    ioapp->setFont(QFont("Arial"));
    ioapp->SetDimension(150,33);
    ioapp->setPos(QPointF( 10 , items.size() + 8 ));
    ioapp->setZValue(MostUp() + 2 );
    //////////////connect(ioapp, SIGNAL(Rebuild()),this, SLOT(ReDrawAll()));
    items.append(ioapp);
    PageSumm = NeedetPageSumm(QCursor::pos().y());
    UpdatePageSumms();
    emit TotalPage(PageSumm);
}

/* FAPanel */


qreal FAPanel::MostUp() 
{
    qreal zValue = 0;
     for (int o=0;o<items.size();o++) {
        if (items[o]->zValue() >= zValue) {
            zValue = items[o]->zValue() + 0.3333;
        }
    }
    
    if (zValue == 0) {
    return 11.333;
    } else {
    return zValue;
    }
}


qreal FAPanel::MostDown() 
{
    qreal zValue = 0;
     for (int o=0;o<items.size();o++) {
        if (items[o]->zValue() <= zValue) {
            zValue = items[o]->zValue() - 0.3333;
        }
    }
    return zValue;
}

void FAPanel::wheelEvent (QWheelEvent * event)
{
  scaleView(pow ((double) 2, event->delta() / 240.0));
}



/* scale the scene view */
void FAPanel::scaleView (qreal scaleFactor)
{
	qreal factor = matrix ().scale (scaleFactor,scaleFactor).mapRect (QRectF (0, 0, 1, 1)).width();
	if (factor < 0.45 || factor > 5)
	{
		return;
	}
	QGraphicsView::scale(scaleFactor, scaleFactor);
}


void FAPanel::LockItem() {
    
    if (items.size() < 1) {
    return;
    }
    
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    CurrentActive->SetModus( FloatDiagram::M_LOCK );
}

void FAPanel::UnLockItem() {
    
     if (items.size() < 1) {
    return;
    }
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    CurrentActive->SetModus( FloatDiagram::M_EDITOR );
}

void FAPanel::RotateReset() 
{
    
     if (items.size() < 1) {
    return;
    }
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    CurrentActive->SPTransForm(0);
}











void FAPanel::RotateCurrentActive( int deeg )
{
    
     if (items.size() < 1) {
    return;
    }
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    if (CurrentActive->GetModus() == FloatDiagram::M_LOCK) {
    QMessageBox::warning(0, tr("Floating Box Message"),tr("This Item is Lock,  Unlock to edit!"));
    return;
    }
    CurrentActive->SPTransForm(deeg);
}



void FAPanel::BigEdit() {
    if (items.size() < 1) {
    return;
    }
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    CurrentActive->EditAdvanceModus();
}


void FAPanel::backgroundColorTriggered() 
{
        QAction *textAction = qobject_cast<QAction *>(sender());
        QColor setNow(qVariantValue<QColor>(textAction->data()));
        ///////qDebug() << "### backgroundColorTriggered ->" << setNow.name();
        SetBackColor( setNow );
        emit PaintButton(CurrentActive);
}



void FAPanel::SetBackColor( QColor col ) {
    if (items.size() < 1) {
    return;
    }
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    if (CurrentActive->GetModus() == FloatDiagram::M_LOCK) {
    QMessageBox::warning(0, tr("Floating Box Message"),tr("This Item is Lock,  Unlock to edit!"));
    return;
    }
    
    CurrentActive->SetBGColor(col);
}





void FAPanel::RemoveItem() {
    
    if (items.size() < 1) {
    return;
    }
    
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    QString msgDB =tr("You are sur to remove this Layer?");
    int removeyes = QMessageBox::question(this, tr("Confirm"),msgDB,
                                tr("&Yes"), tr("&No"),
                                QString(),8888,9999);
    if (removeyes !=0)  {
    return;
    }
    
    int idrms = CurrentActive->Getid();
          for (int o=0;o<items.size();o++) {
              int id = items[o]->Getid();
               if (id == idrms) {
                   items[o]->deleteLater();
                   items.removeAt ( o ); 
               }
          } 
    CurrentResetNow();
    SceneRasterClear();
}

void FAPanel::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = matrix();
    resetMatrix();
    translate(oldMatrix.dx(), oldMatrix.dy());
    QGraphicsView::scale(newScale, newScale);
}
void FAPanel::ReDrawAll() {
     scene->update(scene->sceneRect());
     QMatrix oldMatrix = matrix();
     resetMatrix();
     translate(oldMatrix.dx(), oldMatrix.dy());
     QGraphicsView::scale(1.5555, 1.5555);
}
void FAPanel::bringToFront()
{
     if (items.size() < 1) {
    return;
    }
    
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    CurrentActive->setZValue(MostUp());
}

void FAPanel::sendToBack()
{
    if (items.size() < 1) {
    return;
    }
    
    if (CurrentActive->Getid() < FLOATLAYER_INCREMENT) {
    return;
    }
    CurrentActive->setZValue(MostDown());
}


void FAPanel::FillData() 
{
CurrentPrinterPage = 0;
if (db->connect()) {
UpdatePageSumms();
}
}

void FAPanel::UpdatePageSumms() 
{
int PageCanyonSpacing = PAGESPACING * PageSumm;
PrinterPageRectFix = db->page(CurrentPrinterPage)->drawpage();  /* first document size */
margin = db->page(CurrentPrinterPage)->margin();
qreal NewWallLengh = PrinterPageRectFix.height()  * PageSumm + PageCanyonSpacing;
CurrentPageRect = PrinterPageRectFix;
CurrentPageRect = QRectF(0,BEGINNATOP,PrinterPageRectFix.width(),NewWallLengh);
const int enumids = db->page(CurrentPrinterPage)->printer();
PrintZise = (QPrinter::PageSize)enumids;
scene->setSceneRect(CurrentPageRect);
MakePageformat();  /* draw pages backround grid */
}

void FAPanel::SwapPaperFormat( int index )
{
    CurrentPrinterPage = index;
    if (db->connect()) {
    UpdatePageSumms();
    }
    ReDrawAll();
}

void FAPanel::AppendOnePage()
{
    PageSumm++;
    UpdatePageSumms();
    emit TotalPage(PageSumm);
}

void FAPanel::RemoveOnePage()
{
    PageSumm--;
    if (PageSumm < 1) {
    PageSumm = 1;
    }
    UpdatePageSumms();
    emit TotalPage(PageSumm);
}






void FAPanel::OpenDesktop( QUrl loc )
{ 
    #if defined Q_WS_MAC
    QProcess *m = new QProcess();
    QStringList macs;
    macs << loc.toString();  /* oeffnet der default browser */
    m->startDetached(QString("open") , macs );
    ///////std::cout << "OpenDesktop [" << qPrintable(macs.join(" ")) << "]" << std::endl;
    return;
    #endif 
    
#if defined Q_WS_WIN
    
    QString fullFileName = loc.toString();
    
    if (fullFileName.startsWith("http://", Qt::CaseInsensitive) or 
        fullFileName.startsWith("https://", Qt::CaseInsensitive) or
        fullFileName.startsWith("ftp://", Qt::CaseInsensitive) or
        fullFileName.startsWith("news://", Qt::CaseInsensitive) or
        fullFileName.startsWith("mailto:", Qt::CaseInsensitive) or
        fullFileName.startsWith("webdav://", Qt::CaseInsensitive)  )
       { 
         /* all ok */
       } else {
         fullFileName.prepend("file:///");  
       }
    bool wr = QDesktopServices::openUrl(QUrl(fullFileName));
    if (!wr) {
    QMessageBox::warning(0, tr("Error"),tr("Unable to open file or dir  %1").arg(loc.toString()));
    }
return;
#endif
    
    /* linux */

     bool r = QDesktopServices::openUrl(loc);
    if (!r) {
    QMessageBox::warning(0, tr("Error"),tr("Unable to open file or dir  %1").arg(loc.toString()));
    }
    
}

void FAPanel::NormalPrint()
{
    PrePareToprintPaint();
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    printer.setFromTo (1, PageSumm); 
    
    QPrintDialog printDialog(&printer,this);
    printDialog.setPrintRange( QAbstractPrintDialog::Selection);
    
    
    if (printDialog.exec() == QDialog::Accepted) {
     
     
    
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        
        
        QPainter painter(&printer);
        qreal SourceHight = 0;
        int spaceready = 0;
        qreal PageCanyonSpacing = 0;
        qreal InitOnYtop = BEGINNATOP;
        
            for (int i=0; i<PageSumm;i++) {
                int nextpage = i +1;
                bool havenewpage = false;
                
                if (i != 0) {
                spaceready++;
                PageCanyonSpacing = spaceready * PAGESPACING;  /* lastspacing each page page not at first */
                SourceHight = PrinterPageRectFix.height() * spaceready;
                InitOnYtop = SourceHight + PageCanyonSpacing;
                }
                scene->render(&painter,painter.viewport(),QRectF(0,InitOnYtop,PrinterPageRectFix.width(),PrinterPageRectFix.height()),Qt::IgnoreAspectRatio);   /* Ratio must paint correct! */
                if (nextpage != PageSumm) {
                havenewpage = printer.newPage();
                }
                ////////////" Printer next?->" << havenewpage << " slice-grep->" << PrinterPageRectFix;
                /////////////qDebug() << "### print page on Y ->cm." << PointMetric( InitOnYtop )   <<  " Widht->cm." << PointMetric( PrinterPageRectFix.width() );
                ////////////qDebug() << "### printer rect  ->cm." << PointMetric( painter.viewport().width() ) << "x" << PointMetric( painter.viewport().height() );
            }
            
            
           
    
    }
    QApplication::restoreOverrideCursor();
    ResetFromToprintPaint();
}


void FAPanel::PrintPostcript()
{
    PrePareToprintPaint();
    
     QString fileName = QFileDialog::getSaveFileName(this, "Export PS",QString(setter.value("LastDir").toString()), "*.ps");
    
    if (fileName.size() > 1) {
        setter.setValue("LastDir",fileName.left(fileName.lastIndexOf("/"))+"/");
            if (!fileName.endsWith(".ps")) {
                fileName = fileName+".ps"; 
            }
            MakePrintChoise(QPrinter::PostScriptFormat,QPrinter::Portrait,fileName); 
            /* processing */
            QFile fix( fileName );
            if (fix.exists()) {
                QApplication::restoreOverrideCursor();
                /* show pdf if exist! */
                OpenDesktop(QUrl(fileName));
            }
    }
    QApplication::restoreOverrideCursor();
}

void FAPanel::MakefilePrintPdf()
{
    
    if (Current_file_from_Main_to_PDF.size() > 0) {
        PrintPdfCurrentMainFile();
        return;
    }
    
    
    PrePareToprintPaint();
    
     QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",QString(setter.value("LastDir").toString()), "*.pdf");
    if (fileName.size() > 1) {
        setter.setValue("LastDir",fileName.left(fileName.lastIndexOf("/"))+"/");
            if (!fileName.endsWith(".pdf")) {
            fileName = fileName+".pdf"; 
            }
            MakePrintChoise(QPrinter::PdfFormat,QPrinter::Portrait,fileName); 
            /* processing */
            QFile fix( fileName );
            if (fix.exists()) {
                QApplication::restoreOverrideCursor();
                /* show pdf if exist! */
                OpenDesktop(QUrl(fileName));
            }            

                        
    }
    
    QApplication::restoreOverrideCursor();
}


void FAPanel::MakePrintChoise( QPrinter::OutputFormat form , QPrinter::Orientation  orient , const QString fileName , QPrinter::ColorMode  colmod )
{
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(form);
        printer.setColorMode ( colmod );
    
        QDateTime timer1( QDateTime::currentDateTime() );
    
        printer.setDocName ( timer1.toString() );
        printer.setCreator ( _REGISTER_PROGRAM_NAME_ );
    
        printer.setOrientation ( orient );
        if (fileName.size() > 0) {
        printer.setOutputFileName(fileName);          /* file name from user input */
        }
        printer.setPageSize ( PrintZise );          
        printer.setFullPage(true);
        
        QPainter painter(&printer);
        qreal SourceHight = 0;
        int spaceready = 0;
        qreal PageCanyonSpacing = 0;
        qreal InitOnYtop = BEGINNATOP;
        
            for (int i=0; i<PageSumm;i++) {
                int nextpage = i +1;
                bool havenewpage = false;
                
                if (i != 0) {
                spaceready++;
                PageCanyonSpacing = spaceready * PAGESPACING;  /* lastspacing each page page not at first */
                SourceHight = PrinterPageRectFix.height() * spaceready;
                InitOnYtop = SourceHight + PageCanyonSpacing;
                }
                scene->render(&painter,painter.viewport(),QRectF(0,InitOnYtop,PrinterPageRectFix.width(),PrinterPageRectFix.height()),Qt::IgnoreAspectRatio);   /* Ratio must paint correct! */
                if (nextpage != PageSumm) {
                havenewpage = printer.newPage();
                }
                ////////////" Printer next?->" << havenewpage << " slice-grep->" << PrinterPageRectFix;
                /////////////qDebug() << "### print page on Y ->cm." << PointMetric( InitOnYtop )   <<  " Widht->cm." << PointMetric( PrinterPageRectFix.width() );
                ////////////qDebug() << "### printer rect  ->cm." << PointMetric( painter.viewport().width() ) << "x" << PointMetric( painter.viewport().height() );
            }
            
            
           
    
    
    QApplication::restoreOverrideCursor();
    ResetFromToprintPaint();
}

/* needet page to fill  from Y */
int FAPanel::NeedetPageSumm( qreal fromY ) 
{
    qreal Hips = PrinterPageRectFix.height() + PAGESPACING;
    int maxpge = 100;
      for (int i=1; i<maxpge;i++) {
          qreal statusP = Hips * i;
          int PaintPage = qMax(PageSumm,i);
          if (statusP > fromY) {
            return PaintPage;
          }
      }
}

void FAPanel::FontSetting()
{
  bool ok;
  QFont fontyour = QFontDialog::getFont(&ok,font(),0);
  if (ok) {
   setter.setValue("DefaultFontsW",fontyour.toString());  
   QApplication::setFont(fontyour);
  }
}

void FAPanel::PageFontSetting()
{
      QString msgDB =tr("Warning this function set font to all Lock & Unlock Layer from page:\nYou are like this?\nTip: edit single Layer on click and CTRL+X");
      int removeyes = QMessageBox::question(this, tr("Pleas confirm.."),msgDB,
                                                            tr("&Yes"), tr("&No"),
                                                             QString(),8888,9999);
  if (removeyes == 0) {
          bool ok;
          QFont fontyour = QFontDialog::getFont(&ok,font(),0);
          if (ok) {
                 for (int o=0;o<items.size();o++) {
                    items[o]->PaintAllFonts(fontyour);
                  }
          }
  }
}

void FAPanel::HtmlExport()
{
    QString stream ="<html><head>\n<meta name=\"qrichtext\" content=\"1\" />\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<meta name=\"generator\" content=\""+_PROGRAM_TITLE_+"\" />\n<style type=\"text/css\">\n"
                    "p, li, span { \n"
                      "white-space: pre-wrap;       /* css-3 */ \n"
                      "/* white-space: -moz-pre-wrap;   Mozilla, since 1999 */ \n"
                      "/* white-space: -pre-wrap;       Opera 4-6 */ \n"
                      "/* white-space: -o-pre-wrap;     Opera 7 */ \n"
                      "/* word-wrap: break-word;        Internet Explorer 5.5+ */ \n"
                    "\n}\n"
                    "</style></head>\n"
                    "<body>\n";
    
    QString fileName = QFileDialog::getSaveFileName(this, "Export HTML",QString(setter.value("LastDir").toString()), "*.html");
    
    if (fileName.size() > 0) {
           if (!fileName.endsWith(".html")) {
            fileName = fileName+".html"; 
            }
            
            
            QFileInfo fix( fileName );
            
                  for (int o=0;o<items.size();o++) {
                    stream.append(items[o]->toDivHtml(fix));
                  }
                  
        stream.append("\n</body></html>\n");
                  
        QFile f( fileName );
        if ( f.open( QFile::WriteOnly | QFile::Text ) )
        {
        QTextStream sw( &f );
        sw.setCodec(QTextCodec::codecForName("utf-8"));
        sw << stream;
        f.close();
        }
                  
    }
}


void FAPanel::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen( Qt::NoPen );
    painter->setBrush(Qt::white);
    painter->drawRect(rect);   /* fill all  white */
    
    painter->setBrush(QBrush(chessgrid));   /* prepare chess grid */
    
        qreal SourceHight = 0;
        int spaceready = 0;
        qreal PageCanyonSpacing = 0;
        qreal InitOnYtop = BEGINNATOP;
    
        qreal TopMargin = margin.x();
        qreal RightMargin = margin.y();
        qreal BottomMargin = margin.width();
        qreal LeftMargin = margin.height();
        const QRectF  PageMarginer(LeftMargin,TopMargin + BEGINNATOP,
                       PrinterPageRectFix.width() - LeftMargin - RightMargin,
                       PrinterPageRectFix.height() - TopMargin - BottomMargin);
        QRectF LoopMargin = PageMarginer;
    
     for (int i=0; i<PageSumm;i++) {
                QRectF LoopMargin = PageMarginer;
                if (i != 0) {
                spaceready++;
                PageCanyonSpacing = spaceready * PAGESPACING;  /* lastspacing each page page not at first */
                SourceHight = PrinterPageRectFix.height() * spaceready;
                InitOnYtop = SourceHight + PageCanyonSpacing;
                LoopMargin = QRectF (LeftMargin , InitOnYtop + TopMargin , PageMarginer.width() , PageMarginer.height() );
                }
                QRectF  pagen(0,InitOnYtop,PrinterPageRectFix.width(),PrinterPageRectFix.height());
                   QRectF rightShadow(pagen.right(), pagen.top() + 5, 5, pagen.height());
                   QRectF bottomShadow(pagen.left() + 5, pagen.bottom(), pagen.width(), 5);
                   painter->fillRect(rightShadow, Qt::darkGray);
                   
                   painter->fillRect(bottomShadow, Qt::darkGray);
                   painter->setPen( Qt::NoPen );
                   painter->setBrush(QBrush(chessgrid)); 
                   painter->drawRect(pagen);
                   painter->setBrush( Qt::NoBrush);
                   painter->setPen( QPen(Qt::darkGray,1) );
                   painter->drawRect(LoopMargin);
    }
}


