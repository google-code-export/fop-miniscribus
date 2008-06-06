#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{	
	QApplication app(argc, argv);
	MainWindow mainWin(argc,argv);
	mainWin.show();
	return app.exec();
}
