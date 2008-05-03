#include "GraphicsView.h"
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QGraphicsView>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    a.setOrganizationName("CrossKern");
    a.setOrganizationDomain("crosskern.com");
    a.setApplicationName("Layer Handler Sample");
    GraphicsView panel;
    panel.showMaximized();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
