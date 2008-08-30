#include <QtGui>
#include "xslt_convert.h"

int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    QTextEdit t;
    t.show();

    QMap<QString,QString> Params;
    //////Params.insert(QString("ca ne"),QString("Giallo-sera"));
    Xslt_Convert *job = new Xslt_Convert("key.xml","key.xsl",Params);
    t.setPlainText ( job->dmg() );
    return app.exec();
}
