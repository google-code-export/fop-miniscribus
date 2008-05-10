#include "qdocxhtml.h"
#include "metric_convert.h"


QdocXhtml::QdocXhtml() 
{ 
	cleaner = new QTidy();
	cleaner->Init(QDir::homePath()+"/.qtity/");
}
QdocXhtml::~QdocXhtml() { }
QdocXhtml::QdocXhtml( QTextDocument * docin , LEVEL e )
{
	SetDoc(docin,e);
}
void QdocXhtml::SetDoc( QTextDocument * docin , LEVEL e )
{
  BigframeProcessing = 0;
	doc = docin->clone();
	styler = e;
	QTextFrame  *Tframe = doc->rootFrame();
	dom.clear();
	QDomElement body = dom.createElement("body");
	HandleFrame(Tframe,body);
	dom.appendChild(body);
	QString flat = dom.toString(0);
	flat = flat.replace("\n","");
	cleaner = new QTidy();
	cleaner->Init(QDir::homePath()+"/.qtity/");
  stream = cleaner->TidyCleanhtml(flat);
	
	
}



QString QdocXhtml::Format( QString html )
{
	QmyDom xoc;
	QDomElement body = xoc.createElement("body");
	xoc.insertFragment(body,html);
	xoc.appendChild(body);
	return cleaner->TidyCleanhtml(xoc.toString(2));
}


void QdocXhtml::SetHtml( QString html , LEVEL e )
{
	QString myxml = cleaner->TidyCleanhtml(html);
	QTextDocument *docin = new QTextDocument();
	docin->setHtml(myxml);
	SetDoc(docin,e);
}

void QdocXhtml::HandleFrame( QTextFrame  * Tframe , QDomElement appender ) 
{
    BigframeProcessing++;
	  QTextTable *rootTable;
	  ///////////////////qDebug() << "### HandleFrame ";
	
	
	         if (rootTable = qobject_cast<QTextTable*>(Tframe)) {
						  ///////////////// qDebug() << "### root table ................  ";
						   HandleTable(rootTable,appender);
						   return;
					  }
	   
	  
    QTextFrame::iterator it;
        for (it = Tframe->begin(); !(it.atEnd()); ++it) {
         /* to find tree structure */
         QTextFrame *childFrame = it.currentFrame();
         QTextBlock para = it.currentBlock();
            
             if (childFrame)  {
                QTextTable *childTable = qobject_cast<QTextTable*>(childFrame);
                /* if is table */
                 if (childTable->columns() > 0) {
									 ////////////qDebug() << "### normal table ................  ";
									 HandleTable(childTable,appender); 
                 }  
             }  
             /* param */
             if (para.isValid()) {
							  ///////////qDebug() << "### normal para ................  ";
                HandleBlock(para,appender); 
             }
    }
}


/*  qt -> fop*/
void QdocXhtml::HandleTable( QTextTable  * childTable , QDomElement appender ) 
{
                  const int coolsums = childTable->columns();
                  QTextTableFormat tbforms = childTable->format();
	                QVector<QTextLength> constraints = tbforms.columnWidthConstraints();
	                QString TablebackgroundColor;
	                if (tbforms.background().color().name() !="#000000" ) { 
										TablebackgroundColor = tbforms.background().color().name();
									} else {
										TablebackgroundColor = "#ffffff";
									}
                  QDomElement toptable = dom.createElement("table");
                  toptable.setAttribute ("bgcolor",TablebackgroundColor);
									toptable.setAttribute ("cellspacing",QString("%1").arg(tbforms.cellSpacing()));  /* QString("%1pt").arg(tbforms.cellSpacing()) */
									toptable.setAttribute ("cellpadding",QString("%1").arg(tbforms.cellPadding()));
									toptable.setAttribute ("border",QString("%1").arg(tbforms.border()));
									if (tbforms.alignment() == Qt::AlignRight) {
										toptable.setAttribute ("align","right");
									} else if (tbforms.alignment() == Qt::AlignHCenter) {
										toptable.setAttribute ("align","center");
									}
                  appender.appendChild(toptable);
                  /////qreal onfixcolumsMinimum = pageMin / coolsums;
                  bool setpoint = false;
                  qreal tablewithd = 0;
                  for (int i = 0; i < coolsums; ++i) {
                       const QTextLength length = constraints.at(i);
										   setpoint = false;
										   if (length.type() == QTextLength::FixedLength) {
												 setpoint = true;
												 tablewithd +=length.rawValue();
											 } else if (length.type() == QTextLength::PercentageLength) {
												 tablewithd +=length.rawValue();
											 }
											 ////////ToUnit(
                    }
										if (setpoint && tablewithd > 10) {
											toptable.setAttribute ("width",QString("%1px").arg(Pointo(tablewithd,"mm")));
										} else if (!setpoint && tablewithd > 10) {
											toptable.setAttribute ("width",QString("%1\%").arg(tablewithd));
										}

                    int rowline = childTable->rows();
										
                     for (int ttr = 0; ttr < rowline; ++ttr) {
                        bool rowline = true;
                        QDomElement rows  = dom.createElement("tr");
                            for (int ttd = 0; ttd < coolsums; ++ttd) {
                                QTextTableCell cell = childTable->cellAt(ttr, ttd);
																const int rspan = cell.rowSpan();
																const int cspan = cell.columnSpan();
                                QDomElement celltds = dom.createElement("td");
															
															
															  const QTextLength leni = constraints.at(ttd);
															
															
															  if (leni.type() == QTextLength::FixedLength) {
																celltds.setAttribute ("width",QString("%1mm").arg(Pointo(leni.rawValue(),"mm")));
																} else if (leni.type() == QTextLength::PercentageLength) {
																celltds.setAttribute("width",QString("%1\%").arg(leni.rawValue()));
																}
															
															
                                if (cell.format().background().color().name() !=TablebackgroundColor && !cell.format().background().color().name().contains("000000")) {
                                celltds.setAttribute ("bgcolor",cell.format().background().color().name());
                                }  ///////bgcolor="#55AAFF" 
                                
                                if (cspan > 1) {
                                ttd = ttd + cspan - 1;
                                celltds.setAttribute ("colspan",cspan);
                                }
																
																
                                QTextFrame::iterator di;
                                for (di = cell.begin(); !(di.atEnd()); ++di) {
                                     QTextFrame *tdFrame = di.currentFrame();
                                     QTextBlock tdpara = di.currentBlock();
                                      if (tdFrame)  {
                                           HandleFrame(tdFrame,celltds);
                                           //////qDebug() << "### cell frame  1#";
                                      } else if (tdpara.isValid()) {
                                            HandleBlock(tdpara,celltds); 
                                            //////qDebug() << "### cell block 2 #";
                                      }
                                    
                                }
                                
                                if ( cspan > 1 || cspan == 1 ) {
                                rows.appendChild(celltds);
                                }
                                
                            }
                        
                        
                        
                        
                        toptable.appendChild(rows);
                    }
                    
}

/* paragraph  qt -> fop */
void QdocXhtml::HandleBlock( QTextBlock  para  , QDomElement appender ) 
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
		if (ParaBlockFormat.hasProperty(QTextFormat::BlockNonBreakableLines)) {
			paragraph = dom.createElement("pre");
		} else {
			paragraph = dom.createElement("p");
			if (para.begin().atEnd()) {
			paragraph.setAttribute ("class","Empty");
			}
		}
    ParaFormat(paragraph,ParaBlockFormat);  /* block */
		bool breakline = false;
    PaintLastBlockformat(paragraph,CharFromPara);
    ///////////////////qDebug() << "### open block para br1->" << breakline << "  txt->" << para.text();
		QTextBlock::iterator de;

     
       for (de = para.begin(); !(de.atEnd()); ++de) {
                       
                      //////QTextDocumentFragment 
           
          QTextFragment fr = de.fragment();
              if (fr.isValid()) {
								  QString ptxt = fr.text();
								  
								  int breacks = 0;
								  const int Marks = fr.position();
								  ///////qDebug() << "### Marks " << Marks;
                  positioner++;
                  bool tisalviono;
                  const QTextCharFormat base = fr.charFormat();
                  int staycursorpos =  fr.position();
                  Pics = base.toImageFormat();
                  Tabl = base.toTableFormat();
                  Uls =  base.toListFormat();
                  /* ############################### Image blocks ###########################################*/
                  /* ############################### Image blocks ###########################################*/
                  if (Pics.isValid()) {
                  const QString hrefadress = Pics.name();
									QString titles;
										QVariant xx = Pics.property(100); 
									
										
                  QDomElement imagen = dom.createElement("img");
									            if (Pics.width() > 0) {
                              imagen.setAttribute ("width",QString("%1").arg(Pics.width()));
                              }
															if (Pics.height() > 0) {
                              imagen.setAttribute ("height",QString("%1").arg(Pics.height()));
                              }
															
																if (!xx.isNull()) {
											           SPics pic = xx.value<SPics>();
											          //////// qDebug() << "### QdocXhtml to web Pics.property found handler ..... " << pic.info;
											           titles = pic.info;
																 imagen.setAttribute ("src",pic.indoc().toString());
																 ////////imagen.setAttribute ("onclick","LiveImage('"+pic.name+"')");
																 imagen.setAttribute ("id",pic.name);
																 imagen.setAttribute ("rel","P_img_inline");
																	
																	
										            } else {
																	imagen.setAttribute ("src",hrefadress);
																}
															
															
															
															
															
														 if (titles.size() > 0) {
														  imagen.setAttribute ("alt",titles);
															imagen.setAttribute ("longdesc",titles);
														 }
														 
									           paragraph.appendChild(imagen);
                 
									
                  /* ############################### Image blocks ###########################################*/
                  /* ############################### Image blocks ###########################################*/
                  } else if (Tabl.isValid()) {
                      //////qDebug() << "### Table ";
                      //////////QTextTable *childTable = qobject_cast<QTextTable*>(fr);
                      ///////////HandleTable( QTextTable  * childTable ,paragraph);
                      /////////HandleTable(childTable,paragraph);
                  }  else if (Uls.isValid()) {
                      ////////qDebug() << "### List ";
                  }  else if (base.isAnchor() && base.anchorHref() !="" ) {
                      /* link normal */
                      QDomElement linkers = dom.createElement("a");
                         if (base.anchorHref().trimmed().startsWith("#") ) {
                            linkers.setAttribute ("href",base.anchorHref());
                           } else if (base.anchorHref().trimmed().startsWith("http")) {
														QUrl gotouri(base.anchorHref());
                            linkers.setAttribute ("href",base.anchorHref());
														linkers.setAttribute ("target","_blank");
														linkers.setAttribute ("class","Link_External");
														linkers.setAttribute ("title",gotouri.host());
													 } else if (base.anchorHref().trimmed().startsWith("mailto:")) {
                            linkers.setAttribute ("href",base.anchorHref());
														linkers.setAttribute ("class","Link_Mail");
														linkers.setAttribute ("title",base.anchorHref());
													 } else {
                           linkers.setAttribute ("href",base.anchorHref());
													 linkers.setAttribute ("class","Link_Internal");
                         }
												 QDomText linktext = dom.createTextNode(fr.text());
												 const QString virtualtext = fr.text();
												 if (virtualtext.startsWith("http")) {
													  QUrl xuri(fr.text());
													  linktext = dom.createTextNode(xuri.host());
												 }
                         linkers.appendChild(linktext);
                         paragraph.appendChild(linkers);
                  }  else if (CharFromPara !=base) {
                         /* found diffs from fragment to paragraph ... */
                         if (Actual_Text_Param == fr.text()) {
                         PaintCharFormat(paragraph,base);
                         paragraph.appendChild(dom.createTextNode(fr.text()));
                         } else {
													 /* bold underline */
												 QDomElement inlinestyle = dom.createElement("span");
                         PaintCharFormat(inlinestyle,base);
                         inlinestyle.appendChild(dom.createTextNode(fr.text()));
                         paragraph.appendChild(inlinestyle); 
                         }
                  } else  {
                      /* charformat from block text is same as fragment */
										
										
                      QString txtfrag = fr.text();
										  QString txt = Qt::escape(txtfrag);
										  QString forcedLineBreakRegExp = QString::fromLatin1("[\\na]");
								      forcedLineBreakRegExp[3] = QChar::LineSeparator;
								      const QStringList lines = txt.split(QRegExp(forcedLineBreakRegExp));
								      breacks = lines.size();
										
										  ////////////////////qDebug() << "### lines.size() " << lines.size(); 
										
										
                      if (lines.size() > 0) {
												
												for (int i = 0; i < lines.count(); ++i)  {
													 if (i > 0) {
														 QDomElement breakline = dom.createElement("br");
										         paragraph.appendChild(breakline);
													 }
														const QString piece = lines.at(i);
														paragraph.appendChild(dom.createTextNode(piece));
													 
												}
                      } else {
												paragraph.appendChild(dom.createTextNode(txtfrag));
											}
                      
                  }
              } else {
                 qDebug() << "### unknow out QTextFragment ";
              }
           

                       
       }

			 if (ParaBlockFormat.hasProperty(QTextFormat::BlockTrailingHorizontalRulerWidth)) {
			  QDomElement Horizontal = dom.createElement("hr");
			  paragraph.appendChild(Horizontal);
		   }
       /////////qDebug() << "### close  block fo:block ..................................";
       appender.appendChild(paragraph); 
                              
    ////////////appender.appendChild( param );   /* append result from this block */
    
}


void QdocXhtml::PaintLastBlockformat( QDomElement e , QTextCharFormat bf  )
{
	
			QString styles="";
			if (e.hasAttribute("style")) {
			styles = FilterAttribute(e,"style");
			}
			QFont userfont = bf.font();
      if (bf.foreground().color().name() !="#000000" ) { 
      styles.append(QString("color:%1;").arg(bf.foreground().color().name()));
      }
			
			if (styler == STYLEMEDIUM) {
			int sizefo = qRound(userfont.pixelSize());
			styles.append(QString("font-size:%1px;").arg(sizefo));
			}
			
			if (styler == STYLEMEDIUM) {
			styles.append(QString("font-family:%1;").arg(userfont.family()));
			}
			
			if (bf.fontWeight() > 51) {
				styles.append(QString("font-weight:bold;"));
			}
			if (bf.fontItalic()) {
				styles.append(QString("font-style:italic;"));
			}
			
			/////////  style=" font-weight:600; font-style:italic; text-decoration: underline overline line-through;">kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk</span>
			
			QStringList textdeco;
			            textdeco.clear();
			if (bf.fontUnderline()) {
				textdeco.append(QString("underline"));
			}
			if (bf.fontStrikeOut()) {
				textdeco.append(QString("line-through"));
			}
			if (bf.fontOverline()) {
				textdeco.append(QString("overline"));
			}
			
			if (textdeco.size() > 0) {
				styles.append(QString("text-decoration:%1;").arg(textdeco.join(" ")));
			}

			
			if (styles.size() > 0) {
				e.setAttribute ("style",styles); 
			}
			
			
}



void QdocXhtml::PaintCharFormat( QDomElement e , QTextCharFormat bf  )
{
			QString styles="";
			if (e.hasAttribute("style")) {
			styles = FilterAttribute(e,"style");
			}
			QFont userfont = bf.font();
      if (bf.foreground().color().name() !="#000000" ) { 
      styles.append(QString("color:%1;").arg(bf.foreground().color().name()));
      }
			
			int sizefo = qRound(userfont.pixelSize());
			styles.append(QString("font-size:%1px;").arg(sizefo));
			
			if (styler == STYLEFULL) {
			styles.append(QString("font-family:%1;").arg(userfont.family()));
			}
			
			if (bf.fontWeight() > 51) {
				styles.append(QString("font-weight:bold;"));
			}
			if (bf.fontItalic()) {
				styles.append(QString("font-style:italic;"));
			}
			
			
			
			QStringList textdeco;
			            textdeco.clear();
			if (bf.fontUnderline()) {
				textdeco.append(QString("underline"));
			}
			if (bf.fontStrikeOut()) {
				textdeco.append(QString("line-through"));
			}
			if (bf.fontOverline()) {
				textdeco.append(QString("overline"));
			}
			
			if (textdeco.size() > 0) {
				styles.append(QString("text-decoration:%1;").arg(textdeco.join(" ")));
			}
	
			
			
			if (styles.size() > 0) {
				e.setAttribute ("style",styles); 
			}
}


void QdocXhtml::ParaFormat( QDomElement e , QTextBlockFormat bf )
{
	         						 
	    QString styles="";
			if (e.hasAttribute("style")) {
			styles = FilterAttribute(e,"style");
			}
      if ( bf.alignment() == Qt::AlignRight) {
      e.setAttribute ("align","right");    
      }
      if ( bf.alignment() == Qt::AlignHCenter) {
      e.setAttribute ("align","center");    
      }
      if ( bf.alignment() == Qt::AlignJustify) {
      e.setAttribute ("align","justify");  
      }
      if ( bf.alignment() == Qt::AlignVCenter) {
      e.setAttribute ("align","center");    
      }
      if ( bf.alignment() == Qt::AlignCenter) {
      e.setAttribute ("align","center");   
      }
			
      if (bf.topMargin() !=0) {
			styles.append(QString("padding-top:%1px;").arg(Pointo(bf.topMargin(),"mm")));
      }
      if (bf.bottomMargin() !=0) {
			styles.append(QString("padding-bottom:%1px;").arg(Pointo(bf.bottomMargin(),"mm")));
      }
       if (bf.rightMargin() !=0) {
			styles.append(QString("padding-right:%1px;").arg(Pointo(bf.rightMargin(),"mm")));
      }
      if (bf.leftMargin() !=0) {
      styles.append(QString("padding-left:%1px;").arg(Pointo(bf.leftMargin(),"mm")));				
      }
			
			if (bf.foreground().color().name() !="#000000" ) { 
				styles.append(QString("color:%1;").arg(bf.foreground().color().name()));
			}
			
			if (styler !=STYLESIMPLE) {
				if (bf.background().color().name() !="#000000" ) {
				styles.append(QString("background-color:%1;").arg(bf.background().color().name()));
				}
		  }
			if (styler == STYLESIMPLE) {
				if (bf.background().color().name() !="#000000" ) {   /* error color */
					e.setAttribute ("class","Marker"); 
				}
		  }
			
			if (styles.size() > 0) {
				e.setAttribute ("style",styles); 
			}

}


/* loop  to find attribute name xx */
QString QdocXhtml::FilterAttribute( QDomElement element , QString attribute ) 
{
    QString base = "";
    QDomNamedNodeMap attlist = element.attributes();
    int bigint = attlist.count();
    if ( bigint > 0 ) {
        for (int i=0; i<bigint; i++){
             QDomNode nod = attlist.item(i);
             if (nod.nodeName() == attribute) {
                base = QString(nod.nodeValue());
                return base;
             }
        } 
    }
    return base; 
}





