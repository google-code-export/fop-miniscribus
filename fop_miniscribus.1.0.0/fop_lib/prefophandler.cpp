#include "prefophandler.h"

PageDB::PageDB( QObject * parent )
{
    friendpanel = parent;
    Pages.clear();
    
      PageDimensionName *Pra = new PageDimensionName("CH-A4",FopInt("211mm"),FopInt("297mm"),QRectF(20,20,20,20),QColor(BASECOLOR),0);
                       Pra->display("211mm x 297mm");
    
    PageDimensionName *speciale = new PageDimensionName("CH-A4-Special",898.5,1263,QRectF(20,20,20,20),QColor(BASECOLOR),0);
                       speciale->display("211mm x 297mm");   /* 898.5  1263 */
    
    PageDimensionName *Pra0 = new PageDimensionName("A4",FopInt("210mm"),FopInt("297mm"),QRectF(10,10,10,10),QColor(BASECOLOR),0);
                       Pra0->display("210mm x 297mm");
     PageDimensionName *Pra1 = new PageDimensionName("A0",FopInt("841mm"),FopInt("1189mm"),QRectF(22,22,22,22),QColor(BASECOLOR),5);
                       Pra1->display("841mm x 1189mm");
     PageDimensionName *Pra2 = new PageDimensionName("A1",FopInt("594mm"),FopInt("841mm"),QRectF(22,22,22,22),QColor(BASECOLOR),1);
                       Pra2->display("594mm x 297mm");
     PageDimensionName *Pra3 = new PageDimensionName("B0",FopInt("1030mm"),FopInt("1456mm"),QRectF(22,22,22,22),QColor(BASECOLOR),14);
                       Pra3->display("1030mm x 1456mm");
     PageDimensionName *Pra4 = new PageDimensionName("CD-Label",FopInt("128mm"),FopInt("128mm"),QRectF(22,22,22,22),QColor(BASECOLOR),0);
                       Pra4->display("128mm x 128mm");
     PageDimensionName *Pra5 = new PageDimensionName("C5-Europe",FopInt("162mm"),FopInt("229mm"),QRectF(22,22,22,22),QColor(BASECOLOR),0);
                       Pra5->display("162mm x 229mm");
     PageDimensionName *Pra6 = new PageDimensionName("PA4",FopInt("210mm"),FopInt("280mm"),QRectF(22,22,22,22),QColor(BASECOLOR),0);
                       Pra6->display("210mm x 280mm");
    PageDimensionName *Pra7 = new PageDimensionName("C5-Horizzontal",FopInt("229mm"),FopInt("162mm"),QRectF(22,22,22,22),QColor(BASECOLOR),0);
                       Pra7->display("229mm x 162mm");
     PageDimensionName *Pra8 = new PageDimensionName("C4-Horizzontal",FopInt("229mm"),FopInt("114mm"),QRectF(22,22,22,22),QColor(BASECOLOR),0);
                       Pra8->display("229mm x 114mm");
                       
    PageDimensionName *Pra10 = new PageDimensionName("C5-Vertical",FopInt("162mm"),FopInt("229mm"),QRectF(22,22,22,22),QColor(BASECOLOR),0);
                       Pra10->display("162mm x 229mm");
     PageDimensionName *Pra11 = new PageDimensionName("C4-Vertical",FopInt("114mm"),FopInt("229mm"),QRectF(22,22,22,22),QColor(BASECOLOR),0);
                       Pra11->display("114mm x 229mm");
    
    PageDimensionName *Pra20 = new PageDimensionName("Letter",FopInt("216mm"),FopInt("279mm"),QRectF(22,22,22,22),QColor(BASECOLOR),2);
                       Pra20->display("216mm x 279mm");
                       
                       /* 216 x 279 mm  216 x 356 mm 279 x 432 mm */
      PageDimensionName *Pra21 = new PageDimensionName("Legal",FopInt("216mm"),FopInt("356mm"),QRectF(22,22,22,22),QColor(BASECOLOR),3);
                       Pra21->display("216mm x 356mm");    

     PageDimensionName *Pra22 = new PageDimensionName("DLE",FopInt("110mm"),FopInt("220mm"),QRectF(22,22,22,22),QColor(BASECOLOR),26);
                       Pra22->display("110mm x 220mm");   
                       
                       
       PageDimensionName *Pra23 = new PageDimensionName("Tabloid",FopInt("279mm"),FopInt("432mm"),QRectF(22,22,22,22),QColor(BASECOLOR),29);
                       Pra23->display("279mm x 432mm");   
    
    Pages.append(Pra);
    Pages.append(Pra0);
    Pages.append(Pra1);
    Pages.append(Pra2);
    Pages.append(Pra3);
    Pages.append(Pra4);
    Pages.append(Pra5);
    Pages.append(Pra6);
    Pages.append(Pra7);
    Pages.append(Pra8);
    Pages.append(Pra10);
    Pages.append(Pra11);
    Pages.append(speciale);
    Pages.append(Pra20);
    Pages.append(Pra21);
    Pages.append(Pra22);
    Pages.append(Pra23);
    emit newdata();
}

void PageDB::AppInsert( PageDimensionName *page )
{
    Pages.append(page);
}

PageDimensionName * PageDB::page( int index )
{
    return Pages.at(index);
}


PageDB::~PageDB()
{
    Pages.clear();
}




Fop_Block::Fop_Block( int nr , QDomElement e , const QString tagname )
{
    id = nr;
    element = e;
    QDomDocument doc;
    QDomElement blox = doc.createElement(tagname);
    QDomNamedNodeMap attlist = e.attributes();
    
    for (int i=0; i<attlist.count(); i++){
        QDomNode nod = attlist.item(i);
        blox.setAttribute(nod.nodeName().toLower(),nod.nodeValue());
      }
      
       QDomNode child = e.firstChild();
       while ( !child.isNull() ) {
           if ( child.isElement() ) {
            blox.appendChild(doc.importNode(child,true).toElement());
           } else if (child.isText()) {
             blox.appendChild(doc.createTextNode(child.toText().data()));
           }
         child = child.nextSibling();            
       }
    doc.appendChild(blox);
    streamxml = doc.toString(5);
}

Fop_Block::~Fop_Block()
{
    element.clear();
}



PreFopHandler::PreFopHandler()
{
	PromtToSaveImage.clear();
    notexistFontMap.clear();
    ErrorSumm = 0;
    errorreport;
    startdir = QDir::currentPath();
    contare = 0;
}


void PreFopHandler::ImageBlockAppend( TypImageContainer tocaster  ) 
{
      /* image must save? */
      QMapIterator<QString,QVariant> i(tocaster);
         while (i.hasNext()) {
             i.next();
             contare++;
             PromtToSaveImage.insert(i.key(),i.value());
         } 
}


QVariant PreFopHandler::ResourceOnSaveImage( const QString urlimage  ) 
{
      QVariant nullimage;
      QMapIterator<QString,QVariant> i(PromtToSaveImage);
         while (i.hasNext()) {
             i.next();
             if ( i.key() == urlimage ) {
              return i.value();
             }
         } 
    return nullimage;
}


bool PreFopHandler::BuildSvgInlineonDom( const QString urlrefimage , QDomElement e , QDomDocument doc  ) 
{
    bool buildaction = false;
    QByteArray imagestream = ResourceOnSaveImage(urlrefimage).toByteArray();
    QString errorStr;
    int errorLine, errorColumn, position;
    QDomDocument img;
    if (!img.setContent(imagestream,false, &errorStr, &errorLine, &errorColumn)) {
    return false;
    } 
    QDomElement root_extern = img.documentElement(); 
    root_extern.setTagName("svg:svg");
    QDomNamedNodeMap attlist = root_extern.attributes();   /* copy all attributes from other doc */
    /////////////qDebug() << "### BuildSvgInlineonDom -> " << root_extern.tagName().toLower();
    QDomNode newnode = root_extern.firstChild();
    QDomNode treallsub = doc.importNode(newnode,true);
    QDomElement svg = doc.createElement("svg:svg");
      for (int i=0; i<attlist.count(); i++){
        QDomNode nod = attlist.item(i);
        svg.setAttribute(nod.nodeName().toLower(),nod.nodeValue());
      }
    svg.setAttribute ("xmlns","http://www.w3.org/2000/svg");
    svg.setAttribute ("version","1.2");
    svg.setAttribute ("baseProfile","tiny");
      
       QDomNode child = root_extern.firstChild();
       while ( !child.isNull() ) {
           if ( child.isElement() ) {
            svg.appendChild(doc.importNode(child,true).toElement());
           }
         child = child.nextSibling();            
       }
    
   e.appendChild(svg);
   return true;
}




bool PreFopHandler::SaveImageExterPath( const QString urlrefimage , const QString fulldirlocalfile ) 
{
  bool savedaction = false;
         QByteArray imagestream = ResourceOnSaveImage(urlrefimage).toByteArray();
           if (imagestream.size() > 0)  {
                            QFile f(fulldirlocalfile);
                            if ( f.open( QIODevice::WriteOnly ) ) 
                            {
                                f.write(imagestream);
                                f.close();
                                ///////////////////qDebug() << "### save yes -> " << fulldirlocalfile; 
                                return true;
                            }
                            
            }
            
 return savedaction;
}

QTextBlockFormat PreFopHandler::TextBlockFormFromDom( const QDomElement e , QTextBlockFormat pf  ) 
{
    /* pf = parent format */
    
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
                 
                 if ( nod.nodeName().toLower().contains("-before") || nod.nodeName().toLower() =="margin-top" ) {   /* top */
                    pf.setTopMargin(Unit(nod.nodeValue()));
                 }
                 
                 if ( nod.nodeName().toLower().contains("-after") || nod.nodeName().toLower() =="margin-bottom" ) {    /* bottom */
                    pf.setBottomMargin(Unit(nod.nodeValue()));
                 }
                 
                 if ( nod.nodeName().toLower().contains("-start") || nod.nodeName().toLower() =="margin-right" ) {  /* right */
                    pf.setRightMargin(Unit(nod.nodeValue()));
                 }
                 
                 if ( nod.nodeName().toLower().contains("-end") || nod.nodeName().toLower() =="margin-left" ) {   /* left */
                    pf.setLeftMargin(Unit(nod.nodeValue()));
                 }
                 if ( nod.nodeName().toLower().contains("start-indent") ) {   /* indent */
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




QTextCharFormat PreFopHandler::GlobalCharFormat( const QDomElement e , QTextCharFormat f ) 
{
      QDomNamedNodeMap attlist = e.attributes();
      QStringList option;
                  option.clear();
    
      int fontstrech = e.attribute ("font-stretch",QString("0")).toInt();
     
    
      QString idref="id-NOTSET";  /* id identificativ if having? */
    
        for (int i=0; i<attlist.count(); i++){
             QDomNode nod = attlist.item(i);
               if (_PARSE_DEBUG_FOP_ == 1) {
               option.append(nod.nodeName().toLower()+"="+nod.nodeValue()+"\n");
               }
            
               const QString valore = nod.nodeValue().toLower();
            
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
                       userfont.setKerning(true);
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
             }
             
             
             ////////if (nod.nodeName().toLower() == "font-style" or nod.nodeName().toLower() == "font-weight") {
                  if (valore.contains("bold")) {
                  f.setFontWeight(QFont::Bold); 
                  } 
                  if (valore.contains("italic")) {
                  f.setFontItalic(true);
                  }
                  if (valore.contains("underline")) {
                  f.setUnderlineStyle ( QTextCharFormat::SingleUnderline );
                  }
                  if (valore.contains("oblique")) {
                  f.setFontItalic(true);
                  }
                  if (valore.contains("overline")) {
                  f.setFontOverline(true);
                  }                      
                  
             ////////}
             
    
             
           
             
             
        }
        
        return f;
}



QTextCharFormat PreFopHandler::NewMixedBlockFormat( const QDomElement e  ) 
{
     QDomNamedNodeMap attlist = e.attributes();
     QTextCharFormat pf;
                     pf.setFontFixedPitch(false);
    
            for (int i=0; i<attlist.count(); i++){
                QDomNode nod = attlist.item(i);
                 
                   if (nod.nodeName().toLower() == "background-color") {
                   pf.setBackground (QBrush(QColor(nod.nodeValue())) ); 
                   }
               
                   if (nod.nodeName().toLower() == "color") {
                     pf.setForeground(QBrush(QColor(nod.nodeValue())));
                    }
                 
                 
                
            }
}









qreal PreFopHandler::Unit( const QString datain )
{
  QString ctmp = datain;
  const QString data = ctmp.replace(" ","").trimmed();
  //////////qDebug() << "### request unit data->" << datain << " size->" << datain.size();   
  #define MM_TO_POINT(mm) ((mm)*2.83465058)
  #define CM_TO_POINT(cm) ((cm)*28.3465058)
  #define DM_TO_POINT(dm) ((dm)*283.465058)
  #define INCH_TO_POINT(inch) ((inch)*72.0)
  #define PI_TO_POINT(pi) ((pi)*12)
  #define DD_TO_POINT(dd) ((dd)*154.08124)
  #define CC_TO_POINT(cc) ((cc)*12.840103)
  qreal points = 0;
  if (data.size() < 1) {
  return points;
  }
  if (datain == "0") {
  return points;
  }
  
  
 
  if ( data.endsWith( "pt" ) || data.endsWith( "px" ) ) {
    points = data.left( data.length() - 2 ).toDouble();
  } else if ( data.endsWith( "cm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = CM_TO_POINT( value );
  } else if ( data.endsWith( "em" ) ) {
    points = data.left( data.length() - 2 ).toDouble();
  } else if ( data.endsWith( "mm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = MM_TO_POINT( value );
  } else if ( data.endsWith( "dm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = DM_TO_POINT( value );
  } else if ( data.endsWith( "in" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = INCH_TO_POINT( value );
  } else if ( data.endsWith( "inch" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = INCH_TO_POINT( value );
  } else if ( data.endsWith( "pi" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = PI_TO_POINT( value );
  } else if ( data.endsWith( "dd" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = DD_TO_POINT( value );
  } else if ( data.endsWith( "cc" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = CC_TO_POINT( value );
  } else {
    points = 0;
    ErrorSumm++;
    ErnoMap.insert(ErrorSumm,tr("Fop_Handler::Unit / Unknown Unit \"%1\" ").arg(datain));
  }
  return points;
} 





QTextBlockFormat  PreFopHandler::PaddingToNext( qreal paddingmargin , QTextBlockFormat rootformats ) 
{
    rootformats.setBottomMargin(paddingmargin); 
    rootformats.setTopMargin(paddingmargin);
    rootformats.setRightMargin(paddingmargin);
    rootformats.setLeftMargin(paddingmargin);
    return rootformats;
}





/*

if (nod.nodeName().toLower() == "external-destination" || nod.nodeName().toLower() == "internal-destination") {
                 f.setAnchorHref(nod.nodeValue());
                 f.setAnchor(true);
                 f.setForeground ( QBrush ( QColor ("#ff0000") ) );
             }


*/





void PreFopHandler::PaintCharFormat( QDomElement e , QTextCharFormat bf  )
{
      if (bf.foreground().color().name().size() > 0 ) {
      e.setAttribute ("color",bf.foreground().color().name()); 
      }
      QFont userfont = bf.font(); 
      int stre = userfont.stretch();
      e.setAttribute ("font-family",userfont.family());
            if (stre =!100) {
            e.setAttribute ("font-stretch",stre);
            }
            e.setAttribute ("font-size",QString("%1pt").arg(userfont.pointSize()));
            
            
            
            
            ///////////qDebug() << "### userfont.bold() " << userfont.bold();
            
      
      QStringList fontvario;
            fontvario.clear();
            
            ////////////qDebug() << "### bf.fontWeight() " << bf.fontWeight();
            //////////qDebug() << "### bf.fontUnderline() " << bf.fontUnderline();
            /////////////qDebug() << "### bf.fontItalic() " << bf.fontItalic();
            
            if (bf.fontWeight() > 51) {
            fontvario.append("bold");
            }
            if (bf.fontItalic()) {
            fontvario.append("italic");
            }
            if (bf.fontStrikeOut() ) {
            fontvario.append("oblique");
            }
            
            if (bf.fontUnderline() ) {
            fontvario.append("underline");
            }
            
            
            ///////////qDebug() << "### fontvario.size() " << fontvario.size();
            
            if (fontvario.size() > 0) {
            e.setAttribute ("font-style",fontvario.join(" "));
            }
            
            
            
}

void PreFopHandler::PaintFopBlockFormat( QDomElement e , QTextBlockFormat bf )
{

      if ( bf.alignment() == Qt::AlignLeft) {
      e.setAttribute ("text-align","left");    
      }
      if ( bf.alignment() == Qt::AlignRight) {
      e.setAttribute ("text-align","right");    
      }
      if ( bf.alignment() == Qt::AlignHCenter) {
      e.setAttribute ("text-align","center");    
      }
      if ( bf.alignment() == Qt::AlignJustify) {
      e.setAttribute ("text-align","justify");  
      }
      if ( bf.alignment() == Qt::AlignTop) {
      e.setAttribute ("text-align","left");   
      }
      if ( bf.alignment() == Qt::AlignBottom) {
      e.setAttribute ("text-align","left");    
      }
      if ( bf.alignment() == Qt::AlignVCenter) {
      e.setAttribute ("text-align","center");    
      }
      if ( bf.alignment() == Qt::AlignCenter) {
      e.setAttribute ("text-align","center");   
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
      if (bf.foreground().color().name().size() > 0 ) {
      e.setAttribute ("color",bf.foreground().color().name()); 
      }
      if (bf.background().color().name() !="#000000" ) {
      e.setAttribute ("background-color",bf.background().color().name()); 
      }

}




Qt::PenStyle PreFopHandler::StyleBorder( const QDomElement e ) 
{
    Qt::PenStyle smaks = Qt::SolidLine;
    const QString borderpaint = e.attribute ("border-style","solid").toLower();
    if (borderpaint.size() < 0) {
    return smaks;
    } else {
        /* avaiable penn style from border .... */
        if (borderpaint == "solid") {
        return Qt::SolidLine;
        }
        if (borderpaint == "dotted") {
        return Qt::DotLine;
        }
        if (borderpaint == "dashed") {
        return Qt::DashDotLine;
        }  
    }
    
   return smaks; 
}

qreal PreFopHandler::Get_Cell_Width( QTextTableFormat TableFormat , int position )  
{
    qreal notfound = 0;
    QVector<QTextLength> constraints = TableFormat.columnWidthConstraints();
        for (int i = 0; i < constraints.size(); ++i) {
            if (i == position) {
                QTextLength langecell = constraints.value(i);
                 if (langecell.type() == QTextLength::FixedLength) {
                  return langecell.rawValue();
                 }                     
                                     
            }
        }
   return notfound;
}


qreal PreFopHandler::TakeOneBorder( const QDomElement e )
{
    qreal Boleft = Unit(e.attribute ("border-start-width",QString("0")));
    qreal Botop = Unit(e.attribute ("border-before-width",QString("0")));
    qreal Bobotto = Unit(e.attribute ("border-bottom-width",QString("0")));
    qreal Bobright = Unit(e.attribute ("border-end-width",QString("0")));
    qreal first = qMax (Boleft,Bobright);
    qreal second = qMax (Bobotto,Botop);
    qreal nordsud = qMax (first,second);
    qreal single = Unit(e.attribute ("border-width",QString("0")));
    return qMax (single,nordsud);
}



qreal PreFopHandler::TakeOnePadding( const QDomElement e )
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







/* Paint special border on cell and border hack border html td cell from fop !!!! */
QPixmap PreFopHandler::PaintTableCellPixmap( QRectF r , QColor bg , QColor ma , qreal borderDicks , Qt::PenStyle styls ) 
{
    
    /* QRectF ( qreal x, qreal y, qreal width, qreal height )   */
    
    if (bg == ma) {
    bg.setAlphaF ( 0 ); 
    ma.setAlphaF ( 0 ); 
    }
    
    QPixmap m_tile = QPixmap(r.width() * 4 ,r.height() * 4);
    m_tile.fill(bg);
    QPainter pt(&m_tile);
    pt.setBrush(QBrush(bg));
    pt.setPen(QPen(QBrush(ma),borderDicks,styls,Qt::RoundCap,Qt::RoundJoin));
    if (r.y() !=0) {
    pt.drawRect(QRectF((borderDicks / 2) + r.y(),borderDicks / 2,r.width() - (borderDicks / 2),r.height() - (borderDicks / 2)));
    } else {
    pt.drawRect(QRectF(borderDicks / 2,borderDicks / 2,r.width() - (borderDicks / 2),r.height() - (borderDicks / 2)));
    }
    
    qreal internalborder = r.x();
    if (internalborder !=0) {
        QColor background = QColor(bg);
        background.setAlphaF ( 1 ); 
        pt.setBrush(QBrush(background));
        pt.setPen(QPen(QBrush(ma),internalborder,styls,Qt::SquareCap,Qt::BevelJoin));
        pt.drawRect(QRectF(internalborder,internalborder,r.width() - internalborder ,r.height() - internalborder ));
    }
    pt.end();
    return m_tile;
}






void PreFopHandler::ReportError()
{
errorreport++;
}




PreFopHandler::FOPTAGSINT PreFopHandler::FoTag( const QDomElement e )
{
   const QString fotag = e.tagName().toLower();
    
   if (fotag.size() < 1) {
    return TAG_NOT_KNOW;
   }
    
    /////qDebug() << "### FoTag " << fotag;
    
   
    if (fotag == "fo:root") {
        return FIRST_LAST_TAG;
    } else if (fotag == "fo:simple-page-master") {
        return PAGE_SETUP;
    } else if (fotag == "fo:block") {
        return BLOCK_TAG;
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
    }   else if (fotag == "fo:character") {
        return FOCHAR;
    }   else if (fotag == "fo:basic-link") {
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
    } else {
        ErrorSumm++;
        ErnoMap.insert(ErrorSumm,tr("Fop_Handler::FoTag / Unregister Tagname %1.").arg(e.tagName().toLower()));
        return TAG_NOT_KNOW;
    }
}



/* set all margin to zero qt4 send 12 top 12 bottom by default */
QTextBlockFormat  PreFopHandler::DefaultMargin( QTextBlockFormat rootformats ) 
{
    rootformats.setBottomMargin(0); 
    rootformats.setTopMargin(0);
    rootformats.setRightMargin(0);
    rootformats.setLeftMargin(0);
    return rootformats;
}




/* set all margin to zero qt4 send 12 top 12 bottom by default */
void  PreFopHandler::TableBlockMargin( QDomElement appender , const QString name  ) 
{
   appender.setAttribute ("margin-top",0);
   appender.setAttribute ("margin-bottom",0);
   appender.setAttribute ("margin-left",0);
   appender.setAttribute ("master-right",0);
   appender.setAttribute ("itype",name);
}















