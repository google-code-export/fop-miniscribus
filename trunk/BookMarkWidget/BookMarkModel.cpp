#include "BookMarkModel.h"


 




BookMarkModelRead::BookMarkModelRead( const QString file  )
 :   dev(new StreamMark),foundTree(false),treeLoop(-1)
{
   //////////qDebug() << "### BookMarkModelRead load from file   " << file;
    bold_base_font = qApp->font();
    bold_base_font.setBold ( true );
    internalLinkFound.clear();
    flags = Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    model = new QStandardItemModel();
    setHeader();
	dev->LoadFile(file);
    if (dev->isValid()) {
     doc = dev->Dom();
     read();
    }
}

void BookMarkModelRead::openStream( const QByteArray xml )
{
    internalLinkFound.clear();
    dev = new StreamMark;
    dev->device()->write(xml);
    if (dev->isValid()) {
     internalLinkFound.clear();
     doc = dev->Dom();
     read();
    } else {
        qWarning() << "XML error load on:" << xml;
    }
}





void BookMarkModelRead::setHeader()
{
    const int cools = 3;
    QString Htext;
    for (int i = 0; i < cools; ++i) {
          if (i == 0) {
             Htext = tr("Label Title"); 
          } else if (i == 1 ) {
             Htext = tr("Internal-Destination");
          } else if (i == 2 ) {
             Htext = tr("Level");
          }
         QStandardItem *Hitem = new QStandardItem(Htext);
         Hitem->setData(bold_base_font,Qt::FontRole);
         Hitem->setFlags( Qt::NoItemFlags );
         model->setHorizontalHeaderItem(i,Hitem);
    }
}



void BookMarkModelRead::openRootBookmark( const QDomElement e )
{
    if (e.tagName() !="fo:bookmark-tree")
    {
        return;
    }
    foundTree = true;
    model = new QStandardItemModel();
    internalLinkFound.clear();
    setHeader();
    //////////////qDebug() << "### openRootBookmark -> " << e.tagName();
    QDomElement child = e.firstChildElement("fo:bookmark");
           while (!child.isNull()) {
                model->invisibleRootItem()->appendRow(Compose(child,0));
                child = child.nextSiblingElement("fo:bookmark");
           }
}


QList<QStandardItem *> BookMarkModelRead::newLine( int level )
{
    QList<QStandardItem *> diritto;
    treeLoop++;
    const int SessionID = qrand () % 1200;
    QIcon icob = createBookColorIcon( Qt::darkRed );
    const int UniqueID = level + treeLoop + SessionID;
    
    
                    QStandardItem *item0 = new QStandardItem(tr("Link Text"));
                    item0->setData(UniqueID,Qt::UserRole);
                    item0->setData(bold_base_font,Qt::FontRole);
                    item0->setIcon ( icob );
                    item0->setFlags( flags );
                
                    diritto.append(item0);
    
                    QStandardItem *item1 = new QStandardItem(tr("dest_%1").arg(UniqueID));
                    item1->setFlags( flags );
                    item1->setData(UniqueID,Qt::UserRole);
    
                    diritto.append(item1);
    
                    QStandardItem *item2 = new QStandardItem(QString("%1").arg(level));
                    item2->setFlags( Qt::ItemIsEnabled );
                    item2->setData(UniqueID,Qt::UserRole);
    
                    diritto.append(item2);
                    return diritto;
    
}


QList<QStandardItem *> BookMarkModelRead::Compose( const QDomElement e , const int leveldeep )
{
    Q_ASSERT ( e.tagName() == "fo:bookmark" );
    const int SessionID = qrand () % 1200;
    treeLoop++;
    QList<QStandardItem *> diritto;
    QIcon icob = createBookColorIcon( Qt::darkRed );
    QString txt = e.firstChildElement("fo:bookmark-title").text();
    if (txt.size() < 2) {
        txt = tr("No Title found!");
        icob = createBookColorIcon( Qt::red );
    }
    const QString link = e.attribute("internal-destination","null");
    internalLinkFound.append(link);
    //////////////qDebug() << "### read -> " << txt <<  "-" << treeLoop;


                    QStandardItem *item0 = new QStandardItem(txt);
                    item0->setData(leveldeep + treeLoop + SessionID ,Qt::UserRole);
                    item0->setData(bold_base_font,Qt::FontRole);
                    item0->setIcon ( icob );
                    item0->setFlags( flags );
                
                    diritto.append(item0);
    
                    QStandardItem *item1 = new QStandardItem(link);
                    item1->setFlags( flags );
                    item1->setData(leveldeep + treeLoop + SessionID ,Qt::UserRole);
    
                    diritto.append(item1);
    
                    QStandardItem *item2 = new QStandardItem(QString("%1").arg(leveldeep));
                    item2->setFlags( Qt::ItemIsEnabled );
                    item2->setData(leveldeep + treeLoop + SessionID ,Qt::UserRole);
    
                    diritto.append(item2);
                    
    if (!e.firstChildElement("fo:bookmark").isNull() && leveldeep == 0 ) {
        /* one level deep down child */
         QDomElement child = e.firstChildElement("fo:bookmark");
           while (!child.isNull()) {
                       if ( child.tagName()  == "fo:bookmark") {
                              const QString nextlink = child.attribute("internal-destination","null");
                              if (!internalLinkFound.contains(nextlink)) {
                              diritto.first()->appendRow(Compose(child,leveldeep + 1));
                              }
                        }
               
               child = child.nextSiblingElement();
           }
    } else if (!e.firstChildElement("fo:bookmark").isNull() && leveldeep > 0 ) {
        /*  same level is only next on deep */
            QDomElement child = e.firstChildElement("fo:bookmark");
           while (!child.isNull()) {
                       if ( child.tagName()  == "fo:bookmark") {
                              const QString nextlink = child.attribute("internal-destination","null");
                              if (!internalLinkFound.contains(nextlink)) {
                              diritto.first()->appendRow(Compose(child,leveldeep + 1));
                              }
                        }
               
               child = child.nextSiblingElement("fo:bookmark");
           }
    }
    return diritto;
}



/*

    QDomElement child = e.nextSiblingElement("fo:bookmark");
           while (!child.isNull()) {
               diritto.first()->appendRow(Compose(child,leveldeep));
               child = child.nextSiblingElement("fo:bookmark");
           }

*/


       















void BookMarkModelRead::read( const QDomElement e )
{
         QDomElement child = e.firstChildElement();
           while (!child.isNull()) {
               /////////////qDebug() << "### loop reed -> " << child.tagName();
                        if ( child.tagName()  == "fo:bookmark-tree") {
                            openRootBookmark( child );
                        } else if (!child.firstChild().isNull()) {
                            if (!foundTree) {
                            read( child.firstChild().toElement());
                            }
                        }
               child = child.nextSiblingElement();
           }
    
}




void BookMarkModelRead::read()
{
     QDomElement r = doc.documentElement();
    if (r.tagName() !="fo:root")
    {
        return;
    }
     QDomElement child = r.firstChildElement();
           while (!child.isNull()) {
               
               ///////////////qDebug() << "### loop reed -> " << child.tagName();
               
               if ( child.tagName()  == "fo:bookmark-tree") {
                   openRootBookmark( child );
               } else {
                   if (!foundTree) {
                   read( child);
                   }
               }
               
                child = child.nextSiblingElement();
           }

}





BookMarkModelRead::~BookMarkModelRead()
{
	
}












