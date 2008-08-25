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
    






BooksDelegate::BooksDelegate( QObject *parent  , QStringList link )
   :QItemDelegate(parent)
{
  setParent(parent);
  xi = link;
}

void BooksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int radius = 10;
    
    QPainterPath titlePath;
    titlePath.moveTo( option.rect.bottomLeft() );
    titlePath.lineTo( option.rect.topLeft()+QPoint( 0, radius ) );
    titlePath.arcTo( option.rect.left(), option.rect.top(), 2*radius, 2*radius, 180, -90 );
    titlePath.lineTo( option.rect.topRight()-QPoint( radius, 0 ) );
    titlePath.arcTo( option.rect.right()-2*radius, option.rect.top(), 2*radius, 2*radius, 90, -90 );
    titlePath.lineTo( option.rect.bottomRight() );
    titlePath.closeSubpath();
    
    
    QLinearGradient titleGradient( option.rect.topLeft(), option.rect.topRight() );
    titleGradient.setColorAt( 0.0, Qt::white );
    titleGradient.setColorAt( 0.5, Qt::white );
    titleGradient.setColorAt( 1.0, QColor("#e6e6e6") );
    
    
    QLinearGradient titleGradient1( option.rect.topLeft(), option.rect.topRight() );
    titleGradient1.setColorAt( 0.0, QColor("#e6e6e6") );
    titleGradient1.setColorAt( 0.5, QColor("#e6e6e6") );
    titleGradient1.setColorAt( 1.0, Qt::white );
    
    
    
  if (index.column() == 0 ) {
       painter->save();
       QPen line_pen(QBrush(Qt::red),1,Qt::DotLine,Qt::RoundCap);
       painter->setPen(Qt::NoPen); 
       painter->setBrush(titleGradient);
       painter->drawRect(option.rect);
       painter->restore(); 
  }
  
   if (index.column() == 2 ) {
       painter->save();
       QPen line_pen(QBrush(Qt::red),1,Qt::DotLine,Qt::RoundCap);
       painter->setPen(Qt::NoPen);
       painter->setBrush(titleGradient1);
       painter->drawRect(option.rect);
       painter->restore(); 
  }
  
     if (index.column() == 1 ) {
       painter->save();
       QPen line_pen(QBrush(Qt::red),1,Qt::DotLine,Qt::RoundCap);
       painter->setPen(Qt::NoPen);
       painter->setBrush(QColor("#d4ffdf"));
       painter->drawPath( titlePath );
       painter->restore(); 
  }
  
  
  
  
  
   QItemDelegate::paint(painter,option,index);
}

QWidget *BooksDelegate::createEditor(QWidget *parent0, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
   if (index.column() == 1) {
         QComboBox *editor = new QComboBox(parent0);
         editor->setEditable ( true );
         editor->clear();
         for (int i = 0; i < xi.size(); ++i) {
         editor->addItem(xi.at(i));
         }
         return editor;
         
   }  else {
        QLineEdit *editor = new QLineEdit(parent0); 
        return editor;
   }
}

void BooksDelegate::setEditorData(QWidget *editor, const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    } else {
        const QString dateuser = index.data(Qt::DisplayRole).toString();
        
        if (index.column() == 1) {
            QComboBox *editorax = qobject_cast<QComboBox *>(editor);
            editorax->setItemText(0,dateuser);
        } else {
           QLineEdit *editorax = qobject_cast<QLineEdit *>(editor);
           editorax->setText(dateuser); 
        }
    }
}

QSize BooksDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
   if (index.column() == 2) 
    return QSize( 25, 25 );
  else
    return QSize( 140, 25 );
}


void BooksDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const 
{
    if (index.column() == 0) {
        return;
     } 
     
     if (index.column() == 1) {
            QComboBox *editorax = qobject_cast<QComboBox *>(editor);
            model->setData(index,editorax->currentText());
     } else {
             QLineEdit *editorax = qobject_cast<QLineEdit *>(editor);
             model->setData(index,editorax->text());
     }
 }


    
   


















