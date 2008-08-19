#include <QtGui>
#include "Paper_Text_Edit.h"
#include "Config.h"
#include "SessionManager.h"


#include "Fo_Reader.h"

using namespace ApacheFop;


int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    
    app.setOrganizationName("CrossKern");
    app.setOrganizationDomain("crosskern.com");
    app.setApplicationName(_APPLICATIONS_NAME_);
    
    ApiSession *sx = ApiSession::instance();
    
    
    
    PaperTextEdit p;
    p.show();
    
    

    
    /////////QTextEdit t;
   ///////////// t.show();

    ////QMap<QString,QString> Params;
    //////Params.insert(QString("ca ne"),QString("Giallo-sera"));
    //////Xslt_Convert *job = new Xslt_Convert("key.xml","key.xsl",Params);
    //////t.setPlainText ( job->dmg() );
    
   ///// Fo_Reader * fops = new Fo_Reader("ademo.fop");
    /////////t.setDocument (fops->document() );
    
    return app.exec();
}
