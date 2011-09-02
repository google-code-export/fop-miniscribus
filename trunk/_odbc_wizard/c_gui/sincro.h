#ifndef SINCRO_H
#define SINCRO_H

#include <QDomDocument>
#include <QtDebug>
#include <QDebug> 
#include <QSettings>
#include <QDomDocument>
#include <QDomElement>
#include <QDomImplementation>
#include <QDomProcessingInstruction>
#include <QFile>
#include <QTextCodec>

#include <QPointer>
#include <QApplication>
#include <QtGui>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>
#include <QFrame>
#include <QtGui>
#include <QtSql>
#include <QSqlQuery>
#include "main.h"
#include "ui_sincro.h"
#include <QMap>

#define PALLA 1

//
/*  Save file as sincro.h  */
/*  Class Sincro Created on Thu Oct 5 20:17:37 CEST 2006  */
//
#include <QPointer>
#include <QSettings>

class CateSetting : public QObject
{
    Q_OBJECT
    
public:
        CateSetting( QString pezzo , QString table , QString source , int pos ) { 
          ASA = pezzo;
          TA = table;
          posi = pos;
          YourF = source;
        }
        inline void SetASI( QString a ) { ASA = a; } 
        inline void RemoveNow() { delete this; }
        inline void SetTable( QString a ) { TA = a; } 
        inline void SetPos( int pos ) { posi = pos; } 
        inline void SetSource( QString a ) { YourF = a; } 
        inline QString Getasi() const { return ASA; }
        inline QString Getable()  { return TA; }
        inline QString Gesource()  { return YourF; }
        int Indexnum() const { return posi; }
private:
    int posi;
    QString ASA;
    QString TA;
    QString YourF;
};








typedef QMap<int, QStringList> UpdateDom;
    
class Sincro : public QDialog, public Ui::Sincro
{
	Q_OBJECT
	//
public:
	static Sincro* self( QWidget* = 0 );
    Sincro( QWidget* = 0 );
    void SetConnection( QSqlDatabase * connid );
	//
protected:
	void closeEvent( QCloseEvent* );
    
	//
private:
    QList<CateSetting *> itecats;
    bool openwork;
    UpdateDom readatt;
	QSqlDatabase *dbi;
    QSqlDatabase USDB;
    QSettings setter;
	static QPointer<Sincro> _self;
    int largominimocentro;
    int summline;
    int sumflow;
    QString actualdbname;
    QString betterNameDB;
    QStringList connectionNames;
    QStringList ConnHumanNames;
    QStringList FieldNamesY;
    QStringList tables;
    QString conunam;
    QString ActualTableSel;
    QString ActualSOURCCEFIELD;
    QString ActualFieldName;
    QString QtNameOn(const QSqlDatabase &db);
    QString Att(QDomElement e , QString name );
    QString WriteConfigFile();
    int POSX;
    QStringList END_asi;
    QStringList END_table;
    QString xfileNameii;
    bool FindSavedList( int doid );
    bool FindDoppel( QString so );
    bool SAVE_and_Next();
	//
signals: 
     void FaseNext( int po );
     void MakeParentQuery(QString s);
 public slots:
    void WakeUPAll();
    void BuildField();
    void BuildFeldValueTable( QString ytab );
    void RefreshConnection();
     void ResetFieldQuery();
     void End_All();
     void ReadConfigIn();
     void OpenFieldSteep( int goit );
     void Count_Go_Next();
     void Count_Go_Prev();
     void GoQuery();
     void SetNoExport();

};


//
#endif // SINCRO_H


