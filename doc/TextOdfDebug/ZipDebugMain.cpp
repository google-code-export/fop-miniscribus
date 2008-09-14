#include "ZipDebugMain.h"
//
/*  Save file as ZipDebugMain.cpp  */
/*  incomming class name ZipDebugMain */
//



ZipDebugMain::ZipDebugMain( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
    setWindowTitle(tr("Zip or OpenOffice file debug"));
    setAcceptDrops (true);
    
        textEdit->setAcceptDrops (true);
        textEdit->installEventFilter(this); 

}

bool ZipDebugMain::eventFilter(QObject *obj, QEvent *event) 
{
     if (event->type() == QEvent::DragEnter) {
         QDragEnterEvent *ev = static_cast<QDragEnterEvent *>(event);
            const QMimeData *md = ev->mimeData();
                       if ( md->hasUrls() )  {
                          QList<QUrl> urls = md->urls();
                          for ( int i = 0; i < urls.size(); ++i ) { 
                               QUrl gettyurl(urls.at(i));
                               if (gettyurl.scheme() == "file") {
                                  QFileInfo fi(gettyurl.toLocalFile());
                                  openFile( fi.absoluteFilePath() );
                                  return true;
                               }
                              
                          }
                       }
          return false;
      }
      
      return false;
}


void ZipDebugMain::on_openzip_triggered()
{
     QString file = QFileDialog::getOpenFileName(this, tr( "Choose a zip or a oo file to open" ),"" , "*" );
    if ( file.isEmpty() ) {
    return;
    }
    openFile(file); 
}

void ZipDebugMain::openFile( const QString file )
{
    lineEdit->setText ( file );
    QMap<QString,QByteArray> filist = unzipstream(file);
    tabWidget->clear();
    QByteArray base = filist["content.xml"];
    if (base.size() > 0) {
        drawTab(base,"content.xml");
    }
    QMapIterator<QString,QByteArray> i(filist);
         while (i.hasNext()) {
             i.next();
             QFileInfo fi(i.key());
             const QString ext = fi.completeSuffix().toLower();
             if (ext == "xml" && i.key() != "content.xml") {
               drawTab(i.value(),fi.fileName());
             }
         }   
    Ooo = new OOReader(file);
    connect(Ooo, SIGNAL(ready()), this, SLOT(drawDoc()));
}

void ZipDebugMain::drawDoc()
{
    if (Ooo) {
    drawDoc(Ooo->document()); 
    }
}

void ZipDebugMain::drawDoc( QTextDocument *doc )
{
    
        XMLTextEdit *de = new XMLTextEdit;
        de->setPlainText(Ooo->debugStyle());
        tabWidget->addTab(de,tr("Style"));
    
        QWidget *tabi = new QWidget();
        tabi->setObjectName("result");
        QGridLayout *grid = new QGridLayout(tabi);
        QTextEdit *t = new QTextEdit(tabi);
        t->setDocument ( doc );
        t->setAcceptDrops (true);
        t->installEventFilter(this); 
        t->setObjectName("document");
        grid->addWidget(t, 0, 0, 1, 1);
        tabWidget->addTab(tabi,tr("Document_Debug"));
     
        const int last = tabWidget->count() - 1;
        tabWidget->setCurrentIndex(last);
    
        t->setLineWrapColumnOrWidth(t->lineWrapColumnOrWidth());
}


void ZipDebugMain::drawTab( QByteArray dat ,  const QString file )
{
        QWidget *tabi = new QWidget();
        tabi->setObjectName(file);
        QGridLayout *grid = new QGridLayout(tabi);
        XMLTextEdit *t = new XMLTextEdit(tabi);
        ReadWriteBuf *buf = new ReadWriteBuf();
        buf->write(dat);
        ////QDomDocument 
        if (buf->isValid()) {
        const QString text = QString::fromUtf8(buf->Dom().toByteArray(1).constData());
        t->setPlainText(text);
        }
        t->setAcceptDrops (true);
        t->installEventFilter(this); 
        t->setObjectName(file);
        grid->addWidget(t, 0, 0, 1, 1);
        tabWidget->addTab(tabi,file);
}





















