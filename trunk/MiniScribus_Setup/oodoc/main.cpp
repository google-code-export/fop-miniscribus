
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>

#include <QTextBrowser>
#include "oo_handler.h"

using namespace OOO;


/* catch event from QCoreApplication if haveit more */
#define osApplication \
    (static_cast<OS_application*>(QCoreApplication::instance()))
class OS_application : public QApplication
{
    Q_OBJECT
//
public:
 OS_application( int &argc, char **argv )
  : QApplication(argc, argv)
{
    windowTitle = QString ( "OpenOffice document" );
    OO_Handler *oodoc = new OO_Handler("school_project_svk_lang.odt");
    
    QTextBrowser *w = new QTextBrowser;
    w->show();
    w->setOpenExternalLinks ( true );
    if (oodoc->ParseFiles()) {
       w->setDocument ( oodoc->document() ); 
    }
    /////w->setPlainText(oodoc->text());

}

protected:
private:
   QString windowTitle; 
};


int main(int argc, char *argv[])
{
    OS_application a(argc, argv);
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}

#include "main.moc"
