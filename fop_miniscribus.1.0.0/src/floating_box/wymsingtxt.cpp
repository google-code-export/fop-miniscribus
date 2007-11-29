#include "wymsingtxt.h"
//
/*  Save file as wymsingtxt.cpp  */
/*  incomming class name WymsingTxt */
//
#include <QCloseEvent>

void WymsingTxt::UpdateVars()  {

    BorderDicks = MarginSize->value();
    InitValue.setWidth( BoxLarghezza->value() );
    InitValue.setHeight ( BoxAltezza->value() );
    BGColor.setAlpha ( AlphaColor );
    rootformat.setBottomMargin ( psud->value() );
    rootformat.setLeftMargin ( pwest->value() );
    rootformat.setRightMargin ( post->value() );
    rootformat.setTopMargin ( pnord->value() );
    rootformat.setBackground ( BGColor ); 
    tdoc = wtxt->document()->clone();
    QTextFrame  *TfNewFo = tdoc->rootFrame();
    TfNewFo->setFrameFormat ( rootformat );
    DocFrameFormings = rootformat;
    wtxt->setDocument ( tdoc );
    wtxt->document()->setUseDesignMetrics (true );
    destimage->setDocument ( tdoc );
    destimage->setPos ( QPointF(XXset->value(),YYset->value()) );
    emit RetocValues(InitValue,BorderDicks,BGColor,MarginColor,tdoc,wtxt->GetListBlock());
    DocFrameFormings.setWidth( BoxLarghezza->value() );
    wtxt->document()->rootFrame()->setFrameFormat ( DocFrameFormings ); 
        

}

void WymsingTxt::SetInternalLink( QStringList l )
{
   wtxt->Linkers(l);
}

/* XxAxys */


WymsingTxt::WymsingTxt( QRectF recter , QColor sfondo , QColor margincolor , qreal borders , int rotate , QTextDocument *item , TypImageContainer block ,  QGraphicsItem  * parent )
{
	setupUi( this );
    Qfontlist->setFontFilters( QFontComboBox::ScalableFonts );
    
    
    
    
    QStringList palign;
    palign << "[1] AlignLeft" << "[2] AlignRight" << "[4] AlignHCenter";
    alignetable->clear();
    alignetable->addItems(palign);
    
    
    if (QGraphicsTextItem *image = qgraphicsitem_cast<QGraphicsTextItem *>(parent)) {
    ////////qDebug() << "### trovato " << image->boundingRect();
    
        connect(XxAxys, SIGNAL(valueChanged(int)), this, SLOT(RotateFloat(int)));
        
        wtxt->setDocument ( image->document() );
        wtxt->document()->adjustSize();
        
        DocFrameFormings =  wtxt->document()->rootFrame()->frameFormat();
        DocFrameFormings.setBackground(sfondo);
        DocFrameFormings.setWidth( image->boundingRect().width() );
        wtxt->document()->rootFrame()->setFrameFormat ( DocFrameFormings ); 
        wtxt->document()->setUseDesignMetrics (true );
       
        
        
      tablegr->hide();    /* propryetys setter */
      imagegr->hide();   /* propryetys setter */
      wtxt->AppendImageBlocks(block);
      CatchPixmapCurrent("");
        
        QPointF globalpos = image->pos();
        XXset->setValue(globalpos.x());
        YYset->setValue(globalpos.y());
        
        connect(XXset, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
        connect(YYset, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));

    destimage = image;
    /////wtxt->setEnabled(true);
    loadetConnection = false;
    workfont = item->defaultFont();
    ////////qDebug() << "### textWidth   " << item->textWidth();
    tdoc = item->clone();
    QTextFrame  *Tframe = tdoc->rootFrame();
    rootformat = Tframe->frameFormat();
    pnord->setValue(rootformat.topMargin());
    psud->setValue(rootformat.bottomMargin());
    pwest->setValue(rootformat.leftMargin());
    post->setValue(rootformat.rightMargin());
    
    connect(pnord, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
    connect(psud, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
    connect(pwest, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
    connect(post, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
    LoadButtonControll();
    InitValue = recter;
    
    
   
    
    
    
    BorderDicks = borders;
    BGColor = sfondo;
    AlphaColor = sfondo.alpha();
    MarginColor = margincolor;
    
     QPixmap pixbg(22, 22);
     pixbg.fill(sfondo);
    
     QPixmap pixma(22, 22);
     pixma.fill(margincolor);
    
      Mabg->setIcon(pixma);
      Cobg->setIcon(pixbg);
      MarginSize->setValue ( borders );
    
      BoxAltezza->setValue ( recter.height() );
      BoxLarghezza->setValue ( recter.width() );
    
      ///////Vol_Rotate->setValue ( rotate );
      connect(BoxAltezza, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
      connect(BoxLarghezza, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
      connect(MarginSize, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
      
      connect(Mabg, SIGNAL(clicked()), this,  SLOT(SetMarginColor()));
      connect(Cobg, SIGNAL(clicked()), this,  SLOT(SetBGColor()));
      connect(CurrentImagePlay, SIGNAL(clicked()), this,  SLOT(ImageToClipbord()));
      connect(SaveToPath, SIGNAL(clicked()), this,  SLOT(SaveCurrentImageStream()));
      
      
      connect(Vol_21, SIGNAL(clicked()), wtxt, SLOT(InsertImageonCursor()));
      
      
      connect(pushButton, SIGNAL(clicked()), this, SLOT(SaveNewFormat()));
      connect(EditableOr, SIGNAL(toggled(bool)), wtxt, SLOT(Editablemodus(bool)));
      connect(wtxt, SIGNAL(SaveStreamer()), this, SLOT(SaveNewFormat()));
      connect(wtxt, SIGNAL(DDclick()), this, SLOT(TableImageSetup()));
      
      connect(tabiborder, SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentTable()));
      connect(imghi, SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentImage()));
      connect(imgwi, SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentImage()));
   
      
      
      connect(tabiwi, SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentTable()));
      connect(tabipadding, SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentTable()));
      connect(tabiborder, SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentTable()));
      connect(tabispacing, SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentTable()));
      
      connect(alignetable, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(UpdateCurrentTable()));
      
      connect(Vol_6, SIGNAL(clicked(bool)), wtxt, SLOT(MakeHrefLink()));
      connect(Vol_22, SIGNAL(clicked()), this, SLOT(CurrentParamMargin()));
      
    
    
      
///////////connect(tabiborder, SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentTable()));

      //////////FontChanged(workfont);
      
  }
    
   GoPaintTable = false;  
}

void WymsingTxt::RotateFloat( int rotaz )
{
    int urot = qBound(0,rotaz,359);
    emit TransnRotation(urot);
}


void WymsingTxt::LoadButtonControll()
{
    
    list_ul->clear();
    list_ul->addItem("Standard");
    list_ul->addItem("Bullet List (Disc)");
    list_ul->addItem("Bullet List (Circle)");
    list_ul->addItem("Bullet List (Square)");
    list_ul->addItem("Ordered List (Decimal)");
    list_ul->addItem("Ordered List (Alpha lower)");
    list_ul->addItem("Ordered List (Alpha upper)");
    
    
    connect(Vol_7, SIGNAL(clicked()), this, SLOT(MakeTextBold()));
    connect(Vol_8, SIGNAL(clicked(bool)), wtxt, SLOT(setFontItalic(bool)));
    connect(Vol_9, SIGNAL(clicked(bool)), wtxt, SLOT(setFontUnderline(bool)));
    
    connect(Vol_20, SIGNAL(clicked()), wtxt, SLOT(CreateanewTable()));
    
    
    
    
    connect(list_ul, SIGNAL(activated(int)), this, SLOT(TextStyle(int)));
    
    //////////////////connect(wtxt, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    connect(wtxt, SIGNAL(ActualFormat(const QTextCharFormat &)), this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    connect(Qfontlist, SIGNAL(currentFontChanged(QFont)), this, SLOT(WakeUpFont()));
    connect(strechFont, SIGNAL(valueChanged(int)), this, SLOT(WakeUpFont()));
    connect(Fpoint, SIGNAL(valueChanged(double)), this, SLOT(WakeUpFont()));
    
    QActionGroup *grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(MaketextAlign(QAction *)));
    actionAlignLeft = new QAction(QIcon(":/img/textleft.png"), tr("Text align left"),grp);
    actionAlignLeft->setCheckable(true);
    actionAlignRight = new QAction(QIcon(":/img/textright.png"), tr("Text align right"),grp);
    actionAlignRight->setCheckable(true);
    actionAlignCenter = new QAction(QIcon(":/img/textcenter.png"), tr("Text align center"),grp);
    actionAlignCenter->setCheckable(true);
    actionAlignJustify = new QAction(QIcon(":/img/textjustify.png"), tr("Text align Justify"),grp);
    actionAlignJustify->setCheckable(true);
    
    Vol_10->setDefaultAction(actionAlignLeft);
    Vol_12->setDefaultAction(actionAlignRight);
    Vol_11->setDefaultAction(actionAlignCenter);
    Vol_13->setDefaultAction(actionAlignJustify); 
    
    
    TableColorize = new QAction(tr("Table Background color"),this);    /* QAction *TableColorize;  */
    TableColorize->setData(QVariant(QColor(Qt::white)));
    TableColorize->setIcon(createColorIcon(Qt::white));  
    connect(TableColorize, SIGNAL(triggered()), this, SLOT(MaketableColorBG()));
    tablecolor->setDefaultAction(TableColorize);
    
   
    actionTextColor = new QAction(tr("Font Color"),this);
    actionTextColor->setCheckable(true);
    actionTextColor->setIcon(createColorIcon(Qt::black));  
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(MaketextColor()));
    Vol_19->setDefaultAction(actionTextColor);
    
    //////////////FontChanged(workfont);
    loadetConnection = true;
}



void WymsingTxt::MaketableColorBG()
{
    bool ok;
    QColor col = QColorDialog::getRgba(NULL,&ok, this);
    if (!col.isValid()) {
        return;
    } else {
    TableColorize->setIcon(createColorIcon(col));
    TableColorize->setData(QVariant(col));
    UpdateCurrentTable();
    }
}



QIcon WymsingTxt::createColorIcon(QColor color) 
{
    QPixmap pixmap(22, 22);
    pixmap.fill(color);
    return QIcon(pixmap);
}

void  WymsingTxt::MakeTextBold()
{
  wtxt->setFontWeight(Vol_7->isChecked() ? QFont::Bold : QFont::Normal);
}









void WymsingTxt::currentCharFormatChanged(const QTextCharFormat &format)
{
    workfont = format.font();
    PlaceNowFontChanged(format.font());
    QPixmap pix(22, 22);
    pix.fill(format.foreground().color());
    actionTextColor->setIcon(pix);   /* set  font color pilot */
    MakealignmentChanged(wtxt->alignment());  /* set allignment */
}

void WymsingTxt::TextStyle(int styleIndex)
{
    QTextCursor cursor = wtxt->textCursor();

    if (styleIndex != 0) {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        switch (styleIndex) {
            default:
            case 1:
                style = QTextListFormat::ListDisc;
                break;
            case 2:
                style = QTextListFormat::ListCircle;
                break;
            case 3:
                style = QTextListFormat::ListSquare;
                break;
            case 4:
                style = QTextListFormat::ListDecimal;
                break;
            case 5:
                style = QTextListFormat::ListLowerAlpha;
                break;
            case 6:
                style = QTextListFormat::ListUpperAlpha;
                break;
        }

        cursor.beginEditBlock();

        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }

        listFmt.setStyle(style);

        cursor.createList(listFmt);

        cursor.endEditBlock();
    } else {
        // ####
        cursor.beginEditBlock();
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
        cursor.endEditBlock();
    }
}


void WymsingTxt::SaveNewFormat()  {
    if (!loadetConnection) {
    return;
    }
    emit RetocValues(InitValue,BorderDicks,BGColor,MarginColor,wtxt->document()->clone(),wtxt->GetListBlock());
    deleteLater ();
}


void WymsingTxt::MaketextColor()
{
    bool ok;
    QColor col = QColorDialog::getRgba(NULL,&ok, this);
    if (!col.isValid()) {
        return;
    } else {
        
    wtxt->setTextColor(col);
    //////WakeUpFont();
    QPixmap pix(22, 22);
    pix.fill(col);
    actionTextColor->setIcon(pix);  
    }
}

void WymsingTxt::MaketextAlign(QAction *a)
{
    if (a == actionAlignLeft)
        wtxt->setAlignment(Qt::AlignLeft);
    else if (a == actionAlignCenter)
        wtxt->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        wtxt->setAlignment(Qt::AlignRight);
    else if (a == actionAlignJustify)
        wtxt->setAlignment(Qt::AlignJustify);
}

void WymsingTxt::MakealignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft)
        actionAlignLeft->setChecked(true);
    else if (a & Qt::AlignHCenter)
        actionAlignCenter->setChecked(true);
    else if (a & Qt::AlignRight)
        actionAlignRight->setChecked(true);
    else if (a & Qt::AlignJustify)
        actionAlignJustify->setChecked(true);
}



void WymsingTxt::SetBGColor() {
    bool ok;
     QRgb col = QColorDialog::getRgba(BGColor.rgba(),&ok);
     if (!ok) {
        return; 
     }
     BGColor = QColor( col );
     AlphaColor = qAlpha ( col );
     QPixmap pixbg(22, 22);
     pixbg.fill(BGColor);
     Cobg->setIcon(pixbg);
     UpdateVars();
     ///////////emit RetocValues(InitValue,BorderDicks,BGColor,MarginColor,wtxt->document()->clone());
}

void WymsingTxt::SetMarginColor() {
    bool ok;
     QRgb col = QColorDialog::getRgba(MarginColor.rgba(),&ok);
     if (!ok) {
        return; 
     }
     MarginColor = QColor( col );
     MarginColor.setAlpha(255);
     QPixmap pixma(22, 22);
     pixma.fill(MarginColor);
     Mabg->setIcon(pixma);
     UpdateVars();
     /////////////emit RetocValues(InitValue,BorderDicks,BGColor,MarginColor,wtxt->document()->clone());
}


void WymsingTxt::closeEvent( QCloseEvent* e )
{
    
	e->accept();
}







void WymsingTxt::UpdateCurrentTable()
{
    c = wtxt->textCursor();
    if (c.currentTable() && GoPaintTable ) {
        
        int newalign = BraketNummer(alignetable->currentText());
        
        QTextTableFormat taform = c.currentTable()->format(); 
        taform.setWidth ( QTextLength ( QTextLength::FixedLength,tabiwi->value() )  );

              if (newalign == 1) {
                 taform.setAlignment(Qt::AlignLeft);
                } else if (newalign == 2) {
                 taform.setAlignment(Qt::AlignRight);  
                } else if (newalign == 4) {
                  taform.setAlignment(Qt::AlignHCenter);  
                }
        
        taform.setBorder(tabiborder->value());
        taform.setCellPadding(tabipadding->value());
        taform.setCellSpacing(tabispacing->value());
        QColor setbgcolor(qVariantValue<QColor>(TableColorize->data()));
        taform.setBackground ( setbgcolor ); 
        c.currentTable()->setFormat(taform);
    }
    
    GoPaintTable = false;
    TableImageSetup();
}


void WymsingTxt::ReadNowTable( QTextTable * table )
{
    if (!table) {
    return;
    }
    if (!tablegr->isVisible())  {
    return;
    }
            QTextTableFormat taform = table->format(); 
            QColor CBoloresfondo = taform.background().color();
            ////////qDebug() << "### CBoloresfondo valid -> " << CBoloresfondo.isValid();
            if (taform.width().type() == QTextLength::FixedLength) {
             tabiwi->setValue(taform.width().rawValue());
            } else {
             /* caso tabella non è a lunghezza fissa */
             tabiwi->setValue(BoxLarghezza->value()); 
             taform.setWidth ( QTextLength ( QTextLength::FixedLength, BoxLarghezza->value() )  );
             table->setFormat(taform);                
            }
            tabiborder->setValue(taform.border());
            tabipadding->setValue(taform.cellPadding());
            tabispacing->setValue(taform.cellSpacing());
            /* aggiorna bottone */
            if (CBoloresfondo.isValid()) {
            TableColorize->setData(QVariant(CBoloresfondo));
            TableColorize->setIcon(createColorIcon(CBoloresfondo));
            }
            
           int  setfoundalign = 0;
           int allineaesistente = taform.alignment();
            
        if (allineaesistente > 0) {
                if (allineaesistente == 1) {
                 setfoundalign = 0;
                } else if (allineaesistente == 2) {
                 setfoundalign = 1;
                } else if (allineaesistente == 4) {
                 setfoundalign = 2;
                }
        }
        if (setfoundalign > 0) {
            alignetable->setCurrentIndex(setfoundalign);
        }
            
            
 GoPaintTable = true;
}

void WymsingTxt::ResettingTableControll() 
{
    TableColorize->setData(QVariant(QColor(Qt::white)));
    TableColorize->setIcon(createColorIcon(Qt::white)); 
    tabiwi->setValue(0); 
    tabiborder->setValue(0);
    tabipadding->setValue(0);
    tabispacing->setValue(0);
}


void WymsingTxt::CatchPixmapCurrent( const QString  resname ) 
{
    CurrentResname = resname;
    if (resname.startsWith("/png/",Qt::CaseInsensitive)) {
        
         QPixmap tmpresultimage;
         tmpresultimage.loadFromData( wtxt->ResourceBlock(resname).toByteArray() );
        
    currentim = tmpresultimage;
        
    ///////qDebug() << "### CatchPixmapCurrent png ";
    } else if (resname.startsWith("/svg/",Qt::CaseInsensitive)) {
    currentim = RenderPixmapFromSvgByte(wtxt->ResourceBlock(resname).toByteArray());
    //////////////qDebug() << "### CatchPixmapCurrent svg";
    } else {
    currentim = QPixmap(QString::fromUtf8(":/img/wp.png"));
    ///////////////////////qDebug() << "### CatchPixmapCurrent null 1";
    }
    
    CurrentImagePlay->setIcon(QIcon(currentim));
}

void WymsingTxt::SaveCurrentImageStream() 
{
  QString filenameimg = FilenameAllow(CurrentResname);
  QString filters;
  QByteArray datacaan;
  if (CurrentResname.size() > 0) {
      
      if (CurrentResname.contains("/png/")) {
          filenameimg.append(".png");
          filters ="*.png";
          datacaan = wtxt->ResourceBlock(CurrentResname).toByteArray();
      } else if (CurrentResname.contains("/svg/")) {
          filenameimg.append(".svg");
          filters ="*.svg";
          datacaan = wtxt->ResourceBlock(CurrentResname).toByteArray();
      }
      
         QString fileName = QFileDialog::getSaveFileName(this, "Export Image",QString(setter.value("LastDir").toString()+filenameimg),filters);
             if (fileName.size() > 1) {
                 
                 QString ultimacartellaaperta = fileName.left(fileName.lastIndexOf("/"))+"/";
                 setter.setValue("LastDir",ultimacartellaaperta);  /* modus on cms not editor !!! */
                 
                   
                           QFile f(fileName);
                            if ( f.open( QIODevice::WriteOnly ) ) {
                                f.write(datacaan);
                                f.close();
                                return;
                            } 
                            
                   
                   
                 
             }
  }
}


void WymsingTxt::CurrentParamMargin()
{
    GetMargin *marge = new GetMargin(this);
    marge->setWindowTitle(tr("Current margin on Paragraph..."));
    
    lastcursor = wtxt->textCursor();
    textblocc = lastcursor.block();
    QTextBlockFormat fo = textblocc.blockFormat();
    marge->Set_margin( QRectF (fo.topMargin(), fo.rightMargin() ,fo.bottomMargin() , fo.leftMargin() ) );
    marge->Set_margin( QRectF (fo.topMargin(), fo.rightMargin() ,fo.bottomMargin() , fo.leftMargin() ) );
    int faxme = marge->exec();
    
    if (faxme== 1) {
        QRectF ma = marge->Get_margin();
        fo.setTopMargin(ma.x());
        fo.setRightMargin(ma.y());
        fo.setBottomMargin(ma.width());
        fo.setLeftMargin(ma.height());
        lastcursor.setBlockFormat(fo);
        qDebug() << "### CurrentParamMargin -> " << ma;
         /* QRectF ( qreal x, qreal y, qreal width, qreal height )  */
    }
    
    qDebug() << "### CurrentParamMargin -> " << faxme;
}


void WymsingTxt::ImageToClipbord()
{
    QClipboard *baseram = QApplication::clipboard();
    baseram->setPixmap(currentim,QClipboard::Clipboard); 
    QMessageBox infoset;
                infoset.setWindowTitle (  tr("Image on Clipboard") );
                infoset.setIconPixmap( currentim );
                infoset.exec();  
}


void WymsingTxt::UpdateCurrentImage()
{
    c = wtxt->textCursor();
    CatchPixmapCurrent(c.charFormat().toImageFormat().name());
    
    
    QTextImageFormat imgfo = c.charFormat().toImageFormat();
      if (imgfo.isValid()) {
      if (!imagegr->isVisible())   {
          imagegr->show();
      }
      
      const QString  resname = imgfo.name();
      const qreal oldwi = imgfo.width();
      ////////qreal oldhi = imgfo.height();
      
      ///////const QTextBlock current = imgfo.toBlockFormat();
      
      QAbstractTextDocumentLayout *Layout = wtxt->document()->documentLayout();
      QRectF mesures = Layout->blockBoundingRect ( c.block() );
      
      ///////////////////qDebug() << "image mesures:" << mesures;
      
      QPixmap Scaledoncopy(currentim.size());
              Scaledoncopy.fill(Qt::transparent);    /* scaled on a transparent fast image !! */
      
      QPixmap mesure;
      
      if (propportionimage->isChecked()) {
          if (oldwi != imgwi->value()) {
           mesure =  Scaledoncopy.scaledToWidth(imgwi->value());
          } else {
           mesure = Scaledoncopy.scaledToHeight(imghi->value());
          }
      } else {
          mesure = currentim.scaled(imgwi->value(),imghi->value(),Qt::IgnoreAspectRatio);
      }
      
      if (!mesure.isNull()) {
         imgfo.setHeight ( mesure.height() ); 
         imgfo.setWidth ( mesure.width() );
         imgdimension->setText(QString("Pt.%1X%3").arg(mesure.width()).arg(mesure.height()));
         c.setCharFormat(imgfo);
      }
      
      
      
  }
}

void WymsingTxt::TableImageSetup()  
{
    c = wtxt->textCursor();
    CatchPixmapCurrent(c.charFormat().toImageFormat().name());
    
 
    
    QTextImageFormat imgfo = c.charFormat().toImageFormat();
    /* table start setter */
    if (c.currentTable()) {
        /* setting table alle */
        if (!tablegr->isVisible())   {
        tablegr->show();
        }
        ReadNowTable(c.currentTable());
    } else {
     tablegr->hide();
     ResettingTableControll();
    }
    /* table end  setter */
    
    if (imgfo.isValid()) {
      if (!imagegr->isVisible())   {
          imagegr->show();
      }
          const QString  resname = imgfo.name();
          if (resname.size() > 0) {
          imgname->setText(resname);
          } else {
          imgname->setText(tr("Unknow image!"));
          }
          imgdimension->setText(QString("Pt.%1X%3").arg(imgfo.width()).arg(imgfo.height()));
          imgwi->setValue(imgfo.width());
          imghi->setValue(imgfo.height());
      
      
    } else {
       imagegr->hide();
       CurrentResname = "";
    }
}


void WymsingTxt::WakeUpFont()  
{
    if (!loadetConnection) {
    return;
    }
    wtxt->document()->setUseDesignMetrics (true );
    workfont = Qfontlist->currentFont();
    workfont.setKerning(true);
    workfont.setFixedPitch ( true ); 
    
    
    workfont.setPointSizeF ( Fpoint->value() ) ;
    workfont.setStretch( QFont::UltraExpanded );
    QTextCursor c = wtxt->textCursor();
    QTextCharFormat  format = c.charFormat();
    format.setFont ( workfont );
    c.setCharFormat(format);
    
      QString anchor = format.anchorHref();
    
    if (anchor.size() > 0) {
    Vol_6->setChecked ( true );
    } else {
    Vol_6->setChecked ( false );
    }
    
    qDebug() << "### TextFamily " << workfont.family();
    qDebug() << "### Points " << workfont.pointSizeF();
    qDebug() << "### strechFont" << workfont.stretch();
}

/* read modus !!!!!!!!!!!!!!!!!!!!!!!! */
void WymsingTxt::PlaceNowFontChanged(const QFont &f)
{
    workfont = Qfontlist->currentFont();
    Qfontlist->setCurrentIndex(Qfontlist->findText(f.family()));
    Fpoint->setValue ( f.pointSizeF() );
    Vol_7->setChecked(f.bold());
    Vol_8->setChecked(f.italic());
    Vol_9->setChecked(f.underline());
    Vol_6->setChecked(f.underline());
}





