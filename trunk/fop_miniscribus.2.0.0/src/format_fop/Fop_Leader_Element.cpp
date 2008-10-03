#include "Fop_Leader_Element.h"


#include "BasicFoConfig.h"



FopLeader::FopLeader()
{
    format = QTextFrameFormat();
    display << "visible" << "hidden" << "collapse" << "inherit"; 
    pattern << "space" << "rule" << "dots" << "use-content" << "inherit";
    visibility = 3;
    leaderpattern = 4;
	  ELoriginal = QByteArray("error");
    bg = QColor (0,0,0,0);
	  co = QColor (255,255,255,0);
    area = QRectF(0,0,5,1); /* height font size  default */
    format.setWidth ( area.width() );
    format.setHeight ( area.height() );
    format.setBackground(QBrush(bg));
    format.setLeftMargin(0);
	  format.setBottomMargin(0);
	  format.setTopMargin(0);
		format.setRightMargin(0);
	  format.setPadding(5);
    format.setBorder(0);
	  name = "foleader";
}
QString FopLeader::Name() const
{
	QDateTime timer1( QDateTime::currentDateTime() );
	const QString TimestampsMs = QString("%1-%2-space").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
	QString j_op( QString::number( qrand() % 10000 ) );
	return Imagename(QString("/foleader/%1/%2/%3x%4").arg(TimestampsMs).arg(j_op).arg(area.size().width()).arg(area.size().height()));
}

QPixmap FopLeader::DummyImage()
{
			QPixmap pError = QPixmap(area.size().width(),area.size().height());
	    qreal hic = qBound (0.8,area.size().height() / 3,2.0);
	    QLineF linear(QPointF(0,area.size().height() - hic),QPointF(area.size().width(),area.size().height() - hic));
	    pError.fill(QColor(Qt::transparent));
	  
				
				
			if (!makeimage()) {
				/* not hidden && no use-content */
				QPainter p( &pError );
				if (co.alpha() < 10) {
				co = QColor(Qt::black);
				} 
				///// pattern ==== << "space" << "rule" << "dots" << "use-content" << "inherit";
				if ( leaderpattern == 11 || leaderpattern == 22 ) {
					/* fill color */
					p.setPen(Qt::NoPen);
					p.setBrush( QBrush(co) );
					p.drawRect(area);
				} else if ( leaderpattern == 2 ) {
					/* dots */
					
					///////////////qDebug() << "### paint dots ........................................ " << leaderpattern;
					p.setPen(Qt::NoPen);
					/////p.setBrush( QBrush(Qt::transparent) );
					//////p.drawRect(area);
					
					p.setPen(QPen(QBrush(co),hic,Qt::DotLine,Qt::RoundCap));
					p.drawLine(linear);
					
				}  else if (  leaderpattern == 4 || leaderpattern == 1 ) {
					/* dots */
					p.setPen(QPen(QBrush(co),area.size().height(),Qt::SolidLine,Qt::SquareCap));
					p.drawLine(linear);
				}
				
			}
			
			///////qDebug() << "### paint leaderpattern........................................ " << leaderpattern;
			
			
			return pError;
}

void FopLeader::read( const QDomElement e  , qreal pagewidth)  /* to be % width space */
{
    QDomNamedNodeMap attlist = e.attributes();
    qreal border = 0;
    QStringList WI_rec;
    QStringList HI_rec;
    WI_rec << "content-width" << "width" << "leader-length";
    HI_rec << "content-height" << "height" << "rule-thickness";
    
    for (int i=0; i<attlist.count(); i++)  {
             QDomNode nod = attlist.item(i);
             const QString valore = nod.nodeValue().toLower();
      
          if (nod.nodeName().toLower() == "background-color") {
               bg = ColorFromFoString(valore);
               format.setBackground(QBrush(bg));
          }
					
					if (nod.nodeName().toLower() == "color") {
               co = ColorFromFoString(valore);
               //////format.setBackground(QBrush(bg));
          }
					
					
          if (nod.nodeName().toLower().startsWith("border-") && nod.nodeName().toLower().endsWith("-width") ) {
                if (valore.endsWith("%")) {
                border = 0.5;
                } else {
                border = qMax(border,FopInt(valore));
                }
          }
          /* width */
          if (  WI_rec.contains( nod.nodeName().toLower() ) ) {
                 if (valore.endsWith("%")) {
									 int percentual = valore.left( valore.length() - 1 ).toInt();
									 area.setWidth(pagewidth / 100. * percentual);
									 format.setWidth ( area.width() );
								 } else {
                    if (FopInt(valore) > 0) {
                     area.setWidth(FopInt(valore));
										 format.setWidth ( area.width() );
                    }
                 }
          }
					
					 /* height */
          if (  HI_rec.contains( nod.nodeName().toLower() ) ) {
                 if (valore.endsWith("%")) {
									 int percentual = valore.left( valore.length() - 1 ).toInt();
									 area.setHeight(pagewidth / 100. * percentual);
									 format.setHeight ( area.height() );
								 } else {
                    if (FopInt(valore) > 0) {
                     area.setHeight (FopInt(valore));
										 format.setHeight ( area.height() );
                    }
                 }
          }
					
					
					
					
          if ( nod.nodeName().toLower() == "space-before" || 
                      nod.nodeName().toLower() == "space-before.optimum" ||
                      nod.nodeName().toLower() == "margin-top"  ) {   /* top */
                     if (FopInt(nod.nodeValue()) > 0) {
                     format.setTopMargin(FopInt(nod.nodeValue()));
                     }
           }
           
          if ( nod.nodeName().toLower() == "space-after" || 
                      nod.nodeName().toLower() == "space-after.optimum" ||
                      nod.nodeName().toLower() == "margin-bottom"  ) {   /* top */
                     if (FopInt(nod.nodeValue()) > 0) {
                     format.setBottomMargin(FopInt(nod.nodeValue()));
                     }
           }
           
             
           
          if ( nod.nodeName().toLower() == "space-start" || 
               nod.nodeName().toLower() == "end-indent" ||
               nod.nodeName().toLower() == "margin-right" ) {  /* right */
                    format.setRightMargin(FopInt(nod.nodeValue()));
          }     
          if ( nod.nodeName().toLower() == "space-end" || 
              nod.nodeName().toLower() == "start-indent" || 
              nod.nodeName().toLower() == "margin-left" ) {   /* left */
              format.setLeftMargin(FopInt(nod.nodeValue()));
          }
          if (display.contains(nod.nodeValue())) {
            visibility = display.indexOf(nod.nodeValue());
          }
          
          if (pattern.contains(nod.nodeValue())) {
            leaderpattern = pattern.indexOf(nod.nodeValue());
          }
    }  /* loop attributes */
		
		if ( leaderpattern == 3 ) {
			area = QRectF(0,0,1,1);
		}
		
		///////////qDebug() << "### leaderpattern -------  " << leaderpattern;
		
		
		ELoriginal = SaveElement(e);
		///////format.setProperty(LeaderNummer,this);
}

void FopLeader::RestoreOn( QDomElement appender )
{
	QDomDocument doc;
	
	if (ELoriginal.size() > 8 && doc.setContent(ELoriginal,false) ) {
		    QDomElement root_extern = doc.documentElement(); 
				QDomNamedNodeMap alist = root_extern.attributes();
		
			          QDomElement leader = appender.ownerDocument().createElement("fo:leader");
								for (int i=0; i<alist.count(); i++){
									QDomNode nod = alist.item(i);
									leader.setAttribute(nod.nodeName().toLower(),nod.nodeValue());
								}
								
								QDomNode child = root_extern.firstChild();
										while ( !child.isNull() ) {
											
											if ( child.isElement() ) {
											leader.appendChild(appender.ownerDocument().importNode(child,true).toElement());
											}
											
										child = child.nextSibling();            
										}
				appender.appendChild(leader);
										
	} else {
		
		/* rebuild it but not editable! now  */
		/*
		<fo:leader leader-pattern="rule"
                   rule-thickness="10cm"
                   leader-length="6cm"
                   space-before.optimum="12pt"
                   space-after.optimum="12pt"
                   start-indent="1.5cm"
                   end-indent="2cm"
                   background-color="blue"
                   color="yellow"/>
		*/
	}
	
	
}

QByteArray FopLeader::SaveElement( const QDomElement e )
{
    QDomDocument em;
    QDomElement import = em.createElement(e.tagName().toLower());
    em.appendChild( import );
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
															 
    return em.toByteArray(0);
}



QColor FopLeader::ColorFromFoString( QString focolor )
{
	QString fcol = focolor.trimmed().toLower();
	if (fcol.size() < 2) {
		return QColor(Qt::transparent);
	}
	if (fcol.startsWith("rgb(")) {
		QString grep = fcol.mid(4,fcol.size() - 5);
						grep = grep.replace(")","");
		QStringList v = grep.split(",");
		if (v.size() == 3) {
    const int R1 = v.at(0).toInt();
    const int R2 = v.at(1).toInt();
    const int R3 = v.at(2).toInt();
		return QColor(R1,R2,R3,255);
		} else if(v.size() >= 4) {
    const int Rt1 = v.at(0).toInt();
    const int Rt2 = v.at(1).toInt();
    const int Rt3 = v.at(2).toInt();
    const int Ralph = v.at(3).toInt();
		QColor cctra(Rt1,Rt2,Rt3);
		cctra.setAlpha(Ralph);
		return cctra;
		} else {
    return QColor(Qt::darkBlue); 
    }
	} else if (fcol.startsWith("#")) {
		return QColor(fcol);
	} else {
      QColor fasi(Imagename(fcol));
      if (fasi.isValid () ) {
         return fasi;
      } else {
         return QColor(Qt::black); 
      }
	}
	return QColor(Qt::transparent);
}


