#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include "scribeapi.h"
#include "SessionManager.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ApiSession *sx = ApiSession::instance();
    
    Panel *w = new Panel();
    w->show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
