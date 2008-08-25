#include "BookTree.h"

#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QTextEdit>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    BookTree t;
    t.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
