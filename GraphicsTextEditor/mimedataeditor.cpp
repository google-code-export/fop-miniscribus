#include "mimedataeditor.h"

Layoutpainter::Layoutpainter(QObject *parent)
{
    ComposeAction();
}

/*

QAction *actionBold;
QAction *actionItalic;
QAction *actionUnderline;
QAction *actionBackColor;
QAction *actionTextColor;
QAction *actionBold;
...........................
*/

void Layoutpainter::cursorPosition( const QTextCursor curs )
{
	cursor_position = curs.position();
	///////LastCharFormat = curs.charFormat();
	if (curs.isCopyOf(C_cursor)) {
      
	} else {
		 C_cursor.setPosition(curs.position());
		 cursor_position = C_cursor.position();
	}
	////////qDebug() << "X-Y_Pos_Cursor ->" << X_Pos_Cursor  << "x" << Y_Pos_Cursor << C_cursor.position();
	NewCharformat(C_cursor);
}

void Layoutpainter::undo()
{
    if (!_d) {
    return;
    }
	   qDebug() << "### undo stack go ..... ";
    ////_d->undo(&C_cursor);
	  _d->undo();
}

void Layoutpainter::redo()
{
    if (!_d) {
    return;
    }
    
	  qDebug() << "### redo stack go ..... ";
    ///_d->redo(&C_cursor);
	  _d->redo();
}

void Layoutpainter::ComposeAction()
{
    
    actionBold = new QAction(tr("Bold text CTRL+B"),this);
    const QIcon icon = QIcon(QString::fromUtf8(":/img/textbold.png"));
    actionBold->setIcon(icon);
    actionBold->setCheckable(true);
	  connect(actionBold, SIGNAL(triggered()),this,SLOT(BoldText()));
    
    actionItalic = new QAction(tr("Italic Text CTRL+I"),this);
    const QIcon icon1 = QIcon(QString::fromUtf8(":/img/textitalic.png"));
    actionItalic->setIcon(icon1);
	  actionItalic->setCheckable(true);
	  connect(actionItalic, SIGNAL(triggered()),this,SLOT(ItalicText()));
    
    actionUnderline = new QAction(tr("Text underline CTRL+U"),this);
    const QIcon icon2 = QIcon(QString::fromUtf8(":/img/textunder.png"));
    actionUnderline->setIcon(icon2);
	  actionUnderline->setCheckable(true);
	  connect(actionUnderline, SIGNAL(triggered()),this,SLOT(UnderlineText()));
    
    
    actionOverline = new QAction(tr("Text Overline"),this);
	  actionOverline->setCheckable(true);
	  connect(actionOverline, SIGNAL(triggered()),this,SLOT(OverlineText()));
    
    actionStricktext = new QAction(tr("StrikeOut text"),this);
	  actionStricktext->setCheckable(true);
	  connect(actionStricktext, SIGNAL(triggered()),this,SLOT(StrickText()));

    actionOverline = new QAction(tr("Text Overline"),this);
	  actionOverline->setCheckable(true);
	  connect(actionOverline, SIGNAL(triggered()),this,SLOT(OverlineText()));
    
    actionstretchfont = new QAction(tr("Stretch text font"),this);
	  connect(actionstretchfont, SIGNAL(triggered()),this,SLOT(StretchText()));
    
    actionBackColor = new QAction(tr("Text background color"),this);
    actionBackColor->setIcon(createColorToolButtonIcon(":/img/textpointer.png",Qt::white));
	  connect(actionBackColor, SIGNAL(triggered()),this,SLOT(BGcolor()));
    
    actionTextColor = new QAction(tr("Text color"),this);
    actionTextColor->setIcon(createColorToolButtonIcon(":/img/textpointer.png",Qt::black));
	  connect(actionTextColor, SIGNAL(triggered()),this,SLOT(TXcolor()));
    
    
    actionBlockMargin = new QAction(tr("Block paragraph margin"),this);
    actionBlockMargin->setIcon(QIcon(QString::fromUtf8(":/img/view_text.png")));
    connect(actionBlockMargin, SIGNAL(triggered()),this,SLOT(SetTextBlockMargin()));
    
    grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(MaketextAlign(QAction *)));
    
    actionAlignLeft = new QAction(QIcon(":/img/textleft.png"), tr("Text align left"),grp);
    actionAlignLeft->setCheckable(true);
    actionAlignRight = new QAction(QIcon(":/img/textright.png"), tr("Text align right"),grp);
    actionAlignRight->setCheckable(true);
    actionAlignCenter = new QAction(QIcon(":/img/textcenter.png"), tr("Text align center"),grp);
    actionAlignCenter->setCheckable(true);
    actionAlignJustify = new QAction(QIcon(":/img/textjustify.png"), tr("Text align Justify"),grp);
    actionAlignJustify->setCheckable(true);
     
}

QMenu *Layoutpainter::TableMenu( QWidget * inparent )
{
    QMenu *MenuTables = new QMenu(tr("Table"),inparent);
    MenuTables->setIcon(QIcon(QString::fromUtf8(":/img/newtodo.png")));
    QAction *a;
    a = MenuTables->addAction(tr("Insert new Table"), this, SLOT(CreateanewTable()));
    a->setIcon(QIcon(QString::fromUtf8(":/img/newtodo.png")));
    
    if (textCursor().currentTable()) {
        
        
        a = MenuTables->addAction(tr("Table border"), this, SLOT(MaketableBorder()));
    
        
        a = MenuTables->addAction(tr("Table background color"), this, SLOT(MaketableColorBG()));
        a->setIcon(createColorIcon(textCursor().currentTable()->format().background().color()));
        
        a = MenuTables->addAction(tr("Current cell background color"), this, SLOT(SetTableCellColor()));
        a->setIcon(createColorIcon(textCursor().currentTable()->cellAt(textCursor()).format().background().color()));
        
        a = MenuTables->addAction(tr("Table append cools on cursor"), this, SLOT(AppendTableCools()));
        a->setIcon(QIcon(QString::fromUtf8(":/img/month.png")));
        
         a = MenuTables->addAction(tr("Table append rows on cursor"), this, SLOT(AppendTableRows()));
        a->setIcon(QIcon(QString::fromUtf8(":/img/7days.png")));
        
         a = MenuTables->addAction(tr("Remove rows on cursor"), this, SLOT(RemoveRowByCursorPosition()));
        a->setIcon(QIcon(QString::fromUtf8(":/img/stop.png")));
        
        a = MenuTables->addAction(tr("Remove cools on cursor"), this, SLOT(RemoveCoolByCursorPosition()));
        a->setIcon(QIcon(QString::fromUtf8(":/img/stop.png")));
        
        
        a = MenuTables->addAction(tr("Merge selected cell on cursor"), this, SLOT(MergeCellByCursorPosition()));
        
        a = MenuTables->addAction(tr("Set current column fixed width"), this, SLOT(SetColumLarge()));
        a->setIcon(QIcon(QString::fromUtf8(":/img/month.png")));
        
        ////////////////textCursor().currentTable()->format().background().color().rgba()
    }
    
    
    
    
    /*
void MaketableColorBG();
 void MaketableBorder();
 void CreateanewTable();
 void SetTableCellColor();
 void AppendTableCools();
 void AppendTableRows();
 void RemoveRowByCursorPosition();
 void RemoveCoolByCursorPosition();
 void MergeCellByCursorPosition();
 void SetColumLarge();
    */
    
    
    return MenuTables;
}

void  Layoutpainter::SetColumLarge()
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


qreal Layoutpainter::Get_Cell_Width( QTextTableFormat TableFormat , int position )  
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

void  Layoutpainter::MergeCellByCursorPosition()
{
    if (textCursor().currentTable()) {  
       textCursor().currentTable()->mergeCells(textCursor());
    }  
}


void Layoutpainter::RemoveCoolByCursorPosition()
{
    if (textCursor().currentTable()) {  
    QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
    int cellcoolcursoris = existingcell.column(); /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    textCursor().currentTable()->removeColumns(cellcoolcursoris,1);
    }
}

void  Layoutpainter::RemoveRowByCursorPosition()
{
    if (textCursor().currentTable()) {  
    QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
    int cellcoolcursoris = existingcell.column(); /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    textCursor().currentTable()->removeRows(cellrowcursoris,1);
    }
}


void  Layoutpainter::AppendTableRows()
{
    bool ok = false;
    if (textCursor().currentTable()) {
    QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
    int cellcoolcursoris = existingcell.column(); /* int value start from zero */
    int cellrowcursoris = existingcell.row(); /* int value start from zero */
    int approwtot = QInputDialog::getInteger(0, tr("Append NR. line row"),tr("Row:"), 1, 1, 100, 1, &ok);
        if (ok && approwtot > 0) {
          textCursor().currentTable()->insertRows(cellrowcursoris + 1,approwtot);
        }
    }
}

void  Layoutpainter::AppendTableCools()
{
    bool ok = false;
    if (textCursor().currentTable()) {
        QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
        int cellcoolcursoris = existingcell.column(); /* int value start from zero */
        int cellrowcursoris = existingcell.row(); /* int value start from zero */
        int appcooltot = QInputDialog::getInteger(0, tr("Table append Column"),tr("Cool:"), 1, 1, 10, 1, &ok);
            if (ok && appcooltot > 0) {
              textCursor().currentTable()->insertColumns(cellcoolcursoris + 1,appcooltot);
            }
    }  
}


void  Layoutpainter::SetTableCellColor()
{
     if (textCursor().currentTable()) { 
     bool ok;
     QTextTableCell existingcell =  textCursor().currentTable()->cellAt(textCursor());
        /* reformat this -> existingcell */
        QTextCharFormat existformat = existingcell.format();
                     /* get color */
                     QColor col = QColorDialog::getRgba(textCursor().currentTable()->cellAt(textCursor()).format().background().color().rgba(),&ok, 0);
                     if (!col.isValid()) {
                        return; 
                     }
                        QBrush stylesin(col);
                        existformat.setBackground(stylesin);
                        existingcell.setFormat(existformat); 
    }
}

void  Layoutpainter::CreateanewTable()
{

    QString subtext, collx, rowx,largo;
    bool ok;
    int colonne = QInputDialog::getInteger(0, tr("New Table cool"),tr("Cool:"), 3, 1, 10, 1, &ok);
    int righe = QInputDialog::getInteger(0, tr("New Table row"),tr("Row:"), 3, 1, 100, 1, &ok);
    largo = "100%";
    if (colonne > 0 && righe > 0) {
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

//// textCursor().currentTable()->format().background().color().rgba()

void Layoutpainter::MaketableColorBG()
{
    if (!textCursor().currentTable()) { 
       return; 
    }
    bool ok;
    QColor col = QColorDialog::getRgba(textCursor().currentTable()->format().background().color().rgba(),&ok,0);
    if (!col.isValid()) {
        return;
    } else {
       QTextTableFormat taform = textCursor().currentTable()->format(); 
       taform.setBackground ( col ); 
       textCursor().currentTable()->setFormat(taform);
    }
}

void Layoutpainter::MaketableBorder()
{
    if (!textCursor().currentTable()) { 
       return; 
    }
    bool ok;
    int i = QInputDialog::getInteger(0, tr("Table Border"),tr("Pt units:"), 0, 0, 10, 1, &ok);
    
       QTextTableFormat taform = textCursor().currentTable()->format(); 
       taform.setBorder(i);
       textCursor().currentTable()->setFormat(taform);
}


QMenu *Layoutpainter::TextMenu( QWidget * inparent )
{
    QMenu *MenuText = new QMenu(tr("Text Format"),inparent);
    MenuText->setIcon(QIcon(QString::fromUtf8(":/img/textpointer.png")));
    MenuText->addAction(actionBold);
    MenuText->addAction(actionItalic);
    MenuText->addAction(actionUnderline);
    MenuText->addAction(actionOverline);
    MenuText->addAction(actionStricktext);
    MenuText->addAction(actionstretchfont);
    MenuText->addAction(actionBackColor);
    MenuText->addAction(actionTextColor);
    return MenuText;
}


QMenu *Layoutpainter::BlockMenu( QWidget * inparent )
{
    QMenu *MenuBlock = new QMenu(tr("Block paragraph Format"),inparent);
    MenuBlock->setIcon(QIcon(QString::fromUtf8(":/img/view_text.png")));
    MenuBlock->addAction(actionBlockMargin);
    MenuBlock->addAction(actionAlignLeft);
    MenuBlock->addAction(actionAlignRight);
    MenuBlock->addAction(actionAlignCenter);
    MenuBlock->addAction(actionAlignJustify);
    
    return MenuBlock;
}



QTextCursor Layoutpainter::textCursor() 
{
  return C_cursor;
}


void  Layoutpainter::SetTextBlockMargin()
{
    QTextBlock textblocc = textCursor().block();
        if (textblocc.isValid()) {
            QTextBlockFormat formatibb = textblocc.blockFormat();
             GetMargin *marge = new GetMargin(0);
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


/*

 pnord->setValue(ToUnit(rootformat.topMargin(),"px"));
    psud->setValue(ToUnit(rootformat.bottomMargin(),"px"));
    pwest->setValue(ToUnit(rootformat.leftMargin(),"px"));
    post->setValue(ToUnit(rootformat.rightMargin(),"px"));

*/











void Layoutpainter::BGcolor()
 {
    QRgb col = QColorDialog::getRgba(textCursor().charFormat().background().color().rgb()); 
    QColor col2 = QColor(col);
    int trans = qAlpha(col); 
    col2.setAlpha(trans);
       if (!col2.isValid()) {
        return;
        } 
	  QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();
    format.setProperty(_BG_CHAR_ALPHA_,trans);
    format.setBackground(QBrush(col2));
	  c.setCharFormat(format);
 }
 

void Layoutpainter::TXcolor()
 {
    QRgb col = QColorDialog::getRgba(textCursor().charFormat().foreground().color().rgb()); 
    QColor col2 = QColor(col);
    int trans = qAlpha(col); 
    col2.setAlpha(trans);
       if (!col2.isValid()) {
        return;
        } 
	  QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();
    format.setProperty(_TXT_CHAR_ALPHA_,trans); 
    format.setForeground(QBrush(col2));
	  c.setCharFormat(format);
 }





/* wake up new format from current text !!!! */
void Layoutpainter::NewCharformat( QTextCursor cursor )
{
    QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();
    ////////QTextBlock bf = textCursor().block();
    QFont f = format.font();
     ///////////qDebug() << "### Layoutpainter cursor in .........." << f.bold() << " .............................................";
    actionBold->setChecked( f.bold() );
    actionItalic->setChecked( f.italic() );
    actionUnderline->setChecked(f.underline() );
    MakealignmentChanged(textCursor().block().blockFormat().alignment());
    actionOverline->setChecked( f.overline() );
    actionStricktext->setChecked( f.strikeOut() );
    actionTextColor->setIcon(createColorToolButtonIcon(":/img/textpointer.png",format.foreground().color()));
    actionBackColor->setIcon(createColorToolButtonIcon(":/img/textpointer.png",format.background().color()));
}


void Layoutpainter::MakealignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft)
        actionAlignLeft->setChecked(true);
    else if (a & Qt::AlignHCenter)
        actionAlignCenter->setChecked(true);
    else if (a & Qt::AlignRight)
        actionAlignRight->setChecked(true);
    else if (a & Qt::AlignJustify)
        actionAlignJustify->setChecked(true);
}


void Layoutpainter::MaketextAlign(QAction *a)
{
    QTextBlock bf = textCursor().block();
    QTextBlockFormat format = bf.blockFormat();
    
    Qt::Alignment al = Qt::AlignLeft;
    
    if (a == actionAlignLeft)
        al = Qt::AlignLeft;
    else if (a == actionAlignCenter)
        al = Qt::AlignHCenter;
    else if (a == actionAlignRight)
         al = Qt::AlignRight;
    else if (a == actionAlignJustify)
         al = Qt::AlignJustify;
    
    format.setAlignment(al);
    textCursor().setBlockFormat(format);
}

void Layoutpainter::FosInsertFrame()
{
    QTextFrameFormat frame;
                     frame.setBorder(2);   
                     frame.setBorderBrush(QBrush(Qt::red));  
                     frame.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
                      QTextLength mesure(QTextLength::FixedLength,180); 
                     frame.setWidth(mesure);   
                     frame.setPosition(QTextFrameFormat::FloatRight);   
        textCursor().insertFrame(frame);
    
    
}


void Layoutpainter::ItalicText()
 {
	  QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();
    format.setFontItalic(actionItalic->isChecked());
	  c.setCharFormat(format);
 }
 
void Layoutpainter::UnderlineText()
 {
	  QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();
    format.setUnderlineStyle(actionUnderline->isChecked()  ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline );
	  c.setCharFormat(format);
 }
 
void Layoutpainter::StretchText()
 {
    bool ok;
    int factor = QInputDialog::getInteger(0, tr("Stretch font"),
                                      tr("Percentage:"), 100, 10, 4000, 10, &ok);
    
    if (factor > 0 && factor !=100) {     
	  QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();       
    QFont f = format.font();
    f.setStretch(QFont::UltraExpanded);
    format.setFont(f);
    format.setProperty(_TXT_CHAR_SPACING_,factor); 
	  c.setCharFormat(format);
    }
 }
 
 
 
 void Layoutpainter::OverlineText()
 {
	  QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();
    QFont f = format.font();
    f.setOverline(actionOverline->isChecked() ? true : false);
    format.setFont(f);
	  c.setCharFormat(format);
 }

 
void Layoutpainter::StrickText()
 {
	  QTextCursor c = textCursor();
    QTextCharFormat format = c.charFormat();
    QFont f = format.font();
    f.setStrikeOut(actionStricktext->isChecked() ? true : false);
    format.setFont(f);
	  c.setCharFormat(format);
 }

void Layoutpainter::BoldText()
 {
    QTextCharFormat format = C_cursor.charFormat();
    format.setFontWeight(actionBold->isChecked() ? QFont::Bold : QFont::Normal);
	  C_cursor.setCharFormat(format);
 }







QString Layoutpainter::ImageFilterHaving() const
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
  double gsversion = getGSVersion();
  qDebug() << "### args " << gsversion;
  filterSimple += tr( "Scalable Vector Graphics" ) + " (*.svg *.svg.gz);;";
  if (gsversion > 6.5) {
   filterSimple += tr( "PostScript Vector Graphics" ) + " (*.ps *.eps);;"; 
   filterSimple += tr( "PDF Page units" ) + " (*.pdf);;"; 
  }
  
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

















Layoutpainter::~Layoutpainter()
{
   _d->clear();
   _d->deleteLater(); 

}




QStringList QTextEditMimeData::formats() const
{
    if (!fragment.isEmpty())
        return QStringList() << QString::fromLatin1("text/plain") << QString::fromLatin1("text/html");
    else
        return QMimeData::formats();
}

QVariant QTextEditMimeData::retrieveData(const QString &mimeType, QVariant::Type type) const
{
    if (!fragment.isEmpty())
        setup();
    return QMimeData::retrieveData(mimeType, type);
}

void QTextEditMimeData::setup() const
{
    QTextEditMimeData *that = const_cast<QTextEditMimeData *>(this);
#ifndef QT_NO_TEXTHTMLPARSER
    that->setData(QLatin1String("text/html"), fragment.toHtml("utf-8").toUtf8());
#endif
    that->setText(fragment.toPlainText());
    fragment = QTextDocumentFragment();
}
