
#include "browser.h"
#include "qsqlconnectiondialog.h"

#include <QtGui>
#include <QtSql>

Browser::Browser(QWidget *parent)
{
    setupUi(this);

    
    connect(bsincro, SIGNAL(clicked()), this , SLOT(ConfigSincroWizard()));
    connect(this, SIGNAL(registerQuerySuccess(QString,QString)), this , SLOT(Export_Xml(QString,QString)));
    openwsin = new Sincro(this);
     connect(openwsin, SIGNAL(MakeParentQuery(QString)), this , SLOT(GoSetQuery(QString))); 
    connect(this, SIGNAL(displaystatus(int,int)), this , SLOT(SendDialog(int,int))); 
    
      
    
    
    table->addAction(insertRowAction);
    table->addAction(deleteRowAction);
    QSettings setter;
    //////Make_Arianna();
    setter.setValue("TimeToday", UmanTimeFromUnix(QTime_Null()));
    QString lastq = setter.value("LastQuery").toString();
    if (lastq.size() > 1) {
    sqlEdit->clear();              
    sqlEdit->append(lastq);
    sqlEdit->toPlainText();
    }
    
    
    if (QSqlDatabase::drivers().isEmpty())
        QMessageBox::information(this, tr("No database drivers found"),
                                 tr("This App requires at least one Qt database driver. "
                                    "Please check the documentation."));

    emit statusMessage(tr("Ready."));
}

Browser::~Browser()
{
}

void Browser::exec()
{
    last_query = sqlEdit->toPlainText();
    tablelisten.clear();
    QSqlDatabase currendb(connectionWidget->currentDatabase());
    tablelisten = currendb.tables();
     for (int i = 0; i < tablelisten.size(); ++i)  {
          QString tabi = tablelisten.at(i);
          if (last_query.contains(tabi,Qt::CaseInsensitive)) {
          runningtable = tabi;
          }
     }
    QSqlQueryModel *model = new QSqlQueryModel(table);
    model->setQuery(QSqlQuery(last_query,currendb));
    table->setModel(model);
    if (model->lastError().type() != QSqlError::NoError) {
        emit statusMessage(model->lastError().text());
    } else if (model->query().isSelect()) {
        emit statusMessage(tr("Query OK on table %1").arg(runningtable));
        emit registerQuerySuccess(last_query,runningtable);  /* take last register table name .... */
    } else {
        emit statusMessage(tr("Query OK, number of affected rows: %1").arg(
                           model->query().numRowsAffected()));
    }

    updateActions();
}

QSqlError Browser::addConnection(const QString &driver, const QString &dbName, const QString &host,
                            const QString &user, const QString &passwd, int port)
{
    static int cCount = 0;

    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, QString("Browser%1").arg(++cCount));
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("Browser%1").arg(cCount));
    }
    connectionWidget->refresh();

    return err;
}

void Browser::addConnection()
{
    QSqlConnectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    if (dialog.useInMemoryDatabase()) {
        QSqlDatabase::database("in_mem_db", false).close();
        QSqlDatabase::removeDatabase("in_mem_db");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "in_mem_db");
        db.setDatabaseName(":memory:");
        if (!db.open())
            QMessageBox::warning(this, tr("Unable to open database"), tr("An error occured while "
                                                                         "opening the connection: ") + db.lastError().text());
        QSqlQuery q("", db);
        q.exec("drop table Movies");
        q.exec("drop table Names");
        q.exec("create table Movies (id integer primary key, Title varchar, Director varchar, Rating number)");
        q.exec("insert into Movies values (0, 'Metropolis', 'Fritz Lang', '8.4')");
        q.exec("insert into Movies values (1, 'Nosferatu, eine Symphonie des Grauens', 'F.W. Murnau', '8.1')");
        q.exec("insert into Movies values (2, 'Bis ans Ende der Welt', 'Wim Wenders', '6.5')");
        q.exec("insert into Movies values (3, 'Hardware', 'Richard Stanley', '5.2')");
        q.exec("insert into Movies values (4, 'Mitchell', 'Andrew V. McLaglen', '2.1')");
        q.exec("create table Names (id integer primary key, Firstname varchar, Lastname varchar, City varchar)");
        q.exec("insert into Names values (0, 'Sala', 'Palmer', 'Morristown')");
        q.exec("insert into Names values (1, 'Christopher', 'Walker', 'Morristown')");
        q.exec("insert into Names values (2, 'Donald', 'Duck', 'Andeby')");
        q.exec("insert into Names values (3, 'Buck', 'Rogers', 'Paris')");
        q.exec("insert into Names values (4, 'Sherlock', 'Holmes', 'London')");
        connectionWidget->refresh();
    } else {
        QSqlError err = addConnection(dialog.driverName(), dialog.databaseName(), dialog.hostName(),
                           dialog.userName(), dialog.password(), dialog.port());
        if (err.type() != QSqlError::NoError)
            QMessageBox::warning(this, tr("Unable to open database"), tr("An error occured while "
                                      "opening the connection: ") + err.text());
    }
}

void Browser::showTable(const QString &t)
{
    model = new QSqlTableModel(table, connectionWidget->currentDatabase());
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable(t);
    runningtable = t;
    ////QSqlQuery fatto = model->query();
    ////QString fatto_query = fatto.executedQuery();
    model->select();
    QString tasble = model->tableName();
    QStringList rowsname;
    rowsname.clear();
    QSqlRecord rec = connectionWidget->currentDatabase().record(tasble);
    QString nami;
    SendToArea("",0);
    for (int i = 0; i < rec.count(); ++i) {
             QSqlField fld = rec.field(i);
             nami = fld.name();
             rowsname.append(nami);
    }
    QString brow = rowsname.join(",");
    QString running = QString("SELECT %2 FROM %1").arg(tasble,brow);
    SendToArea(running,1);
    runningtable = tasble;
    
    if (model->lastError().type() != QSqlError::NoError)
        emit statusMessage(model->lastError().text());
    table->setModel(model);
    table->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);

    connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentChanged()));
    
    
    
    
    updateActions();
}

void Browser::showMetaData(const QString &t)
{
    QSqlRecord rec = connectionWidget->currentDatabase().record(t);
    QStandardItemModel *model = new QStandardItemModel(table);

    model->insertRows(0, rec.count());
    model->insertColumns(0, 7);

    model->setHeaderData(0, Qt::Horizontal, "Fieldname");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Length");
    model->setHeaderData(3, Qt::Horizontal, "Precision");
    model->setHeaderData(4, Qt::Horizontal, "Required");
    model->setHeaderData(5, Qt::Horizontal, "AutoValue");
    model->setHeaderData(6, Qt::Horizontal, "DefaultValue");


    for (int i = 0; i < rec.count(); ++i) {
        QSqlField fld = rec.field(i);
        model->setData(model->index(i, 0), fld.name());
        model->setData(model->index(i, 1), fld.typeID() == -1
                ? QString(QVariant::typeToName(fld.type()))
                : QString("%1 (%2)").arg(QVariant::typeToName(fld.type())).arg(fld.typeID()));
        model->setData(model->index(i, 2), fld.length());
        model->setData(model->index(i, 3), fld.precision());
        model->setData(model->index(i, 4), fld.requiredStatus() == -1 ? QVariant("?")
                : QVariant(bool(fld.requiredStatus())));
        model->setData(model->index(i, 5), fld.isAutoValue());
        model->setData(model->index(i, 6), fld.defaultValue());
    }

    table->setModel(model);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    updateActions();
}

void Browser::insertRow()
{
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(table->model());
    if (!model)
        return;

    QModelIndex insertIndex = table->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
    model->insertRow(row);
    insertIndex = model->index(row, 0);
    table->setCurrentIndex(insertIndex);
    table->edit(insertIndex);
}

void Browser::deleteRow()
{
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(table->model());
    if (!model)
        return;

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QModelIndexList currentSelection = table->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        if (currentSelection.at(i).column() != 0)
            continue;
        model->removeRow(currentSelection.at(i).row());
    }

    model->submitAll();
    model->setEditStrategy(QSqlTableModel::OnRowChange);

    updateActions();
}

void Browser::updateActions()
{
    bool enableIns = qobject_cast<QSqlTableModel *>(table->model());
    bool enableDel = enableIns && table->currentIndex().isValid();
     
    insertRowAction->setEnabled(enableIns);
    deleteRowAction->setEnabled(enableDel);
}

void  Browser::SendToArea( QString t , int modus )
{
QString tu = PasteTime();
QString text;
    if (modus == 1) {
    text = QString("Query Log =start=>\n%1\n<=end=").arg(t); 
    } else {
    text = QString("On %1:").arg(tu);   
    }
    
forums->append(text);
forums->setReadOnly(true);
forums->toPlainText();
forums->setAutoFormatting(QTextEdit::AutoAll);
forums->setLineWrapMode(QTextEdit::NoWrap);
}

void Browser::ExportCSVPrimary( QString qx )
{
    QString cfileName = QFileDialog::getSaveFileName(this, "Export CSV text",QDir::homePath(), "*.csv");
    
    if (cfileName.size() > 0) {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     if (!cfileName.endsWith(".csv")) {
         cfileName = cfileName+".csv"; 
     }
     
    QStringList cvslines;
    QStringList Fname;
    QStringList Fvalue;
    
            QSqlQuery query(qx,connectionWidget->currentDatabase());
            QSqlRecord rec = query.record();
            StructureTable(rec);
            int sumcol = rec.count();
            int sumrow = query.numRowsAffected();
            int recnum = 0 - 1;
    
               while (query.next()) { 
                    recnum++;  /* count fake lines */
                    Fvalue.clear();
                    Fname.clear();
                   
                             for(int i=0;i<sumcol;i++){
                             bool oknr;
                             QString value = Strcsvtrimmed(query.value(i).toString());
                             int vnummer = value.toInt(&oknr);
                             QString fname = Strcsvtrimmed(rec.fieldName(i));
                             if (oknr or value.isEmpty()) {
                             Fvalue.append(value);
                             } else {
                             Fvalue.append(QString("\"%1\"").arg(value));  
                             }  
                             Fname.append(QString("\"%1\"").arg(fname));
                             }
                             
                    QString Dvalue = Fvalue.join(";");
                    QString Dname = Fname.join(";");
                    if (recnum == 0) {
                    cvslines.append(Dname);  
                    }
                    cvslines.append(Dvalue);
                    }
                    
               QString contentxt = cvslines.join("\n");
                
                QFile f( cfileName );
                if ( f.open( QFile::WriteOnly | QFile::Text ) ) {
                QTextStream sw( &f );
                sw << contentxt;
                f.close();
                } else {
                    QApplication::restoreOverrideCursor(); 
                    QMessageBox::information(this, tr("Error!"),tr("Unable to save to file %1").arg(cfileName));
                }     
                    
                    
     QApplication::restoreOverrideCursor(); 
    
     }
}


void Browser::ExportSqlite3( QString qx )
{
    QString cfileName = QFileDialog::getSaveFileName(this, "Export SQLITE3 UTF-8 text",QDir::homePath(), "*.sql");
    bool ok;
    QString newtablename = QInputDialog::getText(this, tr("You like sqlite insert on new table name?"),tr("TableName:"), QLineEdit::Normal,runningtable, &ok);
    
    if (newtablename.size() > 0 ) {
    runningtable = newtablename;
    }
    
    
    if (cfileName.size() > 0) {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     if (!cfileName.endsWith(".sql")) {
         cfileName = cfileName+".sql"; 
     }
     
    QStringList cvslines;
    QStringList Fname;
    QStringList Fvalue;
    cvslines.append(QString("### Created on %1  ###").arg(_PROGRAM_TITLE_)); 
    cvslines.append(QString("### Export from %1 ###").arg(UmanTimeFromUnix(QTime_Null())));  
    cvslines.append("### original saved on utf-8 format use vim or so to convert on other format ###");  
    cvslines.append(QString("### Remove comment line if other application not support!   ###")); 
    cvslines.append("BEGIN TRANSACTION;");  
     
     
            QSqlQuery query(qx,connectionWidget->currentDatabase());
            QSqlRecord rec = query.record();
     
     
            ////////cvslines.append(StructureMYSQLTable(rec));
            cvslines.append(StructureTable(rec)); 
     
     
            int sumcol = rec.count();
            int sumrow = query.numRowsAffected();
            int recnum = 0 - 1;
    
               while (query.next()) { 
                    recnum++;  /* count fake lines */
                    Fvalue.clear();
                    Fname.clear();
                   
                             for(int i=0;i<sumcol;i++){
                             bool oknr;
                             QString value = Strsqltrimmed(query.value(i).toString());
                             int vnummer = value.toInt(&oknr);
                             QString fname = Strcsvtrimmed(rec.fieldName(i));
                                 if (oknr) {
                                 Fvalue.append(value);
                                 } else if (value.isEmpty()) {
                                 Fvalue.append("NULL");
                                 } else {
                                 Fvalue.append(QString("'%1'").arg(value));  
                                 }
                             }
                             
                    QString Dvalue = Fvalue.join(",");
                    cvslines.append(QString("INSERT INTO %1 VALUES (%2);").arg(runningtable).arg(Dvalue));
                    }
            
               cvslines.append("COMMIT;");  
               QString contentxt = cvslines.join("\n");
                QTextCodec *setcurrentcodec;
                setcurrentcodec = QTextCodec::codecForMib(106);  /* utf8 */ 
                    
                QFile f( cfileName );
                if ( f.open( QFile::WriteOnly | QFile::Text ) ) {
                QTextStream sw( &f );
                sw.setCodec(setcurrentcodec);
                sw << contentxt;
                f.close();
                } else {
                    QApplication::restoreOverrideCursor(); 
                    QMessageBox::information(this, tr("Error!"),tr("Unable to save to file %1").arg(cfileName));
                }     
                    
                    
     QApplication::restoreOverrideCursor(); 
    
     }
}


void Browser::ExportMysql( QString qx )
{
    QString tta;
    QString cfileName = QFileDialog::getSaveFileName(this, "Export MYSQL locale format text",QDir::homePath(), "*.sql");
    bool ok;
    QString newtablename = QInputDialog::getText(this, tr("You like mysql insert on new table name?"),tr("TableName:"), QLineEdit::Normal,runningtable, &ok);
    
    
    
    QString msgDB =tr("<p>Append create table command? mysql</p>");
      int removeyes = QMessageBox::question(this, tr("Please confirm!"),msgDB,
                                                            tr("&Yes"), tr("&No"),
                                                             QString(),8888,9999);
         QString responder = QString::number(removeyes);
    
    
    
    
    
    if (newtablename.size() > 0 ) {
    runningtable = newtablename;
    }
    
    if (cfileName.size() > 0) {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     if (!cfileName.endsWith(".sql")) {
         cfileName = cfileName+".sql"; 
     }
            dlg = new QProgressDialog(this,Qt::Popup);
            dlg->setLabelText (tr("Export table \"%1\"").arg( runningtable ));
            dlg->setCancelButton(0);
     
    QStringList cvslines;
    QStringList Fname;
    QStringList Fvalue;
    cvslines.append(QString("### Created on %1  ###").arg(_PROGRAM_TITLE_)); 
    cvslines.append(QString("### Export from %1 ###").arg(UmanTimeFromUnix(QTime_Null())));   
    cvslines.append(QString("### Remove comment line if other application not support!   ###")); 
     
     
            QSqlQuery query(qx,connectionWidget->currentDatabase());
            QSqlRecord rec = query.record();
     
            if (responder =="0")  {
            cvslines.append(StructureMYSQLTable(rec));
            }
     
     
            cvslines.append(QString("### Manual rewrite if is from odbc export! ###"));
     
            bool suppoaffect;
            int sumcol = rec.count();
            int sumrow = query.numRowsAffected();
            int recnum = 0;  /* QProgressDialog *dlg; */
            int pasero = 0;
            if (sumrow < 1) {
               suppoaffect = false;
               sumrow = 1000;
            } else {
               suppoaffect = true; 
            }
            
    
               while (query.next()) { 
                    recnum++;  /* count fake lines */
                    
                    emit displaystatus(recnum,sumrow);
                   
                    Fvalue.clear();
                    Fname.clear();
                   
                             for(int i=0;i<sumcol;i++){
                             bool oknr;
                             QSqlField fld = rec.field(i);
                             QString value = Strsqltrimmed(query.value(i).toString());
                             QString typeoffield = QString(QVariant::typeToName(fld.type()));
                                 if (typeoffield == "QByteArray") {
                                   value = encodeBase64(query.value(i).toString()); 
                                 }
                                 
                             int vnummer = value.toInt(&oknr);
                             QString fname = Strcsvtrimmed(rec.fieldName(i));
                                 if (oknr) {
                                 Fvalue.append(value);
                                 } else if (value.isEmpty()) {
                                 Fvalue.append("NULL");
                                 } else {
                                 Fvalue.append(QString("'%1'").arg(value));  
                                 }
                             }
                             
                    QString Dvalue = Fvalue.join(",");
                    cvslines.append(QString("INSERT INTO `%1` VALUES (%2);").arg(runningtable).arg(Dvalue));
                      if (!suppoaffect) {
                          if (recnum == 1001) {
                          recnum = 0;
                          pasero++;
                          dlg->setLabelText (tr("Export table \"%1\" %2x1000 !numRowsAffected").arg( runningtable ).arg(pasero));
                          }
                      }
                    }
             
               QString contentxt = cvslines.join("\n");
                ///////////QTextCodec *setcurrentcodec;
                //////////setcurrentcodec = QTextCodec::codecForMib(106);  /* utf8 */ 
                    
                QFile f( cfileName );
                if ( f.open( QFile::WriteOnly | QFile::Text ) ) {
                QTextStream sw( &f );
                /////////sw.setCodec(setcurrentcodec);
                sw << contentxt;
                f.close();
                   if (dlg->isVisible ()) {
                   dlg->close();
                   }
                } else {
                    if (dlg->isVisible ()) {
                    dlg->close();
                    }
                    QApplication::restoreOverrideCursor(); 
                    QMessageBox::information(this, tr("Error!"),tr("Unable to save to file %1").arg(cfileName));
                }     
                    
                    
     QApplication::restoreOverrideCursor(); 
    
     }
}



QString Browser::StructureMYSQLTable( QSqlRecord now )
{
    QSqlRecord rec = now;
    QString createtable;
    QString sqltipename, mkey;
    QStringList CreateTable;
    int totalcools = rec.count();
    if (totalcools > 0) {
              /* sqlite3 table construct from odbc converter */
              for (int i = 0; i < totalcools; ++i) {
                QSqlField fld = rec.field(i);
                  QString name = Strtrimmed(fld.name());
                  name.toUpper();
                  if (name == "id") {
                  name = "ID";
                  }
                  QString typeoffield = QString(QVariant::typeToName(fld.type()));
                  int sizefe = fld.length();
                  if (sizefe > 255) {
                  sizefe = 255;
                  }
                  if (sizefe < 1) {
                  sizefe = 255;
                  }
                  /* fld.isAutoValue()  never true from auto_increment mysql ??? */
                  if (fld.requiredStatus() and i < 2 and typeoffield == "int") {
                   sqltipename = QString("  `%1` INT(%2) NOT NULL auto_increment").arg(name).arg(sizefe);
                   mkey = QString("   KEY `%1` (`%1`)").arg(name);
                  } else if ( typeoffield == "int" ) {
                   sqltipename = QString("  `%1` int(%2) default NULL").arg(name).arg(sizefe); 
                  } else if (  typeoffield == "double") {
                   sqltipename = QString("  `%1` int(%2) default NULL").arg(name).arg(sizefe);    
                  } else if (  typeoffield == "QByteArray") {
                   sqltipename = QString("  `%1` LONGBLOB default NULL").arg(name);    
                  } else {
                   sqltipename = QString("  `%1` VARCHAR(%2) default NULL").arg(name).arg(sizefe);   
                  }
                  CreateTable.append(sqltipename);
                  
                  ////////qDebug() << "### fieldname "  << name;
                  ///////////qDebug() << "### typeoffield "  << typeoffield;
              }
                  if (mkey.size() > 0) {
                  CreateTable.append(mkey);
                  }
                  
        QString midlecreate = CreateTable.join(",\n");
                midlecreate.prepend(QString("### mysql version 5 or > ###\nDROP TABLE IF EXISTS `%1`;\nCREATE TABLE `%1` (\n").arg(runningtable));
                midlecreate.append("\n) TYPE=MyISAM;");
        createtable = midlecreate;
   }
return createtable;
}




QString Browser::StructureTable( QSqlRecord now )
{
    QSqlRecord rec = now;
    QString createtable;
    QString sqltipename;
    QStringList CreateTable;
    int totalcools = rec.count();
    if (totalcools > 0) {
              /* sqlite3 table construct from odbc converter */
              for (int i = 0; i < totalcools; ++i) {
                QSqlField fld = rec.field(i);
                  QString name = Strtrimmed(fld.name());
                  QString typeoffield = QString(QVariant::typeToName(fld.type()));
                  /* fld.isAutoValue()  never true from auto_increment mysql ??? */
                  if (fld.requiredStatus() and i < 2 and typeoffield == "int") {
                   sqltipename = QString("%1 INTEGER PRIMARY KEY").arg(name); 
                  } else if (  typeoffield == "double" or typeoffield == "int" ) {
                   sqltipename = QString("%1 NUMERIC").arg(name); 
                  } else if (  typeoffield == "QByteArray") {
                   sqltipename = QString("%1 BLOB").arg(name); 
                  } else {
                   sqltipename = QString("%1 TEXT").arg(name);   
                  }
                  CreateTable.append(sqltipename);
                  ////////qDebug() << "### fieldname "  << name;
                  ///////////qDebug() << "### typeoffield "  << typeoffield;
              }
        QString midlecreate = CreateTable.join(",");
                midlecreate.prepend(QString("CREATE TABLE %1 (").arg(runningtable));
                midlecreate.append(");");
        createtable = midlecreate;
   }
return createtable;
}

void Browser::ExportXmlPrimary( QString qx )
{
 
                
              bool ok;
              QString userd = QInputDialog::getText(this, tr("Tell me Your name?... to signed the xml file"),tr("Autor:"), QLineEdit::Normal,"", &ok);
              QString userorg = QInputDialog::getText(this, tr("Tell me Your ORG name?... to signed the xml file"),tr("ORG:"), QLineEdit::Normal,"", &ok);
              QString xfileName = QFileDialog::getSaveFileName(this, "Export XML",QDir::homePath(), "*.xml");
              if (!xfileName.endsWith(".xml")) {
               xfileName = xfileName+".xml"; 
               }
              if (!ok and userd.size() < 2) {
                  userd = "From unknow Autor";
              }
              if (userorg.size() < 2) {
                  userorg = "UNKNOW";
              }
              
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            QSqlQuery query(qx,connectionWidget->currentDatabase());
            QSqlRecord rec = query.record();
            int sumcol = rec.count();
            int sumrow = query.numRowsAffected();
            int recnum = 0 - 1;
             /*  xml ##################################### StructureTable( QSqlRecord now ) */
            QDomDocument em;
            QDomProcessingInstruction header = em.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\" " );
            em.appendChild( header );
            QDomElement root = em.createElement("odbc_root");
            root.setAttribute ("org",userorg.toUpper());
            root.setAttribute ("build",UmanTimeFromUnix(QTime_Null()));
            root.setAttribute ("appsname",_PROGRAM_TITLE_);
            root.setAttribute ("export_from",QString(setter.value("ODBBC_Name").toString()));
            root.setAttribute ("createtable",StructureTable(rec));
            em.appendChild( root );
            QDomElement queril = em.createElement( "query" );
            queril.setAttribute ("name",qx);
            queril.setAttribute ("org",userorg.toUpper());
            if (sumrow > 0) {
              queril.setAttribute ("sumrow",QString::number(sumrow));  
            } else {
              queril.setAttribute ("sumrow","Driver_Not_Support_RowsAffected"); 
            }
            queril.setAttribute ("sumcool",QString::number(sumcol));
            queril.setAttribute ("user",userd);
            root.appendChild(queril);
    
                    while (query.next()) { 
                    recnum++;
                    QDomElement lines = em.createElement( "line" );
                    lines.setAttribute ("id",recnum);
                    queril.appendChild(lines);
                    
                             for(int i=0;i<sumcol;i++){
                             QDomElement line = em.createElement( "row" );
                             QString value = Strtrimmed(query.value(i).toString());
                             QString fname = rec.fieldName(i);
                             ///////qDebug() << "### fieldname "  << fname;
                             line.setAttribute ("fieldname",fname);
                             line.setAttribute ("value",value);
                                 if (is_numeric(value)) {
                                    line.setAttribute ("digit","1"); 
                                 } else {
                                     line.setAttribute ("digit","0");
                                 }
                             line.setAttribute ("id",i);
                             //////qDebug() << "### recnum "  << recnum;
                             lines.appendChild(line);
                             }
                    
                    } 
                    
                QTextCodec *setcurrentcodec;
                setcurrentcodec = QTextCodec::codecForMib(106);  /* utf8 */ 
                QFile f( xfileName );
                if ( f.open( QFile::WriteOnly | QFile::Text ) ) {
                QTextStream sw( &f );
                sw.setCodec(setcurrentcodec);
                sw << em.toString();
                f.close();
                } else {
                    QApplication::restoreOverrideCursor(); 
                    QMessageBox::information(this, tr("Error!"),tr("Unable to save to file %1").arg(xfileName));
                }
                
               QApplication::restoreOverrideCursor(); 
    
    
} 



/* encode to name */
QString Browser::encodeBase64( QString xml )
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

/* decode to name */
QString Browser::decodeBase64( QString xml )
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}



QString Browser::Strtrimmed( QString t )
{
 QString text = t;
 text.replace("\t", " ");
 text.replace("\n", " ");
 text.replace("\r", " ");
 return text.trimmed();
} 

QString Browser::Strcsvtrimmed( QString t )
{
 QString text = t;
 text.replace("\t", " ");
 text.replace("\n", " ");
 text.replace("\r", " ");
 text.replace("\"", "“");
 return text.trimmed();
} 

QString Browser::Strsqltrimmed( QString t )
{
 QString text = t;
 text.replace("\t", " ");
 text.replace("\n", " ");
 text.replace("\r", " ");
 text.replace("'", "`");
 return text.trimmed();
} 








