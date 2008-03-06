#ifndef GMAIN_H
#define GMAIN_H

#include "loadconfig.h"
#include "communicator.h"
#include "xmlhighlighter.h"

#define MAGICNUMMERDATA 0x48611928
#define MMVERSION 2

#include "ui_gmain.h"
/*#include "main.h"*/
//
/*  Save file as gmain.h  */
/*  incomming class name Gmain */
//   public QObject | public QHttp | public QThread                                    // 
#include <QPointer>
//
class Gmain : public QMainWindow, public Ui::Gmain
{
     Q_OBJECT
//
public:
 Gmain( QWidget * parent  = 0 );
protected:
 XmlHighlighter *highlight;
 XmlHighlighter *highlight_0;
 Communicator *http_instance;
 QBuffer *buffer_send;
 QBuffer *buffer_incomming;
QString one;
QMap<QString,QString> HeaderSend;
QMap<QString,QString> HeaderIncome;
void closeEvent( QCloseEvent * e );
void BaseInit();
void opendFile(  const QString fixfile );
private:
signals:
public slots:
  void on_CheckXmlValid_clicked();
  void on_SendRequest_clicked();
  void ReadyStream( int headercode );
  void on_WebdavSpecification_triggered();
  void on_LinkGeneratorSoap_triggered();
  void on_actionAbout_app_triggered();
  void on_actionQT_info_triggered();
  void on_SaveAsFileResult_triggered();
  void on_actionOpen_file_triggered();
  void on_DemoRun_triggered();
  void on_StreamLocalFile_clicked();


};
//
#endif // GMAIN_H

