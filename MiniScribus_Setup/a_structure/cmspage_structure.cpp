#include "cmspage_structure.h"


#include "bestqtidy.h"


CMSDoc::CMSDoc() 
{
langs = AvaiableLanguage();
epoche = 900;
start_t = 0;
stop_t = 0;
cat = 0;
topid = 0;
complete = false;
lshort.clear();
worksource = 0;
bgcolor = "transparent";
active = 0;
globaltitle = QString("Untitled Page");
QString errorStr;
int errorLine, errorColumn;
	
	
cleaner = new QTidy();  
cleaner->Init(QDir::homePath()+"/.qtity/");
	
RichDoc Gd;
QString dummy = QString("<body xmlns=\"http://www.w3.org/1999/xhtml\">%1</body>").arg(Gd.WebSource());
/////qDebug() << "### CMSDoc dummy " << dummy;
	
               QDomDocument dom;
								if (!dom.setContent(dummy)) {
									
								} 
	
	
    for (int i = 0; i < langs.size(); ++i)  {
			
			  QString name = LaTr(langs.at(i));
        lshort.append(name);
			  pager.insert(name,QDomElement());
				inxhtml.insert(LaTr(langs.at(i)),dom);
								
    }
    

}
QString  CMSDoc::xhtml( QString languagetag )
{
  QDomElement e = Page(languagetag);
  QDomElement b = e.firstChildElement("body");
  QDomNode child = b.firstChild();
  QmyDom doc1(false);
	QDomElement base = doc1.createElement("body");
  base.setAttribute("xmlns","http://www.w3.org/1999/xhtml");
                   while ( !child.isNull() ) {
                       if ( child.isElement() ) {
                        base.appendChild(doc1.importNode(child,true).toElement());
                       }
                     child = child.nextSibling();            
                   }
  doc1.appendChild( base );
  return doc1.toString(1);
}

void CMSDoc::RegisterRdoc( QString languagetag )
{
  QString hml = xhtml(languagetag);
    ////////////qDebug() << "#### hml  " << languagetag << " / " << hml;
  QMap<int,RichDoc> divlist;
  QmyDom doc1(false);
	QDomElement base = doc1.createElement("dummy");
	doc1.insertFullFragment(base,hml);
  doc1.appendChild( base );
  int pox = 0;
	QDomNode child = base.firstChild();
	while ( !child.isNull() ) {
		 if ( child.isElement() ) {
			 const QDomElement ele = child.toElement();
			 if (ele.tagName() == "div") {
				 pox++;
				 RichDoc Pin = appendItem(ele);
           
         divlist.insert(pox,Pin);
           
			 }
		 }
		child = child.nextSibling(); 
	}
  DD_Page.SetPage(languagetag,divlist);
}


RichDoc CMSDoc::appendItem( const QDomElement e )
{
  RichDoc Xdoc;
  QmyDom doc1(false);
  QMap<QString,SPics> list;
	QDomElement base = doc1.createElement("body");
	QDomNode child = e.firstChild();
                   while ( !child.isNull() ) {
                       if ( child.isElement() ) {
                        base.appendChild(doc1.importNode(child,true).toElement());
                       }
                     child = child.nextSibling();            
                   }
	doc1.appendChild( base );
                   
                   
  QString crue = doc1.toString();
   crue = crue.replace("\n","");
   crue = crue.replace("\t","");
   crue = crue.replace("\r","");
   crue = crue.replace("&#xd;","");
                   
  QString xmlo = cleaner->TidyCleanhtml(crue);
  QTextDocument *docu = new QTextDocument();
  docu->setHtml(xmlo);
  //////////qDebug() << "#### Xdoc.Register  " << xmlo;
  Xdoc.Register(docu,list,doc1.GetAtt(e,"style"));
  return Xdoc;
}


QString  CMSDoc::title( QString languagetag )
{
QDomElement e = Page(languagetag);
return e.firstChildElement("title").text();
}

QString  CMSDoc::desc( QString languagetag )
{
QDomElement e = Page(languagetag);
return e.firstChildElement("desc").text();
}

QString  CMSDoc::notes( QString languagetag )
{
QDomElement e = Page(languagetag);
return e.firstChildElement("notes").text();
}


QDomElement CMSDoc::Page( QString languagetag )
{
    const QString findLanguage = LaTr(languagetag);
    QDomElement e = pager[findLanguage];
    if (e.attribute("la","0").toLower() == findLanguage) {
     return e;
    } else {
     return QDomElement();
    }
}

QString CMSDoc::LaTr( QString languagetag )
{
languagetag = languagetag.toLower();
return languagetag.left(2);
}


CMSDoc::operator QVariant() const 
{
return QVariant::fromValue(*this);
}

void CMSDoc::openfile( const QString file ) 
{
    QByteArray inside;
    QFile f(file);
    if (f.open(QIODevice::ReadOnly)) {
    inside = f.readAll();
    f.close();
    }
    Load_Stream(inside);
}

void CMSDoc::RewriteFrominit()
{
    doc1.clear();
    lasquery.clear();
    localquery.clear();
    QDomElement base = doc1.createElement("base");
    doc1.appendChild( base );
    QDateTime timer1( QDateTime::currentDateTime() ); 
    QDomElement languagedom = doc1.createElement("langs");
    languagedom.setAttribute ("size",langs.size());
    base.appendChild( languagedom );
	  if (cat == 0) {
			topid = 0;
			active = 0;
			uripage = "/not_online";
		}
		
		uripage = QString("%1%2_%3.html").arg(preurl).arg(UriConvert(globaltitle)).arg(epoche);
		
		  if ( epoche == INDEXNR) {
       uripage = "/index";
      }
      if ( epoche == INDEXLTNR) {
       uripage = "/left";
      }
      if ( epoche == INDEXRINR) {
       uripage = "/right";
      }
		
		
    
      
			base.setAttribute ("uripage",uripage);
		  base.setAttribute ("active",active);
			base.setAttribute ("build",timer1.toTime_t());
			base.setAttribute ("nr",epoche);
			base.setAttribute ("worktext",worksource);
			base.setAttribute ("spath",DataPathServer(epoche));
			base.setAttribute ("album",album);
			base.setAttribute ("cms",2);
			base.setAttribute ("start",start_t);
			base.setAttribute ("stop",stop_t);
			base.setAttribute ("topid",topid);
			base.setAttribute ("cat",cat);
		  base.setAttribute ("bgcolor",bgcolor);
      
                for (int o = 0; o < langs.size(); ++o) {
										QString lamashine = LaTr(langs.at(o));
										QString urlo = QString("/%1%2").arg(lamashine).arg(uripage);
										QDomElement las = doc1.createElement("item");
										las.setAttribute ("humean",langs.at(o));
										las.setAttribute ("la",lamashine);
										las.setAttribute ("uri",urlo);
										las.setAttribute ("id",o);
										languagedom.appendChild( las );
									}
                  
                   for (int i = 0; i < langs.size(); ++i) {
                       const QString name = LaTr(langs.at(i));
                       
                       QDomElement sbody = doc1.createElement("body");
                       QDomDocument Current = inxhtml[name];
                       
                       QDomElement root_extern = Current.documentElement();
                       QDomNode child = root_extern.firstChild();
                               while ( !child.isNull() ) {
                                   if ( child.isElement() ) {
                                    sbody.appendChild(doc1.importNode(child,true).toElement());
                                   }
                                 child = child.nextSibling();            
                               }
                       
                       QString xtit = DD_Page.GetTag("tit_"+name).toString();
                       QString xdesc = DD_Page.GetTag("desc_"+name).toString();
                       QString xnote = DD_Page.GetTag("note_"+name).toString();
                       QString xdbword = DD_Page.GetTag("tit_"+name).toString();
                       QString xGtxt = hantext(GrabRichTxt(DD_Page.Getpage(name)));
                       QString xkeyword = KeywordGenerator(xGtxt+" "+xdesc+ " " +xtit);
                       QString xdbwo = xtit+" "+xdesc+" "+xGtxt;
                       QDomElement page = doc1.createElement("page");
									     page.setAttribute ("la",name);
									     page.setAttribute ("human",langs.at(i));
									     page.setAttribute ("id",i);
                       QDomElement t0 = doc1.insertTagValue("keyword",xkeyword);
                       QDomElement t1 = doc1.insertTagValue("title",xtit);
                       QDomElement t2 = doc1.insertTagValue("desc",xdesc);
                       QDomElement t4 = doc1.insertTagValue("notes",xnote);
                       QDomElement t3 = doc1.insertTagValue("dbword",xdbwo);
                       page.appendChild( t0 );
									     page.appendChild( t1 );
									     page.appendChild( t2 );
									     page.appendChild( t3 );
									     page.appendChild( t4 );
                       page.appendChild( sbody );
                       base.appendChild( page );
                       
											 if (name == QString("no")) {
											  /////////query.exec("create table pagememo (id INTEGER,cat INTEGER,title varchar(255),active INTEGER,start INTEGER,stop INTEGER,uri varchar(255))");
		                   liteup.clear();
											 liteup.append(QString("uri='%1'").arg(base.attribute("uripage","")));
											 liteup.append(QString("title='%1'").arg(G_Quote(xtit)));
											 liteup.append(QString("cat=%1").arg(base.attribute("cat","0")));
											 liteup.append(QString("active=%1").arg(base.attribute("active","0")));
											 liteup.append(QString("start=%1").arg(base.attribute("start","0")));
											 liteup.append(QString("stop=%1").arg(base.attribute("stop","0")));
											 }
											 
											 
                        QStringList param;
												param.clear();
												param.append(QString("'%1-%2'").arg(epoche).arg(name));
												param.append(QString("'%1'").arg(epoche));
												param.append(QString("'%1'").arg(G_Quote(xtit)));
												param.append(QString("'%1'").arg(G_Quote(xdesc)));
												param.append(QString("'%1'").arg(G_Quote(xdbwo)));
												param.append(QString("'%1'").arg(name));
												param.append(QString("%1").arg(base.attribute("cat","0")));
												param.append(QString("%1").arg(base.attribute("active","0")));
												param.append(QString("%1").arg(base.attribute("start","0")));
												param.append(QString("%1").arg(base.attribute("stop","0")));
												param.append(QString("'%1'").arg(base.attribute("uripage","")));
												param.append(QString("'%1'").arg(base.attribute("spath","")));
											  const QString onqsql = QString("REPLACE INTO %1 VALUES (%2)").arg(TSEARCH).arg(param.join(","));
                        const QString onqsql_2 = QString("REPLACE INTO %1 VALUES (%2)").arg(LITESEARCH).arg(param.join(","));
												lasquery.append(onqsql);
                        localquery.append(onqsql_2);   
                   }
                  
   tmpstream = doc1.toString(1);
}

QVariant CMSDoc::Root_Tag( const QString tag )
{
	root1 = doc1.documentElement();
	return QVariant(root1.attribute(tag,"0"));
}

void CMSDoc::Load_Stream( const QByteArray xmlextern )
    {
    doc.clear();
    QString errorStr, obname, inhalt;
    int errorLine,errorColumn;
    if (!doc.setContent(xmlextern,false, &errorStr, &errorLine, &errorColumn)) {
     /////BuildNew();
     return;
    }
    root = doc.documentElement();
    if( root.tagName() != "base" ) {
    //////BuildNew();
    return;
    } 
    worksource = root.attribute("worktext","0").toInt();
    active = root.attribute("active","0").toInt();
    topid = root.attribute("topid","0").toInt();
    cat = root.attribute("cat","0").toInt();
    epoche = root.attribute("nr","0").toUInt();
    start_t = root.attribute("start","0").toUInt();
    stop_t = root.attribute("stop","0").toUInt();
    bgcolor = root.attribute("bgcolor","transparent");
    uripage = root.attribute("uripage","/");
    spath = root.attribute("spath","transparent");
    album = root.attribute("album","0").toInt();
    QDomElement pages = root.firstChildElement("page"); 
    while (!pages.isNull()) { 
        const QString Hlang = pages.attribute("la","0").toLower();
        ////////qDebug() << "####  Hlang  " << Hlang;
        /////////qDebug() << "####  lshort  " << lshort;
        if (lshort.contains(Hlang)) {
        ////////qDebug() << "#### found Hlang  " << Hlang;
        pager.insert(Hlang,pages);
        }
        pages = pages.nextSiblingElement("page"); 
    }
    
    
    for (int i = 0; i < lshort.size(); ++i)  {
        QString name = lshort.at(i);
        RegisterRdoc(name);
        DD_Page.SetValue("tit_"+name,title(name));  ///////  toPlainText () 
        DD_Page.SetValue("desc_"+name,desc(name));
        DD_Page.SetValue("note_"+name,notes(name));
    }
    
    DD_Page.save();
    
}

void CMSDoc::savefile_small(  QString fullFileName  )
{
  DD_Page.savefile(fullFileName);  
}
void CMSDoc::openfile_small(  QString fullFileName  )
{
  DD_Page.openfile(fullFileName);
}


void CMSDoc::openfile_small(  QDomDocument doc  )
{
  DD_Page.Load_Doc(doc);
}














