#ifndef SWISSLANG_STRUCTURE_H
#define SWISSLANG_STRUCTURE_H


#include "QmyDom.h"
#include "qdocxhtml.h"
#include "richdoc_structure.h"
#include "pics_structure.h"


class SwissLang
{
    public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 1 };
    SwissLang() {
		langs.clear();
    langs << "Italiano" << "Deutsch" << "Francais" << "English" << "Noteblock";
		lasmall.clear();
			for (int i = 0; i < langs.size(); ++i)  {
				lasmall.append(LaTr(langs.at(i)));
			}
		BuildNew();
    }
		operator QVariant() const {
    return QVariant::fromValue(*this);
    }
		SwissLang& operator=( SwissLang& d )
    {
      xml = d.xml;
			Load_Stream(xml);
			save();
    }
		SwissLang operator=( SwissLang d )
    {
      xml = d.xml;
			Load_Stream(xml);
			save();
    }
		void Load_Stream( const QByteArray xmlextern )
    {
    doc.clear();
    QString errorStr, obname, inhalt;
    int errorLine;
    int errorColumn;
     if (!doc.setContent(xmlextern,true, &errorStr, &errorLine, &errorColumn)) {
     BuildNew();
     return;
     }
		  Load_Doc(doc); 
	 }
		 void Load_Doc( QDomDocument newformat )
    {
		 doc = newformat;
     root = doc.documentElement();
     if( root.tagName() != "setting" ) {
      BuildNew();
      return;
     } 
            QDomNode n = root.firstChild();
            while( !n.isNull() )
            {
                QDomElement e = n.toElement();
                if( !e.isNull() )
                {
                    if (e.tagName().size() > 3) {
                     seTTing[QString(e.tagName())] = e.text();
                    }     
                 n = n.nextSibling();
                } 
            }
						QVariant value = GetTag("page_it");
						if (value.isNull()) {
							BuildNew();
						} else {
							     for (int i = 0; i < langs.size(); ++i)  {
										 QVariant dd = GetTag("page_"+LaTr(langs.at(i)));
										   if (dd.isNull()) {
												 seTTing["page_"+LaTr(langs.at(i))] = value;
											 }
									 }
							
						}
		}
		void BuildNew()
		{
		doc.clear();
			
			QDir homen(QDir::homePath());
			
		QDomProcessingInstruction header = doc.createProcessingInstruction( "xml",QString("version=\"1.0\" encoding=\"utf-8\""));
		doc.appendChild( header );
		root = doc.createElement("setting");
		doc.appendChild(root);
			 for (int i = 0; i < langs.size(); ++i)  {
				      RichDoc sample;
				      QString txt = QString("<p>Your text %1</p>").arg(langs.at(i));
			        sample.html = txt.toAscii();
				      sample.style = "background-color:#ffffff;";
				      const QString one = SaveRichDoc(sample);
							QStringList divs;
													divs.clear();
				                  divs.append(one);
				      seTTing["page_"+LaTr(langs.at(i))] = divs.join("|");
				      seTTing["tit_"+LaTr(langs.at(i))] = QString("Page Title");
				      seTTing["desc_"+LaTr(langs.at(i))] = QString("Page description to Google");
				      seTTing["note_"+LaTr(langs.at(i))] = QString("Note");
				      seTTing["autor_"+LaTr(langs.at(i))] = homen.dirName();
			 }
		}
		void openfile(  QString fullFileName  )
		{
			  QByteArray inside;
				QFile f(fullFileName);
				if (f.open(QIODevice::ReadOnly)) {
				inside = f.readAll();
				f.close();
				}
				Load_Stream(inside);
		}
		QString save()
		{
			if (seTTing.size() < 5) {
      return QString(".");
      }
			doc.clear();
			QDomProcessingInstruction header = doc.createProcessingInstruction( "xml",QString("version=\"1.0\" encoding=\"utf-8\""));
		  doc.appendChild( header );
		  root = doc.createElement("setting");
			doc.appendChild(root);
			
			
			               QMapIterator<QString, QVariant> i(seTTing);
                         while (i.hasNext()) {
                             i.next();
                                QString tagname = i.key();
													      if (!tagname.endsWith("_")) {
                                QDomElement primo = doc.createElement(tagname);
																const QString tval = QString(i.value().toString());
																QDomText texte = doc.createTextNode( tval );
																primo.appendChild(texte);
                                root.appendChild(primo);
																}
                         }
			xml = doc.toByteArray(1); 
			return doc.toString(1);
		}
		bool savefile(  QString fullFileName  )
		{
			 ///////qDebug() << "savefile ................" << fullFileName << ".......................................";
			
		    QTextCodec *codecx;
        codecx = QTextCodec::codecForMib(106);
        QFile f( fullFileName );
        if ( f.open( QFile::WriteOnly | QFile::Text ) )
        {
        QTextStream sw( &f );
        sw.setCodec(codecx);
        sw << save();
        f.close();
        return true;
        }
        return false;
		}
		
		void SetValue( const QString tagname , QVariant value )
		{
				seTTing.insert(tagname,value);
		}
		void SetPage( const QString lang , QStringList e  )
		{
				seTTing.insert("page_"+LaTr(lang),e.join("|"));
		}
		void SetPage( const QString lang , QMap<int,RichDoc> item , bool clone = false )
		{
			QMapIterator<int,RichDoc> i(item);
      QStringList dd;
      dd.clear();
         while (i.hasNext()) {
             i.next();
             RichDoc e = i.value();
					   const QString Hddclose = SaveRichDoc(e);
             dd.append(Hddclose);
         } 
				 if (clone) {
						 for (int o = 0; o < langs.size(); ++o)  {
							SetPage(LaTr(langs.at(o)),dd);
						 }
			   } else {
					 SetPage(lang,dd);
				 }
		}
		
		/* clone a page */
		void clonePage( const QString lang , QStringList e  )
		{
			for (int i = 0; i < langs.size(); ++i)  {
				seTTing.insert("page_"+LaTr(langs.at(i)),e.join("|"));
			}
		}
		/* clone one item */
		void cloneitem(  QString tagname , QVariant value )
		{
			if (!tagname.contains("_")) {
				return;
			}
			QStringList list = tagname.split("_");
			const QString startname = list.at(0)+"_";
			
			if (!lasmall.contains(list.at(1))) {
				return;
			}
			
			for (int i = 0; i < langs.size(); ++i)  {
				SetValue(startname+LaTr(langs.at(i)),value);
			}
			
		}
		QVariant GetTag( const QString tag )
		{
			 return seTTing[tag]; 
		}
		QStringList Getpage( const QString lang )
		{
			 return seTTing["page_"+LaTr(lang)].toString().split("|"); 
		}
		QString LaTr( QString languagetag )
		{
		languagetag = languagetag.toLower();
		return languagetag.left(2);
		}
		
		QStringList lasmall;
		
		
		QDomElement root;
    QDomDocument doc;
		QStringList langs;
		QMap<QString, QVariant> seTTing;
		QByteArray xml; 
		
};


Q_DECLARE_METATYPE(SwissLang);





static inline QStringList AvaiableLanguage()
{
   SwissLang pak;
   return pak.langs;
}




//
#endif // SWISSLANG_STRUCTURE_H

