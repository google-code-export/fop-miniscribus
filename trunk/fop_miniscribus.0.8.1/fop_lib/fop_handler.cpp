#include "fop_handler.h"

/* paragraph  fop -> qt  */
void  Fop_Handler::ParserParagraph( const QDomElement e , QTextDocument * d , Fop_Layer * layer , int CursorPositon , bool firsttdcell ) 
{
    
    /////if (FoTag(e.firstChild().toElement()) != BLOCK_TAG) {
    //////////ParserParagraph(e.firstChild().toElement(),d,layer,0);
   ///////// }
    
    if (FoTag(e) != BLOCK_TAG) {
    return;
    }
    ParaGraphCounter++;
    Fop_Block *foppi = new Fop_Block(ParaGraphCounter,e);
    int CursorEndPosition = 0;
    
    bool IsOnTable = false;
    const QString Actual_Line_Text = e.text();
    QTextCursor Tcursor = QTextCursor(d);
    bool IsCorrectCursor = Tcursor.movePosition(QTextCursor::End);
    CursorEndPosition = Tcursor.position();
    ///////qDebug() << "### ParserParagraph ###  (" << e.tagName().toLower() << " txt->" << Actual_Line_Text << "  nr." << ParaGraphCounter  << " cursorP->" << CursorPositon << " cursor_end->" << CursorEndPosition  << ")";
    /* elements not on block format fop */
    if (CursorPositon !=0) {
    ////////qDebug() << "### ParserParagraph  cursor 1 ### from CursorPositon ";
    Tcursor.setPosition(CursorPositon);  
    } else if (CursorStatementPosition > 4 ) {
    //////qDebug() << "### ParserParagraph  cursor 2 ### from CursorStatementPosition ";
    Tcursor.setPosition(CursorStatementPosition);   /* the last end from paragraph line or table cell paragraph ! */
    } else {
    //////qDebug() << "### ParserParagraph  cursor 3 ### from goto end document ";
    Tcursor.setPosition(CursorEndPosition);         /* end of document if no ram position */
    }
    
    
    
    
    QTextBlockFormat GlobalZeroMargin = DefaultMargin();    /* default  QTextBlockFormat */
    QTextBlockFormat EntireBlockFormat = TextBlockFormFromDom(e,GlobalZeroMargin);  /* overwrite */
    QTextCharFormat  ParCharFormat = GlobalCharFormat(e);
    ParCharFormat.setToolTip ( foppi->Get_XML() ); 
    
    
    if (CursorEndPosition !=0 && !firsttdcell ) {
    Tcursor.insertBlock();
    }
    
    
    /*
    Tcursor.beginEditBlock();
    Tcursor.setBlockFormat(EntireBlockFormat);
    Tcursor.insertText(Actual_Line_Text+QString(" -%1-").arg(ParaGraphCounter));
    Tcursor.endEditBlock();
    */
    
    
    
     Tcursor.beginEditBlock();
     QTextBlock blockNow = Tcursor.block();
     blockNow.setUserData(foppi);  /* fop source original to show */
    
     
     
   
    
     Tcursor.setBlockFormat(EntireBlockFormat);
     Tcursor.setCharFormat(ParCharFormat);
     
    if (ParaGraphCounter == 1) {
    Tcursor.insertText("",targetAnchor);   /* layer destination link  #layernummer */
    }
     
     QDomNode child = e.firstChild();
    
       while ( !child.isNull() ) {
           if ( child.isElement() ) {
               const QDomElement childElement = child.toElement();
               const QTextCharFormat InlineStyle = GlobalCharFormat(childElement);
               
               if ( FoTag(childElement) == INLINE_STYLE) {
                Tcursor.setCharFormat(InlineStyle);
                Tcursor.insertText(childElement.text().replace("\n"," "));
                Tcursor.insertText("");
                Tcursor.setCharFormat(ParCharFormat);
                } else if ( FoTag(childElement) == LINK_DOC ) {
                Tcursor.setCharFormat(InlineStyle);
                Tcursor.insertText(childElement.text().replace("\n"," "));
                Tcursor.setCharFormat(ParCharFormat);
                } else if ( FoTag(childElement) == FOCHAR  ) {
                 Tcursor.setCharFormat(InlineStyle);
                 Tcursor.insertText(childElement.attribute ("character"));
                 Tcursor.setCharFormat(ParCharFormat); 
                }  else if ( FoTag(childElement) == TABLE_TAG  ) {
                    Tcursor.setBlockFormat(DefaultMargin());
                    ParserTable(childElement,d,layer,Tcursor.position());
                } else if ( FoTag(childElement) == BLOCK_TAG  ) {
                    ParserParagraph(childElement,d,layer,Tcursor.position());       /* only block/block loops */
                }  else if ( FoTag(childElement) == IMAGE_SRC ) {
                     Tcursor.setBlockFormat(DefaultMargin());
                     ImageParserTag(childElement,d,layer,Tcursor.position());   /* fo:external-graphic */
                } else if ( FoTag(childElement) == IMAGE_INLINE ) {
                     Tcursor.setBlockFormat(DefaultMargin());
                     ObjectParserTag(childElement,d,layer,Tcursor.position());  /* inline svg images */
                }
               
           } else if (child.isText()) {
               
               const QString FragText = child.toText().data().replace("\n"," ");
               if (FragText.size() > 0) {
               Tcursor.insertText(FragText);
               } else {
               Tcursor.insertText("error null text ");
               }
               
            Tcursor.setCharFormat(ParCharFormat);
           }
           
          child = child.nextSibling(); 
       }
       
       Tcursor.endEditBlock();
       Tcursor.atBlockEnd();    /* bring cursor on the same end line block ! not new line */
       if (firsttdcell) {
       CursorStatementPosition = Tcursor.position();   /* latest cursor position  */
       } else {
       CursorStatementPosition = Tcursor.position();
       }
       
    
}




/* paragraph  qt -> fop */
void Fop_Handler::HandleBlock( QTextBlock  para  , QDomElement appender ) 
{
    if (!para.isValid()) {
    return;
    }
    const QString Actual_Text_Param = para.text();
    QTextBlockFormat ParaBlockFormat = para.blockFormat();
    const QTextCharFormat CharFromPara = para.charFormat();  
    QDomElement paragraph;
    QTextImageFormat Pics;
    QTextTableFormat Tabl;
    QTextListFormat Uls;
    QTextCharFormat paraformats;
    QString newnameimage;
    QString ImageFilename;
    
       int positioner = -1;
    
    paragraph = wdoc.createElement("fo:block");
    PaintFopBlockFormat(paragraph,ParaBlockFormat);  /* block */
    PaintCharFormat(paragraph,CharFromPara);
    //////////////qDebug() << "### open block fo:block ..................................";
    
                    
       QTextBlock::iterator de;

     
       for (de = para.begin(); !(de.atEnd()); ++de) {
                       
                       
           
          QTextFragment fr = de.fragment();
              if (fr.isValid()) {
               positioner++;
                  const QTextCharFormat base = fr.charFormat();
                  int staycursorpos =  fr.position();
                  Pics = base.toImageFormat();
                  Tabl = base.toTableFormat();
                  Uls =  base.toListFormat();
                  /* ############################### Image blocks ###########################################*/
                  /* ############################### Image blocks ###########################################*/
                  if (Pics.isValid()) {
                  const QString hrefadress = Pics.name();
                  if (hrefadress.startsWith("/svg/",Qt::CaseInsensitive)) {
                  /* difference only dwo way vector and other as png! */
                  newnameimage = FilenameAllow( CurrentIdLayer + "-" + hrefadress + ".svg");
                  } else {
                  newnameimage = FilenameAllow( CurrentIdLayer + "-" + hrefadress + ".png");
                  }
                  
                  
                  ImageFilename = newnameimage;
                  newnameimage = newnameimage.prepend(ImageFopBaserRefDir);
                    if (hrefadress.contains("external")) {
                           bool aisalvato = SaveImageExterPath(hrefadress,newnameimage);
                              QDomElement imageELO = wdoc.createElement("fo:external-graphic");
                              
                              imageELO.setAttribute ("src",FOPIMAGEDIR + ImageFilename);
                              if (Pics.width() > 0) {
                              imageELO.setAttribute ("width",QString("%1pt").arg(Pics.width()));
                              }
                              if (Pics.height() > 0) {
                              imageELO.setAttribute ("height",QString("%1pt").arg(Pics.height()));
                              }
                              /* if saved image append */
                              if (aisalvato) {
                              paragraph.appendChild(imageELO);
                              }
                              //////////qDebug() << "### salvataggio di  -> " << ImageFilename << " si/no->" << aisalvato; 
                          
                   }  else if (hrefadress.contains("SVG_instream-foreign-object")) {
                       QDomElement imageinline = wdoc.createElement("fo:instream-foreign-object");  /* svg inline */
                       
                              if (Pics.width() > 0) {
                              imageinline.setAttribute ("width",QString("%1pt").arg(Pics.width()));
                              }
                              if (Pics.height() > 0) {
                              imageinline.setAttribute ("height",QString("%1pt").arg(Pics.height()));
                              }
                              /* lets grab xml from vector image */
                              bool takesvghaving = BuildSvgInlineonDom(hrefadress,imageinline,wdoc);
                              if (takesvghaving) {
                              paragraph.appendChild(imageinline);
                              }
                       
                   }
                  /* ############################### Image blocks ###########################################*/
                  /* ############################### Image blocks ###########################################*/
                  } else if (Tabl.isValid()) {
                      //////qDebug() << "### Table ";
                      //////////QTextTable *childTable = qobject_cast<QTextTable*>(fr);
                      ///////////HandleTable( QTextTable  * childTable ,paragraph);
                      /////////HandleTable(childTable,paragraph);
                  }  else if (Uls.isValid()) {
                      ////////qDebug() << "### List ";
                  }  else if (base.isAnchor()) {
                      /* link normal */
                      QDomElement linkers = wdoc.createElement("fo:basic-link");
                        if ( !base.anchorHref().startsWith("#") ) {
                            linkers.setAttribute ("external-destination",base.anchorHref());
                           } else {
                           linkers.setAttribute ("internal-destination",base.anchorHref());
                         }
                         linkers.setAttribute ("color","red");
                         linkers.setAttribute ("text-decoration","underline");  /* text-decoration="underline" */
                         QDomText linktext = wdoc.createTextNode(fr.text());
                         linkers.appendChild(linktext);
                         paragraph.appendChild(linkers);
                  }  else if (CharFromPara !=base) {
                         /* found diffs from fragment to paragraph ... */
                         if (Actual_Text_Param == fr.text()) {
                         PaintCharFormat(paragraph,base);
                         paragraph.appendChild(wdoc.createTextNode(fr.text()));
                         } else {
                         QDomElement inlinestyle = wdoc.createElement("fo:inline");
                         PaintCharFormat(inlinestyle,base);
                         QDomText normaltext = wdoc.createTextNode(fr.text());
                         inlinestyle.appendChild(normaltext);
                         paragraph.appendChild(inlinestyle);
                         }
                  } else  {
                      /* charformat from block text is same as fragment */
                      QString txtfrag = fr.text();
                      if (txtfrag.size() > 0) {
                      ////////////qDebug() << "### not on fragment 3/4 -> " << txtfrag;
                      }
                      paragraph.appendChild(wdoc.createTextNode(txtfrag));
                  }
              } else {
                 /////////////qDebug() << "### unknow out QTextFragment "; 
              }
           

                       
       }

       /////////qDebug() << "### close  block fo:block ..................................";
       appender.appendChild(paragraph); 
                              
    ////////////appender.appendChild( param );   /* append result from this block */
    
}



/*  qt -> fop*/
void Fop_Handler::HandleTable( QTextTable  * childTable , QDomElement appender ) 
{
                  const int coolsums = childTable->columns();
                  QTextTableFormat tbforms = childTable->format();
                  const QString TablebackgroundColor = tbforms.background().color().name();
    
                  QDomElement toptable = wdoc.createElement("fo:table");
                  toptable.setAttribute ("table-layout","fixed");
                  
                  toptable.setAttribute ("background-color",TablebackgroundColor);
                  appender.appendChild(toptable);
    
                  
                  QVector<QTextLength> constraints = tbforms.columnWidthConstraints();
    
                  qreal pageMin = FopInt("17cm");
    
                  qreal onfixcolumsMinimum = pageMin / coolsums;
    
                  qreal tablewithd = 0;
                  for (int i = 0; i < coolsums; ++i) {
                       const QTextLength length = constraints.at(i);
                       qreal lenghs = length.rawValue();
                       QDomElement cools  = wdoc.createElement("fo:table-column");
                        if (length.type() == QTextLength::FixedLength) {
                         cools.setAttribute ("column-width",QString("%1pt").arg(lenghs));
                         tablewithd +=lenghs;
                        } else {
                         /* xsl-fo dont having % onfixcolumsMinimum */
                         cools.setAttribute ("column-width",QString("%1pt").arg(onfixcolumsMinimum));  
                         tablewithd +=onfixcolumsMinimum;
                        }
                        toptable.appendChild(cools);
                    }
                    
                    toptable.setAttribute ("width",QString("%1pt").arg(tablewithd));
                    
                    QDomElement tbody  = wdoc.createElement("fo:table-body");
                    tbody.setAttribute ("background-color",TablebackgroundColor);
                    tbody.setAttribute ("cell-space",QString("%1pt").arg(tbforms.cellSpacing()));  /* QString("%1pt").arg(tbforms.cellSpacing()) */
                    tbody.setAttribute ("padding-start",QString("%1pt").arg(tbforms.cellPadding()));
                    tbody.setAttribute ("border-width",QString("%1pt").arg(tbforms.border()));
                    toptable.appendChild(tbody);
                    
                    int rowline = childTable->rows();
                    
                    
                     for (int ttr = 0; ttr < rowline; ++ttr) {
                        bool rowline = true;
                        QDomElement rows  = wdoc.createElement("fo:table-row");
                            for (int ttd = 0; ttd < coolsums; ++ttd) {
                                QTextTableCell cell = childTable->cellAt(ttr, ttd);
                                    const int rspan = cell.rowSpan();
                                    const int cspan = cell.columnSpan();
                                
                                
                                
                                QDomElement celltds  = wdoc.createElement("fo:table-cell");
                                if (cell.format().background().color().name() !=TablebackgroundColor && !cell.format().background().color().name().contains("000000")) {
                                celltds.setAttribute ("background-color",cell.format().background().color().name());
                                }
                                
                                if (cspan > 1) {
                                ttd = ttd + cspan - 1;
                                celltds.setAttribute ("number-columns-spanned",cspan);
                                }
                                QTextFrame::iterator di;
                                for (di = cell.begin(); !(di.atEnd()); ++di) {
                                     QTextFrame *tdFrame = di.currentFrame();
                                     QTextBlock tdpara = di.currentBlock();
                                      if (tdFrame)  {
                                           HandleFrame(tdFrame,FRAME_TDCELL,celltds);
                                           //////////qDebug() << "### cell frame  1#";
                                      } else if (tdpara.isValid()) {
                                            HandleBlock(tdpara,celltds); 
                                            ///////////qDebug() << "### cell block 2 #";
                                      }
                                    
                                }
                                
                                if ( cspan > 1 || cspan == 1 ) {
                                rows.appendChild(celltds);
                                }
                                
                            }
                        
                        
                        
                        
                        tbody.appendChild(rows);
                    }
                    
}






Fop_Handler::Fop_Handler( const  QString  file , bool modus , PageDB *dbin , QWidget * parent )
{
	ErrorSumm = 0;
    db = dbin;
    BigParent = parent;
    canContinue = true;
    ErnoMap.clear();
    codecx = QTextCodec::codecForMib(106);   /* utf-8 codec to write */
    fopinfo = QFileInfo(file);
    Fopfile = fopinfo.absoluteFilePath();
    FopBaserRefDir = fopinfo.absolutePath();
    if (!FopBaserRefDir.endsWith("/")) {
        FopBaserRefDir.append("/");
    }
    ImageFopBaserRefDir = FopBaserRefDir;
    ImageFopBaserRefDir.append(FOPIMAGEDIR);
    /////////Cache( ImageFopBaserRefDir );
    
    QString errorStr;
    int errorLine, errorColumn;
    remotefilecounter = 0;
    
    Fopdevice = new QFile(Fopfile);
    if (modus) {
        /* open file */
                doc.clear();
            if(!Fopdevice->open( QIODevice::ReadOnly ) ) {
                ErrorSumm++;
                canContinue = false;
                ErnoMap.insert(ErrorSumm,tr("Fop_Handler::Fop_Handler / Unable to open  file %1").arg(fopinfo.fileName()));
                Fopdevice->close();
             }
            if (canContinue) {
                if (!doc.setContent(Fopdevice,false, &errorStr, &errorLine, &errorColumn)) {
                        ErrorSumm++;
                        canContinue = false;
                        ErnoMap.insert(ErrorSumm,tr("Fop_Handler::Fop_Handler / XML error on line %2 column %1 string %3.").arg(fopinfo.fileName()).arg(errorColumn).arg(errorLine).arg(errorStr));
                }
            }             
             
     } else {
         
             /* go to write on file !! */
            if (!Fopdevice->open( QFile::WriteOnly | QFile::Text ) )  {
                ErrorSumm++;
                canContinue = false;
                ErnoMap.insert(ErrorSumm,tr("Fop_Handler::Fop_Handler / Unable to write on file %1").arg(fopinfo.fileName()));
                Fopdevice->close();
            } else {
               Fopdevice->close(); 
            }
         
     }
    
     
     if (parent !=0) {
          //////qDebug() << "### objectName " << parent->objectName();
          if (canContinue && modus) {
          /* continue to open file to read + remote image work on signal  */ 
          QTimer::singleShot(110, this, SLOT(CheckRemoteImage()));
          } 
     }  else {
         if (canContinue && modus) {
             /* continue to open file to read */ 
             OpenModus();
         }
     }
}


/* ################################################ write fop file############################################################*/
/* ################################################ write fop file############################################################*/
/* ################################################ write fop file############################################################*/
/* ################################################ write fop file############################################################*/
/* ################################################ write fop file############################################################*/


/* Init to save new pageitem */
QDomDocument Fop_Handler::GetStructure( QRectF page , QRectF margin , QColor pagebg , const QString PaperName )
{
    wdoc.clear();
    CurrentPrinterNameFormat = PaperName;
    QDomProcessingInstruction header = wdoc.createProcessingInstruction( "xml",QString("version=\"1.0\" encoding=\"utf-8\"" ));
    wdoc.appendChild( header );
    
    
    //////////const QString masterefName = QString("Simple-%1-Page").arg(PaperName);
    const QString bodyrefName = "PageBody";
    
    QDateTime timer1( QDateTime::currentDateTime() );  /* time root */
    QDomElement basexslforoot = wdoc.createElement("fo:root");
                basexslforoot.setAttribute ("xmlns:fo","http://www.w3.org/1999/XSL/Format");
                basexslforoot.setAttribute ("xmlns:svg","http://www.w3.org/2000/svg");
                basexslforoot.setAttribute ("xmlns:cms","http://www.pulitzer.ch/2007/CMSFormat");
                basexslforoot.setAttribute ("xmlns:fox","http://xmlgraphics.apache.org/fop/extensions");
    
    QDomElement fopeditor = wdoc.createElement("cms:pager");
                fopeditor.setAttribute ("build",timer1.toString(Qt::LocalDate));
                fopeditor.setAttribute ("editor",_PROGRAM_NAME_LIB_ );
                fopeditor.setAttribute ("printer_format",PaperName);
                
                basexslforoot.appendChild( fopeditor );
                
    
    wdoc.appendChild( basexslforoot );
    QDomElement layout = wdoc.createElement("fo:layout-master-set");
    basexslforoot.appendChild( layout );
    QDomElement pagesetup = wdoc.createElement("fo:simple-page-master");
    
    
        const qreal TopMargin = margin.x();
        const qreal RightMargin = margin.y();
        const qreal BottomMargin = margin.width();
        const qreal LeftMargin = margin.height(); 
    
    
    
    
    pagesetup.setAttribute ("master-name",PaperName);
    pagesetup.setAttribute ("margin-top",QString("%1pt").arg(TopMargin));
    pagesetup.setAttribute ("margin-bottom",QString("%1pt").arg(BottomMargin));
    pagesetup.setAttribute ("margin-left",QString("%1pt").arg(LeftMargin));
    pagesetup.setAttribute ("margin-right",QString("%1pt").arg(RightMargin));
    pagesetup.setAttribute ("page-width",QString("%1pt").arg(page.width()));
    pagesetup.setAttribute ("page-height",QString("%1pt").arg(page.height()));
    
    layout.appendChild( pagesetup );
    
    /* margin QRectF margin(top,bottom,right,left); */
    
    
    /*  */
    
    QDomElement rb = wdoc.createElement("fo:region-body");
    rb.setAttribute ("margin-top",QString("%1pt").arg(margin.x()));
    rb.setAttribute ("margin-bottom",QString("%1pt").arg(margin.y()));
    rb.setAttribute ("margin-left",QString("%1pt").arg(margin.height()));
    rb.setAttribute ("margin-right",QString("%1pt").arg(margin.width()));
    rb.setAttribute ("background-color",pagebg.name());
    rb.setAttribute ("region-name","xsl-region-body");
    
    pagesetup.appendChild( rb );
    
    QDomElement pageseq1 = wdoc.createElement("fo:page-sequence");
    pageseq1.setAttribute ("master-reference",PaperName);
    basexslforoot.appendChild( pageseq1 );
    
    flowwrite = wdoc.createElement("fo:flow");
    flowwrite.setAttribute ("flow-name","xsl-region-body");
    pageseq1.appendChild( flowwrite );
    
    if (canContinue) {
    QByteArray data = wdoc.toByteArray(5); 
                            /* tmp write to find error ... */
                            if (data.size() > 0) {
                            if ( Fopdevice->open( QIODevice::WriteOnly ) ) {
                                Fopdevice->write(data);
                                Fopdevice->close();
                            } 
                            }
    }
    
    
    
    return wdoc;
}


void Fop_Handler::AppendLayer( QDomElement e , QTextDocument * d )
{
    Cache( ImageFopBaserRefDir );
    BigframeProcessing = 0;
    QTextFrame  *Tframe = d->rootFrame();
    QChar letter('A' + (qrand() % 26));
    QString Alternate = QString("layer%1-%2").arg(letter).arg(qrand() % 255);
    CurrentIdLayer = e.attribute ("id",Alternate);
    HandleFrame(Tframe,FRAME_ROOT,e);
    flowwrite.appendChild(e);
    //////////IncommingFromLayer = d->clone();
    
}


void Fop_Handler::HandleFrame( QTextFrame  * Tframe , FRAME_TYP dd , QDomElement appender ) 
{
    BigframeProcessing++;
    QTextFrame::iterator it;
        for (it = Tframe->begin(); !(it.atEnd()); ++it) {
         /* to find tree structure */
         QTextFrame *childFrame = it.currentFrame();
         QTextBlock para = it.currentBlock();
            
             if (childFrame)  {
                QTextTable *childTable = qobject_cast<QTextTable*>(childFrame);
                /* if is table */
                 if (childTable->columns() > 0) {
                     HandleTable(childTable,appender); 
                 }  
             }  
             /* param */
             if (para.isValid()) {
                HandleBlock(para,appender); 
             }
             
             
             
    }
    
    if (dd == FRAME_ROOT) {
        
    }
    
}


void Fop_Handler::Writtelnend() {
    QByteArray data = wdoc.toByteArray(5); 
     if (data.size() > 0) {
    
                            if ( Fopdevice->open( QIODevice::WriteOnly ) ) {
                                Fopdevice->write(data);
                                Fopdevice->close();
                            } 
     }
    
}


/* ################################################ write fop file############################################################*/
/* ################################################ write fop file############################################################*/
/* ################################################ write fop file############################################################*/
/* ################################################ write fop file############################################################*/
/* ################################################ write fop file############################################################*/







/* fop -> qt */

void  Fop_Handler::ParserTable( const QDomElement e , QTextDocument * d , Fop_Layer * layer , int CursorPositon ) 
{
    
    if (FoTag(e) != TABLE_TAG) {
    return;
    }
    
    Fop_Block *foppi = new Fop_Block(imagecounter,e,"fo:table");
    
    qreal tableborderborder = TakeOneBorder(e);
    qreal tablewidth = Unit(e.attribute ("width",QString("0")));
    QDomElement co = e.parentNode().toElement();
    
    QTextCursor Tcursor = QTextCursor(d);
    bool IsCorrectCursor = Tcursor.movePosition(QTextCursor::End);
    
     
    int mappos = Tcursor.position();  /* last position from last cursor on class ! */
    
    
  QTextCursor cellCursor;  /* new cursor only for cell tabbing */
  QString tatype = e.attribute( "table-layout" );  /* on attribute root table */
  QStringList coolsizes;  /* grep  column size in pt cm mm ecc... */
  coolsizes.clear();
  QDomElement bodytable = e.firstChildElement("fo:table-body");
  int rowCounter = 0;
  int columnCounter = 0;
    QDomElement column = e.firstChildElement("fo:table-column"); 
    while (!column.isNull()) {
           const QString sizefromcool = column.attribute( "column-width" );
           if (sizefromcool.size() > 0) {
           coolsizes.append(sizefromcool);  /* append unit widht  string 22pt */
           }
           column = column.nextSiblingElement("fo:table-column"); 
       }
       
    if (coolsizes.size() > 0) {
       columnCounter =  coolsizes.size();
    }
    
    QDomElement rows = bodytable.firstChildElement("fo:table-row");
    while (!rows.isNull()) {
        rowCounter++;
        rows = rows.nextSiblingElement("fo:table-row");
    }
    if (rowCounter < 1) {
    //////qDebug() << "### false row? " << columnCounter << " list" << coolsizes;
    return;
    }
    ////////qDebug() << "### true row  " << columnCounter << " list" << coolsizes;
    
    /* #########################base table #######################################*/
    QTextTable *qtable = Tcursor.insertTable( rowCounter, columnCounter );
    
    
    
    
    
    ///////qDebug() << "### parse table... ";
    ////////qDebug() << "### rowCounter " << rowCounter;
    ////////qDebug() << "### columnCounter " << columnCounter;
    
    QTextTableFormat tableFormat;
    QString tbg = bodytable.attribute("background-color");
    
    if (tablewidth !=0) {
    tableFormat.setWidth ( QTextLength ( QTextLength::FixedLength, tablewidth )  );
    ///////tableFormat.setHeight ( QTextLength ( QTextLength::FixedLength, tablewidth ) );        
    }
    qreal borderDik = TakeOneBorder(bodytable);
    ////////qDebug() << "### table borderDik " << borderDik;   /* qt4 4.3 bug unable to set table border on QGraphicsTextItem  */
    if (!tbg.isEmpty()) {
    tableFormat.setBackground ( QColor(tbg) ); 
    }
    
    tableFormat.setBorder(borderDik);
    tableFormat.setCellSpacing(Unit(bodytable.attribute ("cell-space",QString("0"))));
    tableFormat.setCellPadding(TakeOnePadding(bodytable));
    
    
    /* colums  on mm cm pt <->  */
    if (coolsizes.size() > 0) {
        tableFormat.clearColumnWidthConstraints();
        QVector<QTextLength> constraints;
        for (int i = 0; i < coolsizes.size(); ++i) {
            const qreal cellmesure = Unit(coolsizes.at(i));
            constraints.insert(i,QTextLength(QTextLength::FixedLength, cellmesure ));
        }
        tableFormat.setColumnWidthConstraints(constraints);
    }
    
    if (tatype == "fixed") {
        tableFormat.setAlignment ( Qt::AlignLeft );
    } else {
        tableFormat.setAlignment ( Qt::AlignJustify );
    }
    
    int qlistlargeNr = -1;   /* cell and row count from 0 */
    QDomElement setrows = bodytable.firstChildElement("fo:table-row");
    while (!setrows.isNull()) {
        int is_spancol = 0;
        int startStorno = 0;
        int stopStorno = 0;
        bool bypassisactive = false;
        qlistlargeNr++;
        QTextBlockFormat tdformat;
        tdformat.setBottomMargin(0);
        tdformat.setTopMargin(0); 
        QDomElement columnElement = setrows.firstChildElement();   /* sub element from row */
        int columnCounter = -1;   /* cell and row count from 0 */
        
        while ( !columnElement.isNull() ) {
           if ( columnElement.tagName().toLower() == "fo:table-cell" ) {
           columnCounter++;
               
                  QTextTableCell cell;
               
                  
                  is_spancol = columnElement.attribute( "number-columns-spanned" ).trimmed().toInt();
                  if (is_spancol > 1) {
                      for (int i = 0; i < is_spancol; ++i) {
                          QTextTableCell cellstart = qtable->cellAt( qlistlargeNr , columnCounter + i);
                          QTextCharFormat firster = cellstart.format(); 
                          if (!columnElement.attribute("background-color").isEmpty()) {
                          firster.setBackground(QColor(columnElement.attribute("background-color")));
                          }
                          cellstart.setFormat(firster);
                      }
                      /* point to last cell number-columns-spanned  to fill text */
                      qtable->mergeCells ( qlistlargeNr ,columnCounter,1,is_spancol);
                      cell = qtable->cellAt( qlistlargeNr , columnCounter );
                       
                  } else {
                      cell = qtable->cellAt( qlistlargeNr , columnCounter );
                  }
                  
               Tcursor = cell.firstCursorPosition();
               const qreal cellpadding = Unit(columnElement.attribute ("padding",QString("0")));
               const qreal cellwidht = Get_Cell_Width(tableFormat,columnCounter);
                  
                  
               /* paint cell Background and table border here */
              QTextCharFormat existformat = cell.format();
              if (!columnElement.attribute("background-color").isEmpty()) {
              existformat.setBackground(QColor(columnElement.attribute("background-color")));
              }
              existformat.setToolTip ( foppi->Get_XML() );
              cell.setFormat(existformat);
                  
                  Tcursor = cell.firstCursorPosition();
                  int FirstcellInitCursor = Tcursor.position();
                  
                                int blocksDD = 0;
                                QDomElement cellinside = columnElement.firstChildElement("fo:block");
                                while ( !cellinside.isNull() ) {
                                    blocksDD++;
                                    if (blocksDD == 1) {
                                    ParserParagraph(cellinside,d,layer,FirstcellInitCursor,true); 
                                    } else {
                                    ParserParagraph(cellinside,d,layer,CursorStatementPosition,false);    
                                    }
                                    
                                cellinside = cellinside.nextSiblingElement("fo:block");
                                }
                  
                                              
            }
           columnElement = columnElement.nextSiblingElement(); 
        }
            
        
        
        setrows = setrows.nextSiblingElement("fo:table-row");
    }
    
    
    
    qtable->setFormat( tableFormat );
    ActualBackgroundColor = QString(default_background);   /* reset color background */
    CursorStatementPosition = 0; /* go to end document ! */
    
}









void  Fop_Handler::ParserLayerFloating(  Fop_Layer * layer ,  const QDomElement e ) 
{
    const QString t = e.tagName().toLower();
    if (FoTag(e) !=BLOCK_CONTAINER) {
    return;
    }
    ParaGraphCounter = 0;
    CursorStatementPosition = 0;
    
    qreal wqwi = Unit(e.attribute ("width",QString("500mm")));
    int LockStatus = e.attribute ("lock","0").toInt();
    int RotateVar = e.attribute ("rotate","0").toInt();
    QTextDocument *d = layer->Qdoc();
    if (LockStatus !=0) {
    layer->SetLock();
    }
    if (RotateVar > 0) {
    layer->SetRotate( RotateVar );
    }
    QTextCursor Tcursor = QTextCursor(d);    /* reset cursor to the new layer */
    QTextFrame  *Tframe = d->rootFrame();
    QTextFrameFormat rootformats = Tframe->frameFormat();
    rootformats.setBottomMargin (  qMax ( Unit(e.attribute ("margin-after",QString("0"))),  Unit(e.attribute ("margin-bottom",QString("0")) )   )); 
    rootformats.setTopMargin(  qMax (Unit(e.attribute ("margin-before",QString("0"))), Unit(e.attribute ("margin-top",QString("0")) ) )); 
    rootformats.setRightMargin ( qMax ( Unit(e.attribute ("margin-end",QString("0"))), Unit(e.attribute ("margin-right",QString("0")) ) )); 
    rootformats.setLeftMargin ( qMax (Unit(e.attribute ("margin-start",QString("0"))),Unit(e.attribute ("margin-left",QString("0")) ) )); 
    rootformats.setPadding ( TakeOnePadding(e)); 
    rootformats.setWidth ( wqwi  ); 
    Tframe->setFrameFormat ( rootformats );
    d->setTextWidth ( wqwi - 2 );
    /* set the base document */
    const QString idname = e.attribute ("id",layer->IdName());
    qreal Zvalue = e.attribute ("z-index",QString("0")).toDouble();
    if (Zvalue !=0) {
    layer->SetZindex(Zvalue);
    }
    
    if (!layerNames.contains(idname)) {
    layer->SetRealName( idname );
    } else {
    layer->SetRealName( layer->IdName() );
    }
    
    qreal BorderDicks = TakeOneBorder(e);
    QString Setcolor;
    int opacity = e.attribute("opacity",QString("1")).toInt();  /* opacity transparent from background color */
    
    QString usercolorxml = e.attribute("background-color","errorcolor");
    
    if (opacity < 22) {
    opacity = 1;
    }
    
    if (opacity > 255) {
    opacity = 1;
    }
    
    
    if (usercolorxml !="errorcolor") {
    Setcolor = usercolorxml;
    } else {
    Setcolor = default_background;
    opacity = 1;
    }
    
    if (opacity == 0) {
    opacity = 1;
    }
    
    
    layer->SetRects(Unit(e.attribute ("left",QString("10mm"))), 
                    Unit(e.attribute ("top",QString("10mm"))), 
                    Unit(e.attribute ("width",QString("500mm"))), 
                    Unit(e.attribute ("height",QString("500mm"))), 
                    BorderDicks, 
                    QString(e.attribute("border-start-color",QString(default_background))), 
                    Setcolor, 
                    opacity);
    
    /* grep frame format */
    
                   targetAnchor = QTextCharFormat();  /* new layer target link */
                   targetAnchor.setAnchor(true); 
                   targetAnchor.setAnchorNames ( QStringList ( idname )  ); 
    
    
    QTextBlockFormat blockFormat = GetBlockFormat(e);
      /* summ attribute from fo:block */
    QTextCharFormat defaultformat = GetCharFormat(e);
        QDomNode child = e.firstChild();
    while ( !child.isNull() ) {
        if ( child.isElement() ) {
             const QDomElement childElement = child.toElement();
             const QTextCharFormat inliner = GetCharFormat(childElement);
             /////////////////////qDebug() << "### ParserLayerFloating  loop (" << childElement.tagName().toLower() << ") ############";
            
             if ( FoTag(childElement) == INLINE_STYLE  ) {
                ///////////Tcursor.insertText(childElement.text(), inliner  );
             } else if ( FoTag(childElement) == LINK_DOC ) {
                /////ParserLink(childElement,d);
                 ////////////Tcursor.insertText( " " +childElement.text().trimmed() + " ", inliner  );
             }  else if ( FoTag(childElement) == IMAGE_SRC ) {
                     ImageParserTag(childElement,d,layer);   /* fo:external-graphic */
             } else if ( FoTag(childElement) == BLOCK_CONTAINER ) {
                     /* relax NG http://relaxng.org/  not allowed here! */
             } else if ( FoTag(childElement) == BLOCK_TAG ) {
                     ////////qDebug() << "### ParserLayerFloating go para 1 (" << FoTag(childElement) << ")";
                     ParserParagraph(childElement,d,layer);
             } else if ( FoTag(childElement) == IMAGE_INLINE ) {
                     ObjectParserTag(childElement,d,layer);  /* inline svg images */
             } else if ( FoTag(childElement) == TABLE_TAG  ) {
                    ParserTable(childElement,d,layer);
             }

        } else if (child.isText()) {
            //////////const QDomText childText = child.toText();
            /////////QString stremtext = childText.data();
            //////////Tcursor.setBlockFormat( blockFormat );
            /////////qDebug() << "### fo:block  text (" << stremtext << ")";
            //////////Tcursor.insertText( stremtext);
            ////////Tcursor.insertBlock();
        }
        child = child.nextSibling();
    }
    


    /////////qDebug() << "### errorsize " << ErnoMap.size();
    if (ErnoMap.size() > 0) {
        //////ReportError();
    }
    
    
    
}









QTextBlockFormat Fop_Handler::GetBlockFormat( const QDomElement e ) 
{
    /* pf = parent format */
    QTextBlockFormat pf;
    QDomNamedNodeMap attlist = e.attributes();
    
            for (int i=0; i<attlist.count(); i++){
                QDomNode nod = attlist.item(i);
                 if (nod.nodeName().toLower() == "break-before") {
                 pf.setPageBreakPolicy ( QTextFormat::PageBreak_AlwaysBefore );
                 }
                 if (nod.nodeName().toLower() == "break-after") {
                 pf.setPageBreakPolicy ( QTextFormat::PageBreak_AlwaysAfter );
                 }
                 
                 
                 if (nod.nodeName().toLower() == "text-align") {
                      if (nod.nodeValue().toLower() == "center" ) {
                      pf.setAlignment( Qt::AlignCenter );
                      } else if (nod.nodeValue().toLower() == "left" || nod.nodeValue().toLower() == "start" || nod.nodeValue().toLower() == "inside" ) {
                      pf.setAlignment( Qt::AlignLeft );
                      } else if (nod.nodeValue().toLower() == "right" || nod.nodeValue().toLower() == "end"  ) {
                      pf.setAlignment( Qt::AlignRight );
                      } else if (nod.nodeValue().toLower() == "justify") {
                      pf.setAlignment( Qt::AlignJustify );
                      }  else if (nod.nodeValue().toLower() == "inherit") {
                         /* align same as parent before */
                          pf.setAlignment( Qt::AlignAbsolute );
                      }
                 }
                 
            }
            
    return pf;
}



QTextBlockFormat Fop_Handler::ParamFormat( const QDomElement e , QTextBlockFormat f ) 
{
    /* pf = parent format */
    QTextBlockFormat pf = GetCharFormat(e).toBlockFormat();
    pf.merge(f);
    
    QDomNamedNodeMap attlist = e.attributes();
    
            for (int i=0; i<attlist.count(); i++){
                QDomNode nod = attlist.item(i);
                if (nod.nodeName().toLower() == "space-after.optimum") {
                 pf.setBottomMargin(Unit(nod.nodeValue()));
                }
                
                 if (nod.nodeName().toLower() == "break-before") {
                 pf.setPageBreakPolicy ( QTextFormat::PageBreak_AlwaysBefore );
                 }
                 
                 if (nod.nodeName().toLower() == "break-after") {
                 pf.setPageBreakPolicy ( QTextFormat::PageBreak_AlwaysAfter );
                 }
                 
                 if (nod.nodeName().toLower() == "line-height") {
                 
                 }
                 if (nod.nodeName().toLower() == "background-color") {
                 pf.setBackground ( QBrush(QColor(nod.nodeValue()) ) ); 
                 }
                 
                 if ( nod.nodeName().toLower().contains("space-before") ) {
                    pf.setTopMargin(Unit(nod.nodeValue()));
                 }
                 
                 if ( nod.nodeName().toLower().contains("space-after") ) {
                    pf.setBottomMargin(Unit(nod.nodeValue()));
                 }
                 
                 if ( nod.nodeName().toLower().contains("space-start") ) {  /* right */
                    pf.setRightMargin(Unit(nod.nodeValue()));
                 }
                 
                 if ( nod.nodeName().toLower().contains("space-end") ) {   /* left */
                    pf.setLeftMargin(Unit(nod.nodeValue()));
                 }
                 if ( nod.nodeName().toLower().contains("start-indent") ) {   /* left */
                    pf.setIndent(Unit(nod.nodeValue()) / 7);
                 }
                 
                 
                 
                 
                 
                 if (nod.nodeName().toLower() == "text-align") {
                      if (nod.nodeValue().toLower() == "center" ) {
                      pf.setAlignment( Qt::AlignCenter );
                      } else if (nod.nodeValue().toLower() == "left" || nod.nodeValue().toLower() == "start" || nod.nodeValue().toLower() == "inside" ) {
                      pf.setAlignment( Qt::AlignLeft );
                      } else if (nod.nodeValue().toLower() == "right" || nod.nodeValue().toLower() == "end"  ) {
                      pf.setAlignment( Qt::AlignRight );
                      } else if (nod.nodeValue().toLower() == "justify") {
                      pf.setAlignment( Qt::AlignJustify );
                      }  else if (nod.nodeValue().toLower() == "inherit") {
                         /* align same as parent before */
                          pf.setAlignment( Qt::AlignAbsolute );
                      }
                 }
                 
            }
            
            
    return pf;
}










QTextCharFormat Fop_Handler::GetCharFormat( const QDomElement e , QTextCharFormat f ) 
{
      QDomNamedNodeMap attlist = e.attributes();
      QStringList option;
                  option.clear();
    
      int fontstrech = e.attribute ("font-stretch",QString("0")).toInt();
     
    
      QString idref="id-NOTSET";  /* id identificativ if having? */
    
        for (int i=0; i<attlist.count(); i++){
             QDomNode nod = attlist.item(i);
               if (_PARSE_DEBUG_FOP_ == 1) {
               option.append(nod.nodeName().toLower()+"="+nod.nodeValue());
               }
            
               
            
            if (nod.nodeName().toLower() == "background-color") {
                f.setBackground (QBrush(QColor(nod.nodeValue())) ); 
            }
               
             if (nod.nodeName().toLower() == "color") {
                 f.setForeground(QBrush(QColor(nod.nodeValue())));
             }
             if (nod.nodeName().toLower() == "id") {
                 idref = nod.nodeValue();
             }
             if (nod.nodeName().toLower() == "font-family") {
                 QFont userfont( nod.nodeValue() ); 
                 if (userfont.exactMatch()) {
                          if (fontstrech !=0) {
                          userfont.setStretch ( fontstrech ); 
                          }
      
                     f.setFont(userfont);
                 } else {
                    notexistFontMap.insert(nod.nodeValue(),QApplication::font());    /* prepare replace font item ? */
                    f.setFont(QApplication::font());
                 }
             } 
             if (nod.nodeName().toLower() == "font-size") {
                f.setFontPointSize(Unit(nod.nodeValue()));
             }
             if (nod.nodeName().toLower() == "external-destination" || nod.nodeName().toLower() == "internal-destination") {
                 f.setAnchorHref(nod.nodeValue());
                 f.setAnchor(true);
                 f.setForeground ( QBrush ( QColor ("#ff0000") ) );  /* red link */
             } else {
                 ///////////f.setAnchor(false);
             }
             if (nod.nodeName().toLower() == "font-style" or nod.nodeName().toLower() == "font-weight") {
                  if (nod.nodeValue().toLower() == "italic") {
                  f.setFontItalic(true);
                  } else if (nod.nodeValue().toLower() == "oblique") {
                  f.setFontItalic(true); 
                  } else if (nod.nodeValue().toLower() == "bold") {
                  f.setFontWeight(QFont::Bold); 
                  }
             }
             
             if (nod.nodeName().toLower() == "text-decoration" ) {
                  if (nod.nodeValue().toLower() == "underline") {
                  f.setFontUnderline(true);
                  f.setUnderlineStyle ( QTextCharFormat::SingleUnderline );
                  } else if (nod.nodeValue().toLower() == "overline") {
                  f.setFontOverline(true);
                  } else if (nod.nodeValue().toLower() == "blink") {
                  f.setFontUnderline(true);
                  f.setUnderlineStyle ( QTextCharFormat::DotLine );
                  }
             } else {
               ///////f.setFontUnderline(false);
             }
             
             /* text-decoration */
             
             
        }
             if (_PARSE_DEBUG_FOP_ == 1) {
             idref.append("\n");
             idref.append(option.join("\n"));
             f.setToolTip(idref);
             } else {
             f.setToolTip(idref);
             }
             
        
        if (f.isValid()) {
           return f;  
        } else {
              QTextCharFormat erno;
              erno.setToolTip("Error format result!");
              ErrorSumm++;
              ErnoMap.insert(ErrorSumm,tr("Fop_Handler::GetCharFormat / Format Error on file %1 tagname %2.").arg(fopinfo.fileName()).arg(e.tagName().toLower()));
             return erno;
        }
}












/* QTimer::singleShot(10000, this, SLOT(ReloadFileAttach())); */


/* ------------------------------ remote action  ---------------------------------------------------------*/


QVariant Fop_Handler::ResourceBlockRemoteImage( const QString urlimage  ) 
{
      QVariant nullimage;
      QMapIterator<QString,QVariant> i(RemoteImageMap);
         while (i.hasNext()) {
             i.next();
             if ( i.key() == urlimage ) {
              return i.value();
             }
         } 
    return nullimage;
}


void Fop_Handler::SaveSreamList( int nr , QByteArray data , QString url ) 
{
    RemoteImageMap.insert(url,data);
    ///////qDebug() << "### SaveSreamList  position  " << nr;
    ///////////qDebug() << "### SaveSreamList  chain  " << RemoteImageMap.size();
    QApplication::restoreOverrideCursor();
    if (dlg->isVisible ()) {
    dlg->close();
    dlg->deleteLater();
    }
    
    int nextItemtoGet = nr + 1;
    
    if (WaitList.size() !=nextItemtoGet) {
        GetFileatPos(nextItemtoGet);
        return;
    }
    
    if (WaitList.size() == RemoteImageMap.size()) {
       WaitList.clear();
       OpenModus(); 
    }
}

void Fop_Handler::UpdateStatus( int sums  ) 
{
    if (sums < 1) {
    return;
    } 
    dlg->setValue(sums);
}


void Fop_Handler::GetFileatPos( int on ) 
{
        dlg = new QProgressDialog(BigParent,Qt::Popup);
        LoadPage *imageremote = new LoadPage(WaitList.at(on),on);
        imageremote->Start();
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        dlg->setLabelText (tr("Get File \"%1\"").arg( WaitList.at(on) ));
        dlg->setCancelButton(0);
        dlg->setMinimumDuration(700);
        ///////dlg->setMaximum(100);
        connect(imageremote, SIGNAL(LastDone(int,QByteArray,QString)),this, SLOT(SaveSreamList(int,QByteArray,QString)));
        connect(imageremote, SIGNAL(SendLog(int)),this, SLOT(UpdateStatus(int)));
}

void Fop_Handler::RegisterRemoteFile( const QString urlimage ) 
{
        remotefilecounter++;
        WaitList.append(urlimage);
}



void  Fop_Handler::CheckRemoteImage()
{
     root = doc.documentElement();
     if (root.tagName() !="fo:root") {
      ErrorSumm++;
      ErnoMap.insert(ErrorSumm,tr("Fop_Handler::OpenModus / Unable to find a fo:root tag on root by file %1").arg(fopinfo.fileName()));
      return; 
     }
     
     
     
     /* reset network activity */
     RemoteImageMap.clear();
     WaitList.clear();
     remotefilecounter = 0;
     
     
     /////////RegisterRemoteFile( FIXEDIMAGE_CIZ );
     
     layout_master = root.firstChildElement("fo:layout-master-set");
     master = root.firstChildElement("fo:page-sequence");
     page = master.firstChildElement("fo:flow");
     LoopOnTagRemoteFile(page);
     
     if (WaitList.size() < 1) {
        OpenModus(); 
     } else {
        GetFileatPos(0);
     }
}

/* Finder from remote image on tag  IMAGE_SRC*/
void Fop_Handler::LoopOnTagRemoteFile( const QDomElement e ) 
{
       /* grep block container */
        QDomNode child = e.firstChild();
        while ( !child.isNull() ) {
                if ( child.isElement() ) {
                    const QDomElement el = child.toElement();
                      if (FoTag(el) == IMAGE_SRC) {
                          const QString url = ImagesrcUrl(el);
                          if (IsNetFile(url)) {
                            RegisterRemoteFile(url);  
                          }
                      } else if (FoTag(el) == BLOCK_CONTAINER) {
                          LoopOnTagRemoteFile(el);
                      } else if (FoTag(el) == BLOCK_TAG) {
                          LoopOnTagRemoteFile(el);
                      }  else if (FoTag(el) == IMAGE_INLINE) {
                          /* not follow svg dom tag !! */
                      } else {
                          LoopOnTagRemoteFile(el);
                      }
                    /////qDebug() << "### LoopOnTagRemoteFile " << el.tagName();
                    
                }    
            child = child.nextSibling();        
        }
}


/* Final render from the fop page ...........  last emit */
void  Fop_Handler::OpenModus() {
    
    
     if (!db) {
          ErrorSumm++;
          ErnoMap.insert(ErrorSumm,tr("Fop_Handler::OpenModus/ Unable to find Printer data-base list on  file %1 try to handle.").arg(fopinfo.fileName()));
     return;
     }
    
     LayerSum = 0;
     
     /////////////remotefilecounter = 0;
     imagecounter = 0;
     layerlist.clear();
     layerNames.clear();
     
     root = doc.documentElement();
     if (root.tagName() !="fo:root") {
      ErrorSumm++;
      ErnoMap.insert(ErrorSumm,tr("Fop_Handler::OpenModus / Unable to find a fo:root tag on root by file %1").arg(fopinfo.fileName()));
      return; 
     }
     
     layout_master = root.firstChildElement("fo:layout-master-set");
     
     ////////qreal pagerwi = Unit(layout.attribute ("page-width",0));
     
                   QDomElement layout = layout_master.firstChildElement("fo:simple-page-master"); 
                               while (!layout.isNull()) {
                                   
                                   if (layout.tagName() == "fo:simple-page-master" )  {
                                   
                                 qreal xBottomMargin =  Unit(layout.attribute ("margin-bottom",QString("1cm")) ); 
                                 qreal xTopMargin =  Unit(layout.attribute ("margin-top",QString("1cm")) ) ; 
                                 qreal xRightMargin = Unit(layout.attribute ("margin-right",QString("1cm")) ) ; 
                                 qreal xLeftMargin = Unit(layout.attribute ("margin-left",QString("1cm")) ) ; 
                                 /* register name and dimension page and margin ! */
                                 CurrentPrinterNameFormat = layout.attribute ("master-name","unknow_master_name");
                                   PageDimensionName *dim = new PageDimensionName(CurrentPrinterNameFormat,
                                           Unit(layout.attribute ("page-width",QString("21cm"))) , 
                                           Unit(layout.attribute ("page-height",QString("29.8cm"))), 
                                           QRectF (xTopMargin,xRightMargin,xBottomMargin,xLeftMargin),
                                           QColor(layout.attribute ("background-color",default_background)),0); 
                                   
                                   db->AppInsert(dim);   /* formato pagines */
                                   
                                   }
                                   layout = layout.nextSiblingElement("fo:simple-page-master");
                                }
     if (db->connect()) {
     Pmargin = db->last()->margin();   /* to first default page */
     db->reload();
     } else {
        ErrorSumm++;
        ErnoMap.insert(ErrorSumm,tr("Fop_Handler::OpenModus/ Unable to send new data on data-base list dimension on  file %1 try to handle.").arg(fopinfo.fileName()));
     }
     
     master = root.firstChildElement("fo:page-sequence");
     page = master.firstChildElement("fo:flow");
     
     ActualBackgroundColor = QString(default_background);
     
     
     QDomElement  testhavingfloatingelement = page.firstChildElement("fo:block-container");
     
     if (testhavingfloatingelement.isNull() && db->connect() ) {
         
         
         /*
         QRectF (xTopMargin,xRightMargin,xBottomMargin,xLeftMargin), 
         QRectF ( qreal x, qreal y, qreal width, qreal height )  
         */
         
         QDomElement box = doc.createElement("fo:block-container");
         box.setAttribute("width",QString("%1pt").arg(db->last()->internalwi()));
         box.setAttribute("height",QString("%1pt").arg(db->last()->internalhi()));
         box.setAttribute("left",QString("%1pt").arg(db->last()->margin().y()));
         box.setAttribute("top",QString("%1pt").arg(db->last()->margin().x()));
         
            QDomNode chi = page.firstChild();
                   while ( !chi.isNull() ) {
                       if ( chi.isElement() ) {
                        box.appendChild(doc.importNode(chi,true).toElement());
                       } else if (chi.isText()) {
                         box.appendChild(doc.createTextNode(chi.toText().data()));
                       }
                     chi = chi.nextSibling();            
                   }
                   
                        Fop_Layer *ihacke = new Fop_Layer(fopinfo,0);
                        layerlist.prepend(ihacke);
                        ParserLayerFloating(ihacke,box); 
                   
                        
                    /*
                           QFile f( "testout.html" );
                            if ( f.open( QFile::WriteOnly | QFile::Text ) )
                            {
                            QTextStream sw( &f );
                            sw.setCodec(QTextCodec::codecForMib(106));
                            sw << ihacke->Qdoc()->toHtml("utf-8");
                            f.close();
                            }
                            
                            
                   qDebug() << "### not conform block container " << wihack;
                   
                   */
                   
                   
          ErrorSumm++;
          ErnoMap.insert(ErrorSumm,tr("Fop_Handler::OpenModus/ Unable to find a fo:block-container element on file %1 try to handle.").arg(fopinfo.fileName()));
   
                   
                   
                   emit ConnectList(true);
                   return;
       }
     
     
        /* grep block container */
        QDomNode child = page.firstChild();
        while ( !child.isNull() ) {
                if ( child.isElement() ) {
                    const QDomElement el = child.toElement(); 
                    if (FoTag(el) == BLOCK_CONTAINER) {
                        LayerSum++;
                        Fop_Layer *item = new Fop_Layer(fopinfo,LayerSum);
                        layerlist.prepend(item);
                        ParserLayerFloating(item,el); 
                    }
                }    
            child = child.nextSibling();        
        }
     
    ////////////qDebug() << "### OpenModus end / remotefilecounter ->" << remotefilecounter;
    /////////qDebug() << "### OpenModus end / ErrorSize ->" << ErrorSize();
        
    emit ConnectList(true);
}





QPixmap Fop_Handler::RenderSvg(  const QDomElement e  , Fop_Layer * layer , const QString nameresource )
{
    ///////nrloop++;   /* count element to addresource ! */
    /* QString trytosave = QString("svg_%1.svg").arg(nrloop); */
    QDomDocument em;
            QDomProcessingInstruction header = em.createProcessingInstruction( "xml", "version=\"1.0\" standalone=\"no\"" );
            em.appendChild( header );
            em.appendChild( e.firstChildElement() );
    QDomElement root = em.documentElement();
    root.setAttribute ("xmlns","http://www.w3.org/2000/svg");
    root.setAttribute ("version","1.2");
    root.setAttribute ("baseProfile","tiny");
    QString srcsv = em.toString();
    
    if (srcsv.contains("svg:svg")) {
    srcsv = srcsv.replace("svg:","");
    }
    
    QByteArray    streamsvg;
    streamsvg.append ( srcsv ); 
    layer->AppendImage(nameresource,streamsvg);
    QPixmap pix = RenderPixmapFromSvgByte(  streamsvg );   /* on main.h  static inline */
    if (pix.isNull() ) {
    QPixmap pixe(22,22);
    pixe.fill(QColor("crimson"));   /* a error red pixel */ 
          ErrorSumm++;
          ErnoMap.insert(ErrorSumm,tr("Fop_Handler::RenderSvg / Unable to render SVG resource \"%1\" on file %2").arg(nameresource).arg(fopinfo.fileName()));
    return pixe;
    } else {
     return pix;
    }
}







void Fop_Handler::ImageParserTag(  const QDomElement e ,  QTextDocument * d  , Fop_Layer * layer  , int CursorPositon  )
{
    ////////////qDebug() << "### ImageParserTag " << FoTag(e);
    QDir::setCurrent(FopBaserRefDir);   /* cd to dir from file fop */
    imagecounter++;
    Fop_Block *foppi = new Fop_Block(imagecounter,e,"fo:external-graphic");
    /* <fo:external-graphic src="url(boxes.svg)"/>  */
    /* read dimension setting from tag */
    QTextCursor Tcursor = QTextCursor(d);
    if (CursorPositon !=0) {
    Tcursor.setPosition(CursorPositon);
    } else {
    Tcursor.movePosition(QTextCursor::End);
    }
    
    qreal wi = qMax ( Unit( e.attribute( "width" , "0" )) , Unit( e.attribute( "content-width" , "0" )));;
    qreal hi =  qMax ( Unit( e.attribute( "height" , "0")) , Unit( e.attribute( "content-height" , "0" )));
    QString scaling = e.attribute("scaling","0");
    qreal havingnummer = qMax(wi,hi);
    /* dont scale if not having set nothing width / height */
    if (havingnummer < 1) {
    scaling = "0"; 
    }
    QString extension;
    QByteArray              derangedata;
    QPixmap                 resultimage;
    QPixmap                 scaledsimage;
    QString                 resourceName;
    
    const QString hrefimageplace = ImagesrcUrl(e);   /* grab url from image on main.h */
    ////////////qDebug() << "### hrefimageplace " << hrefimageplace;
    ////////qDebug() << "### ImageParserTag " << wi << "X" << hi;
    if (IsNetFile(hrefimageplace)) {
        /* remote file is placed on qmap container is downloaded before init QTextDocument */
        
        QUrl neturi(hrefimageplace); 
        QString findfile = neturi.path();
        findfile = findfile.right(findfile.lastIndexOf("/")).toLower();
        if (findfile.contains(".")) {
          extension = findfile.right(findfile.lastIndexOf("."));  
        } else {
          extension = findfile;
        }
     /* remote file on container qmap  typedef  QMap<QString,QVariant> TypImageContainer;  url data */
     derangedata = ResourceBlockRemoteImage( hrefimageplace  ).toByteArray();
        
    }  else {
     QFileInfo fixurl(hrefimageplace);
     extension =  fixurl.completeSuffix().toLower();

                    /* decompressed item xx */
                    if (extension.endsWith(".gz"))  {
                    derangedata = OpenGzipOneFileByte( fixurl.absoluteFilePath() );
                    } else {
                     /* normal image file */
                         QFile f(fixurl.absoluteFilePath());
                         if (f.open(QIODevice::ReadOnly)) {
                            derangedata = f.readAll();
                            f.close();
                         }
                    }
                    
    }
    /* all data avaiable */
        
     /////////qDebug() << "### extension " << extension;
    
                    /* begin render image */
                    if (extension.contains("sv"))  {
                       resultimage = RenderPixmapFromSvgByte(  derangedata );
                       resourceName = QString("/svg/Z_SVG_external-graphic_%1").arg(imagecounter);
                    } else {
                        resultimage.loadFromData( derangedata );
                        resourceName = QString("/png/Z_PNG_external-bytearray-graphic_%1").arg(imagecounter);
                    }

    
     if (resultimage.isNull()) {
         /* not valid image to register ! */
         ErrorSumm++;
         ErnoMap.insert(ErrorSumm,tr("Fop_Handler::ImageParserTag / Unable to render image \"%1\" from file %2").arg(hrefimageplace).arg(fopinfo.fileName()));  
         QDir::setCurrent(startdir);  /* reset to start dir xx */
         return;
         
    }  else {
               
          if (wi !=0 && wi !=resultimage.width() && scaling=="0") {
             scaling = "uniform";
          }  
          if (hi !=0 && hi !=resultimage.height() && scaling=="0") {
             scaling = "uniform";
          }           
                        
    /* scaling image size ?? */
    if (scaling == "uniform") {
         if (wi !=0)  {
          scaledsimage = resultimage.scaledToWidth(wi);
         } else if (hi !=0) {
         scaledsimage = resultimage.scaledToHeight(hi); 
         }
    } else if (scaling == "non-uniform" && wi!=0 && hi!=0) {
        scaledsimage = resultimage.scaled(wi,hi,Qt::IgnoreAspectRatio);
    } else {
       scaledsimage = resultimage;
        
    }
    /* scaling image size ?? */
    
    QUrl recresourcein(resourceName);
                        /* fill image on QTextDocument */
                        d->addResource( QTextDocument::ImageResource,recresourcein, resultimage );
    
                        layer->AppendImage(resourceName,derangedata);
    
    
                        QTextImageFormat format;
                        format.setName( resourceName );
                        if (hi !=0 && scaling!="0") {
                        format.setHeight ( hi );
                        } else {
                        format.setHeight ( scaledsimage.height() );
                        }
                        if (wi !=0 && scaling!="0") {
                        format.setWidth ( wi );
                        } else {
                        format.setWidth ( scaledsimage.width() );
                        }
                        format.setToolTip ( foppi->Get_XML() ); 
                        Tcursor.insertImage( format );  /* cursor insert image QPixmap */
                        
                        
    QDir::setCurrent(startdir);
    }
}


/* Handle inline svg format image ......... */
bool Fop_Handler::ObjectParserTag(  const QDomElement e ,  QTextDocument * d  , Fop_Layer * layer , int CursorPositon )
{
   
    
    QTextCursor Tcursor = QTextCursor(d);
    if (CursorPositon !=0) {
    Tcursor.setPosition(CursorPositon);
    } else {
    Tcursor.movePosition(QTextCursor::End);
    }
    
    qreal wi = qMax ( Unit( e.attribute( "width" , "0" )) , Unit( e.attribute( "content-width" , "0" )));;
    qreal hi =  qMax ( Unit( e.attribute( "height" , "0")) , Unit( e.attribute( "content-height" , "0" )));
    
    Fop_Block *foppi = new Fop_Block(imagecounter,e,"fo:instream-foreign-object");

    
    
    imagecounter++;
    QDomElement domObject = e.firstChildElement();
    if ( domObject.tagName().toLower() == "svg:svg" || domObject.tagName().toLower() == "svg" ) {
        QDir::setCurrent(FopBaserRefDir);
        const QString resourceName = QString("/svg/Z_SVG_instream-foreign-object_%1").arg(imagecounter);
        QPixmap paintsvg = RenderSvg(e,layer,resourceName);
        QUrl recresourcein(resourceName);
        d->addResource( QTextDocument::ImageResource,recresourcein, paintsvg );
        QTextImageFormat format;
        format.setName( resourceName );
        if (wi !=0) {
        format.setWidth ( wi );
        } else {
        format.setWidth ( paintsvg.width() );
        }
        if (hi !=0) {
        format.setHeight ( hi ); 
        } else {
        format.setHeight ( paintsvg.height() );
        }
        
        format.setToolTip ( foppi->Get_XML() ); 
        Tcursor.insertImage( format );  /* cursor insert image QPixmap */
        QDir::setCurrent(startdir);
    return true;        
    }
    
    return false;
}






/* remove objekt */
Fop_Handler::~Fop_Handler() 
{
    Fopdevice->close();
    deleteLater();
}





