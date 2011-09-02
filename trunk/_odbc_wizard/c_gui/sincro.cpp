#include "sincro.h"
//
/*  Save file as sincro.cpp  */
/*  Class Sincro Created on Thu Oct 5 20:17:37 CEST 2006  */
//
#include <QCloseEvent>
//
QPointer<Sincro> Sincro::_self = 0L;
//
Sincro* Sincro::self( QWidget* parent )
{
	if ( !_self )
		_self = new Sincro( parent );
	return _self;
}
/* base constructo */
Sincro::Sincro( QWidget* parent )
	: QDialog( parent )
{
	setupUi( this );
    openwork = false;
    
    bnext->setEnabled(true);
    bpre->setEnabled(false);
}
/* connector */
void Sincro::SetConnection( QSqlDatabase * connid ) 
{ 
        POSX = 0;
        dbi = connid; 
        conunam = QString(setter.value("ODBBC_Name").toString());
        if (!dbi->isOpen()) {
        QMessageBox::information(this, tr("Error!"),tr("DB connection loast!"));
        accept();        
        }  else {
        RefreshConnection();
            field->setHidden(true);
            connect(alltable, SIGNAL(currentIndexChanged(QString)), this , SLOT(BuildField())); 
            connect(alltable, SIGNAL(highlighted(int)), this , SLOT(BuildField())); 
            connect(alltable, SIGNAL(activated (QString)), this , SLOT(BuildField())); 
            connect(goquery, SIGNAL(clicked()), this , SLOT(GoQuery()));  
            connect(bnext, SIGNAL(clicked()), this , SLOT(Count_Go_Next()));  
            connect(bpre, SIGNAL(clicked()), this , SLOT(Count_Go_Prev()));  
            connect(reiniti, SIGNAL(clicked()), this , SLOT(ResetFieldQuery()));  
            
        }            
}

void Sincro::ResetFieldQuery()
{
     for ( int x = 0; x < itecats.size(); ++x ) {
       itecats[x]->RemoveNow();
    }
    itecats.clear();
    readatt.clear();
    remama->clear();
    ReadConfigIn();
    POSX = 0;
    qDebug() << "### itecats size =  " << itecats.size();
}

void Sincro::GoQuery()
{
    QString qzu = remama->document()->toPlainText();
    setter.setValue("LastQuery", qzu);
    ResetFieldQuery();
    openwork = false;
    emit MakeParentQuery(qzu);
    accept();
}




/* salva e vai al prossimo */
bool Sincro::SAVE_and_Next()
{
    QString msgDB =tr("You can use the same field name only one time!\n(From your own table)\nCan not export this field?");
    ActualFieldName = field->currentText();
    ActualTableSel = alltable->currentText();
    /* salvatutti */
    bool aifatto = FindDoppel(ActualFieldName);
    if (aifatto) {
       ///////////QMessageBox::information(this, tr("Error!"),tr(""));  
        
      int removeyes = QMessageBox::question(this, tr("Pleas confirm"),msgDB,
                                                            tr("&No Export"), tr("&Yes Export"),
                                                             QString(),8888,9999);
         QString responder = QString::number(removeyes);
         
         if (responder =="0")  {
          SetNoExport();
         } else {
             field->setFocus();
         }
        
     return false;
    }
    QString tonow;
    if (ActualFieldName =="NO_EXPORT") {
       field->setCurrentIndex(0);
       return true; 
    }
    if (ActualFieldName !="NO_EXPORT") {
        if (ActualSOURCCEFIELD == ActualFieldName) {
            tonow = QString("%1").arg(ActualFieldName);
        } else {
           tonow = QString("%1 AS %2").arg(ActualFieldName).arg(ActualSOURCCEFIELD); 
        }
     
        if (FindSavedList(POSX)) {
            qDebug() << "### vecchio ";
            
                 for ( int x = 0; x < itecats.size(); ++x ) {
                     int ahere = itecats[x]->Indexnum();
                     
                      if (POSX == ahere) {
                        itecats[x]->SetASI(tonow);
                        itecats[x]->SetTable(ActualTableSel); 
                      }
                 }
            
            
            
        } else {
            qDebug() << "### nuovo ";
           CateSetting *mes = new CateSetting(tonow,ActualTableSel,ActualFieldName,POSX); 
            itecats.append(mes);
        }
    }
    
    int douoi = itecats.size();
    /*
    inline QString Getasi() const { return ASA; }
        inline QString Getable()  { return TA; }
    */
    
    int totale = readatt.size();
    int gira = totale;
    
   
    
    END_asi.clear();
    END_table.clear();
                for ( int x = 0; x < itecats.size(); ++x ) {
                   QString t = itecats[x]->Getable(); 
                    QString as  = itecats[x]->Getasi(); 
                    int loz = itecats[x]->Indexnum();
                     for ( int i = 0; i < gira; ++i ) {
                         if (loz  == i) {
                               END_asi.append(as);
                               END_table.append(t);
                         }
                         
                         
                      }
                    
                 }
    
                 int toi = tables.size();
                 QStringList uniquetable;
                  uniquetable.clear();
                  QStringListIterator i(tables);
                             while (i.hasNext())  {
                                  QString rectable = i.next();
                                  if (END_table.contains(rectable)) {
                                   uniquetable.append(rectable);   
                                  }
                             }
                 
                 
                 
    QString Xasi = END_asi.join(",");
    QString Xtable = uniquetable.join(",");
    QString tonowdhdshdsh = QString("SELECT %1 FROM %2 LIMIT 10000").arg(Xasi).arg(Xtable);
    
remama->clear();              
remama->append(tonowdhdshdsh);
remama->toPlainText();
remama->setAutoFormatting(QTextEdit::AutoAll);
return true;
}

bool  Sincro::FindSavedList( int doid ) 
{
     bool make = false;
     int bigsize = itecats.size();
     int results = -1;
        if (bigsize > 0) {
                 for ( int x = 0; x < bigsize; ++x ) {
                     int sto = itecats[x]->Indexnum();
                     
                      if (doid == sto) {
                        return true;
                      }
                 }
                 
        }
     return make;
}

bool  Sincro::FindDoppel( QString so ) 
{
     bool make = false;
     int bigsize = itecats.size();
     int results = -1;
        if (bigsize > 0) {
                 for ( int x = 0; x < bigsize; ++x ) {
                     QString a = itecats[x]->Gesource();
                     
                      if (a == so) {
                        return true;
                      }
                 }
                 
        }
     return make;
}




void Sincro::End_All()
{
    QString msgDB =tr("OK Looks Good, You Like to Configure the Next Item?");
    int removeyes = QMessageBox::question(this, tr("Please Confirm:"),msgDB,
                                                            tr("&Yes"), tr("&No"),
                                                             QString(),8888,9999);
         QString responder = QString::number(removeyes);
    
}




void Sincro::RefreshConnection()
{
     bool workdb = false;
     POSX = 0;
     actualdbname = dbi->databaseName();
     connectionNames = dbi->connectionNames();
    
    /////qDebug() << "### connectionNames.count() " << connectionNames.count();
    /* QSqlDatabase USDB */
    
     for (int i = 0; i < connectionNames.count(); ++i) {
         QSqlDatabase db = QSqlDatabase::database(connectionNames.at(i), false);        
             betterNameDB = QtNameOn(db);
             if (connectionNames.at(i) == actualdbname) {
              workdb = true;  
             }
             if (db.isOpen()) {
             USDB = db;
             tables = db.tables();
             }
     }
     dbname->setText(QString("N.%1").arg(betterNameDB));
     alltable->clear();
     alltable->addItems(tables);     
     //////////ReadConfigIn();
}

/*
void WakeUPAll();
void BuildField();
void BuildFeldValueTable( QString ytab );
*/

void Sincro::BuildFeldValueTable( QString ytab )
{
     FieldNamesY.clear();
     QSqlRecord pas = USDB.record(ytab);
                int totf = pas.count();
                    for (int i = 0; i < totf; ++i) {
                        QString fn = pas.fieldName(i);
                         FieldNamesY.append(fn);
                    }
    
     field->setHidden(false); 
     field->clear();
     field->addItems(FieldNamesY);
     field->addItem("NO_EXPORT");           
     ///////WakeUPAll();
}


/* read config */
void Sincro::ReadConfigIn()
{
    POSX = 0;
    ///////connectionNames.clear();
    ////ConnHumanNames.clear();
    /////FieldNamesY.clear();
    /////tables.clear();;
      /* Qxml sdom = Qxml(); */
    if (xfileNameii.size() < 1) {
     xfileNameii = QFileDialog::getOpenFileName(this, "Field Name Translation ",QString(setter.value("LASTDIRYOUOPEN").toString()), "*.xml");
    }
    if (xfileNameii.size() > 0) {
     QString lastdirer = xfileNameii.left(xfileNameii.lastIndexOf("/"))+"/";
     setter.setValue("LASTDIRYOUOPEN", lastdirer);
        
     QFile xmlfile( xfileNameii );
     QString errorStr, obname, inhalt;
     int errorLine, errorColumn, position;
     position = 0 -1;
      QDomDocument doc;
     if(!xmlfile.open( QIODevice::ReadOnly ) ) {
      QMessageBox::information(this, tr("Error!"),tr("Unable to open Config file %1!").arg(xfileNameii));
      return;
     }
     if (!doc.setContent(&xmlfile,true, &errorStr, &errorLine, &errorColumn)) {
      QMessageBox::information(this, tr("Error!"),tr("Config file is broken! Error %1 Line %2 Column %3").arg(errorStr).arg(errorLine).arg(errorColumn));
      xmlfile.close();
      return; 
     }
     
     QDomElement root = doc.documentElement();
     if (root.tagName() != "reg_required") {
       QMessageBox::information(this, tr("Error!"),tr("Config Translation FieldName file %1 is not corect!\nRoot tag not match.").arg(xfileNameii));
      xmlfile.close();
      return;     
     }
     
        
        QDomElement child = root.firstChildElement("table_name"); 
        QDomElement lines = child.firstChildElement("field");
          
           int cane = 0;
           readatt.clear();
           while (!lines.isNull()) {
               QDomElement fhumo = lines.firstChildElement("human_readable");
               QString name = Att(lines,"name");
               QString id = Att(lines,"id");
               QString title = fhumo.text();
               cane++;
               readatt.insert(cane,QStringList() << name << title << id );
               lines = lines.nextSiblingElement("field");
            }
    
    actualpos->setText(QString("Position .%1").arg(POSX));
    summpos->setText(QString("Field .%1").arg(readatt.size()));
    openwork = true;
    OpenFieldSteep(0);
    WakeUPAll();     
    } else {
        close();
    }
    ///////////QMessageBox::information(this, tr("check"),tr("Size loop %1").arg(itecats.size()));  
}

QString Sincro::WriteConfigFile()
{
           QDomDocument em;
            QDomProcessingInstruction header = em.createProcessingInstruction( "xml", "version=\"1.0\"" );
            em.appendChild( header );
            QDomElement root = em.createElement("odbc_root");
            root.setAttribute ("build",QString(setter.value("TimeToday").toString()));
            root.setAttribute ("export_from",QString(setter.value("ODBBC_Name").toString()));
            em.appendChild( root );


}

/* remove file */
QString Sincro::Att(QDomElement e , QString name )
{
    QString textvalue;
    QString errorvalue = "ERROR";
    QDomAttr a1 = e.attributeNode(name);
    textvalue = a1.value();
    if (textvalue.size() > 0) {
      return textvalue;  
    } else {
      return errorvalue;  
    }
}

void Sincro::SetNoExport()
{
             int summfield = field->count();
             for (int i=0;i<summfield;i++) {
                  QString value = field->itemText(i);
                   if (value == "NO_EXPORT") {
                    field->setCurrentIndex(i);
                    }
              } 
}


void Sincro::WakeUPAll()
{
              if (POSX == 0) {
               bnext->setEnabled(true);
               POSX = 0;
              }

             int summ = alltable->count();
             for (int i=0;i<summ;i++){
                  QString value = alltable->itemText(i);
                   if (ActualTableSel == value) {
                    alltable->setCurrentIndex(i);
                    }
              } 
              
              int totale = readatt.size();
              int fermati = totale - 1;
              
              
              
              if (POSX  <= 1  or POSX > fermati ) {
                bpre->setEnabled(false);  
              } else {
                bpre->setEnabled(true);
              }
              
              if (POSX > 1) {
               bpre->setEnabled(true);
              }
              
              
              
              if (POSX <= fermati ) {
               bnext->setEnabled(true);
              } else {
                bnext->setEnabled(false);
              }
              
              if (POSX == fermati ) {
                  bnext->setEnabled(false);
              }

              if (itecats.size() > 0) {
              goquery->setEnabled(true);
              } else {
              goquery->setEnabled(false);
              }
              
              
              
              
              if (POSX == 0) {
               bnext->setEnabled(true);
               POSX = 0;
              }
              
              if (openwork) {   /* work status */
              ////////losize->setText(QString("WTot.%1.%2.%3").arg(itecats.size()).arg(readatt.size()).arg(POSX));   /* debug */
              QTimer::singleShot(900, this, SLOT(WakeUPAll()));
              } else {
              ////////losize->setText(QString("NTot.%1.%2.%3").arg(itecats.size()).arg(readatt.size()).arg(POSX));   /* debug */
              }
}







/* ##########################100%#################################################################### */
/* ##########################100%#################################################################### */
/* ##########################100%#################################################################### */
/* ##########################100%#################################################################### */

//
void Sincro::closeEvent( QCloseEvent* e )
{
	e->accept();
}

/* funzione per il prossimo */
void Sincro::Count_Go_Prev()
{
    int summfield = field->count();
    if (summfield == 0) {
       alltable->setFocus();
    return;        
    }
    int sx = POSX - 1;
    if (sx < 0) {
        sx = 0;
    }
    bool permission = SAVE_and_Next();
    qDebug() << "### Count_Go_Prev send  " << sx;
    if (permission) {
    OpenFieldSteep(sx);
    }
}

/* funzione per andare uno indietro */
void Sincro::Count_Go_Next()
{
    int summfield = field->count();
    if (summfield == 0) {
       alltable->setFocus();
    return;        
    }
    int sx = POSX + 1;
    bool permission = SAVE_and_Next();
    qDebug() << "### Count_Go_Next send  " << sx;
    if (permission) {
    OpenFieldSteep(sx);
    }
}

/* funzione per mostrare il numero corrente e il tag corrente da tradurre */
void Sincro::OpenFieldSteep( int goit )
{
    qDebug() << "### OpenFieldSteep " << goit;
    POSX = goit;
    const int attualesono = goit;
    int dove = readatt.size();
    actualpos->setText(QString("Position .%1").arg(attualesono + 1));
    summpos->setText(QString("Field .%1").arg(dove));
                            int sotree = -1;
                            UpdateDom::Iterator it;
                            for ( it = readatt.begin(); it != readatt.end(); ++it ) { 
                            sotree++;
                            QStringList itemsetter = it.value(); 
                            QString ptagname = QString(itemsetter.at(0));
                            QString ptagname1 = QString(itemsetter.at(1));
                            if (attualesono == sotree) {
                                ActualSOURCCEFIELD = ptagname;
                                exp_name->setText(QString("%1").arg(ptagname));
                                exp_human->setText(QString("%1").arg(ptagname1));
                                exp_name->setFocus();
                                exp_human->setFocus();
                            }
                        } 
}


QString Sincro::QtNameOn(const QSqlDatabase &db)
{
    QString nm = db.driverName();
    nm.append(QLatin1Char(':'));
    if (!db.userName().isEmpty())
        nm.append(db.userName()).append(QLatin1Char('@'));
    nm.append(db.databaseName());
    return nm;
}


void Sincro::BuildField()
{
    ActualTableSel = alltable->currentText();
    field->setHidden(false);
    BuildFeldValueTable(ActualTableSel);
}


