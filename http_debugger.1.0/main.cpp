#include "gmain.h"
#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("CrossKern");
    a.setOrganizationDomain("crosskern.com");
    a.setApplicationName("Http qt debugger");
    Gmain w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}


