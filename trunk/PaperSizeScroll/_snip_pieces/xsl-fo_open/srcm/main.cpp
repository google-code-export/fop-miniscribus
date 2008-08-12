
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>


#include "SessionManager.h"
#include "qwwrichtextedit.h"
#include "Fo_Reader.h"

using namespace ApacheFop;

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	
	  a.setOrganizationName("CrossKern");
    a.setOrganizationDomain("crosskern.com");
    a.setApplicationName("Miniscribus Beta Engine");
	
  SessionManager *session = SessionManager::instance();
	///////QMap<int,M_PageSize> Alist = session->mpages();
	/////////qDebug() << "### main" << session->FontList();
  QwwRichTextEdit w;
	w.resize (650,550);
	w.show();
	///////////Fo_Reader *fapi = new Fo_Reader("a.fo");
	////////////w.setDocument ( fapi->document() );
	
  
  QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
