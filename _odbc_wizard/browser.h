#ifndef BROWSER_H
#define BROWSER_H



#include <QSqlQuery>
#include <QWidget>
#include "ui_browserwidget.h"
#include "qbasefile.h"
#include "gui_about.h"
#include "main.h"
#include <QSettings>
#include "sincro.h"

#include <QtGui>
#include <QtSql>
#include <QProgressDialog>




class ConnectionWidget;
class QTableView;
class QPushButton;
class QTextEdit;
class QSqlError;
class QSqlTableModel;

class Browser:  public Base_QBaseFile, private Ui::Browser
{
    Q_OBJECT
public:
    Browser(QWidget *parent);
    virtual ~Browser();

    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                  const QString &user, const QString &passwd, int port = -1);

    void insertRow();
    void deleteRow();
    void updateActions();
    void ExportXmlPrimary( QString qx );
    void ExportCSVPrimary( QString qx );
    void ExportSqlite3( QString qx );
    void ExportMysql( QString qx );

public slots:
    void exec();
    void showTable(const QString &table);
    void showMetaData(const QString &table);
    void addConnection();
    void currentChanged() { updateActions(); }

    void on_insertRowAction_triggered()
    { insertRow(); }
    void on_deleteRowAction_triggered()
    { deleteRow(); }
    void on_connectionWidget_tableActivated(const QString &table)
    { showTable(table); }
    void on_connectionWidget_metaDataRequested(const QString &table)
    { showMetaData(table); }
    void on_submitButton_clicked()
    {
        exec();
        sqlEdit->setFocus();
    }
    void on_clearButton_clicked()
    {
        sqlEdit->clear();
        forums->clear();
        sqlEdit->setFocus();
    }
    
    void abouthere()
    {
        Gui_About::self( this )->exec();
    }
     void reportbug()
    {
        #if defined(Q_WS_WIN)
        OpenUrl_File_Dir_Dektop(BUG_REPORT);
        #endif
        #if defined Q_WS_MAC
        OpenUrl_File_Dir_Dektop("mailto:ciz@ciz.ch");
        #endif
    }
    
    /* open file url mail ecc on other programm */
void OpenUrl_File_Dir_Dektop( QString item )
{
    #if defined(Q_WS_WIN)
    QProcess p;
    QStringList s;
    s << "url.dll,FileProtocolHandler" << item;
    p.startDetached(QString("rundll32.exe") , s );
    #endif
    #if defined Q_WS_MAC
    QProcess m;
    QStringList macs;
    macs << item;  /* oeffnet der default browser */
    m.startDetached(QString("open") , macs );
    #endif 
}
    void ConfigSincroWizard()
    {
                 
                 openwsin->SetConnection(&connectionWidget->currentDatabase());
                 openwsin->remama->clear();
                 openwsin->ReadConfigIn();
                 openwsin->exec();
      
         
    }
    
    void GoSetQuery( QString r )
    {
        //////////////////QString lastqe = setter.value("LastQuery").toString();
        
                QString make = openwsin->remama->document()->toPlainText();
        
                sqlEdit->clear();              
                sqlEdit->append(r);
                sqlEdit->toPlainText();
                
    }
    
    void Export_Xml( QString qx , QString table )
    {           
        /* SELECT RID,AHV,AUTOUP,REGART,BERUFSCODE,MUTATION FROM back_up_reg LIMIT 50 */
        /* ########################################################################################## */
        bool okformat;
        if (table.size() > 0) {
        runningtable = table;
        }
        QStringList items;
        items << "XML" << "CSV" << "SQLITE3" << "MYSQL";
        QString msgDB =tr("You like to export last \"Query\" from table \"%1\"?").arg(runningtable);
        int removeyes = QMessageBox::question(this, tr("Please Confirm:"),msgDB,tr("&Yes"), tr("&No"),QString(),8888,9999);
        QString responder = QString::number(removeyes);
        if (responder =="0")  {
        QString form = QInputDialog::getItem(this, tr("Select one export format..."),tr("Format:"), items, 0, false, &okformat);
                    if (okformat && !form.isEmpty()) {
                        if (form == "XML") {
                            ExportXmlPrimary(qx);
                        } else if (form == "CSV") {
                            ExportCSVPrimary(qx);
                        } else if (form == "MYSQL") {
                            ExportMysql(qx);
                        } else if (form == "SQLITE3") {
                            ExportSqlite3(qx);
                        }
                    }
        }
         
    QApplication::restoreOverrideCursor();
    }
    void  SendToArea( QString t , int modus  );
    
    /* EXPORT_FIELD_NAME */
void SendDialog( int bytesRead, int totalBytes )
{
    qDebug() << "### status "  << totalBytes << " " << bytesRead;
    
   if (bytesRead == totalBytes) {
           if (dlg->isVisible ()) {
              dlg->close();
           }
   }
   
   dlg->setMaximum(totalBytes);
   dlg->setValue(bytesRead);
}

signals:
    void statusMessage(const QString &message);
    void registerQuerySuccess(const QString &message,const QString &tablename);
    void displaystatus(int re , int tot );
private:
    QSqlTableModel *model;
    QProgressDialog *dlg;
    Sincro  *openwsin;
    QString last_query;
    QString runningtable;
    QStringList tablelisten;
    QSettings setter;
    QString Strtrimmed( QString t );
    QString Strcsvtrimmed( QString t );
    QString StructureTable( QSqlRecord now );
    QString StructureMYSQLTable( QSqlRecord now );
    QString Strsqltrimmed( QString t );
    QString encodeBase64( QString xml );
    QString decodeBase64( QString xml );
};

#endif
