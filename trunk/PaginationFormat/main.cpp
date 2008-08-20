#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QTextEdit>
#include "FoColorName.h"
#include "PageFormatDlg.h"
#include "SessionManager.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    
    ApiSession *sx = ApiSession::instance();

    
    QTextEdit t;
    t.show();
    /////FopColor *fc = new FopColor();
    
    qDebug() << "### load color   ";
 
    

    int rec = PageFormatDlg::self( 0 )->exec();
    
    if (rec == 1) {
        qDebug() << "### responder   " << rec;
    }

    
    /*
    QStringList items = fc->fopListColor();
    for (int i = 0; i < items.size(); ++i)  {
        const QString name = items.at(i);
        QTextCursor c = t.textCursor();
        QPixmap e =  fc->createColorMap(name);
        t.document()->addResource(QTextDocument::ImageResource,QUrl(name),e);
        QTextImageFormat format;
        format.setName( name );
        format.setHeight ( e.height() );
        format.setWidth ( e.width() );
        format.setToolTip(name);
        t.textCursor().insertImage( format );
    }
    */
    
    
    /////////t.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
