#ifndef INVOICE_H
#define INVOICE_H
//
#include "ui_invoice.h"
//
/*  Save file as invoice.h  */
/*  Class Invoice Created on Fri Sep 29 18:55:33 CEST 2006  */
//
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
#include "flowlayout.h"


class ItemLine : public QWidget
{
    Q_OBJECT
public:
	ItemLine(QWidget *parent = 0L);
    void setNumerate( int pos );
    void setTitle( QString title );
    void setQuant( double menge );
    void setUnitprice( double unitario );
    QString dicituratitolo;
    double menge;
    double afranchi;
    double sommaline;
    int chain;
    inline QString Get_Title_Pos()  { return dicituratitolo; }
    inline double Get_Menge_Pos()  { return menge; }
    inline double Get_Unitprice_Pos()  { return afranchi; }
    inline int Get_Nummer_Pos()  { return chain; }
    inline double Get_Line_Tot()  { return sommaline; }
private:
   QGridLayout *gridLayout;
   QHBoxLayout *hboxLayout;
   QLineEdit *fatextline;
   QDoubleSpinBox *fquant;
   QDoubleSpinBox *unitafr;
   QDoubleSpinBox *sommalineafr;
   QToolButton *eliminalinea;
   QToolButton *moveup;
   QLabel *capitolo;
   QLabel *fine;
  
signals:
    void RemoveItems(int pos);
    void MoveUp(int pos);
    void RecalCalc();
public slots:
   void CalcSumLine();
   void RemoveActuals();
   void MoveUpActuals();
};



class FlowLayout;

class Invoice : public QWidget, public Ui::Invoice
{
	Q_OBJECT
	//
public:
	Invoice();
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
    int summlinerfak;
    int      sumflow;
    int qlistersizert;
    int largominimocentro;
    ItemLine *fiocco;
    FlowLayout *flow;
    QList<ItemLine *> lines;
    int Linefindstatus( int nummerfak );
    void FirstSetup();
    QString riferimNR;

signals:
    void ReloadPageFull();
	//
 public slots:
     void AppendLine();
     void RegisterUpLine( int nummerfak );
     void WakeUpsAll();
     void RemoveItemFromList( int nummerfak );
     void  SummarizeTotal();


};
//
















#endif // INVOICE_H


