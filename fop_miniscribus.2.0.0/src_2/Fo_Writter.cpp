#include "Fo_Writter.h"
#include "SessionManager.h"



#include "Config.h"


/* http://www.bessrc.aps.anl.gov/software/qt4-x11-4.2.2-browser/d0/dbc/qtextdocument_8cpp-source.html */
using namespace ApacheFop;


FopDom::~FopDom() { }

FopDom::FopDom( QTextDocument * docin , M_PageSize page , LEVEL e )
 : sumblox(0)
{
	 m_page = page;
	 PageName = Imagename(m_page.name);
   SetDoc(docin,page,e);
	 
}

void FopDom::SetDoc( QTextDocument * docin , M_PageSize page , LEVEL e /*FOP_APACHE*/  )
{
	
	dom.clear();  
	const QString ReferenzeNames = Imagename(page.name);
	Q_ASSERT(ReferenzeNames.size() > 0);
	
	QDomProcessingInstruction header = dom.createProcessingInstruction( "xml",QString("version=\"1.0\" encoding=\"utf-8\"" ));
	dom.appendChild( header );
	
	QDateTime timer1( QDateTime::currentDateTime() );  /* time root */
	QDomElement basexslforoot = dom.createElement("fo:root");
                basexslforoot.setAttribute ("xmlns:fo","http://www.w3.org/1999/XSL/Format");
                basexslforoot.setAttribute ("xmlns:svg","http://www.w3.org/2000/svg");
                basexslforoot.setAttribute ("xmlns:cms","http://www.pulitzer.ch/2007/CMSFormat");
                basexslforoot.setAttribute ("xmlns:fox","http://xmlgraphics.apache.org/fop/extensions");
  dom.appendChild( basexslforoot );
	QDomElement layout = dom.createElement("fo:layout-master-set");
	basexslforoot.appendChild( layout );
	QDomElement pagesetup = dom.createElement("fo:simple-page-master");
	m_page.ReportPage(pagesetup );
	pagesetup.setAttribute ("master-name",ReferenzeNames);
	layout.appendChild( pagesetup );
	QDomElement rb = dom.createElement("fo:region-body");
	m_page.ReportPage(rb);
	rb.setAttribute ("region-name","xsl-region-body");
	pagesetup.appendChild( rb );
	QDomElement pageseq1 = dom.createElement("fo:page-sequence");
	pageseq1.setAttribute ("master-reference",ReferenzeNames);
	basexslforoot.appendChild( pageseq1 );
	doc = docin->clone();
	styler = e;
	RootDocFrame = doc->rootFrame();   /* go null margin fop pagination */
	QDomElement body = dom.createElement("fo:flow");
	body.setAttribute ("flow-name","xsl-region-body"); 
	
	qDebug() << "### Start read doc ..........................................";
	
	FrameLoop(doc->rootFrame()->begin(),body);
	pageseq1.appendChild( body );
	emit DomReady();
}

QDomElement FopDom::NodeFooter()
{
    QDomDocumentFragment  anode;
		QString dot1 = QString("<fo:instream-foreign-object content-width=\"11pt\">"
		"<svg:svg width=\"20\" height=\"20\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">"
		"<svg:circle cx=\"10\" cy=\"10\" r=\"5\" stroke=\"red\" stroke-width=\"0.3\" fill=\"black\"/>"
		"</svg:svg>"
		"</fo:instream-foreign-object>");
	  anode.setNodeValue(dot1);
	
	
	  QDomNode panew = anode.cloneNode( true );
	  return panew.toElement();
}



/* loop frame contenents */
/* http://www.bessrc.aps.anl.gov/software/qt4-x11-4.2.2-browser/d0/dbc/qtextdocument_8cpp-source.html */
void FopDom::FrameLoop( QTextFrame::Iterator frameIt , QDomElement appender )
{
	/* root frame! */
	BigframeProcessing++;
	QStringList  pretext;
	if (!frameIt.atEnd()) {
		QTextFrame::Iterator next = frameIt;
		++next;
		if (next.atEnd() && frameIt.currentFrame() == 0 &&  frameIt.parentFrame() != doc->rootFrame() &&  frameIt.currentBlock().begin().atEnd())
	 {
		return;    /* last frame */
	 }
	
	 }
		 
		 for (QTextFrame::Iterator it = frameIt;  !it.atEnd(); ++it) {
			 if (QTextFrame *f = it.currentFrame()) {
			          if (QTextTable *table = qobject_cast<QTextTable *>(f)) {
			          /* table here */
							  HandleTable(table,appender); 
								} 
								
								else {
							  /* other simple table from QTextDocument like <div> htmls */
							  QTextFrameFormat format = f->frameFormat();
							  QDomElement inlinediv = dom.createElement("fo:block-container");
								PaintFrameFormat(inlinediv,format);
								QString widhtarea = TranslateTextLengh( format.width());
                inlinediv.setAttribute ("width",widhtarea);
								HandleFrameInline(f->begin(),inlinediv);
							  appender.appendChild(inlinediv);
						}
			 } else if (it.currentBlock().isValid()) {
				  qDebug() << "### Read Root blockNumber / HandleBlock    " << it.currentBlock().blockNumber();
					HandleBlock(it.currentBlock(),appender);
			 }
		 }
		 
		 qDebug() << "### END read doc ..........................................";
}

void FopDom::HandleFrameInline( QTextFrame::Iterator frameIt , QDomElement appender ) /* frame tag is set !!!!! */
{
	/* root frame! */
	QTextFrame *actualframe = frameIt.currentFrame();
	BigframeProcessing++;
  if (!frameIt.atEnd()) {
		QTextFrame::Iterator next = frameIt;
		++next;
		if (next.atEnd() && actualframe == 0 &&  frameIt.parentFrame() != doc->rootFrame() &&  frameIt.currentBlock().begin().atEnd())  {
		return;    /* last frame */
	  }
	}
	 
	 for (QTextFrame::Iterator it = frameIt;  !it.atEnd(); ++it) {
			 if (QTextFrame *f = it.currentFrame()) {
			          if (QTextTable *table = qobject_cast<QTextTable *>(f)) {
			          /* table here */
							  HandleTable(table,appender); 
								} 
								else {
							  /* other simple table from QTextDocument like <div> htmls */
									if ( actualframe != f ) {
									/* next child */
									QTextFrameFormat format = f->frameFormat();
									QDomElement inlinediv = dom.createElement("fo:block-container");
									PaintFrameFormat(inlinediv,format);
									QString widhtarea = TranslateTextLengh( format.width());
									inlinediv.setAttribute ("width",widhtarea);
									HandleFrameInline(f->begin(),inlinediv);
									appender.appendChild(inlinediv);
								  }
									
									
						}
			 } else if (it.currentBlock().isValid()) {
				HandleBlock(it.currentBlock(),appender);
			 }
		 }
}


void FopDom::PaintFrameFormat(QDomElement e , QTextFrameFormat bf  )
{
	    if ( bf.position() == QTextFrameFormat::InFlow) {
			e.setAttribute ("display-align","auto"); /* center */
			e.setAttribute ("float","inherit"); 
			} else if (bf.position() == QTextFrameFormat::FloatLeft) {
			e.setAttribute ("display-align","before");
			} else if (bf.position() == QTextFrameFormat::FloatRight) {
			e.setAttribute ("display-align","after");
			e.setAttribute ("float","right");  ///// inherit
			}
			
			if (bf.background().color().name() !="#000000" && bf.background().color().isValid() ) {
      e.setAttribute ("background-color",ColorFopString(bf.background()));
      }
			
			if (bf.hasProperty(QTextFormat::FrameMargin)) {
					if (bf.topMargin() !=0) {
					e.setAttribute ("margin-top",QString("%1pt").arg(bf.topMargin()));  
					}
					if (bf.bottomMargin() !=0) {
					e.setAttribute ("margin-bottom",QString("%1pt").arg(bf.bottomMargin()));  
					}
					if (bf.rightMargin() !=0) {
					e.setAttribute ("margin-right",QString("%1pt").arg(bf.rightMargin()));  
					}
					if (bf.leftMargin() !=0) {
					e.setAttribute ("margin-left",QString("%1pt").arg(bf.leftMargin()));  
					}
			}
			
			QBrush boof = bf.borderBrush();
			
			if (bf.hasProperty(QTextFormat::FrameBorder))  {
				  const QString bordertype = BorderStyleCss(bf.borderStyle());
				  if (bordertype != "none" ) {
						////////  style width color   "border-left-" << "border-right-" << "border-bottom-" << "border-top-"
						QStringList SSBorder;
	          SSBorder << "border-";
						      for (int i = 0; i < SSBorder.size(); ++i)  {
										e.setAttribute (SSBorder.at(i)+"style",bordertype);  
										e.setAttribute (SSBorder.at(i)+"width",QString("%1pt;").arg(bf.border()));
										if (boof != Qt::NoBrush)  {
										e.setAttribute (SSBorder.at(i)+"color",ColorFopString(boof)); 
										}
									}
					}
			}
}

QString FopDom::TranslateTextLengh( const QTextLength unit ) 
{
	  if (unit.rawValue() < 1) {
			return QString();
		}
	  if (unit.type() == QTextLength::FixedLength) {
			return QString("%1pt").arg(Pointo(unit.rawValue(),"pt"));
		} else {
			return QString("%1\%").arg(unit.rawValue());
		}
}


/*  qt -> fop*/
void FopDom::HandleTable( QTextTable  * childTable , QDomElement appender ) 
{
	if (!childTable) {
	return;
	}
	QDomElement base;
	QTextTableFormat tbforms = childTable->format();
	if (tbforms.alignment() == Qt::AlignHCenter || tbforms.alignment() == Qt::AlignJustify || tbforms.alignment() == Qt::AlignRight) {
		QDomElement floater = dom.createElement("fo:float");
		if (tbforms.alignment() == Qt::AlignHCenter || tbforms.alignment() ==  Qt::AlignJustify ) {
			floater.setAttribute ("float","inline");
		} else if (tbforms.alignment() == Qt::AlignRight) {
			floater.setAttribute ("float","right");
		} else {
			floater.setAttribute ("float","left");
		}
		appender.appendChild(floater);
		base = dom.createElement("fo:table");
		floater.appendChild(base);
	} else {
		base = dom.createElement("fo:table");
		appender.appendChild(base);
	}
    
    
	
	FoBorder TableBorder;
	TableBorder.Change(TableBorder.rgb,QString("%1pt").arg(tbforms.border()),BorderStyleCss(tbforms.borderStyle()));
	const int coolsums = childTable->columns();
	if (tbforms.background().color().name() !="#000000") {
	base.setAttribute("background-color",ColorFopString(tbforms.background()));
	}
	
	
	
	
	TableBorder.SetBorder(base);  /* only if not 0 */
	QVector<QTextLength> constraints = tbforms.columnWidthConstraints();
	qDebug() << "### table coolsumns " << coolsums;
	qDebug() << "### table size large .............  " << constraints.size();
	
	///////qDebug() << "### coolsums b " << constraints.size();
	if (constraints.size() != coolsums) {
		 constraints.clear();
	}
	for (int i = 0; i < coolsums; ++i) {
		QDomElement cools  = dom.createElement("fo:table-column");
		if ( constraints.size() != 0 ) {
			QString sizecool = TranslateTextLengh(constraints.at(i));
			if (!sizecool.isEmpty()) { 
		  cools.setAttribute("column-width",sizecool);
			} else {
			cools.setAttribute("column-width",QString("%1\%").arg(100 / coolsums));
			}
		} else {
		cools.setAttribute("column-width",QString("%1\%").arg(100 / coolsums));
		}
		base.appendChild(cools);
	}
	QDomElement tbody  = dom.createElement("fo:table-body");
	if (tbforms.background().color().name() !="#000000") {
	tbody.setAttribute ("background-color",ColorFopString(tbforms.background()));
	}     
	TableBorder.SetBorder(tbody);  /* only if not 0 */
	
	base.appendChild(tbody);
  const int rowline = childTable->rows();
	for (int ttr = 0; ttr < rowline; ++ttr) {
	QDomElement rows  = dom.createElement("fo:table-row");
	tbody.appendChild(rows);
		              for (int ttd = 0; ttd < coolsums; ++ttd) {
										QTextTableCell cell = childTable->cellAt(ttr,ttd);
										const int rspan = cell.rowSpan();
										const int cspan = cell.columnSpan();
										QDomElement celltds  = dom.createElement("fo:table-cell");
										if (cell.format().background().color().name() !="#000000") {
										celltds.setAttribute ("background-color",ColorFopString(cell.format().background()));
										}
										
										            if (cspan > 1) {
                                ttd = ttd + cspan - 1;
                                celltds.setAttribute ("number-columns-spanned",cspan);
                                }
																
																QTextFrame::iterator di;
                                for (di = cell.begin(); !(di.atEnd()); ++di) {
                                     //////QTextFrame *tdFrame = di.currentFrame();
                                      QTextBlock tdpara = di.currentBlock();
                                      if (tdpara.isValid()) {
																			HandleBlock(tdpara,celltds);
                                      }
                                }
											if ( cspan > 1 || cspan == 1 ) {
											rows.appendChild(celltds);
											}
									}
	}
	
                    
}

/* paragraph  qt -> fop */
void FopDom::HandleBlock( QTextBlock  para  , QDomElement appender ) 
{
    if (!para.isValid()) {
    return;
    }
    sumblox++;
    QVariant footernote = para.blockFormat().property(FootNoteNummer); 
    if (!footernote.isNull()) {
    qDebug() << "### footernote founddddddddddddddddddd   " << para.blockNumber();
    }
	/* page breack policy */	
		
		
		
		//////qDebug() << "### block init build ..  ";
    const QString Actual_Text_Param = Qt::escape(para.text()).simplified();
    QDomElement paragraph;
    QTextImageFormat Pics;
    QTextTableFormat Tabl;
    QTextListFormat Uls;
		QTextFrameFormat Frameinline;
    QTextCharFormat paraformats;
    QString newnameimage;
    QString ImageFilename;
		bool nobreackline = false;
		int positioner = -1;
		paragraph = dom.createElement("fo:block");
        paragraph.setAttribute("id",QString("blocknr_%1").arg(para.blockNumber()));    
		if (para.blockFormat().nonBreakableLines()) {
		paragraph.setAttribute("white-space-collapse","false");
		nobreackline = true;
		}
		////const QTextBlockFormat BBnormal = DefaultMargin();
		//////const QTextCharFormat CCnormal = DefaultCharFormats();
        //////////////  QTextFormat::PageBreakFlags actual = bbformat.pageBreakPolicy();
		PaintFopBlockFormat(paragraph,para.blockFormat());
		PaintFopCharFormat(paragraph,para.charFormat());
		
		
		if (para.blockFormat().nonBreakableLines()) {
		PaintFopCharFormat(paragraph,PreFormatChar());
		}
		
		LINEHIGHT_CURRENT = LineHightCurrent(para.blockFormat(),para.charFormat());
		qreal currentHi = qBound (4.9,LINEHIGHT_CURRENT,33.2);
		
		
		
		
		
		if (Actual_Text_Param.isEmpty()) {
		appender.appendChild(paragraph);
		paragraph.setAttribute("line-height",QString("%1pt").arg(currentHi));
		SendBreakLine(paragraph,false);
		return;
		}
		
		 
		
		QTextList *list = para.textList();
		if (list) {
			 const int sumlist = list->count();
			 const int BListPosition = list->itemNumber(para);
			            /* fo list block like html ul / li */
			          if ( BListPosition == 0) { // first item?
									
									QDomComment listblocke = dom.createComment(QString("Start list block nr %1 from %2 .").arg(BListPosition).arg(sumlist));
	                appender.appendChild(listblocke);
									
									
								ulblock = dom.createElement("fo:list-block");
								appender.appendChild(ulblock);
								}
			
								
			       
									
									        QDomElement listitem = dom.createElement("fo:list-item");
													ulblock.appendChild(listitem);
													QDomElement labelli = dom.createElement("fo:list-item-label");
													labelli.setAttribute ("end-indent","label-end()");
													ListUlLiSymbol(labelli,list);
													listitem.appendChild(labelli);  /* label header */
													QDomElement libody = dom.createElement("fo:list-item-body");
													libody.setAttribute ("start-indent","body-start()");
													QDomElement textlabel = dom.createElement("fo:block");
								          if (para.blockFormat().nonBreakableLines()) {
		                       textlabel.setAttribute("white-space-collapse","false");
													}
													PaintFopCharFormat(textlabel,para.charFormat());
													PaintFopBlockFormat(textlabel,para.blockFormat());
													libody.appendChild(textlabel);
													listitem.appendChild(libody);
									
									         QTextBlock::iterator li;
																for (li = para.begin(); !(li.atEnd()); ++li) {
																	QTextFragment lifr = li.fragment();
																	 if (lifr.isValid()) {
																		  HandleFragment(lifr,textlabel,nobreackline);
																	 }
																}
									
								if ( BListPosition == (sumlist - 1) ) {
									///////ulblock.clear();
									////qDebug() << "### last BListPosition   " << BListPosition  << " from " << sumlist;
									QDomComment listblocke = dom.createComment(QString("End list block nr %1 from %2 .").arg(BListPosition).arg(sumlist));
	                ulblock.appendChild(listblocke);
									
								}
				
				return;
		}
		QTextBlock::iterator de;
    for (de = para.begin(); !(de.atEnd()); ++de) {
			        /////////////qDebug() << "### para  " << para.text();
              QTextFragment fr = de.fragment();
              if (fr.isValid()) {
								  ///////LINEHIGHT_CURRENT = LineHightCurrent(para.blockFormat(),fr.charFormat());
								 if (para.text() == fr.text() && !fr.text().contains(QChar::ObjectReplacementCharacter)) {
                    /////////paragraph.appendChild(dom.createTextNode(para.text()));
									  PaintFopCharFormat(paragraph,fr.charFormat());
									 
									    QString txt = Qt::escape(para.text());
										  QString forcedLineBreakRegExp = QString::fromLatin1("[\\na]");
								      forcedLineBreakRegExp[3] = QChar::LineSeparator;
								      const QStringList lines = txt.split(QRegExp(forcedLineBreakRegExp));
									       for (int i = 0; i < lines.count(); ++i) {
														if (i > 0) {
															  if (nobreackline) {
																paragraph.appendChild(dom.createTextNode(QString(" ")));
																} else {
																SendBreakLine(paragraph,true);
																}
														}
														if (!nobreackline) {
														HumanRead(paragraph,lines.at(i));
														} else {
														paragraph.appendChild(dom.createTextNode(lines.at(i)));
														}
												}
								 } else {
									  HandleFragment(fr,paragraph,nobreackline);
								 }
              }
		}
		
		paragraph.appendChild(NodeFooter());
		
							
		if (para.blockFormat().hasProperty(QTextFormat::BlockTrailingHorizontalRulerWidth)) {
		/////QDomElement Horizontal = dom.createElement("hr");  /* svg line 100% */
		//////paragraph.appendChild(Horizontal);
		}
		
		if (para.blockFormat().nonBreakableLines()) {
		paragraph.setAttribute("white-space-collapse","false");
		}
		
		/* check footenotes to resave*/
		if (!footernote.isNull()) {
			FopLeader FooterBlock = footernote.value<FopLeader>();
			FooterBlock.RestoreOn(paragraph);
		}
												
	appender.appendChild(paragraph);
	//////////QDomComment footerBlock = dom.createComment(QString("End block %1.").arg(sumblox));
	//////////appender.appendChild(footerBlock);
}

/* the big xsl-fo format not know <br> !!!!!!  */
void FopDom::SendBreakLine( QDomElement appender , bool blockcreate )
{
    /*    http://www.zvon.org/xxl/xslfoReference/Output/el_leader.html       */
	qreal currentHi = qBound (4.9,LINEHIGHT_CURRENT,33.2);
	if (blockcreate) {
	QDomElement spaceline = dom.createElement("fo:block"); 
	spaceline.setAttribute("space-after",QString("%1pt").arg(currentHi / 2));
	} else {
	QDomElement spaceline = dom.createElement("fo:leader"); 
	spaceline.setAttribute("content-height",QString("%1pt").arg(currentHi));
	spaceline.setAttribute("leader-pattern","space");
	spaceline.setAttribute("leader-length",RecoveryBreackLineParagraph());
    spaceline.setAttribute("speak-numeral",ApplicationsVersionFopConvert);   
        
    spaceline.appendChild(dom.createComment(QString("\nSipmle Breack line / empty Paragraph version %1. \nUse margin space to make space!\n").arg(ApplicationsVersionFopConvert)));
        
        
        
	appender.appendChild(spaceline);	
	}
}



void FopDom::HumanRead( QDomElement appender , QString longtext ) 
{
     QStringList list = longtext.split(" ");  ///// QString::SkipEmptyParts
	   QStringList roottxt;
	   QString one ="";
	   int fox = -1;
	
	         for (int i = 0; i < list.size(); ++i) {
						 const QString word = list.at(i).simplified();
						 bool islast = list.at(i) == list.last() ? true : false;
						 if (!islast) {
							 
							     if (!word.endsWith(" ")) {
						       one.append(word+" ");
									 } else {
									 one.append(word);
									 }
									 
						 } else {
						 one.append(word);
						 }
						 fox++;
						 if (fox == 15) {
							fox = -1;
							roottxt.append(one);
							one ="";
						 }
						 
						 if (islast) {
							 roottxt.append(one);
							 one ="";
						 }
						 
					 }
					 if (roottxt.size() > 0) {
					 appender.appendChild(appender.ownerDocument().createTextNode("\n"));
					 }
					 for (int x = 0; x < roottxt.size(); ++x) {
						 //////QDomComment footerBlock = appender.ownerDocument().createComment(QString("L.%1").arg(x));
	           //////appender.appendChild(footerBlock);
						 appender.appendChild(appender.ownerDocument().createTextNode(roottxt.at(x)));
						 if (roottxt.at(x) != roottxt.last()) {
						 appender.appendChild(appender.ownerDocument().createTextNode("\n"));
						 }
					 }
}




void FopDom::HandleFragment( QTextFragment fr  , QDomElement appender , bool pref ) /* pref = nobreack line space = true */
{

	QDomElement linkers;
	QDomElement span;
	QDomElement onechar;
	QString txt = fr.text();
	ApiSession *session = ApiSession::instance(); 
	const QTextCharFormat format = fr.charFormat();
	
	
		  if (format.isAnchor()) {
				         QStringList namelist = format.anchorNames();
								 const QString href = format.anchorHref().trimmed();
				         if (namelist.size() > 0 || !format.anchorName().isEmpty() ) {
									 	    QString first;
												if (!format.anchorName().isEmpty()) {
												first = format.anchorName();
												} else {
												first = namelist.first();
												}
									 appender.setAttribute ("id",namelist.first().trimmed()); /* bookmark target here */
									 return;
								 }
								 
								 if (!href.isEmpty()) {
											linkers = dom.createElement("fo:basic-link");
									    if (pref) {
									    linkers.setAttribute("white-space-collapse","false");
											}
											PaintFopCharFormat(linkers,fr.charFormat());
											linkers.setAttribute ("external-destination",href);
									    appender.appendChild(linkers);
											QString txt = Qt::escape(fr.text());
										  QString forcedLineBreakRegExp = QString::fromLatin1("[\\na]");
								      forcedLineBreakRegExp[3] = QChar::LineSeparator;
								      const QStringList lines = txt.split(QRegExp(forcedLineBreakRegExp));
												for (int i = 0; i < lines.count(); ++i) {
														if (i > 0) {
															  if (pref) {
																linkers.appendChild(dom.createTextNode(QString(" ")));
																} else {
																SendBreakLine(linkers,true);
																}
														}
														linkers.appendChild(dom.createTextNode(lines.at(i)));
												}
								 }
				return;
			}
			/* image creator xml */
			if (txt.count() == 1 && txt.at(0) == QChar::ObjectReplacementCharacter) {
				    if (format.isImageFormat()) {
							  bool svg_inline_image = false;
							
							   
							
							      QTextImageFormat Pics = format.toImageFormat();
							      const QString hrefadress = Pics.name();
							      qDebug() << "### QTextImageFormat ..  " << hrefadress;
							
							      if (hrefadress.startsWith("foleader")) {
											
											  qDebug() << "### foleader found ..  ";
											
											  QVariant xfx = Pics.property(LeaderNummer); 
											  if (!xfx.isNull()) {
													FopLeader spacedoc = xfx.value<FopLeader>();
													spacedoc.RestoreOn(appender);
													return;
												}
											
											
										}
							
							
							
							      if (hrefadress.startsWith("/svg/")) {
											svg_inline_image = true;
										}
							      
										qDebug() << "### svg having ..  " << svg_inline_image;
										
									  QString titles;
							      QString Ivariant = "Gf-200";
                    QVariant xx = Pics.property(_IMAGE_PICS_ITEM_); 
										
										
										
							      if (xx.isNull()) {
											       bool session_havingimg = false;
											      /* session having image? */
											      QMapIterator<QString,SPics> i(session->ImagePageList);
																while (i.hasNext()) {
																 i.next();
																 SPics record = i.value();
																 if ( record.name == hrefadress ) {
																	 session_havingimg = true;
																	 xx = QVariant(record);
																 }
																}  
											      if (!session_havingimg) {
											      Ivariant = "Gf-404";
														}
										}
										
										if (svg_inline_image) {
											QByteArray domsvg;
											QDomDocument dimg;
											bool session_havingimg = false;
											
											         QMapIterator<QString,QByteArray> e(session->SvgList);
																while (e.hasNext()) {
																 e.next();
																 domsvg = e.value();
																 if ( e.key() == hrefadress ) {
																	 qDebug() << "### session found e.key() " << e.key();
																  session_havingimg = true;
																 }
																}
																
                           if (session_havingimg && dimg.setContent(domsvg,false)) {
                                   QDomElement root_extern = dimg.documentElement(); 
														       QDomNamedNodeMap alist = root_extern.attributes();   /* copy all attributes from other doc */
																		QDomElement svg = dom.createElement("svg:svg");
																				for (int i=0; i<alist.count(); i++){
																					QDomNode nod = alist.item(i);
																					svg.setAttribute(nod.nodeName().toLower(),nod.nodeValue());
																				}
																	    svg.setAttribute ("xmlns","http://www.w3.org/2000/svg");
                                      svg.setAttribute ("version","1.2");
                                      svg.setAttribute ("baseProfile","tiny");
																				
																			QDomNode child = root_extern.firstChild();
																						 while ( !child.isNull() ) {
																								 if ( child.isElement() ) {
																									svg.appendChild(dom.importNode(child,true).toElement());
																								 }
																							 child = child.nextSibling();            
																						 }
																						 
																						 
																				QDomElement simagen = dom.createElement("fo:instream-foreign-object");
																				simagen.setAttribute ("width",QString("%1px").arg(Pics.width()));
																				simagen.setAttribute ("height",QString("%1px").arg(Pics.height()));
																				simagen.setAttribute ("id",Imagename(hrefadress));
																						 
																				simagen.setAttribute ("scaling","non-uniform");  /* forever wi x hi corect ! */
										                    simagen.setAttribute ("content-width",QString("%1px").arg(Pics.width()));
										                    simagen.setAttribute ("content-height",QString("%1px").arg(Pics.height()));
																						 
																						 
																				////////simagen.setAttribute ("id",Imagename(hrefadress)+"-"+Ivariant);
																				simagen.appendChild(svg);
																				appender.appendChild(simagen);
																					

													 }														 
                            
																
										}
										
										if (!svg_inline_image) {
                    QDomElement imagen = dom.createElement("fo:external-graphic");
							      imagen.setAttribute ("width",QString("%1px").arg(Pics.width()));
										imagen.setAttribute ("height",QString("%1px").arg(Pics.height()));
										imagen.setAttribute ("scaling","non-uniform");  /* forever wi x hi corect ! */
										imagen.setAttribute ("content-width",QString("%1px").arg(Pics.width()));
										imagen.setAttribute ("content-height",QString("%1px").arg(Pics.height()));
							      imagen.setAttribute ("src",hrefadress);   /* session check if having this file !! */
							      imagen.setAttribute ("id",Imagename(hrefadress)+"-"+Ivariant);
										if (!xx.isNull()) {
											  SPics pico = xx.value<SPics>();
                        pico.FopSaveImage(imagen);
												imagen.setAttribute ("id",Imagename(pico.info)+"-"+Ivariant);
										}
							      appender.appendChild(imagen);
									 }
						}
						
			}  else {
        Q_ASSERT(!txt.contains(QChar::ObjectReplacementCharacter));   /* no image no other choises */
				              QString txt = Qt::escape(fr.text());
										  QString forcedLineBreakRegExp = QString::fromLatin1("[\\na]");
								      forcedLineBreakRegExp[3] = QChar::LineSeparator;
								      const QStringList lines = txt.split(QRegExp(forcedLineBreakRegExp));
				              span = dom.createElement("fo:inline");
				              PaintFopCharFormat(span,fr.charFormat());
				              if (pref) {
									    span.setAttribute("white-space-collapse","false");
											}
				              appender.appendChild(span);
											
												for (int i = 0; i < lines.count(); ++i) {
													
														if (i > 0) {
														SendBreakLine(span,true);
														}
														
												    if (!pref) {
														HumanRead(span,lines.at(i));
														} else {
														span.appendChild(dom.createTextNode(lines.at(i)));
														}
												}
			}
}
void FopDom::PaintLastBlockformat( QDomElement e , QTextCharFormat bf  )
{
	
			
}






void FopDom::PaintFopCharFormat( QDomElement e , QTextCharFormat bf  )
{
	    QTextCharFormat dax = DefaultCharFormats();
	    QFont userfont = bf.font();
	    QColor bb(Qt::black);
	    const QString col = ColorFopString(bf.foreground());
	
	    if ( bf.foreground().color().name() != "#000000" ) {   
						if (bf.foreground().color().alpha() > 20)   /* no trasparent zero */
						{
						e.setAttribute ("color",col);
						}
			}
			
			if ( bf.background().color().name() != "#000000" ) {   
						if (bf.background().color().alpha() > 20)   /* no trasparent zero */
						{
						e.setAttribute("background-color",ColorFopString(bf.background()));
						}
			}
			
			
			if ( bf.font().family() != QApplication::font().family() ) { 
      e.setAttribute ("font-family",userfont.family());
			}
			if (dax.font().pointSize() != userfont.pointSize()) {
			e.setAttribute ("font-size",QString("%1pt").arg(userfont.pointSize()));
			}
			QStringList fontvario;
			fontvario.clear();
			
			QStringList txtdeco;
			txtdeco.clear();
			
			
			if (bf.fontWeight() > 51) {
			///////const int fonftwbold = qBound (100,bf.fontWeight() * 6,900);
			e.setAttribute ("font-weight","bold");
			}
			if (bf.fontItalic()) {
			fontvario.append("italic");
			}
			if (bf.fontStrikeOut() ) {
			///////fontvario.append("line-through");
			txtdeco.append("line-through");
			}  
			if (bf.fontUnderline() ) {
			//////fontvario.append("underline");
			txtdeco.append("underline");
			}
			if (bf.fontOverline()) {
			////////fontvario.append("overline");
			txtdeco.append("overline");
			}
			
			
			if (fontvario.size() > 0) {
			e.setAttribute ("font-style",fontvario.join(" "));
			}
			if (txtdeco.size() > 0) {
			e.setAttribute ("text-decoration",txtdeco.join(" "));
			}
			
			///////int hundert = bf.fontLetterSpacing();
			///////if (hundert !=0 && hundert != 100.00) {
                  QFont userfontee = bf.font();
            
                  if ( userfontee.letterSpacingType() != QFont::PercentageSpacing) {
                     e.setAttribute ("letter-spacing",QString("%1pt").arg(userfontee.letterSpacing()));
                  }
                  
			/////}
			if (bf.verticalAlignment() == QTextCharFormat::AlignSuperScript) {
			e.setAttribute ("baseline-shift","super");
			}
			if (bf.verticalAlignment() == QTextCharFormat::AlignSubScript) {
			e.setAttribute ("baseline-shift","sub");
			}
			
			
			
}



void FopDom::TextAlignment(Qt::Alignment align , QDomElement e )
{
     if (align & Qt::AlignLeft)
         return;
    else if (align & Qt::AlignRight)
         e.setAttribute ("text-align","right");  
    else if (align & Qt::AlignHCenter)
         e.setAttribute ("text-align","center");  
     else if (align & Qt::AlignJustify)
         e.setAttribute ("text-align","justify"); 
}

void FopDom::PaintFopBlockFormat( QDomElement e , QTextBlockFormat bf )
{
      TextAlignment(bf.alignment(),e);
    
      QTextFormat::PageBreakFlags actual = bf.pageBreakPolicy();
      if (actual == QTextFormat::PageBreak_AlwaysBefore) {
      e.setAttribute("break-before","page"); 
      } else if ( actual == QTextFormat::PageBreak_AlwaysAfter ) {
      e.setAttribute("break-after","page");  
      }
    
    
      if (bf.topMargin() !=0) {
      e.setAttribute ("margin-top",QString("%1pt").arg(bf.topMargin()));  
      }
      if (bf.bottomMargin() !=0) {
      e.setAttribute ("margin-bottom",QString("%1pt").arg(bf.bottomMargin()));  
      }
       if (bf.rightMargin() !=0) {
      e.setAttribute ("margin-right",QString("%1pt").arg(bf.rightMargin()));  
      }
      if (bf.leftMargin() !=0) {
      e.setAttribute ("margin-left",QString("%1pt").arg(bf.leftMargin()));  
      }
      if (bf.background().color().name() !="#000000" && bf.background().color().isValid() ) {
      e.setAttribute ("background-color",ColorFopString(bf.background()));
      }
			e.setAttribute ("color",ColorFopString(bf.foreground()));
}


/* loop  to find attribute name xx */
QString FopDom::FilterAttribute( QDomElement element , QString attribute ) 
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


QString FopDom::BorderStyleCss(QTextFrameFormat::BorderStyle style)
{
    Q_ASSERT(style <= QTextFrameFormat::BorderStyle_Outset);
    QString html ="";
	
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
        html += QLatin1String("none");
        break;
    };
		return html;
}






QString FopDom::ColorFopString( const QBrush paintcolor ) const 
{
	  ///////QColor::QColor ( int r, int g, int b, int a = 255 )
	  QColor c = paintcolor.color();
		QString StringColor = QString("rgb(%1,%2,%3,%4)").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
	  return StringColor;
}
QString FopDom::ColorFopString( const QColor c ) const 
{
	  ///////QColor::QColor ( int r, int g, int b, int a = 255 )
		QString StringColor = QString("rgb(%1,%2,%3,%4)").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
	  return StringColor;
}


void FopDom::ListUlLiSymbol( QDomElement appender , QTextList *list )
{
	QDomElement blocklabel = dom.createElement("fo:block");   /* list style !!!! */
	appender.appendChild(blocklabel);	
	DotList_Circle( blocklabel );  /* append svg dot 20pt */
  
}




qreal FopDom::LineHightCurrent(  QTextBlockFormat bf , QTextCharFormat cf  )
{
  const qreal s1 = cf.font().pointSize();
	const qreal s2 = bf.topMargin();
	const qreal s3 = bf.bottomMargin();
	return s1 + s2 + s3;
}


