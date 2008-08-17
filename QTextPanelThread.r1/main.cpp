#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>

#include "panel.h"
#include "qtextpanel.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Panel *w = new Panel();
	w->show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
