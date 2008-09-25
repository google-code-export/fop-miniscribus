#include "ZipDebugMain.h"
//
/*  Save file as ZipDebugMain.cpp  */
/*  incomming class name ZipDebugMain */
//



#include "OOFormat.h"


ZipDebugMain::ZipDebugMain( QWidget* parent )
	: QMainWindow( parent ),t(0)
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
         
    PushDoc *force = new PushDoc(this);
    Ooo = new OOReader(file);
    Ooo->moveToThread(force);
    connect(force, SIGNAL(started()),Ooo,SLOT(read())); 
    connect(Ooo, SIGNAL(ready()), this, SLOT(drawDoc()));
    connect(Ooo, SIGNAL(statusRead(int,int)), this, SLOT(onRead(int,int)));    
    force->start();
}

void ZipDebugMain::onRead( int now ,int tot )
{
    if (now == tot) {
    qDebug() << "### reading last line -> " << now <<  " t." << tot;
    }
    
}

void ZipDebugMain::drawDoc()
{
    
    if (Ooo) {
     
     if (t) {
      t->setDocument ( Ooo->document() );
      de->setPlainText( Ooo->document()->toHtml("utf-8") );
     } else {
     drawDoc(Ooo->document()); 
     }         
    }
}

void ZipDebugMain::drawDoc( QTextDocument *doc )
{
    
        de = new XMLTextEdit;
        de->setPlainText( doc->toHtml("utf-8") );
        tabWidget->addTab(de,tr("Style"));
    
        QWidget *tabi = new QWidget();
        tabi->setObjectName(QString::fromUtf8("debugdocs"));
        tabi->setObjectName("result");
        QGridLayout *grid = new QGridLayout(tabi);
        t = new QTextEdit(tabi);
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





















