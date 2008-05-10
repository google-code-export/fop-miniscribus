
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>

#include <QTextBrowser>
#include "oo_handler.h"
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>
#include <QFileDialog>

using namespace OOO;


class Ui_MainWindow
{
public:
    QAction *actionOpen_OO_document;
    QAction *actionExit;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(800, 600);
    actionOpen_OO_document = new QAction(MainWindow);
    actionOpen_OO_document->setObjectName(QString::fromUtf8("actionOpen_OO_document"));
    actionExit = new QAction(MainWindow);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    textBrowser = new QTextBrowser(centralwidget);
    textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

    gridLayout->addWidget(textBrowser, 0, 0, 1, 1);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 800, 21));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    menubar->addAction(menuFile->menuAction());
    menuFile->addAction(actionOpen_OO_document);
    menuFile->addAction(actionExit);

    retranslateUi(MainWindow);
    QObject::connect(actionExit, SIGNAL(activated()), MainWindow, SLOT(close()));

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "OpenOffice documents Display", 0, QApplication::UnicodeUTF8));
    actionOpen_OO_document->setText(QApplication::translate("MainWindow", "Open OO document", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui



class  MainWindow : public QMainWindow, public Ui:: MainWindow
{
    Q_OBJECT
//
public:
  MainWindow( QWidget * parent = 0 )
{
 setupUi( this );
 QObject::connect(actionOpen_OO_document, SIGNAL(activated()),this, SLOT(opendoc()));
}
protected:
private:
signals:
public slots:
void opendoc()
{
    QString file = QFileDialog::getOpenFileName(this, tr( "Choose Open Office File..." ), QString() ,tr("OO doc (*.odt *.ott)")); /// ott // sdw !
    if (file.size() > 0) {
    OO_Handler *oodoc = new OO_Handler(file);
      if (oodoc->ParseFiles()) {
       textBrowser->setDocument ( oodoc->document() ); 
      }
        
    }
}
};





/* catch event from QCoreApplication if haveit more */
#define osApplication \
    (static_cast<OS_application*>(QCoreApplication::instance()))
class OS_application : public QApplication
{
    Q_OBJECT
//
public:
 OS_application( int &argc, char **argv )
  : QApplication(argc, argv)
{
    windowTitle = QString ( "OpenOffice document convert to QTextDocument" );
  
    MainWindow *w = new MainWindow;
    w->setWindowTitle(windowTitle);
    w->show();
    

}
protected:
private:
   QString windowTitle; 
public slots:
void opendoc()
{
    /// OO_Handler *oodoc = new OO_Handler("school_project_svk_lang.odt");
      /////if (oodoc->ParseFiles()) {
       ////w->setDocument ( oodoc->document() ); 
     ////}
}

};


int main(int argc, char *argv[])
{
    OS_application a(argc, argv);
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}

#include "main.moc"
