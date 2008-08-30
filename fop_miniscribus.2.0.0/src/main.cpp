#include <QtGui>

#include "Config.h"
#include "SessionManager.h"
#include "MainWindow.h"

int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    app.setOrganizationName("CrossKern");
    app.setOrganizationDomain("crosskern.com");
    app.setApplicationName(_APPLICATIONS_NAME_);
    
    //////////////qDebug() << "### main start--------------- ";
    
    ApiSession *sx = ApiSession::instance();
    MainWindow *gmain = new MainWindow();
    gmain->show();
    return app.exec();
}
