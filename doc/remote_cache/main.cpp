
#include <QApplication>
#include <QTextEdit>

#if QT_VERSION >= 0x040500
#include "FillCache.h"
#endif


#define Set_ApplicationTitle\
             QString("Remote Cache Sample")




int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    a.setOrganizationName("CrossKern");
    a.setOrganizationDomain("crosskern.com");
    a.setApplicationName("Cache Loader");
    QString msg = "Running only on qt version 4.5 >";
    #if QT_VERSION >= 0x040500
    NetCacheSwap *netswap = new NetCacheSwap();
    
    msg="Loading....";
    #endif
    QTextEdit mw;
    mw.setWindowTitle(Set_ApplicationTitle);
    mw.resize( 700, 450 );
    mw.show();
    mw.setText ( msg );
    QObject::connect(netswap, SIGNAL(inTextUtf(QString)), &mw, SLOT(setPlainText(QString)));
    
    
    QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}








