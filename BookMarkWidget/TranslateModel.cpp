#include "TranslateModel.h"


QIcon createBookColorIcon( QColor color )
{
    QPixmap pixmap(50, 50);
    pixmap.fill(color);
    return QIcon(pixmap);
}



QList<QStandardItem*> childList( QStandardItem *qi )
{
    QStandardItemModel *pm = qi->model();
    const int coolsbase = qi->column();
    
    QList<QStandardItem*> rList;
    QModelIndex in = qi->index();
    if (!in.isValid()) {
    return rList; 
    }
    if (qi->rowCount() == 0) {
    return rList;  
    }

    for (int e = 0; e < qi->rowCount(); ++e) {
        QModelIndex si = pm->index(e,coolsbase,in);
        QStandardItem *iz = pm->itemFromIndex(si);
        if (iz) {
           rList << iz; 
        }
    }
    return rList; 
}

/////////  QStandardItem* col 0 + sibilling 
QMap<QStandardItem*,QModelIndexList> childListmap( QStandardItem *qi )
{
    QStandardItemModel *pm = qi->model();
    const int coolsbase = qi->column();
    const int roowbase = qi->row();
    QMap<QStandardItem*,QModelIndexList> rList;
    QModelIndex in = qi->index();
    
    if (!in.isValid()) {
    return rList; 
    }
    if (qi->rowCount() == 0) {
    return rList;  
    }
    
    for (int e = 0; e < qi->rowCount(); ++e) {
        QModelIndex si = pm->index(e,coolsbase,in);
        QModelIndexList linesibi;
        linesibi.clear();
        /* try 11 column */
        for (int x = 0; x < 11; ++x) {
            QModelIndex line = si.sibling(si.row(),si.column() + x);
            if (line.isValid()) {
                linesibi << line;
            }
        }
        QStandardItem *iz = pm->itemFromIndex(si);
        if (iz) {
           rList.insert(iz,linesibi);
            
        }
    }
    return rList; 
}



using namespace ApacheFop;


ModelDomRead::ModelDomRead( QStandardItemModel  *mod ,  
                          ModusReadModel  e , 
                          const QString xlinktxt )
{
    linktxt = xlinktxt;
    modus = e;
    model = mod;
    read();
}

void ModelDomRead::read()
{
    dom = new QDomDocument;
    QDomProcessingInstruction header = dom->createProcessingInstruction( "xml",QString("version=\"1.0\" encoding=\"utf-8\"" ));
	dom->appendChild( header );
	//////QDateTime timer1( QDateTime::currentDateTime() );  /* time root */
	root = dom->createElement("fo:root");
    root.setAttribute ("xmlns:fo","http://www.w3.org/1999/XSL/Format");
    root.setAttribute ("xmlns:svg","http://www.w3.org/2000/svg");
    root.setAttribute ("xmlns:cms","http://www.pulitzer.ch/2007/CMSFormat");
    root.setAttribute ("xmlns:fox","http://xmlgraphics.apache.org/fop/extensions");
    dom->appendChild( root );
    appendOnTreeDom();    

    
////////qDebug() << "# books .... " << dom->toString(1);
}

void ModelDomRead::appendOnTreeDom()
{
    QDomElement bookmarkroot = dom->createElement("fo:bookmark-tree");
    root.appendChild(bookmarkroot);
    
    for (int e = 0; e < model->rowCount(); ++e) {
        
           QStandardItem *first = model->item(e,0);
           QStandardItem *second = model->item(e,1);
           const QString txt = first->text();
           const QString href = second->text();
           //////////////qDebug() << "# parse " << txt;
          
           appender = dom->createElement("fo:bookmark");
           appender.setAttribute ("internal-destination",href);
           QDomElement label= dom->createElement("fo:bookmark-title");
           label.appendChild( dom->createTextNode ( txt ));
           appender.appendChild( label );
        
            
           QDomElement esub = appender;
          if (modus == Mommit && first->index().data(Qt::UserRole).toString() == linktxt) {
             /* remove full tree if need */ 
          } else {
           bookmarkroot.appendChild(appender);
           parserChild(first->index(),esub);  
          }
          
           
       }
}
    

void ModelDomRead::parserChild( QModelIndex index , QDomElement e )
{
     QStandardItem *ix = model->itemFromIndex(index);
     QList<QStandardItem*> list;
         list.clear();
         list = childList(ix);
         int ctot = list.size();
    if (ctot !=0) {
        foreach (QStandardItem *ix,list) {
             QModelIndex in = ix->index();
             QModelIndex cel2 = in.sibling(in.row(),in.column() + 1);
            
            
             ///////qDebug() << "# remmmmmmmmmmmmmmmmmm " << linktxt;
            /////////qDebug() << "# remmmmmmmmmmmmmmmmmm " << modus;
            ////////////qDebug() << "# remmmmmmmmmmmmmmmmmm " << in.data(Qt::DisplayRole).toString();
            
             if (cel2.isValid()) {
                 if (modus == Mommit && in.data(Qt::UserRole).toString() == linktxt) {
                    /* not take this tree */
                 } else {
                  QDomElement clink = linkPdf(in,cel2);
                  e.appendChild(clink);
                  QDomElement a = clink;
                  parserChild(in,a);                 
                 }
                 
                 
             }
        }
        
    }
        
}
    

QDomElement ModelDomRead::linkPdf( QModelIndex first ,  QModelIndex second )
{
    
           const QString txt = first.data(Qt::DisplayRole).toString();
           const QString href = second.data(Qt::DisplayRole).toString();
    
           QDomElement xlink = dom->createElement("fo:bookmark");
           xlink.setAttribute ("internal-destination", href );
           QDomElement label= dom->createElement("fo:bookmark-title");
           label.appendChild( dom->createTextNode ( txt ));
           xlink.appendChild( label );
           return xlink;
}
    
    




    
   


















