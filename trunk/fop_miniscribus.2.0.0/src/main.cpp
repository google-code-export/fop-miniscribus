#include <QtGui>

#include "Config.h"
#include "SessionManager.h"
#include "MainWindow.h"


#if defined _COMPOSE_STATIC_
#include <QtPlugin>
#if defined _USE_qjpeg
Q_IMPORT_PLUGIN(qjpeg)
#endif
#if defined _USE_qmng
Q_IMPORT_PLUGIN(qmng)
#endif
#if defined _USE_qgif
Q_IMPORT_PLUGIN(qgif)
#endif
#if defined _USE_qtiff
Q_IMPORT_PLUGIN(qtiff)
#endif
#endif









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
