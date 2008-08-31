#include "Fo_Format.h"

using namespace ApacheFop;


Fo_Format::Fo_Format( QObject *parent )
	: QObject( parent ),ErrorSumm(0),apacheFopColor( new FopColor() )
{
	LoopChar = 0;
	LoopBlock = 0;
	ApiSession *session = ApiSession::instance();
	session->LastCharOpen.clear();
	session->LastBlockOpen.clear();
}

QTextListFormat Fo_Format::TextListFromFromDom(const QDomElement e)
{
    QTextListFormat pf;
    QDomNamedNodeMap attlist = e.attributes();
    for (int i=0; i<attlist.count(); i++) {
    QDomNode nod = attlist.item(i);
                 if (nod.nodeName().toLower() == "background-color") {
                 pf.setBackground ( QBrush(ColorFromFoString(nod.nodeValue()) ) ); 
                 }
        
    }
    return pf;
}

FoRegion Fo_Format::regionFormat(const QDomElement e)
{
        FoRegion pf;
    
            /* body disable but not read flag */
            /*  tag not exist is false */
            pf.enable = false;
    
        
        if (e.tagName() == "fo:region-body"  ||
            e.tagName() == "fo:simple-page-master"  ||
            e.tagName() == "fo:region-before"  ||
            e.tagName() == "fo:region-after"  ||
            e.tagName() == "fo:region-start"  ||
            e.tagName() == "fo:region-end"
        )  {
         pf.enable = true;
        }
    
        const qreal gmargin =  Unit(e.attribute ("margin"));
        if (gmargin > 0) {
         pf.toAll( gmargin );
        } else {
            pf.margin_top = Unit(e.attribute ("margin-top",QString("0cm")));
            pf.margin_bottom  = Unit(e.attribute ("margin-bottom",QString("0cm")));
            pf.margin_left  = Unit(e.attribute ("margin-left",QString("0cm")));
            pf.margin_right  = Unit(e.attribute ("margin-right",QString("0cm")));
        }
        
        pf.padding  = Unit(e.attribute ("padding",QString("0cm")));
        
        QString tagN = e.tagName();
        
        qreal stendi= Unit(e.attribute("extent"));
        ////////qDebug() << "### stendi  -------- " << stendi;
        
        if (!e.attribute ("background-color").isEmpty())  {
            pf.bg = ColorFromFoString(e.attribute ("background-color"),FopColor::LightColor);
            pf.enable = true;
        } else {
            ////////pf.enable = false;
        }
        
        
    return pf;
}


/* css2 like format attributes */
QStringList Fo_Format::attributeList(const QDomElement e)
{
    QStringList pf;
    QDomNamedNodeMap attlist = e.attributes();
    for (int i=0; i<attlist.count(); i++) {
    QDomNode nod = attlist.item(i);
    QString line = QString("%1:%2").arg(nod.nodeName().toLower()).arg(nod.nodeValue());
    pf.append(line);
    }
    return pf;
}







QTextBlockFormat Fo_Format::TextBlockFormFromDom( const QDomElement e , QTextBlockFormat pf  ) 
{
    /* pf = parent format */
    LoopBlock++;
	ApiSession *session = ApiSession::instance();
    QDomNamedNodeMap attlist = e.attributes();
    pf.setTopMargin(0);
    pf.setAlignment( TagAlignElement(e));

	    QStringList option;
                  option.clear();
	    QString idref = QString("BlockFormat_%1").arg(LoopBlock);  /* id identificativ if to bookmark area having? */
    
            for (int i=0; i<attlist.count(); i++) {
                QDomNode nod = attlist.item(i);
							
							 if (_PARSE_DEBUG_FOP_ == 1) {
               option.append(nod.nodeName().toLower()+"="+nod.nodeValue()+"\n");
               }
							
							
                /////if (nod.nodeName().toLower() == "space-after.optimum") {
                 //////pf.setBottomMargin(Unit(nod.nodeValue()));
                ////}
                
                 if (nod.nodeName().toLower() == "break-before") {
                 pf.setPageBreakPolicy ( QTextFormat::PageBreak_AlwaysBefore );
                 }
                 
                 if (nod.nodeName().toLower() == "break-after") {
                 pf.setPageBreakPolicy ( QTextFormat::PageBreak_AlwaysAfter );
                 }
                 
                 if (nod.nodeName().toLower() == "line-height") {
                 
                 }
								 
								 
                 if (nod.nodeName().toLower() == "background-color") {
                 pf.setBackground ( QBrush(ColorFromFoString(nod.nodeValue()) ) ); 
                 }
                 
                 if ( nod.nodeName().toLower() == "space-before" || 
                      nod.nodeName().toLower() == "space-before.optimum" ||
                      nod.nodeName().toLower() == "margin-top"  ) {   /* top */
                      const qreal topspace = Unit(nod.nodeValue());
                     ///////////// qDebug() << "### TopSpacing   " << topspace;
                          
                     if (topspace > 0) {
                     pf.setTopMargin(topspace);
                     }
                 }
                 
                 if ( nod.nodeName().toLower() == "space-after" || nod.nodeName().toLower() == "margin-bottom" ) {    /* bottom */
                    pf.setBottomMargin(Unit(nod.nodeValue()));
                 }
                 
                 if ( nod.nodeName().toLower() == "space-start" || nod.nodeName().toLower() == "margin-right" ) {  /* right */
                    pf.setRightMargin(Unit(nod.nodeValue()));
                 }
                 
                 if ( nod.nodeName().toLower() == "space-end" || nod.nodeName().toLower() == "margin-left" ) {   /* left */
                    pf.setLeftMargin(Unit(nod.nodeValue()));
                 }
                 if ( nod.nodeName().toLower().contains("start-indent") ) {   /* indent */
                    pf.setLeftMargin(Unit(nod.nodeValue()) + pf.leftMargin() );
                    pf.setProperty(QTextFormat::TextIndent,Unit(nod.nodeValue()));
                 }
            }
            
         if (_PARSE_DEBUG_FOP_ == 1) {
				session->LastBlockOpen.insert( LoopBlock ,option.join("\n"));
         }
				
    return pf;
}

Qt::Alignment Fo_Format::TagAlignElement(const QDomElement e ) 
{
    QString value;
    QString FAli0 = e.attribute( "text-align","-");
    QString FAli1 = e.attribute( "align","-");
    if (FAli0 !=QString("-")) {
        value = FAli0.simplified().toLower();
    }
    if (FAli1 !=QString("-")) {
        value = FAli1.simplified().toLower();
    }
    
    
    if ( e.attribute( "vertical-align") == "middle" || e.attribute( "display-align") == "center" ) {
    return Qt::AlignVCenter;
    }
    
    ////////qDebug() << "###TagAlignElement...->" << value;
    
    /* display-align   vertical-align="middle" */
    
    //////////  Qt::AlignCenter
    
    
    if (value.size() < 1) {
    return Qt::AlignLeft;
    } else if (value == "center" ) {
     return Qt::AlignCenter;
    } else if (value == "right" || value == "end" ) {
     return Qt::AlignRight;
    } else if (value == "justify" ) {
      return Qt::AlignJustify;
    } else if (value == "inherit" || value == "inside" ) {
      return Qt::AlignAbsolute;
    } else if (value == "start" || value == "left" ) {
      return Qt::AlignLeft;
    } else {
      return Qt::AlignLeft;
    }
}

/* dom to qtxt */
QTextCharFormat Fo_Format::GlobalCharFormat( const QDomElement e , QTextCharFormat f ) 
{
      LoopChar++;
      f.setForeground(QBrush(Qt::black));
      QDomNamedNodeMap attlist = e.attributes();
      QStringList option;
                  option.clear();
      //////int fontstrech = e.attribute ("font-stretch",QString("0")).toInt();
      QString idref = QString("CharFormat_%1").arg(LoopChar);  /* id identificativ if to bookmark area having? */
	
	   ApiSession *session = ApiSession::instance();
    
        for (int i=0; i<attlist.count(); i++){
             QDomNode nod = attlist.item(i);
               if (_PARSE_DEBUG_FOP_ == 1) {
               option.append(nod.nodeName().toLower()+"="+nod.nodeValue()+"\n");
               }
            
               const QString valore = nod.nodeValue().toLower();
            
            if (nod.nodeName().toLower() == "background-color") {
                f.setBackground (QBrush(ColorFromFoString(nod.nodeValue())) ); 
            }
            
            if (nod.nodeName().toLower() == "baseline-shift") {
                ///////f.setUnderlineStyle( QTextCharFormat::SingleUnderline );
                ///////f.setUnderlineColor(QColor(Qt::red));
                
                if (nod.nodeValue().toLower().startsWith("super") || 
                    nod.nodeValue().toLower().startsWith("+") ||
                    nod.nodeValue().toLower().startsWith("1")) {
                f.setVerticalAlignment ( QTextCharFormat::AlignSuperScript );
                } else {
                f.setVerticalAlignment ( QTextCharFormat::AlignSubScript ); 
                }
            }
            
            
               
             if (nod.nodeName().toLower() == "color") {
                 ///////////qDebug() << "### txt color  " << nod.nodeValue();
                 f.setForeground(QBrush(ColorFromFoString(nod.nodeValue())));
             }
             
             
             if (nod.nodeName().toLower() == "id") {
                 idref = nod.nodeValue();
             }
             if (nod.nodeName().toLower() == "font-family") {
                 QFont userfont( nod.nodeValue() ); 
                 if (session->FontList().contains(nod.nodeValue())) {
                     f.setFont(userfont);
                 } else {
                    ////////////notexistFontMap.insert(nod.nodeValue(),QApplication::font());    /* prepare replace font item ? */
                    f.setFont(QApplication::font());
                 }
             } 
             if (nod.nodeName().toLower() == "font-size") {
                  qreal Hispoint = Unit(nod.nodeValue(),FONT);
                  if (Hispoint > 2) {
                  f.setFontPointSize(Hispoint);
                  } else {
                  f.setFontPointSize(10);  
                  }
             }
             
             if (nod.nodeName().toLower() == "external-destination" || nod.nodeName().toLower() == "internal-destination") {
                 f.setAnchorHref(nod.nodeValue());
                 f.setAnchor(true);
                 f.setForeground ( QBrush ( _LINK_COLOR_ ) );  /* red link */
             }
             
             if (valore.contains("bold")) {
                  f.setFontWeight(QFont::Bold); 
             } 
                  
             
             if (nod.nodeName().toLower() == "font-weight") {
                 if (nod.nodeValue().toInt() > 2) {
                 f.setFontWeight(QFont::Bold);
                 }
             }
               
                  if (valore.contains("italic")) {
                  f.setFontItalic(true);
                  }
                  if (valore.contains("underline")) {
                  f.setUnderlineStyle ( QTextCharFormat::SingleUnderline );
                  }
                  if (valore.contains("oblique")) {
                  ///////f.setFontItalic(true);
                  }
                  if (valore.contains("overline")) {
                  f.setFontOverline(true);
                  }   
                  
                  if (valore.contains("line-through")) {
                  f.setFontStrikeOut ( true );
                  }  
                  
                  
                  
                  if (nod.nodeName().toLower() == "letter-spacing" ) {
                      const qreal needspace = Unit(nod.nodeValue(),FONT);
                      QFont ss = f.font();
                      ss.setLetterSpacing(QFont::AbsoluteSpacing,needspace);
                      f.setFont(ss);
                  }
                  
                 

                  
        
             
    
             
           
             
             
        }
				
				if (_PARSE_DEBUG_FOP_ == 1) {
				session->LastCharOpen.insert( LoopChar ,option.join("\n"));
				}
        
        /*
        if (f.background().color().name() == f.foreground().color().name()) {
        const QColor contrastbackcolor(
        f.background().color().red()   > 127 ? 0 : 255,
        f.background().color().green() > 127 ? 0 : 255,
        f.background().color().blue()  > 127 ? 0 : 255);
        f.setForeground(contrastbackcolor); 
        }
        */
        return f;
}


/* dom to qtext */
QTextFrameFormat::BorderStyle Fo_Format::StyleBorder( const QDomElement e ) 
{
	QStringList findit;
	findit << "border-style" << "border-before-style" << "border-bottom-style" << "border-left-style" << "border-right-style" << "border-start-style" << "border-top-style";
	   for (int i = 0; i < findit.size(); ++i)  {
				if ( StyleBorder(e.attribute(findit.at(i)).toLower()) != QTextFrameFormat::BorderStyle_None) {
				return StyleBorder(e.attribute(findit.at(i)).toLower());
				}
		 }
	return QTextFrameFormat::BorderStyle_None;
}


/* dom to qtext */
QTextFrameFormat::BorderStyle Fo_Format::StyleBorder( const QString Sborder ) 
{
    QTextFrameFormat::BorderStyle BBorder = QTextFrameFormat::BorderStyle_None;
    if (Sborder.size() < 1) {
    return QTextFrameFormat::BorderStyle_None;
    } else {
			
        const QString borderpaint = Sborder;
        if (borderpaint == "solid") {
        return QTextFrameFormat::BorderStyle_Solid;
        } else if (borderpaint == "dotted") {
        return QTextFrameFormat::BorderStyle_Dotted;
        } else if (borderpaint == "none") {
        return QTextFrameFormat::BorderStyle_None;
        } else if (borderpaint == "dashed") {
        return QTextFrameFormat::BorderStyle_Dashed;
        }  else if (borderpaint == "dot-dash") {
        return QTextFrameFormat::BorderStyle_DotDash;
        } else if (borderpaint == "double") {
        return QTextFrameFormat::BorderStyle_Double;
        } else if (borderpaint == "groove") {
        return QTextFrameFormat::BorderStyle_Groove;
        } else if (borderpaint == "inset") {
        return QTextFrameFormat::BorderStyle_Inset;
        } else if (borderpaint == "outset") {
        return QTextFrameFormat::BorderStyle_Outset;
        }  else if (borderpaint == "dot-dot-dash") {
        return QTextFrameFormat::BorderStyle_DotDotDash;
        } else {
					return QTextFrameFormat::BorderStyle_None;
				}
    }
    
   return QTextFrameFormat::BorderStyle_None; 
}
   
/* qtext to dom */
QString Fo_Format::BorderStyleCss(QTextFrameFormat::BorderStyle style)
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
        Q_ASSERT(false);
        break;
    };
		return html;
}


QBrush Fo_Format::GetFrameBorderColor( const QDomElement e )
{
	QBrush none = QBrush(Qt::black);
	QStringList findit;
	findit << "border-color" << "border-bottom-color" << "border-left-color" << "border-right-color" << "border-start-color" << "border-top-color";
	      for (int i = 0; i < findit.size(); ++i)  {
				if ( ColorFromFoString(e.attribute(findit.at(i)).toLower()).isValid()) {
				return QBrush(ColorFromFoString(e.attribute(findit.at(i)).toLower()));
				}
		 }
	return none;
}

qreal Fo_Format::isBorder( const QDomElement e )
{
QTextFrameFormat findBorder = PaintFrameFormat(e,QTextFrameFormat());
return findBorder.border(); 
}


QColor Fo_Format::ColorFromFoString( QString focolor , FopColor::AlternateColor col )
{
	return apacheFopColor->foColor(focolor,col);
    //////////   FopColor::AlternateColor col = DarkColor 
}




void Fo_Format::FindMargin( const QDomElement e )
{
	  if (!e.hasAttributes()) {
		return;
		}
		//////////////////  QRectF(top,right,bottom,left);
		/////////////////// QRectF ( qreal x, qreal y, qreal width, qreal height )
		////////  MarginPage = QRectF(xTopMargin,xRightMargin,xBottomMargin,xLeftMargin);
    qreal Boleft = Unit(e.attribute ("margin-top",QString("0")));
		if (Boleft !=0) {
			MarginPage.setX ( Boleft );
		}
    qreal Botop = Unit(e.attribute ("margin-bottom",QString("0")));
		if (Botop !=0) {
			MarginPage.setWidth( Botop );
		}
    qreal Bobotto = Unit(e.attribute ("margin-left",QString("0")));
		if (Bobotto !=0) {
			MarginPage.setHeight ( Bobotto );
		}
    qreal Bobright = Unit(e.attribute ("margin-right",QString("0")));
		
    if (Bobright !=0) {
			MarginPage.setY ( Bobright );
		}
    qreal Bolefta = Unit(e.attribute ("margin-start",QString("0")));
		if (Bolefta !=0) {
			MarginPage.setX ( Bolefta );
		}
    qreal Botopa = Unit(e.attribute ("margin-end",QString("0")));
		if (Botopa !=0) {
			MarginPage.setX ( Botopa );
		}
    qreal Bobottoa = Unit(e.attribute ("margin-after",QString("0")));
		if (Bobottoa !=0) {
			MarginPage.setX ( Bobottoa );
		}
    qreal Bobrighta = Unit(e.attribute ("margin-before",QString("0")));
		if (Bobrighta !=0) {
			MarginPage.setX ( Bobrighta );
		}
	  
	
}


/* qt only one padding */
qreal Fo_Format::TakeOnePadding( const QDomElement e )
{
    qreal Boleft = Unit(e.attribute ("padding-top",QString("0")));
    qreal Botop = Unit(e.attribute ("padding-bottom",QString("0")));
    qreal Bobotto = Unit(e.attribute ("padding-left",QString("0")));
    qreal Bobright = Unit(e.attribute ("padding-right",QString("0")));
    
    qreal Bolefta = Unit(e.attribute ("padding-start",QString("0")));
    qreal Botopa = Unit(e.attribute ("padding-end",QString("0")));
    qreal Bobottoa = Unit(e.attribute ("padding-after",QString("0")));
    qreal Bobrighta = Unit(e.attribute ("padding-before",QString("0")));
    
    qreal first = qMax (Boleft,Bobright);
    qreal second = qMax (Bobotto,Botop);
    
    qreal firsta = qMax (Bolefta,Bobrighta);
    qreal seconda = qMax (Bobottoa,Botopa);
    
    qreal Fnordsud = qMax (first,second);
    qreal Enordsud = qMax (firsta,seconda);
    return qMax (Fnordsud,Enordsud);
}


/* qt only one padding */
qreal Fo_Format::TakeOneMargin( const QDomElement e , bool largespace )
{
    QTextBlockFormat Ftf = TextBlockFormFromDom(e,DefaultMargin());
    qreal first = qMax (Ftf.bottomMargin(),Ftf.topMargin());
    qreal second = qMax (Ftf.rightMargin(),Ftf.leftMargin());
    if (largespace) {
    return second;
    } else {
    return first;  
    }
}









QTextFrameFormat Fo_Format::PaintFrameFormat( const QDomElement e , QTextFrameFormat format )
{
    qreal Padding = TakeOnePadding(e);
    format.setPadding(Padding);  
    QBrush BorderBrush = GetFrameBorderColor(e);
    QColor Bgcolor = QColor(Qt::white);
    QColor BorderColor = QColor(Qt::white);
    QStringList Bocolo;
	  Bocolo << "border-left-color" << "border-right-color" << "border-top-color" << "border-bottom-color" << "border-color";
    QDomNamedNodeMap attlist = e.attributes();
    qreal border = 0.9;
    
        for (int i=0; i<attlist.count(); i++){
             QDomNode nod = attlist.item(i);
             const QString valore = nod.nodeValue().toLower();
            
            
             if (nod.nodeName().toLower().startsWith("border-") && nod.nodeName().toLower().endsWith("-width") ) {
                if (valore.endsWith("%")) {
                border = 0.5;
                } else {
                border = qMax(border,FopInt(valore));
                }
             }
            
            
            
            if (nod.nodeName().toLower() == "border-after-width" || 
                nod.nodeName().toLower() == "border-before-width" ||
                nod.nodeName().toLower() == "border-bottom-width" ||
                nod.nodeName().toLower() == "border-end-width" ||
                nod.nodeName().toLower() == "border-left-width" ||
                nod.nodeName().toLower() == "border-right-width" ) {
                border = qMax(border,Unit(valore));
            }
            
            
            if (Bocolo.contains(nod.nodeName().toLower())) {
                BorderColor = ColorFromFoString(valore);
            }
            
            if (nod.nodeName().toLower() == "background-color") {
                Bgcolor = ColorFromFoString(valore);
            }
          }
          
    QTextBlockFormat Ftf = TextBlockFormFromDom(e,DefaultMargin());
    format.setPadding ( 0);  
    format.setBottomMargin(Ftf.bottomMargin()); 
          
          qreal FFtop = Ftf.topMargin();
          if (FFtop > 0) {
             format.setTopMargin(FFtop);
          } else {
             format.setTopMargin(0); 
          }
    
    format.setRightMargin(Ftf.rightMargin());
    format.setLeftMargin(Ftf.leftMargin()); 
    format.setBackground(QBrush(BorderColor));
    QTextFrameFormat::BorderStyle BBorder = StyleBorder(e);
          
    /////////QColor NoBorderColor = QColor(Qt::white);
          
 
        /* having border */
    format.setBorderBrush ( QBrush(Bgcolor) );
    format.setBorder ( border );
    format.setBorderStyle(BBorder); 
    format.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    
    
    /////////////qDebug() << "###floatflo---------------  " << e.attribute("float") << e.tagName().toLower();
    
    if (e.attribute("float") == "left") {
    format.setPosition ( QTextFrameFormat::FloatLeft );  
    } else if (e.attribute("float") == "right") {
    format.setPosition ( QTextFrameFormat::FloatRight );  
    } else if (e.attribute("float") == "inline") {
    format.setPosition( QTextFrameFormat::InFlow );
    } else {
    format.setPosition ( QTextFrameFormat::InFlow ); 
    }
    return format;
}

QTextLength Fo_Format::BlockMesure( const QDomElement e )
{
    bool allowlengh = false;
    if (FoTag(e) == BLOCK_TAG || FoTag(e) == FLOATBLOCK || FoTag(e) == BLOCK_CONTAINER || FoTag(e) == TABLE_COL || FoTag(e) == TABLE_TAG ) {
     allowlengh = true;
    }
    if (!allowlengh) {
    return QTextLength(QTextLength::PercentageLength,100);  
    }
    
    QString data1;
    QString data2;
    if (FoTag(e) == TABLE_COL) {
    data1 = e.attribute("column-width").replace(" ","").trimmed();
    } else {
    data1 = e.attribute("width").replace(" ","").trimmed(); 
    data2 = e.attribute("content-width").replace(" ","").trimmed();
    }
    if (data1.isEmpty() && data2.isEmpty()) {
    return QTextLength(QTextLength::PercentageLength,100);   
    }
    bool percent1 = data1.endsWith("%") == true ? true : false;
    bool percent2 = data2.endsWith("%") == true ? true : false;
    if (percent1) {
    return QTextLength(QTextLength::PercentageLength,data1.left( data1.length() - 1 ).toInt()); 
    }
    if (percent2) {
    return QTextLength(QTextLength::PercentageLength,data2.left( data2.length() - 1 ).toInt()); 
    }
    if (data1.isEmpty()) {
    return QTextLength(QTextLength::FixedLength,FopInt(data2));  
    }
    if (data2.isEmpty()) {
    return QTextLength(QTextLength::FixedLength,FopInt(data1));  
    }
    return QTextLength(QTextLength::PercentageLength,100);
}

bool Fo_Format::IsAbsoluteLayer( const QDomElement e )
{
    const qreal LargeWi = Unit(e.attribute("width","0"));
    const qreal Top = Unit(e.attribute("top","0"));
    const qreal Left = Unit(e.attribute("left","0"));
    if (!e.attribute("absolute-position").isEmpty() || !e.attribute("position").isEmpty()) {
        if (LargeWi !=0 && Top !=0 && Left !=0) {
        return true;
        }
    }
    return false;
}

FOPTAGSINT Fo_Format::FoTag( const QDomElement e )
{
   const QString fotag = e.tagName().toLower();
   if (fotag.size() < 1) {
    return TAG_NOT_KNOW;
   }
   QDomNamedNodeMap attlist = e.attributes();
   bool breackline = false;
   
   
    if ( !e.hasAttributes()  && !e.hasChildNodes() ) {
    breackline = true;
    }
    if (fotag == "fo:block" && Unit(e.attribute("space-after")) > 0.03 && attlist.count() == 1 ) {
    breackline = true;
    }
        
	 
    if (fotag == "fo:root") {
        return FIRST_LAST_TAG;
    } else if (fotag == "fo:simple-page-master") {
        return PAGE_SETUP;
    }  else if (fotag == "fo:br" || breackline ) {
        return INLINE_BR;
    }  else if (fotag == "fo:footnote" || fotag == "fo:footnote-body" ) {
        return FOOTNOTEBOTTOM;
    }  else if (fotag == "fo:block") {
        return BLOCK_TAG;
    } else if (fotag == "fo:leader") {
        return FOLEADER;
    }  else if (fotag == "fo:page-number-citation") {         //////////FOPAGENRPRINT
        return FOPAGENRCITATION;
    } else if (fotag == "fo:page-number") {
        return FOPAGENRPRINT;
    } else if (fotag == "fo:table") {
        return TABLE_TAG;
    } else if (fotag == "table-footer") {
        return TABLE_FOOTER;
    } else if (fotag == "table-header") {
        return TABLE_HEADER;
    } else if (fotag == "fo:table-body") {
        return TABLE_BODY;
    } else if (fotag == "fo:table-column") {
        return TABLE_COL;
    } else if (fotag == "fo:table-row") {
        return TABLE_ROW;
    }  else if (fotag == "fo:table-cell") {
        return TABLE_CELL;
    }  else if (fotag == "fo:block-container") {
        return BLOCK_CONTAINER;
    }  else if (fotag == "fo:inline") {
        return INLINE_STYLE;
    }    else if (fotag == "fo:inline-container") {
        return INLINE_CONTAINER;
    }  else if (fotag == "fo:character") {
        return FOCHAR;
    }   else if (fotag == "fo:float") {
        return FLOATBLOCK;
    }  else if (fotag == "fo:basic-link") {
        return LINK_DOC;
    }  else if (fotag == "fo:external-graphic") {
        return IMAGE_SRC;
    }  else if (fotag == "fo:instream-foreign-object") {
        return IMAGE_INLINE;
    }  else if (fotag == "fo:list-block") {
        return LIST_BLOCK;
    }  else if (fotag == "fo:list-item") {
        return LIST_ITEM;
    }  else if (fotag == "fo:list-item-body") {
        return LIST_BODY;
    }  else if (fotag == "fo:list-item-label") {
        return LIST_LABEL;
    }  else {
        ErrorSumm++;
        ErnoMap.insert(ErrorSumm,tr("Fo_Format::FoTag / Unregister Tagname %1 .").arg(e.tagName().toLower()));
        return TAG_NOT_KNOW;
    }
}

qreal Fo_Format::Unit( const QString datain , UNITELEMENT element )
{
    QString ctmp = datain;
    QString data = ctmp.replace(" ","").trimmed();
    /* css 2 font chunk */
    if (element == FONT && data == "smaller" || element == FONT && data == "small" )
    {
      return 9.1;
    }
    if (element == FONT && data == "large" || element == FONT && data == "larger" )
    {
      return 15.4;
    }
    if (element == FONT && data == "x-large" || element == FONT && data == "x-larger" )
    {
      return 22.4;
    }
     if (element == FONT && data == "xx-small" || element == FONT && data == "xx-smaller" )
    {
      return 7.4;
    }
     if (element == FONT && data == "medium" || element == FONT && data == "mediumer" )
    {
      return 12.2;
    }
    
    
    
    if ( data.endsWith("%"))  {
        int percentual = data.left( data.length() - 1 ).toInt();
        if (element == IMAGE) {
        return 0.;  
        } else if (element == FONT) {
        QTextCharFormat ForChar = DefaultCharFormats();
        return  ForChar.fontPointSize() / 100. * percentual;
        } else {
        ApiSession *session = ApiSession::instance();
        int doclarge = session->CurrentDocWidth;
        return  doclarge / 100. * percentual;
        }
    } else {
      return FopInt(datain);
    }
}
//////////  qreal CoolWii = ColumnLarge(constraints, int atcool )

qreal Fo_Format::TrLineMaxHight( const QDomElement e , const QVector<QTextLength> constraints )
{
    int cool = -1;
    qreal MaxHii = 0.;
    QDomElement columnElement = e.firstChildElement("fo:table-cell"); 
    while ( !columnElement.isNull() ) {
        if ( columnElement.tagName().toLower() == "fo:table-cell" ) {
            cool++;
            qreal actual = ColumnLarge(constraints,cool);
            QDomElement  firstPara = columnElement.firstChildElement("fo:block");
            QRect cellrect = BlockRect(firstPara,actual);
            const qreal hiper = cellrect.height();
            MaxHii = qMax(MaxHii,hiper);
        }
     columnElement = columnElement.nextSiblingElement("fo:table-cell"); 
    }
  return MaxHii;
}


qreal Fo_Format::ColumnLarge( const QVector<QTextLength> constraints , int atcool )
{
       for (int i = 0; i < constraints.size(); ++i) {
            if (i == atcool) {
                  QTextLength langecell = constraints.value(i);
                  if (langecell.type() == QTextLength::FixedLength) {
                  return langecell.rawValue();
                  } 
            }
        }
      return 0.;  
}

QRect  Fo_Format::Fo_Format::BlockRect( const QDomElement e , qreal largespace )
{
    QRect fun(0,0,0,0);
    if (e.isNull()) {
    return fun;
	  }
    if (FoTag(e) != BLOCK_TAG) {
	  return fun;
	  }
    QTextFrameFormat findBorder = PaintFrameFormat(e,QTextFrameFormat());
	  qreal MergeBorder = findBorder.border(); 
    
    QTextBlockFormat format = TextBlockFormFromDom(e,DefaultMargin());  /* null margin */
	  format.setAlignment(  TagAlignElement(e) ); /////TagAlignElement(e)
    
    QTextCharFormat  Charformat = GlobalCharFormat(e,DefaultCharFormats());
    
    QTextDocument *doc = new QTextDocument();
    QTextFrame  *Tframe = doc->rootFrame();
      QTextFrameFormat Ftf = Tframe->frameFormat();
      Ftf.setLeftMargin(0);
      Ftf.setBottomMargin(0);
      Ftf.setTopMargin(0);
      Ftf.setRightMargin(0);
      Ftf.setPadding ( 0);
      if (largespace > 0) {
      Ftf.setWidth ( largespace - MergeBorder);
      }
      
   bool HandleSpace = e.attribute("white-space-collapse","true") == "true" ? true : false;
	 bool NoWrapLine = e.attribute("wrap-option","wrap") == "wrap" ? false : true;
      
    if (!HandleSpace) {
	  format.setProperty(QTextFormat::BlockNonBreakableLines,1);
	  format.setNonBreakableLines(true);
	  }
	
	  if (NoWrapLine) {
	  format.setNonBreakableLines(true);
	  }
    
      Tframe->setFrameFormat(Ftf);
      QTextCursor Cur(doc);
      Cur.movePosition(QTextCursor::End);
      Cur.beginEditBlock();
      Cur.setBlockFormat(format);
	    Cur.setCharFormat(Charformat);
    
      QDomNode child = e.firstChild();
      while ( !child.isNull() ) {
    
            if ( child.isElement() ) {
               /* is elements ..................................*/
               const QDomElement childElement = child.toElement();
						   const QTextCharFormat InlineStyle = GlobalCharFormat(childElement,Charformat);
                
                if ( FoTag(childElement) == INLINE_STYLE) {
										Cur.setCharFormat(InlineStyle);
										QStringList texter = childElement.text().split("\n");
											for (int i = 0; i < texter.size(); ++i) {
												  if (HandleSpace) {
												  Cur.insertText(foptrimmed(texter.at(i),childElement.attribute("text-transform")));
													} else {
													Cur.insertText(texter.at(i));
													}
												 if (i > 0) {
													 Cur.insertText(QString(QChar::Nbsp)); /////
												 }
											}
										
										/////////Tcursor.insertText(texter.join("|")); /////
										Cur.setCharFormat(Charformat);
                } else if ( FoTag(childElement) == LINK_DOC ) {
									Cur.setCharFormat(InlineStyle);
									Cur.insertText(foptrimmed(childElement.text(),childElement.attribute("text-transform")));
									Cur.setCharFormat(Charformat);
                }  else if ( childElement.tagName().toLower() == "fo:page-number" ) {
									Cur.setCharFormat(InlineStyle);
									Cur.insertText("#Page#");
									Cur.setCharFormat(Charformat);
                } else if ( FoTag(childElement) == FOCHAR  ) {
									/* One letter format make frame float left right?  fo:page-number*/
									Cur.setCharFormat(InlineStyle);
                  Cur.insertText(childElement.attribute("character"));
                  Cur.setCharFormat(Charformat);
								}
            

           
            /* is elements ..................................*/
            } /* is element */ 
              else if (child.isCDATASection()) {
						 const QDomCDATASection ChunkPRE = child.toCDATASection();
						         QString pretag = Qt::escape(child.nodeValue());
						         pretag.prepend("<pre>");
						         pretag.append("</pre>");
						         QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(pretag);
                     Cur.insertFragment(fragment);
					 }  else if (child.isText()) {
						   QString paratext = child.nodeValue();
						   QStringList texter = paratext.split("\n");
						   if (texter.size() > 0) {
								 
								       for (int i = 0; i < texter.size(); ++i) {
												     if (HandleSpace) {
															Cur.insertText(foptrimmed(texter.at(i)),Charformat);
															} else {
															Cur.insertText(texter.at(i),Charformat);
															}
                              
												 if (i > 0) {
													 Cur.insertText(QString(QChar::Nbsp),Charformat); /////
												 }
											}
							 } else {
                Cur.insertText(QString(QChar::Nbsp),Charformat); /////
               }                   
               
           } else {
            Cur.insertText("ERROR..DOM......");
           }
           
           
           
       child = child.nextSibling();
      }
    doc->adjustSize();
    QAbstractTextDocumentLayout *Layout = doc->documentLayout();
    QRect BlockRect = Layout->frameBoundingRect(doc->rootFrame()).toRect();
    delete doc;
	  return BlockRect;
}


QDomDocument Fo_Format::DomelenentToString ( const QDomElement e , const QString msg )
{
    QDomDocument em;
    ///////QDomProcessingInstruction header = em.createProcessingInstruction( "xml", "version=\"2.0\" standalone=\"no\"" );
    /////////em.appendChild( header );
    QDomElement dummyss = em.createElement("ALERT_ROOT");
    dummyss.setAttribute ("message",msg);
    QDomElement import = em.createElement(e.tagName().toLower());
    em.appendChild( dummyss );
    dummyss.appendChild( import );
    QDomNamedNodeMap attlist = e.attributes();
    
    for (int i=0; i<attlist.count(); i++){
    QDomNode nod = attlist.item(i);
    import.setAttribute (nod.nodeName().toLower(),nod.nodeValue()); 
    }
                            QDomNode child = e.firstChild();
                               while ( !child.isNull() ) {
                                   if ( child.isElement() ) {
                                    import.appendChild(em.importNode(child,true).toElement());
                                   }
                                 child = child.nextSibling();            
                               }
    return em;
}


QPixmap Fo_Format::RenderSvg(  const QDomElement e  ,  const QString nameresource )
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
    ApiSession *session = ApiSession::instance();
    session->SvgList.insert(nameresource,streamsvg);
    QPixmap pix = RenderPixmapFromSvgByte(  streamsvg );   /* on main.h  static inline */
    
    if (pix.isNull() ) {
    QPixmap pixe(22,22);
    pixe.fill(QColor("crimson"));   /* a error red pixel */
    return pixe;
    } else {
     return pix;
    }
}



