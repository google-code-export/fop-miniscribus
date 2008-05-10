
#include "ModelSwap.h"

ModelSwap::ModelSwap()
{
    bold_base_font = qApp->font();
    bold_base_font.setBold ( true );
    flags = Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


void ModelSwap::set( QTableView  *table , bool style )
{
    model = qobject_cast<QStandardItemModel *>(table->model());
    if (!model) {
    set(table->model());
        qDebug() << "###  QTableView->cast not OK :-( " << model->rowCount();
        
    } else {
        qDebug() << "###  QTableView->cast OK :-) " << model->rowCount();
    }
}

void ModelSwap::set( QTableWidget  *table , bool style )
{
    model = qobject_cast<QStandardItemModel *>(table->model());
    if (!model) {
    set(table->model());
        qDebug() << "###  QTableWidget->cast not OK :-( " << model->rowCount();
        
    } else {
        qDebug() << "###  QTableWidget->cast OK :-) " << model->rowCount();
    }
}


void ModelSwap::set( const QStandardItemModel  *in_model , bool style )
{
     model = new QStandardItemModel();
     const int summline = in_model->rowCount();
     QList<QStandardItem *> one_line_data;
     row = summline;
     headers.clear();
     for (int i = 0; i < in_model->columnCount(); ++i) {
         const QString Htext = in_model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
         headers.append(Htext);
         QStandardItem *Hitem = new QStandardItem(Htext);
         Hitem->setData(bold_base_font,Qt::FontRole);
         model->setHorizontalHeaderItem(i,Hitem);
     }
     
     for (int e = 0; e < summline; ++e) {
         one_line_data.clear();
             
                  for (int x = 0; x < headers.size(); ++x)  {
                              QModelIndex index = in_model->index(e,x);
                              const QString cellTxt = in_model->data(index,Qt::DisplayRole).toString();
                              QStandardItem *Cell_item;
                              if (style) {
                              Cell_item = Style(index,cellTxt,e);
                              } else {
                              Cell_item = new QStandardItem(cellTxt);
                              }
                              Cell_item->setFlags(flags);
                              one_line_data.append(Cell_item);

                  } 
          
                  
         model->invisibleRootItem()->appendRow(one_line_data);
     }
}

void ModelSwap::nulldata()
{
    model = new QStandardItemModel();
    headers.clear();
    headers.append("Error");
         QStandardItem *Hitem = new QStandardItem("Error");
         Hitem->setData(bold_base_font,Qt::FontRole);
         model->setHorizontalHeaderItem(0,Hitem);
    
    QList<QStandardItem *> one_line_data;
    QStandardItem *Litem = new QStandardItem("No data incomming!");
    one_line_data.append(Litem);
    model->invisibleRootItem()->appendRow(one_line_data);
}

void ModelSwap::set( const QAbstractItemModel  *in_model , bool style )
{
     model = new QStandardItemModel();
     const int summline = in_model->rowCount();
     QList<QStandardItem *> one_line_data;
     row = summline;
     headers.clear();
     for (int i = 0; i < in_model->columnCount(); ++i) {
         const QString Htext = in_model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
         headers.append(Htext);
         QStandardItem *Hitem = new QStandardItem(Htext);
         Hitem->setData(bold_base_font,Qt::FontRole);
         model->setHorizontalHeaderItem(i,Hitem);
     }
     
     for (int e = 0; e < summline; ++e) {
         one_line_data.clear();
         ///////one_line_data = in_model->takeRow(e);
 
           
             
                  for (int x = 0; x < headers.size(); ++x)  {
                              QModelIndex index = in_model->index(e,x);
                              const QString cellTxt = in_model->data(index,Qt::DisplayRole).toString();
                              QStandardItem *Cell_item;
                              if (style) {
                              Cell_item = Style(index,cellTxt,e);
                              } else {
                              Cell_item = new QStandardItem(cellTxt);
                              }
                              Cell_item->setFlags(flags);
                              one_line_data.append(Cell_item);

                  }  

                               
         model->invisibleRootItem()->appendRow(one_line_data);
     }
}

QStandardItem *ModelSwap::Style( const QModelIndex index , const QString txt  , const int line )
{
    QStandardItem *Cell = new QStandardItem(txt);
    
    QFont fx = index.data(Qt::FontRole).value<QFont>();
    QColor TextColor = index.data(Qt::TextColorRole).value<QColor>();
    QColor BackgroundColor = index.data(Qt::BackgroundRole).value<QColor>();
    /////Qt::Alignment align = ;
    if ( !BackgroundColor.isValid ()) {
               if (line%2) {
                BackgroundColor = QColor("#e1e1e1");
               } else {
                BackgroundColor = QColor("#ffffff");
               }
     }
     if ( !TextColor.isValid ()) {
         TextColor = QColor(Qt::darkBlue);
     }
    Cell->setData(fx,Qt::FontRole);
    Cell->setTextAlignment((Qt::Alignment)index.data(Qt::TextAlignmentRole).toInt());  
    Cell->setData(TextColor,Qt::TextColorRole);
    Cell->setData(BackgroundColor,Qt::BackgroundRole);
    Cell->setFlags(flags);
    return Cell;
}


void ModelSwap::set( const QSqlQueryModel  *in_model )
{
     model = new QStandardItemModel();
     const int summline = in_model->rowCount();
     QList<QStandardItem *> one_line_data;
     row = summline;
     headers.clear();
    
    for (int i = 0; i < in_model->columnCount(); ++i) {
         const QString Htext = in_model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
         headers.append(Htext);
         QStandardItem *Hitem = new QStandardItem(Htext);
         Hitem->setData(bold_base_font,Qt::FontRole);
         model->setHorizontalHeaderItem(i,Hitem);
     }
     
    for (int e = 0; e < summline; ++e) {
         one_line_data.clear();
         QSqlRecord r = in_model->record(e);
                  for (int x = 0; x < headers.size(); ++x)  {
                              const QString cellTxt = r.value(x).toString();
                              QStandardItem *Cell_item = new QStandardItem(cellTxt);
                              Cell_item->setFlags(flags);
                              one_line_data.append(Cell_item);

                  }   
                  
         model->invisibleRootItem()->appendRow(one_line_data);
     }
}




/* increment the model at end */
void ModelSwap::increment( const int i )
{
    const int nrcools = model->columnCount();
    if (nrcools > 0) {
    QList<QStandardItem *> lineaH;
    
        if (i > 0) {
         for (int b = 0; b < i; ++b) {
            lineaH.clear();
               for (int o = 0; o < nrcools; ++o) {
                              QStandardItem *fr = new QStandardItem();
                              if (o == 1) {
                              fr->setData(QString("# %1 #increment").arg(b + 1),Qt::ToolTipRole);
                              }
                              lineaH.append(fr);
                }
             
            model->invisibleRootItem()->appendRow(lineaH); 
         }
        }
        
    }
}

/* simple prepare QTableView - > model to print return QTextDocument */
QTextDocument *ModelSwap::TabletoDom( int remove_last_line )
{
    /* remove last line if needet to remove Empty line to scroll more down on QTableView */
    const int righe = qBound(0,model->rowCount() - remove_last_line ,model->rowCount()); 
    const int colonne = model->columnCount();
    /* normal paragraph format */
    QTextBlockFormat paraformat;
    paraformat.setBottomMargin(0);
    paraformat.setTopMargin(0); 
    /* Empty paragraph format */
    QTextBlockFormat paraspace;
    paraspace.setBottomMargin(8);
    paraspace.setTopMargin(8); 
    QColor BackgroundColor;
    int CursorPointer = 0;
    QTextDocument *doc = new QTextDocument();
    QTextCursor Tcursor = QTextCursor(doc);
    ///////////bool IsCorrectCursor = Tcursor.movePosition(QTextCursor::End);
    CursorPointer = Tcursor.position();
    
    QTextTable *qtable = Tcursor.insertTable(righe+1,colonne);  /////// righe
    QTextTableFormat tableFormat;
    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength,98));
    tableFormat.setBackground ( QColor(Qt::white) );
    tableFormat.setBorder(0);
    tableFormat.setCellSpacing(2);
    tableFormat.setCellPadding(2);
    tableFormat.setAlignment ( Qt::AlignLeft );
    /* START HEADER ONLY ONE TD LINE  */
    for (int e = 0; e < colonne; ++e) {
         const QString testosopra = model->headerData(e,Qt::Horizontal,Qt::DisplayRole).toString();
         QTextTableCell cellheader = qtable->cellAt(0,e);
         /* set cursor position on first cell! */
         Tcursor = cellheader.firstCursorPosition();
                       QFont fx1 = model->headerData(e,Qt::Horizontal,Qt::FontRole).value<QFont>();
                       QColor TextColor1 = model->headerData(e,Qt::Horizontal,Qt::TextColorRole).value<QColor>();
                       ////////QColor BackgroundColor1 = model->headerData(e,Qt::Horizontal,Qt::BackgroundRole).value<QColor>();
                       QTextCharFormat td_format = cellheader.format();
                       td_format.setBackground(QColor("#dfdfdf"));   /* set BG color on header only palette */
                       cellheader.setFormat(td_format);
        
                       QTextCharFormat InlineStyle1;
                       InlineStyle1.setForeground(QBrush(TextColor1));
                       InlineStyle1.setFont(fx1);
                       Tcursor.insertText(testosopra,InlineStyle1);
    /////////////////qDebug() << "testa  ->" << testosopra;
    }
		
		QString imagen;
    /* START LINE ROW LOOPS */
        for (int i = 0; i < righe; ++i) {
        
                      for (int o = 0; o < colonne; ++o) {
											 //////////bool Make_Image = false;
                       QTextTableCell cellstart = qtable->cellAt(i+1,o);
                       QModelIndex index = model->index(i,o);
											 
                                if (index.isValid ()) {
                                           const QString txt = model->data(index,Qt::DisplayRole).toString();
                                           //////////qDebug() << "corpo  ->" << txt;
                                            QFont fx = model->data(index,Qt::FontRole).value<QFont>();
																					 
																	
																	
																	
                                           QColor TextColor = model->data(index,Qt::TextColorRole).value<QColor>();
                                           BackgroundColor = model->data(index,Qt::BackgroundRole).value<QColor>();
                                           /* alignment txt */
                                           paraformat.setAlignment((Qt::Alignment)model->data(index,Qt::TextAlignmentRole).toInt());                                   

                                           if ( !BackgroundColor.isValid ()) {
                                               if (i%2) {
                                                 BackgroundColor = QColor("#e1e1e1");
                                               } else {
                                                 BackgroundColor = QColor("#ffffff");
                                               }
                                           }                                               
                                              
                                           QTextCharFormat existformat = cellstart.format();
                                           existformat.setBackground(BackgroundColor);
                                           cellstart.setFormat(existformat);
                                              
                                           QTextCharFormat InlineStyle;
                                           InlineStyle.setForeground(QBrush(TextColor));
                                           InlineStyle.setFont(fx);
                                              
                                           Tcursor = cellstart.firstCursorPosition();
																					 
																					 if (txt.trimmed().size() < 1) {
                                           Tcursor.setBlockFormat(paraspace);
                                           } else {
                                           Tcursor.setBlockFormat(paraformat);
                                           }
                                           
                                           Tcursor.insertText(txt,InlineStyle);
																					 
																					 
																					 QPixmap pixmap = model->data(index,Qt::DecorationRole).value<QPixmap>();
																					 
																	          
																	          ///////QIcon icon;
																					  ///////qDebug() << "##### dv.isNull() " << dv.isNull();
																					 
																					 
					
																							if (!pixmap.isNull()) {
																									imagen =  QString("/%1_%2.png").arg(i).arg(o);
																									QUrl recresourcein(imagen);
                                                  doc->addResource( QTextDocument::ImageResource,recresourcein,pixmap);
																									QTextImageFormat format;
                                                  format.setName(imagen);
																									format.setToolTip(imagen);
																									Tcursor.insertImage( format );
																									Tcursor.insertText(imagen,InlineStyle);
																									qDebug() << "##### imagen  " << imagen;
																							} else {
																								qDebug() << "##### imagen no " << o;
																								
																							}
																					 
																					 
																					 
																					 
																					 
																					 
																					 
																					 
																					 
                                           
                                }
                      }
        
    }
    
    
    qtable->setFormat( tableFormat );
    /////////QString xdhtml = doc->toHtml("utf-8");
    //////qDebug() << "xdhtml0000000000  ->" << xdhtml;
    return doc;  
}



ModelSwap::~ModelSwap()
{
}



