#include <QtCore>

#include "animationdevice.h"


int main(int argc, char** argv)
{
    QApplication a( argc, argv );
    QStringList imglist = a.arguments();
    PMovie w;
    w.setWindowTitle(QString("APNG Label (use ContextMenu to play file)"));
    w.show();
    /////FrameIterator *xdev = new FrameIterator(QString("demo.png"));
    return a.exec();
}

