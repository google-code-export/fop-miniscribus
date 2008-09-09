#include <QtGui>
/////#include "qzipreader_p.h"
//////#include "qzipwriter_p.h"
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "ZipDebugMain.h"


int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    ZipDebugMain w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}

//////#include "main.moc"
