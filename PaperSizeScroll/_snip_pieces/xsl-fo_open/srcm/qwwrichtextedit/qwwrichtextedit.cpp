#include "qwwrichtextedit.h"
#include <QKeyEvent>
#include <QToolBar>
#include <QResizeEvent>
#include <QAction>
#include <QFontComboBox>
#include <QTextList>
#include <QSettings>
#include "Fo_Reader.h"
#include "oo_handler.h"
#include "Fo_Writter.h"


using namespace ApacheFop;





/**
 *  @class QwwRichTextEdit
 *
 *
 *
 *
 *
 */

QwwRichTextEdit::QwwRichTextEdit(QWidget *parent) : QTextEdit(parent){
    currentList = 0;
  
    M_PageSize Opage;
    DocumentSizeFop = Opage;
  
  
    tb = new QToolBar(this);
    tb->setIconSize(QSize(24,24));
  
    acfoppdf = new QAction(QIcon(QPixmap(":/img/apachefop_logo.png")), "Fop File", this);
    connect(acfoppdf, SIGNAL(triggered()), this, SLOT(fop2pdf()));
    acfoppdf->setEnabled ( false );
    tb->addAction(acfoppdf);
  
  
  
  
    opfo = new QAction(sysicon.icon(QFileIconProvider::Folder), "Open Document", this);
    connect(opfo, SIGNAL(triggered()), this, SLOT(openfopdocument()));
    tb->addAction(opfo);
  
    savf = new QAction(QIcon(QPixmap(":/img/filesave.png")), "Save as Document", this);
    connect(savf, SIGNAL(triggered()), this, SLOT(SaveFopFormat()));
    tb->addAction(savf);
    
    
    
    
    
  
    fcb = new QFontComboBox(this);
    tb->addWidget(fcb);
    connect(fcb, SIGNAL(activated(int)), this, SLOT(setFont_h()));
    fsp = new QComboBox(this);
    tb->addWidget(fsp);
    connect(fsp, SIGNAL(activated(const QString &)), this, SLOT(setFont_h()));
    foreach(int s, QFontDatabase::standardSizes())
    fsp->addItem(QString::number(s));
    
    
    color1 = new QAction(createColorIcon(Qt::black), "Text Color", this);
    connect(color1, SIGNAL(triggered()), this, SLOT(textcolor()));
    tb->addAction(color1);
    
    color2 = new QAction(createColorIcon(Qt::white), "Background Color", this);
    connect(color2, SIGNAL(triggered()), this, SLOT(bgcolor()));
    tb->addAction(color2);
    

    ba = new QAction(QIcon(QPixmap(":/img/textbold.png")), "Bold", this);
    ba->setCheckable(true);
    tb->addAction(ba);
    connect(ba, SIGNAL(triggered(bool)), this, SLOT(setBold(bool)));
    ia = new QAction(QPixmap(":/img/textitalic.png"), "Italic", this);
    ia->setCheckable(true);
    tb->addAction(ia);
    connect(ia, SIGNAL(triggered(bool)), this, SLOT(setItalic(bool)));
    ua = new QAction(QPixmap(":/img/textunder.png"), "Underline", this);
    ua->setCheckable(true);
    tb->addAction(ua);
    tb->addSeparator();
    connect(ua, SIGNAL(triggered(bool)), this, SLOT(setUnderline(bool)));
    al = new QAction(QPixmap(":/img/textleft.png"), "Align left", this);
    ar = new QAction(QPixmap(":/img/textright.png"), "Align right", this);
    ac = new QAction(QPixmap(":/img/textcenter.png"), "Center", this);
    aj = new QAction(QPixmap(":/img/textjustify.png"), "Justify", this);
    QActionGroup *alignmentGroup = new QActionGroup(this);
    al->setCheckable(true);
    ar->setCheckable(true);
    ac->setCheckable(true);
    aj->setCheckable(true);
    alignmentGroup->addAction(al);
    alignmentGroup->addAction(ar);
    alignmentGroup->addAction(ac);
    alignmentGroup->addAction(aj);
    tb->addAction(al);
    tb->addAction(ac);
    tb->addAction(ar);
    tb->addAction(aj);
    tb->addSeparator();

    connect(alignmentGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeAlignment(QAction*)));

    li = new QAction(QPixmap(":/img/unordered-list.png"), "List", this);
    tb->addAction(li);
    li->setCheckable(true);
    connect(li, SIGNAL(toggled(bool)), this, SLOT(setList(bool)));



    setViewportMargins(0, tb->sizeHint().height()+1, 0, 0);
    setContentsMargins(0, tb->sizeHint().height(), 0, 0);
    connect(this, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
            this, SLOT(updateCurrentCharFormat(const QTextCharFormat &)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCurrentBlockFormat()));
    updateCurrentBlockFormat();
    updateCurrentCharFormat(textCursor().charFormat());
  }

void QwwRichTextEdit::changeAlignment(QAction *a){
    QTextCursor cur = textCursor();
    QTextBlockFormat fmt;
    if(a==al)
      fmt.setAlignment(Qt::AlignLeft);
    else if(a==ar)
      fmt.setAlignment(Qt::AlignRight);
    else if(a==ac)
      fmt.setAlignment(Qt::AlignCenter);
    else if(a==aj)
      fmt.setAlignment(Qt::AlignJustify);
    cur.mergeBlockFormat(fmt);
    setTextCursor(cur);
  }
  void QwwRichTextEdit::setBold(bool v){
    QTextCursor cur = textCursor();
    QTextCharFormat fmt;
//    fmt.setFontWeight(QFont::Bold);
//    cur.mergeCharFormat(fmt);
     fmt.setFontWeight(!v ? QFont::Normal : QFont::Bold);
     cur.mergeCharFormat(fmt);
    setTextCursor(cur);
  }
  void QwwRichTextEdit::setItalic(bool v){
    QTextCursor cur = textCursor();
    QTextCharFormat fmt;
    fmt.setFontItalic(v);
    cur.mergeCharFormat(fmt);
    setTextCursor(cur);
  }
  void QwwRichTextEdit::setUnderline(bool v){
    QTextCursor cur = textCursor();
    QTextCharFormat fmt;
    fmt.setFontUnderline(v);
    cur.mergeCharFormat(fmt);
    setTextCursor(cur);
  }
  void QwwRichTextEdit::setColor(const QColor &c)
{
    QTextCursor cur = textCursor();
    QTextCharFormat fmt;
    fmt.setForeground(c);
    cur.mergeCharFormat(fmt);
    setTextCursor(cur);
}

  void QwwRichTextEdit::setFont_h(){
    QFont f = fcb->currentFont();
    f.setPointSize(fsp->currentText().toInt());
    setFont(f);
    setFocus();

  }
  void QwwRichTextEdit::setFont(const QFont &f){
    QTextCursor cur = textCursor();
    QTextCharFormat fmt;
    fmt.setFontFamily(f.family());
    fmt.setFontPointSize(f.pointSize());
    cur.mergeCharFormat(fmt);
    setTextCursor(cur);
    if(fcb->currentFont().family()!=f.family())
        fcb->setCurrentFont(f);
    if(f.pointSize()!=fsp->currentText().toInt()){
      fsp->setCurrentIndex(fsp->findText(QString::number(f.pointSize())));
    }
  }
  void QwwRichTextEdit::setList(bool v){
    QTextCursor cur = textCursor();
    if(v){
      QTextListFormat listFormat;
      listFormat.setStyle(QTextListFormat::ListDisc);
      currentList = cur.createList(listFormat);
    } else {
      cur.setBlockFormat(QTextBlockFormat());
  //    cur.movePosition(QTextCursor::NextBlock);
//      cur.insertBlock(QTextBlockFormat());
      setTextCursor(cur);
      currentList = 0;
    }
  }


  void QwwRichTextEdit::updateCurrentCharFormat(const QTextCharFormat &fmt)
  {
    ba->setChecked(fmt.fontWeight()==QFont::Bold);
    ia->setChecked(fmt.fontItalic());
    ua->setChecked(fmt.fontUnderline());
    color1->setIcon ( createColorIcon(fmt.foreground().color()) );
    color2->setIcon ( createColorIcon(fmt.background().color()) );
    /*
        color1 = new QAction(createColorIcon(Qt::black), "Text Color", this);
    connect(color1, SIGNAL(triggered()), this, SLOT(textcolor()));
    tb->addAction(color1);
    
    color2 = new QAction(createColorIcon(Qt::white), "Background Color", this);
    */
    

    //if(fmt.font().family()!=fcb->currentFont().family()){
    //  fcb->setCurrentFont(fmt.font());
    //}
  }
  void QwwRichTextEdit::updateCurrentBlockFormat()
  {
    QTextBlockFormat fmt = textCursor().blockFormat();
    switch(fmt.alignment()){
      case Qt::AlignLeft: al->setChecked(true); break;
      case Qt::AlignRight: ar->setChecked(true); break;
      case Qt::AlignCenter: ac->setChecked(true); break;
      case Qt::AlignJustify: aj->setChecked(true); break;
      default: al->setChecked(true);
    }
    if(!textCursor().hasSelection())
      li->setChecked(textCursor().currentList()!=0);
//    currentList = textCursor().currentList();
    QTextCharFormat cf = textCursor().charFormat();
    if(cf.font().family()!=fcb->currentFont().family()){
      fcb->setCurrentFont(cf.font());
    }
    if(cf.font().pointSize()!=fsp->currentText().toInt()){
      fsp->setCurrentIndex(fsp->findText(QString::number(cf.font().pointSize())));
    }

  }

  bool QwwRichTextEdit::event(QEvent *e){
    if(e->type()!=QEvent::KeyPress)
      return QTextEdit::event(e);
    QKeyEvent *ke = (QKeyEvent*)e;
    currentList = textCursor().currentList();
    if(currentList){
      if(ke->key()==Qt::Key_Tab || ke->key() ==Qt::Key_Backtab){
        QTextCursor cur = textCursor();
        QTextListFormat listFormat = currentList->format();
        listFormat.setIndent(currentList->format().indent()+ (ke->key()== Qt::Key_Tab ? 1 : -1));
//        listFormat.setStyle(QTextListFormat::ListDisc);
        currentList = cur.createList(listFormat);
        return true;
      }
    }
    return QTextEdit::event(e);
  }
  void QwwRichTextEdit::resizeEvent(QResizeEvent *re){
    tb->setGeometry(0,0,width(), tb->sizeHint().height());
    QTextEdit::resizeEvent(re);
  }
  
  void QwwRichTextEdit::OpenFop(  const QString file  )
  {
    Fo_Reader *fapi = new Fo_Reader(file,this);
    DocumentSizeFop = fapi->PSize();
    qDebug() << "### Register name." << DocumentSizeFop.name; 
    setDocument ( fapi->document() );
    CurrentFopFile = QFileInfo(file);
    acfoppdf->setEnabled ( true );
  }
  
  void QwwRichTextEdit::OpenHtml(  const QString file  )
  {
    const QString chunk = ReadFile(file);  /* utf8 -read */
    setHtml(chunk);
    M_PageSize Opage;
    DocumentSizeFop = Opage;
  }
  
  
  
  void QwwRichTextEdit::OpenOOO(  const QString file  )
  {
    OOO::OO_Handler *oodoc = new OOO::OO_Handler(file);
    SessionManager *session = SessionManager::instance();   /* register image on buffer */
    M_PageSize Opage;
    DocumentSizeFop = Opage;
    
      if (oodoc->ParseFiles()) {
				  QString htmloooo = oodoc->document()->toHtml("utf-8");
					QMap<QString,QByteArray> pics = oodoc->imagelist();
				  QMap<QString,SPics> list;
				
				   QMapIterator<QString, QByteArray> it( pics );
						while ( it.hasNext() ) {
							 it.next();
							 QString oldname = it.key();
							 const int charsi = oldname.size() - 9 - 3;
							 QString newnamepic;
															 if (oldname.size() > 20) {
																 newnamepic = Imagename(oldname.mid(9,charsi));
															 } else {
																 newnamepic = Imagename(oldname);
															 }
							 htmloooo = htmloooo.replace(it.key(),newnamepic);
							 QPixmap io;
							         io.loadFromData(it.value());
							 SPics base;
							 SPics image;
							       image.name = newnamepic;
							       image.set_pics( io ); /* set error image if null automatic */
										 image.name = newnamepic;
                     image.info = "Import OOO image";
							 list.insert(newnamepic,image);
               session->current_pics.insert(newnamepic,image);
						}
      RichDoc xx;
      xx.Register(htmloooo,list);
      setDocument ( xx.todoc() );
      }
   
  }
  
  
  void QwwRichTextEdit::openfopdocument()
  {
    QString filter;
    filter = tr( "All supported Types" ) + " (";
    filter += tr( "Apache fop format" ) + " (*.fo *.fop);;"; 
    filter += tr( "Miniscribus Page format" ) + " (*.page);;"; 
    filter += tr( "Miniscribus Layer format append modus" ) + " (*.layer);;"; 
    filter += tr( "Html format" ) + " (*.xhtml *.html *.htm);;"; 
    filter += tr( "OpenOffice file" ) + " (*.odt *.ott);;"; 
    QString file = QFileDialog::getOpenFileName(this, tr( "Choose Open Office File..." ), QString(setter.value("LastDir").toString()) ,filter); /// ott // sdw !
	  if (file.size() > 0) {
      
      acfoppdf->setEnabled ( false );
      
      QFileInfo finfo(file);
      setter.setValue("LastDir",finfo.absolutePath());
      qDebug() << "### open file  " << finfo.absoluteFilePath();
      const QString exte = finfo.completeSuffix().toLower();
      if (exte == "fo" || exte == "fop") {
      OpenFop(finfo.absoluteFilePath());
      } else if (exte == "odt" || exte == "ott") {
      OpenOOO(finfo.absoluteFilePath());
      } else if (exte == "xhtml" || exte == "html" || exte == "htm") {
        OpenHtml(finfo.absoluteFilePath());
      } else if (exte == "layer") {
        QFile f(finfo.absoluteFilePath()); 
				if (f.open(QFile::ReadOnly | QFile::Text)) {
        RichDoc addoc = OpenRichDoc(QString::fromUtf8(f.readAll()));
				addoc.margin(false);
				f.close();
        AppendLayerDoc(addoc);
        }
      }
      
    }
    DocRootFrameDefault();
  }
  
  
  void QwwRichTextEdit::SaveFopFormat()
  {
    FopDom *docfop = new FopDom(document(),DocumentSizeFop,FOP_APACHE);  
    QDomDocument fdoc = docfop->Foxml();
    fwriteutf8("resu.fo",fdoc.toString(0));
    fwriteutf8("resu.html",document()->toHtml("utf-8"));
  }
  
  
  void QwwRichTextEdit::fop2pdf()
  {
    if (  !CurrentFopFile.exists() ) {
    return;
    }
    QString fopapplication, foptipe, impdf, errfoop;
    #if defined Q_WS_WIN
    foptipe = "Bat file";
    #endif
    #if defined Q_WS_X11
    foptipe = "sh script";
    #endif
    #if defined Q_WS_MAC
    foptipe = "sh script";
    #endif
    qreal jsversion = JavaVersion();
    const QString resuteil = CurrentFopFile.absoluteFilePath();
    qDebug() << "### jsversion " << jsversion;
    qDebug() << "### foxmlfile " << resuteil;
    
    if (jsversion < 1.5) {
    QMessageBox::warning(this, tr("Fop convert"),tr("Java version found:% 1 install java version 1,6 > to fop file").arg(jsversion));
    return;
    }
    QString exfop = QString(setter.value("FopApplicationfi").toString());
    /////QSettings setter;
    if (exfop.size() < 1) {
    exfop = QFileDialog::getOpenFileName(this, tr("Place of Fop \"%1\" application ").arg(foptipe),"",tr("fop (*)"));
    setter.setValue("FopApplicationfi",exfop);  /* modus on cms not editor !!! */
    }
    if (exfop.size() < 1) {
    return;
    }
    
          if (exfop.size() > 1) {
            QString msgDB =tr("Use fop %2 location path = %1 ?").arg(exfop).arg(foptipe);
            int removeyes = QMessageBox::question(this, tr("Pleas Confirm!"),msgDB,tr("&Yes"), tr("&No"),QString(),8888,9999);
            QString responder = QString::number(removeyes);
             if (responder =="0")  {
                /* use old is ok...... */
             } else {
              exfop ="";
              exfop = QFileDialog::getOpenFileName(this, tr("Place of Fop \"%1\" application ").arg(foptipe),"",tr("fop (*)"));  
                 if (exfop.size() < 1) {
                  return;
                 }
                 setter.setValue("FopApplicationfi",exfop);  /* modus on cms not editor !!! */
             }
         }
         
         
                /* save location */
            impdf = QFileDialog::getSaveFileName(this, "Save as",QString(setter.value("LastDir").toString()), "Pdf  (*.pdf)");
            if (impdf.endsWith(".pdf")) {
             /* ok */
            } else {
            impdf = impdf+".pdf";  
            }
         
    if (impdf.size() < 6) {
    return;
    }
         
    
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            QProcess process;
            process.setReadChannelMode(QProcess::MergedChannels);
            process.start( exfop ,  QStringList() << "-fo" << resuteil << "-pdf" << impdf);
                         if (!process.waitForFinished()) {
                          QApplication::restoreOverrideCursor(); 
                          errfoop = QString(process.errorString());
                          QMessageBox::critical(this, tr("Error by XSLT-FO Apache"),tr("Unable to convert Your file!\nError %1...").arg(errfoop.left(30)));
                          ShowText(errfoop);
                          return;
                         } else {
                            QApplication::restoreOverrideCursor();
                             errfoop = QString(process.readAll());
                             if (errfoop.size() > 0) {
                             ShowText(errfoop);
                             } 
                             QFileInfo pdfr(impdf);
                             if (pdfr.exists()) {
                             OpenDeskBrowser(QUrl(impdf));
                             }
                           return;  
                         }
            
  QApplication::restoreOverrideCursor();
  }
  
  void QwwRichTextEdit::textcolor()
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
    format.setForeground(QBrush(col2));
	  c.setCharFormat(format);
  }
  /* char block root frame table ??? */
  void QwwRichTextEdit::bgcolor()
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
    format.setBackground(QBrush(col2));
	  c.setCharFormat(format);
  }
  
  
void QwwRichTextEdit::DocRootFrameDefault()
{
  SessionManager *session = SessionManager::instance();
  // DocumentSizeFop
  session->CurrentDocWidth = DocumentSizeFop.HandleDocument( document() );
  document()->adjustSize();
  setLineWrapColumnOrWidth(lineWrapColumnOrWidth());
  update();
}

  
void QwwRichTextEdit::ShowText( QString txt )
{
              
    qDebug() << "### ShowText " << txt;
}  

void QwwRichTextEdit::AppendLayerDoc( RichDoc ldoc )
{
        SessionManager *session = SessionManager::instance(); 
        QTextCursor c = textCursor();
        QTextDocument *str = ldoc.todoc();  
        QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(str->toHtml("utf-8"));
        c.insertFragment(fragment);
  
  
           QMapIterator<QString,SPics> i(ldoc.resource);
		          while (i.hasNext()) {
               i.next();
				       SPics record = i.value();
				       document()->addResource( QTextDocument::ImageResource,QUrl(record.name),record.pix());
               session->current_pics.insert(record.name,record); 
              }  
              
         
}











