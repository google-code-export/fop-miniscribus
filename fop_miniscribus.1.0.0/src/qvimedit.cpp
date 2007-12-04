#include "qvimedit.h"
//
/*  Save file as qvimedit.cpp  */
/*  incomming class name QVimedit */
//

QString QVimedit::ImageFilterHaving() const
{
  QString filter;
  filter = tr( "All supported Types" ) + " (";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for ( int x = 0; x < formats.count(); ++x ) {

    filter += QString( "*.%1" ).arg( QString( formats[ x ] ) );
    if ( x != formats.count() - 1 )
      filter += " ";
  }
  filter += ");;";
  QString filterSimple;
  
  filterSimple += tr( "Scalable Vector Graphics" ) + " (*.svg *.svg.gz);;";
  for ( int y = 0; y < formats.count(); ++y ) {

    QString form( formats[ y ] );
    if ( form == "bmp" )
      filterSimple += tr( "Windows Bitmap" ) + " (*.bmp)";
    else if ( form == "gif" )
      filterSimple += tr( "Graphic Interchange Format" ) + " (*.gif)";
    else if ( form == "jpeg" || form == "jpg" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "Joint Photographic Experts Group" ) + " (*.jpeg *.jpg)";
    } else if ( form == "mng" )
      filterSimple += tr( "Multiple-image Network Graphics" ) + " (*.mng)";
    else if ( form == "png" )
      filterSimple += tr( "Portable Network Graphics" ) + " (*.png)";
    else if ( form == "pbm" || form == "ppm" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "Portable Bitmap" ) + " (*.pbm *.ppm)";
    } else if ( form == "pgm" )
      filterSimple += tr( "Portable Graymap" ) + " (*.pgm)";
    else if ( form == "xbm" || form == "xpm" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "X11 Bitmap" ) + " (*.xbm *.xpm)";
    } else if ( form == "ico" )
      filterSimple += tr( "Icon Image File Format" ) + " (*.ico)";
    else if ( form == "jp2" || form == "j2k" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "JPEG 2000" ) + " (*.jp2 *.j2k)";
    } else if ( form == "tif" || form == "tiff" ) {

      if ( !filterSimple.contains( form ) )
        filterSimple += tr( "Tagged Image File Format" ) + " (*.tif *.tiff)";
    } else
      filterSimple += tr( "Unknown Format" ) + QString( " (*.%1)" ).arg( form );

    if ( y != formats.count() - 1 && !filterSimple.endsWith( ";;" ) )
      filterSimple += ";;";
  }
  
  
  
  
  filterSimple = filterSimple.left( filterSimple.length() - 2 );
  filter += filterSimple;

   return filter;

}


void  QVimedit::mousePressEvent ( QMouseEvent * e )
{
        QTextCharFormat ChFo = textCursor().charFormat();
        if (ChFo.isValid()) {
            emit ActualFormat(ChFo);
        }
        
        QTextBrowser::mousePressEvent(e);
}
void  QVimedit::mouseReleaseEvent ( QMouseEvent * e )
{
        QTextCharFormat ChFo = textCursor().charFormat();
        if (ChFo.isValid()) {
            emit ActualFormat(ChFo);
        }
        
        QTextBrowser::mouseReleaseEvent(e);
}




void  QVimedit::InsertImageonCursor()
{
    QDateTime timer1( QDateTime::currentDateTime() );
    const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
    
    QString file = QFileDialog::getOpenFileName( this, tr( "Choose Image to insert..." ), QString(setter.value("LastDir").toString()) , ImageFilterHaving() );
    if ( file.isEmpty() ) {
    return;
    }
    
    setter.setValue("LastDir",file.left(file.lastIndexOf("/"))+"/");
    
    
    
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     QFileInfo fixurl(file);
     const QString extension =  fixurl.completeSuffix().toLower();
     QByteArray              derangedata;
     QPixmap                 resultimage;
     QPixmap                 scaledsimage;
     QUrl                    imgresor;
     
        if (extension.contains(".gz")) {
        derangedata = OpenGzipOneFileByte( fixurl.absoluteFilePath() );
        } else {
                         QFile f(fixurl.absoluteFilePath());
                         if (f.open(QIODevice::ReadOnly)) {
                            derangedata = f.readAll();
                            f.close();
                         }
        }
        
        //////////////qDebug() << "### image" << derangedata.size();
        if (derangedata.size() < 1) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical( this, tr( "Alert! image format." ), tr( "Unable to read file %1" ).arg(fixurl.fileName()) );  
        return;
        }
        /* read image */
        if (extension.contains("sv")) {
            /* no build instream here */
        imgresor = QUrl(QString(":/svg/external-paste-%1").arg(TimestampsMs));
        resultimage = RenderPixmapFromSvgByte(  derangedata );
        } else {
        imgresor = QUrl(QString(":/png/external-paste-%1").arg(TimestampsMs));
        resultimage.loadFromData( derangedata );
        }
        
        /* read in */
        if ( resultimage.isNull() ) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical( this, tr( "Alert! image format." ), tr( "Unable to render to image file %1 from type %2." ).arg(fixurl.fileName()).arg(extension) ); 
        return;
        }
        
        ImageContainer.insert(imgresor.toString(),derangedata);
        scaledsimage = resultimage.scaledToWidth(200);
        document()->addResource(QTextDocument::ImageResource,imgresor,resultimage);
        QTextImageFormat format;
        format.setName( imgresor.toString() );
        format.setHeight ( scaledsimage.height() );
        format.setWidth ( scaledsimage.width() );
        format.setToolTip(imgresor.toString());
        format.setProperty(2007,"frompaste");
        textCursor().insertImage( format );
        QApplication::restoreOverrideCursor();
}





QVariant QVimedit::ResourceBlock( const QString resourcename ) 
{
      QVariant nullimage;
      QMapIterator<QString,QVariant> i(ImageContainer);
         while (i.hasNext()) {
             i.next();
             if (i.key() == resourcename) {
              return i.value();
             }
         } 
    return nullimage;
}



void  QVimedit::AppendImageBlocks( TypImageContainer block ) 
{
      QMapIterator<QString,QVariant> i(block);
    
         while (i.hasNext()) {
         i.next();
         //////qDebug() << "### AppendImageBlocks -> " << i.key();
         ImageContainer.insert(i.key(),i.value());
         } 
  
}

void QVimedit::insertFromMimeData ( const QMimeData * source )
{
    /* external  paste drag  */
    ////////////qDebug() << "### insertFromMimeData ";
    
    if ( source->hasImage() ) {
         QDateTime timer1( QDateTime::currentDateTime() );
         const QString TimestampsMs = QString("%1-%2-image").arg(timer1.toTime_t()).arg(timer1.toString("zzz"));
         QPixmap aspixmape = qvariant_cast<QPixmap>(source->imageData());
         QUrl imgresor(QString(":/png/external-paste-%1").arg(TimestampsMs));
        document()->addResource(QTextDocument::ImageResource,imgresor,aspixmape);
        QTextImageFormat format;
        format.setName( imgresor.toString() );
        format.setHeight ( aspixmape.height() );
        format.setWidth ( aspixmape.width() );
        format.setToolTip(imgresor.toString());
        format.setProperty(2007,"frompaste");
        textCursor().insertImage( format );
        
         QByteArray bytes;
         QBuffer buffer(&bytes);
         buffer.open(QIODevice::WriteOnly);
         aspixmape.save(&buffer, "PNG",100); // writes pixmap into bytes in PNG format 
         ImageContainer.insert(imgresor.toString(),bytes);
         ///////////// qDebug() << "###incolla  -> " << imgresor.toString();
         return;
    }
    /* external html  */
    if ( source->formats().contains("text/html") ) {
       /////////////// qDebug() << "### insertFromMimeData html ";
        QString draghtml = source->html();
        /////////qDebug() << "### external image paste";
        ///////fwriteutf8("copy_in.html",xhtmlnew);
         QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(draghtml);
         textCursor().insertFragment(fragment);
         return;
    }
    /* external plain text incomming   */
      if ( source->hasText() )  {
        textCursor().insertText(source->text());
          /////////////////qDebug() << "### insertFromMimeData txt  ";
        return;
      }
    
    
}








QVimedit::QVimedit( QWidget* parent )
 : QTextBrowser(parent)
{
     #if defined Q_WS_X11
     QDesktopWidget *desk = QApplication::desktop();
     QRect winsize = desk->screenGeometry();
     setMinimumHeight((winsize.height() / 10) * 7); 
     #endif
     ramclip = QApplication::clipboard();
     ImageContainer.clear();
     setAcceptDrops(true);
	 numerobase = 0;
     LastScrolling = 0;
     shortcut1 = new QShortcut(QKeySequence::Save,this);
     connect(shortcut1, SIGNAL(activated()),this, SLOT(SaveCurrentDoc()));
     scroll = verticalScrollBar();
     connect( scroll, SIGNAL(valueChanged(int)),this, SLOT(CachePosition(int))); 
     shortcut7 = new QShortcut(QKeySequence::ZoomOut,this);  /* CTRL- */
     shortcut8 = new QShortcut(QKeySequence::ZoomIn,this);  /* CTRL+ */
     connect(shortcut7, SIGNAL(activated()),this, SLOT(zoomOut()));
     connect(shortcut8, SIGNAL(activated()),this, SLOT(zoomIn()));
     connect(ramclip, SIGNAL(dataChanged()),this, SLOT(ClipbordNewdata()));
     
    
     setContextMenuPolicy(Qt::CustomContextMenu);
     connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(RContext(QPoint)));
     Editablemodus( false );
}

void QVimedit::ClipbordNewdata()
{
    qDebug() << "### ClipbordNewdata";
    emit selectionChanged();
    emit copyAvailable(true);
}

QMenu *QVimedit::createOwnStandardContextMenu() 
{
QMenu *TContext = createStandardContextMenu();
TContext->addAction(QIcon(QString::fromUtf8(":/img/zoomin.png")),tr( "Zoom In" ), this , SLOT( zoomIn() ) );
TContext->addAction(QIcon(QString::fromUtf8(":/img/zoomout.png")),tr( "Zoom Out" ), this , SLOT( zoomOut() ) );
if (textCursor().currentTable()) {
TContext->addAction(tr( "Table: Remove Columns Cells if select" ), this , SLOT( RemoveCoolByCursorPosition() ) );
TContext->addAction(tr( "Table: Remove Rows Cells if select" ), this , SLOT( RemoveRowByCursorPosition() ) );
TContext->addAction(tr( "Table: Append Rows" ), this , SLOT( AppendTableRows() ) );
TContext->addAction(tr( "Table: Append Columns" ), this , SLOT( AppendTableCools() ) );
TContext->addAction(tr( "Table: Merge / Union Cells if select" ), this , SLOT( MergeCellByCursorPosition() ) );
TContext->addAction(tr( "Table: Set Columns FixedLength" ), this , SLOT( SetColumLarge() ) );
TContext->addAction(tr( "Table: Set Cell Background Color" ), this , SLOT( SetTableCellColor() ) );
}
TContext->addAction(QIcon(QString::fromUtf8(":/img/pictures.png")),tr( "Insert a new image." ), this , SLOT( InsertImageonCursor() ) );
TContext->addAction(QIcon(QString::fromUtf8(":/img/table.png")),tr( "Insert new Table." ), this , SLOT( CreateanewTable() ) );
TContext->addAction(tr( "Set Block margin to selected text." ), this , SLOT( SetTextBlockMargin() ) );


return TContext;
}


void QVimedit::RContext( const QPoint & pos ) 
{
QMenu *RContext = createOwnStandardContextMenu();
RContext->exec(QCursor::pos());
delete RContext;   
}


void  QVimedit::SetColumLarge()
{
    if (textCursor().currentTable()) { 
    QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
    QTextTableFormat tbforms = textCursor().currentTable()->format();
    int cellcoolcursoris = existingcell.column(); /* int value start from zero */
        bool ok;
        int LargeSet = QInputDialog::getInteger(0, tr("Set Cell Width"),
                                      tr("Point Length:"),Get_Cell_Width(tbforms,cellcoolcursoris), 1, 2000, 1, &ok);
        if (ok && LargeSet > 0) {
        QVector<QTextLength> constraints = tbforms.columnWidthConstraints();
        for (int i = 0; i < constraints.size(); ++i) {
            if (i == cellcoolcursoris) {
                constraints.replace(i,QTextLength(QTextLength::FixedLength, LargeSet)); 
            }
        }
        tbforms.setColumnWidthConstraints(constraints);
        textCursor().currentTable()->setFormat(tbforms);
        }
    }
}



qreal QVimedit::Get_Cell_Width( QTextTableFormat TableFormat , int position )  
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


void  QVimedit::MergeCellByCursorPosition()
{
    if (textCursor().currentTable()) {  
       textCursor().currentTable()->mergeCells(textCursor());
    }  
}



void QVimedit::RemoveCoolByCursorPosition()
{
    if (textCursor().currentTable()) {  
    QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
    int cellcoolcursoris = existingcell.column(); /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    textCursor().currentTable()->removeColumns(cellcoolcursoris,1);
    }
}


void  QVimedit::RemoveRowByCursorPosition()
{
    if (textCursor().currentTable()) {  
    QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
    int cellcoolcursoris = existingcell.column(); /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    textCursor().currentTable()->removeRows(cellrowcursoris,1);
    }
}




void  QVimedit::AppendTableRows()
{
    bool ok = false;
    if (textCursor().currentTable()) {
    QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
    int cellcoolcursoris = existingcell.column(); /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    int approwtot = QInputDialog::getInteger(this, tr("Append NR. line row"),tr("Row:"), 1, 1, 100, 1, &ok);
        if (ok and approwtot > 0) {
          textCursor().currentTable()->insertRows(cellrowcursoris + 1,approwtot);
        }
    }
}

void  QVimedit::AppendTableCools()
{
    bool ok = false;
    if (textCursor().currentTable()) {
        QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
        int cellcoolcursoris = existingcell.column(); /* int value start from zero */
        int cellrowcursoris = existingcell.row(); /* int value start from zero */
        int appcooltot = QInputDialog::getInteger(this, tr("Table append Column"),tr("Cool:"), 1, 1, 10, 1, &ok);
            if (ok and appcooltot > 0) {
              textCursor().currentTable()->insertColumns(cellcoolcursoris + 1,appcooltot);
            }
    }  
}


void  QVimedit::SetTableCellColor()
{
     if (textCursor().currentTable()) { 
     bool ok;
     QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
        /* reformat this -> existingcell */
        QTextCharFormat existformat = existingcell.format();
                     /* get color */
                     QColor col = QColorDialog::getRgba(NULL,&ok, this);
                     if (!col.isValid()) {
                        return; 
                     }
                        QBrush stylesin(col);
                        existformat.setBackground(stylesin);
                        existingcell.setFormat(existformat); 
    }
}











void QVimedit::dragEnterEvent(QDragEnterEvent *event)
{
  event->acceptProposedAction();
}

void QVimedit::dropEvent(QDropEvent *event)
{ 
  if(event && event->mimeData()) {
      if (!event->mimeData()) {
        return;
      } else {
       insertFromMimeData(event->mimeData());
      }
  }
}

bool QVimedit::canInsertFromMimeData ( const QMimeData * source )
{
    return QTextBrowser::canInsertFromMimeData(source);
}

void QVimedit::Editablemodus( bool modus )
{
setReadOnly(modus);
setOpenExternalLinks(modus);
}


QVimedit::~QVimedit()
{
    numerobase = 0;
}

void QVimedit::CachePosition( int lastscroll )
{
   LastScrolling = lastscroll;
}


void QVimedit::SaveCurrentDoc()
{
emit SaveStreamer();
}


void QVimedit::mouseDoubleClickEvent( QMouseEvent *e )
{
    ////////////////qDebug() << "### mouseDoubleClickEvent ";
    emit DDclick();
    QTextBrowser::mouseDoubleClickEvent(e);
}


void QVimedit::keyPressEvent(QKeyEvent *e)
{
    emit DDclick();
    QTextBrowser::keyPressEvent(e);
}






void  QVimedit::CreateanewTable()
{

    QString subtext, collx, rowx;
    bool ok;
    int colonne = QInputDialog::getInteger(this, tr("New Table cool"),tr("Cool:"), 3, 1, 10, 1, &ok);
    int righe = QInputDialog::getInteger(this, tr("New Table row"),tr("Row:"), 3, 1, 100, 1, &ok);
    int largo = width() / 3;
    if (colonne > 0 and righe > 0 and largo > 0) {
    QStringList tables;
    tables.clear();
    tables.append(QString("<table border=\"1\" align=\"left\" width=\"%1\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#ffffff\">").arg(largo));
    for (int i=0;i<righe;i++){
        tables.append(QString("<tr>")); 
           for (int o=0;o<colonne;o++){
               tables.append(QString("<td><p></p></td>")); 
           }
        tables.append(QString("</tr>")); 
    }  
    tables.append(QString("</table>"));  
    
    subtext = tables.join("\n");
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(subtext);
    textCursor().insertFragment(fragment);
    }
}





void  QVimedit::SetTextBlockMargin()
{
    QTextBlock textblocc = textCursor().block();
        if (textblocc.isValid()) {
            QTextBlockFormat formatibb = textblocc.blockFormat();
             GetMargin *marge = new GetMargin(this);
             marge->setWindowTitle(tr("Set Paragraph BlockFormat margin"));
             marge->Set_margin( QRectF (formatibb.topMargin(), formatibb.rightMargin() ,formatibb.bottomMargin() , formatibb.leftMargin() ) );
             int faxme = marge->exec();
             if (faxme == 1) {
                 QRectF setFrams = marge->Get_margin();
                 const qreal TopMargin = setFrams.x();
                 const qreal RightMargin = setFrams.y();
                 const qreal BottomMargin = setFrams.width();
                 const qreal LeftMargin = setFrams.height();
                 formatibb.setLeftMargin(LeftMargin);
                 formatibb.setBottomMargin(BottomMargin);
                 formatibb.setTopMargin(TopMargin);
                 formatibb.setRightMargin(RightMargin);
                 textCursor().setBlockFormat(formatibb);
             }
             
         }
}




void QVimedit::Linkers( QStringList l )
{
  InternalLinkAvaiable = l;
}

void QVimedit::MakeHrefLink()
{
  //////InternalLinkAvaiable;
    
    QTextCursor c = textCursor();
    QTextCharFormat  format = c.charFormat();
    QString anchor = format.anchorHref();
    
    if (anchor.size() > 0) {
     c.setCharFormat(QTextCharFormat());
    }
    
    
     bool ok;
     QString item = QInputDialog::getItem(this, tr("Set Link to.."), tr("Internal Name or http://:"), InternalLinkAvaiable,-1,true, &ok);
     if (ok && !item.isEmpty()) {
         
                      if (!IsNetFile( item )) {
                         item = item.prepend("#"); 
                      }
         
                          c = textCursor();
                          format = c.charFormat();
                          format.setAnchor (true); 
                          format.setAnchorHref(item); 
                          format.setFontUnderline(true);
                          format.setForeground ( QBrush ( QColor ("#ff0000") ) ); 
                          c.setCharFormat(format);
         
     }
}














