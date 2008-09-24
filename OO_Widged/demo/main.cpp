#include "textedit.h"
#include <QApplication>

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    a.setOrganizationName("Oasis Test Reader");
    a.setOrganizationDomain("QTuser");
    a.setApplicationName("Mini Office");
    #if QT_VERSION >= 0x040500
    qDebug() << "### QT_VERSION main  -> " << QT_VERSION;
    qDebug() << "### QT_VERSION_STR main -> " << QT_VERSION_STR;
    #endif
    TextEdit mw;
    mw.resize( 700, 450 );
    mw.show();
    QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
