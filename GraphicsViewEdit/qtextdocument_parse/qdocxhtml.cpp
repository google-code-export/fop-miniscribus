#include "qdocxhtml.h"
#include "cmspage_structure.h"



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
	
	////////fwriteutf8("tmppaint.html",docin->toHtml("utf-8"));
	
  BigframeProcessing = 0;
	doc = docin->clone();
	styler = e;
	QTextFrame  *Tframe = doc->rootFrame();
	dom.clear();
	QDomElement body = dom.createElement("body");
	FrameLoop(doc->rootFrame()->begin(),body);
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
/* http://www.bessrc.aps.anl.gov/software/qt4-x11-4.2.2-browser/d0/dbc/qtextdocument_8cpp-source.html */
void QdocXhtml::FrameLoop( QTextFrame::Iterator frameIt , QDomElement appender )
{
	BigframeProcessing++;
	
	  if (!frameIt.atEnd()) {
         QTextFrame::Iterator next = frameIt;
         ++next;
         if (next.atEnd() && frameIt.currentFrame() == 0 && 
					   frameIt.parentFrame() != doc->rootFrame() && 
				     frameIt.currentBlock().begin().atEnd()) {
             return;
				 }
     }
		 
		 for (QTextFrame::Iterator it = frameIt;  !it.atEnd(); ++it) {
			 if (QTextFrame *f = it.currentFrame()) {
			     if (QTextTable *table = qobject_cast<QTextTable *>(f)) {
						   /* table here */
               HandleTable(table,appender); 
            } else {
							  /* other simple table from QTextDocument like <div> htmls */
							  QTextFrameFormat format = f->frameFormat();
							  QDomElement inlinediv = dom.createElement("var");
							  PaintFrameFormat(inlinediv,format);
							  appender.appendChild(inlinediv);
							  FrameLoop(f->begin(),inlinediv);
						}
			 } else if (it.currentBlock().isValid()) {
					HandleBlock(it.currentBlock(),appender); 
			 }
		 }
}


void QdocXhtml::PaintFrameFormat(QDomElement e , QTextFrameFormat bf  )
{
		  QString styles="";
	
			if ( bf.position() == QTextFrameFormat::InFlow) {
			styles.append(QString("display:inline;float:none;"));	
			} else if (bf.position() == QTextFrameFormat::FloatLeft) {
			styles.append(QString("display:block;float:left;"));	
			} else if (bf.position() == QTextFrameFormat::FloatRight) {
			styles.append(QString("display:block;float:right;"));	
			}
	
	
	    if (bf.hasProperty(QTextFormat::FrameMargin)) {
				
					if (bf.topMargin() !=0) {
					styles.append(QString("margin-top:%1mm;").arg(Pointo(bf.topMargin(),"mm")));
					}
					if (bf.bottomMargin() !=0) {
					styles.append(QString("margin-top:%1mm;").arg(Pointo(bf.bottomMargin(),"mm")));
					}
					if (bf.rightMargin() !=0) {
					styles.append(QString("margin-top:%1mm;").arg(Pointo(bf.rightMargin(),"mm")));
					}
					if (bf.leftMargin() !=0) {
					styles.append(QString("margin-top:%1mm;").arg(Pointo(bf.leftMargin(),"mm")));				
					}
				
			}
			if (bf.hasProperty(QTextFormat::FrameBorder))  {
				styles.append(BorderStyleCss(bf.borderStyle()));
			}
			if (bf.hasProperty(QTextFormat::FrameBorder)) {
			styles.append(QString("border:%1pt;").arg(bf.border()));	
			}
			
			QBrush boof = bf.borderBrush();
			if (boof != Qt::NoBrush)  {
				styles.append(QString("border-color:%1;").arg(boof.color().name()));
			}
			
			QBrush bg = bf.background();
			if (bg != Qt::NoBrush)  {
				styles.append(QString("background-color:%1;").arg(bg.color().name()));	
			}
			
			if ( bf.padding() > 0) {
				styles.append(QString("padding:%1;").arg(bf.padding()));
			}
			
		  if (TranslateTextLengh(bf.width()).size() > 0) {
				styles.append(QString("width:%1;").arg(TranslateTextLengh(bf.width())));
			}
			if (TranslateTextLengh(bf.height()).size() > 0) {
				styles.append(QString("height :%1;").arg(TranslateTextLengh(bf.height())));
			}
	    if (styles.size() > 0) {
				e.setAttribute ("style",styles); 
			}
}

QString QdocXhtml::TranslateTextLengh( QTextLength unit ) 
{
	
	  qDebug() << "### TranslateTextLengh  " << unit.rawValue();
	
	  if (unit.rawValue() < 1) {
			return QString();
		}
	  if (unit.type() == QTextLength::FixedLength) {
			return QString("%1mm").arg(Pointo(unit.rawValue(),"mm"));
		} else {
			return QString("%1\%").arg(unit.rawValue());
		}
}



/*  qt -> fop*/
void QdocXhtml::HandleTable( QTextTable  * childTable , QDomElement appender ) 
{
	if (!childTable) {
	return;
	}
	QList<QByteArray> propi = childTable->dynamicPropertyNames();
	
	qDebug() << "### table init build ..propi  " << propi.size();
	const int coolsums = childTable->columns();
	qDebug() << "### coolsums  " << coolsums;
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
											toptable.setAttribute ("width",QString("%1mm").arg(Pointo(tablewithd,"mm")));
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
															
															
															 
															if ( TranslateTextLengh(constraints.at(ttd)).size() > 0) {
																celltds.setAttribute("width",QString("%1").arg(TranslateTextLengh(constraints.at(ttd))));
															}
															
															
															if (cell.format().background().color().name() !=TablebackgroundColor && !cell.format().background().color().name().contains("000000")) {
															celltds.setAttribute ("bgcolor",cell.format().background().color().name());
															}  ///////bgcolor="#55AAFF" 
                                
                                if (cspan > 1) {
                                ttd = ttd + cspan - 1;
                                celltds.setAttribute ("colspan",cspan);
                                }
																FrameLoop(cell.begin(),celltds);
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
		
		qDebug() << "### block init build ..  ";
		
    const QString Actual_Text_Param = para.text();
    QTextBlockFormat ParaBlockFormat = para.blockFormat();
    const QTextCharFormat CharFromPara = para.charFormat();  
    QDomElement paragraph;
    QTextImageFormat Pics;
    QTextTableFormat Tabl;
    QTextListFormat Uls;
		QTextFrameFormat Frameinline;
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
								  Frameinline = base.toFrameFormat();
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
                      ////////QTextTable *childTable = qobject_cast<QTextTable*>(fr);
										  ////////if (childTable) {
                      /////HandleTable(childTable,paragraph);
											/////////}
                  }  else if (Uls.isValid()) {
                      ////////qDebug() << "### List ";
                  }  else if (Frameinline.isValid()) {
										 //// QTextFrame *Iframe = qobject_cast<QTextFrame*>(fr);
										  ///if (Iframe) {
												///FrameLoop(Iframe->begin(),paragraph);
											////}
                      ////////qDebug() << "### List  QTextFrame::begin() const ";
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
				if (sizefo > 10) {
			  styles.append(QString("font-size:%1px;").arg(sizefo));
				}
				
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
			if (sizefo > 10) {
			styles.append(QString("font-size:%1px;").arg(sizefo));
			}
			
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
			styles.append(QString("padding-top:%1mm;").arg(Pointo(bf.topMargin(),"mm")));
      }
      if (bf.bottomMargin() !=0) {
			styles.append(QString("padding-bottom:%1mm;").arg(Pointo(bf.bottomMargin(),"mm")));
      }
       if (bf.rightMargin() !=0) {
			styles.append(QString("padding-right:%1mm;").arg(Pointo(bf.rightMargin(),"mm")));
      }
      if (bf.leftMargin() !=0) {
      styles.append(QString("padding-left:%1mm;").arg(Pointo(bf.leftMargin(),"mm")));				
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


QString QdocXhtml::BorderStyleCss(QTextFrameFormat::BorderStyle style)
{
    Q_ASSERT(style <= QTextFrameFormat::BorderStyle_Outset);
    QString html ="";
    html += QLatin1String(" border-style:");

    switch (style) {
    case QTextFrameFormat::BorderStyle_None:
        html += QLatin1String("none");
        break;
    case QTextFrameFormat::BorderStyle_Dotted:
        html += QLatin1String("dotted");
        break;
    case QTextFrameFormat::BorderStyle_Dashed:
        html += QLatin1String("dashed");
        break;
    case QTextFrameFormat::BorderStyle_Solid:
        html += QLatin1String("solid");
        break;
    case QTextFrameFormat::BorderStyle_Double:
        html += QLatin1String("double");
        break;
    case QTextFrameFormat::BorderStyle_DotDash:
        html += QLatin1String("dot-dash");
        break;
    case QTextFrameFormat::BorderStyle_DotDotDash:
        html += QLatin1String("dot-dot-dash");
        break;
    case QTextFrameFormat::BorderStyle_Groove:
        html += QLatin1String("groove");
        break;
    case QTextFrameFormat::BorderStyle_Ridge:
        html += QLatin1String("ridge");
        break;
    case QTextFrameFormat::BorderStyle_Inset:
        html += QLatin1String("inset");
        break;
    case QTextFrameFormat::BorderStyle_Outset:
        html += QLatin1String("outset");
        break;
    default:
        Q_ASSERT(false);
        break;
    };

    html += QLatin1Char(';');
		return html;
}






Html_Transformer::Html_Transformer()
{
}

Html_Transformer::Html_Transformer( QMap<int,RichDoc> divs )
{
    work(divs);
}

void Html_Transformer::work( QMap<int,RichDoc> divs )
{
	  xml = "<body xmlns=\"http://www.w3.org/1999/xhtml\">\n";
	  passage = 0;
		QMapIterator<int,RichDoc> i(divs);
         while (i.hasNext()) {
             i.next();
             RichDoc e = i.value();
					   const QString stream = WebSource(e);
             xml.append(stream);
					   passage++;
					   qDebug() << "### passage   " << passage;
					   qDebug() << "### stream   " << stream;
         }
				 
				 
    xml.append("</body>\n");
}


QString Html_Transformer::WebSource( RichDoc edoc )
{
			QString base = edoc.Hxtml();
			QdocXhtml *handler = new QdocXhtml(edoc.todoc());
			base = handler->Docxml();
			base.prepend(QString("<div class=\"qtpage\" xmlns=\"http://www.w3.org/1999/xhtml\" style=\"%1\">\n").arg(edoc.style));
			base.append(QString("\n</div>\n"));
			return base;
}


bool Html_Transformer::Conform()
{
	  if (passage == 0) {
			return false;
		}
	  QString errorStr;
    int errorLine, errorColumn;
    QDomDocument doc;
	  return doc.setContent(xml,false, &errorStr, &errorLine, &errorColumn);
}

QDomDocument Html_Transformer::xml_document()
{
	  QString errorStr;
    int errorLine, errorColumn;
    QDomDocument doc;
	  doc.setContent(xml,false, &errorStr, &errorLine, &errorColumn);
	  return doc;
}

void Html_Transformer::Syntaxcheck()
{
	if (xml.size() > 0 ) {
		QString errorStr;
    int errorLine, errorColumn;
    QDomDocument doc;
					if (!doc.setContent(xml,false, &errorStr, &errorLine, &errorColumn)) {
							//////return doc.toString(5);
						 QMessageBox::information(0, tr("Found xml error"),tr("Check line %1 column %2 on string \"%3\"!\nLayer total %4")
						                     .arg(errorLine - 1)
						                     .arg(errorColumn - 1)
						                     .arg(errorStr)
						                     .arg(passage));
						
						  if (errorLine >= 0 ) {
								
							}
						
					} else {
						 /////////////QMessageBox::information(0, tr("XML valid."),tr("All tag are valid size %1.").arg(xml.size()));
					}
	} else {
		QMessageBox::information(0, tr("XML not found!"),tr("Null size xml document!"));
	}
	
}

QString Html_Transformer::source()
{
	 if (Conform()) {
		 return xml_document().toString(5);
	 } else {
		 Syntaxcheck();
		 return xml;
	 }
}






