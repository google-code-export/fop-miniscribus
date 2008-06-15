#include <QApplication>
#include "qvcursor.h"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	QVCursor win;
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
