


#include "main.h"
#include "browser.h"

#include <QtCore>
#include <QtGui>
#include <QtSql>

#if defined Q_WS_MAC  
#include <QSqlDriverPlugin>
#include <QtPlugin>
Q_IMPORT_PLUGIN(qsqlite)
Q_IMPORT_PLUGIN(qsqlodbc)
Q_IMPORT_PLUGIN(qsqlmysql)
#endif 


///////  QTPLUGIN     += qsqlite qsqlodbc
 








void addConnectionsFromCommandline(const QStringList &args, Browser *browser)
{
    for (int i = 1; i < args.count(); ++i) {
        QUrl url(args.at(i), QUrl::TolerantMode);
        if (!url.isValid()) {
            qWarning("Invalid URL: %s", qPrintable(args.at(i)));
            continue;
        }
        QSqlError err = browser->addConnection(url.scheme(), url.path().mid(1), url.host(),
                                               url.userName(), url.password(), url.port(-1));
        if (err.type() != QSqlError::NoError)
            qDebug() << "Unable to open connection:" << err;
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
        QCoreApplication::setOrganizationName(_ORGANIZATION_NAME_);
        QCoreApplication::setOrganizationDomain(_PROGRAM_NAME_DOMAINE_);
        QCoreApplication::setApplicationName(_PROGRAM_NAME_);
    
    
    QDir dira(WORK_CACHEDIR);
    if ( dira.mkpath(WORK_CACHEDIR) ) { } else {
    QString ERROR_MSG = QString("ERROR! on not Possibel to make dir \"%1\"\n").arg(WORK_CACHEDIR );
        QMessageBox::information(0, QObject::tr("Error!"),ERROR_MSG);
    }
       QMainWindow mainWin;
       mainWin.setWindowTitle(_PROGRAM_TITLE_);
       Browser browser(&mainWin);
       mainWin.setCentralWidget(&browser);
    
    
       /////////QString psdf = QString::fromUtf8(":/img/revert.png");
    
       //////QFile fp(psdf);
          ////fp.copy(BUG_REPORT);
      /*
      QFile fconf( EXPORT_FIELD_NAME );
            if (!fconf.exists(EXPORT_FIELD_NAME)) {
    
      QString listo = "RID,AUTOUP,REGART,BERUFSCODE,EINTRAGSNR,KANTON,EURING,ANREDE,TITEL,NAME,NNAME,VNAME,KEYNAME,CO1,STRASSE,LAND,PLZX,PLZ,ORT,TELP,TELG,FAX,NATEL,SEX,BUERGERORT,GEBURTSDAT,GEBJAHR,SPRACHE,DOSSIER,BILDUNG,DIPLOME,STELLUNG,VERBAENDE,EINTRAGDAT,EINTRAG1,EINTRAG2,STREICHDAT,STREICHUNG,BCODE,MUTATION";
      QStringList list = listo.split(",");
    
        QDomDocument em;
        QDomProcessingInstruction header = em.createProcessingInstruction( "xml", "version=\"1.0\"" );
        em.appendChild( header );
        QDomElement root = em.createElement("reg_required");
        /////root.setAttribute ("build",&browser.UmanTimeFromUnix(&browser.QTime_Null()));
        em.appendChild( root );
        QDomElement queril = em.createElement( "table_name" );
        queril.setAttribute ("name","REG_BASE");
        root.appendChild(queril);
    
            for (int i = 0; i < list.size(); ++i) {
                
                     QString rnam = list.at(i);
                     QString hum = "Human Name";
                     QDomElement lines = em.createElement( "field" );
                     lines.setAttribute ("id",i);
                     lines.setAttribute ("required","1");
                     lines.setAttribute ("name",rnam);
                
                    QDomElement human = em.createElement("human_readable");
                    human.appendChild(em.createTextNode(hum+" "+rnam));
                    lines.appendChild(human);
                
                    queril.appendChild(lines);
                
                   

            }
            
       
                QTextCodec *setcurrentcodec;
                setcurrentcodec = QTextCodec::codecForMib(106);
                QFile f( EXPORT_FIELD_NAME );
                if ( f.open( QFile::WriteOnly | QFile::Text ) ) {
                QTextStream sw( &f );
                sw.setCodec(setcurrentcodec);
                sw << em.toString();
                f.close();
                } else {
                    QMessageBox::information(0, QObject::tr("Error!"),QObject::tr("Unable to save to file %1").arg(EXPORT_FIELD_NAME));
                }
    
            }
    */

    QMenu *menu = mainWin.menuBar()->addMenu(QObject::tr("&File"));
    menu->addAction(QObject::tr("Add &Connection..."), &browser, SLOT(addConnection()));
    //////////menu->addAction(QObject::tr("Import configuration file xml format (utf8)"), &browser, SLOT(ImportConfig()));
    /////////menu->addAction(QObject::tr("Export configuration file xml format (utf8)"), &browser, SLOT(ExportConfig()));      
            
    menu->addSeparator();
    menu->addAction(QObject::tr("&Quit"), &app, SLOT(quit()));
    QMenu *showMenu = mainWin.menuBar()->addMenu(QObject::tr("&About - Info"));
    showMenu->addAction(QObject::tr("About ODBC Connection Wizard"), &browser, SLOT(abouthere()));
    showMenu->addAction(QObject::tr("Report a Bug"), &browser, SLOT(reportbug()));
    QObject::connect(&browser, SIGNAL(statusMessage(QString)), mainWin.statusBar(), SLOT(showMessage(QString)));

    addConnectionsFromCommandline(app.arguments(), &browser);
    mainWin.show();
    if (QSqlDatabase::connectionNames().isEmpty()) QMetaObject::invokeMethod(&browser, "addConnection", Qt::QueuedConnection);

    return app.exec();
}
