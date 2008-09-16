#include "textedit.h"
#include <QApplication>

int main( int argc, char ** argv )
{

    QApplication a( argc, argv );
    
    a.setOrganizationName("Oasis Test Reader");
    a.setOrganizationDomain("QTuser");
    a.setApplicationName("Mini Office");

    
    
    
    TextEdit mw;
    mw.resize( 700, 450 );
    mw.show();
    QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
