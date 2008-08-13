#include <QtGui>
#include "Paper_Text_Edit.h"




int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    
    Panel *paperedit = new Panel();
    paperedit->show();
    
    ////QTextEdit t;
    //////t.show();

    ////QMap<QString,QString> Params;
    //////Params.insert(QString("ca ne"),QString("Giallo-sera"));
    //////Xslt_Convert *job = new Xslt_Convert("key.xml","key.xsl",Params);
    //////t.setPlainText ( job->dmg() );
    
    ///////Fo_Reader * fops = new Fo_Reader("a.fo");
    //////t.setDocument (fops->document() );
    
    return app.exec();
}
