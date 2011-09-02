#include "invoice.h"
//
/*  Save file as invoice.cpp  */
/*  Class Invoice Created on Fri Sep 29 18:55:33 CEST 2006  */
//
#include <QCloseEvent>
//

//
Invoice::Invoice()
{
	setupUi( this );
    largominimocentro = 760;
    summlinerfak = 1;
    FirstSetup();
    flow = new FlowLayout(piano_centrale,2,8);
    piano_centrale->setMinimumSize(QSize(largominimocentro, 15 ));
    setMinimumSize(QSize(largominimocentro + 110, 600));
    connect( aggiungilineax, SIGNAL( clicked() ),this, SLOT( AppendLine()));
    /* aggiungilineax */
    AppendLine();
}


void Invoice::FirstSetup()
{
    QDateTime timer( QDateTime::currentDateTime() ); 
    const uint rifermineto = timer.toTime_t();
    riferimNR = QString("RT-%1").arg(QString::number(rifermineto)); 
    riferimento_generato->setText(riferimNR);
    /* verifica se il numero esiste */
}

void Invoice::AppendLine()
{
    sumflow = flow->count();
    if ( sumflow <= 9) {
    aggiungilineax->setDisabled (false);   /* disable button */
    summlinerfak++;
    fiocco = new ItemLine(piano_centrale);
    fiocco->setNumerate(summlinerfak);
    fiocco->setTitle(tr("Testo di descrizione %1").arg(summlinerfak));
    fiocco->setQuant(1);
    fiocco->setUnitprice(0);
    connect(fiocco, SIGNAL(RemoveItems(int)), this, SLOT(RemoveItemFromList(int)));
    connect(fiocco, SIGNAL(RecalCalc()), this, SLOT(WakeUpsAll())); 
    connect(fiocco, SIGNAL(MoveUp(int)), this, SLOT(RegisterUpLine(int))); 
    flow->addWidget(fiocco);
    flow->setGeometry(QRect(QPoint(10,10),QSize(800,800)));
    WakeUpsAll();
    /*qDebug() << "### sendaltos "  << sendaltos;*/ 
    lines.append( fiocco );   /* QList<ItemLine *> lines; */
    } else {
    WakeUpsAll();
    aggiungilineax->setDisabled (true);   /* disable button */
    }
}


void Invoice::RegisterUpLine( int nummerfak )
{
    if ( flow->count() >= 1 and lines.size() >= 2) {
       int effettivo =  Linefindstatus(nummerfak);
       int movetonew =  effettivo - 1;
          if (movetonew >= 0) {
           /* replace items start  */
                            lines[effettivo]->CalcSumLine();
              QString asx = lines[effettivo]->Get_Title_Pos();
              double bsx = lines[effettivo]->Get_Menge_Pos();
              double csx = lines[effettivo]->Get_Unitprice_Pos();
              
                             lines[movetonew]->CalcSumLine();
              QString fsx = lines[movetonew]->Get_Title_Pos();
              double gsx = lines[movetonew]->Get_Menge_Pos();
              double hsx = lines[movetonew]->Get_Unitprice_Pos();
              /* swap line up & down */
              lines[movetonew]->setTitle(asx);
              lines[movetonew]->setQuant(bsx);
              lines[movetonew]->setUnitprice(csx);
              /* swap line up & down */
              lines[effettivo]->setTitle(fsx);
              lines[effettivo]->setQuant(gsx);
              lines[effettivo]->setUnitprice(hsx);
              /* recalc to new value */
              lines[effettivo]->CalcSumLine();
              lines[movetonew]->CalcSumLine();
              
           /* replace items stop  */
          } 
    }
WakeUpsAll();
}




void Invoice::WakeUpsAll()
{
      sumflow = flow->count();
      qlistersizert = lines.size();
      qDebug() << "### WakeUpsAll sumflow ->"  << sumflow << " qlist ->" << qlistersizert;
    
      if ( sumflow <= 9) {
       aggiungilineax->setDisabled (false);    
      } else {
       aggiungilineax->setDisabled (true);     
      } 
      SummarizeTotal();
      int altor = 35;
      int sendaltos, totalwidg;
          sendaltos = altor * (sumflow + 1);
          totalwidg = 340 + sendaltos;
      
            if (sumflow > 0) {
            piano_centrale->setMinimumSize(QSize(largominimocentro, sendaltos));
            setMinimumSize(QSize(largominimocentro, totalwidg));
            } else {
            piano_centrale->setMinimumSize(QSize(largominimocentro, altor)); 
            }
      
      
      
}


void Invoice::RemoveItemFromList( int nummerfak )
{
       int removesx =  Linefindstatus(nummerfak);
       if (removesx >= 0) {
       lines.removeAt(removesx);
       }
       WakeUpsAll();
       sumflow = flow->count();
       qlistersizert = lines.size();
       if (qlistersizert < 1) {
          AppendLine(); 
       }
}

int Invoice::Linefindstatus( int nummerfak )
{
    int finder = -1;
    for ( int x = 0; x < lines.size() ; ++x ) {
        int grab = lines[x]->Get_Nummer_Pos();
        if ( grab == nummerfak ) {
          finder = x;  
        }
    }
return finder;
}




void  Invoice::SummarizeTotal()
{
    double summsval = 0;
    for ( int x = 0; x < lines.size() ; ++x ) {
        summsval +=lines[x]->Get_Line_Tot();
    }
    bool ok;
    double d;
    QString istx = QString("%1").arg(summsval);
    QLocale c(QLocale::C);
    d = c.toDouble( istx , &ok );
    fatt_totale_fr->setText(QString("Valuta  %1 ").arg(d) ); 
}




void Invoice::closeEvent( QCloseEvent* e )
{
	e->accept();
}



/* -------------------------ItemLine::ItemLine----------------------------------------------*/



ItemLine::ItemLine(QWidget *parent)
: QWidget( parent )
{
	gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(1);
    gridLayout->setMargin(1);
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(0);
    hboxLayout->setMargin(0);
    
    capitolo = new QLabel(this);
    capitolo->setText(tr(" a1."));
    fine = new QLabel(this);
    fine->setText(tr(" "));
    
    
    
    fatextline = new QLineEdit(this);
    fatextline->setMinimumSize(QSize(380, 18));
    fatextline->setMaximumSize(QSize(380, 18));
    fquant = new QDoubleSpinBox(this);
    fquant->setMaximum(9999.99);
    unitafr = new QDoubleSpinBox(this);
    unitafr->setMaximum(1e+06);
    sommalineafr = new QDoubleSpinBox(this);
    sommalineafr->setMaximum(1e+06);
    eliminalinea = new QToolButton(this);
    eliminalinea->setText(tr("Del"));
    
    moveup = new QToolButton(this);
    moveup->setText(tr("Up"));
    
    hboxLayout->addWidget(capitolo);
    hboxLayout->addWidget(fatextline);
    hboxLayout->addWidget(fine);
    hboxLayout->addWidget(fquant);
    hboxLayout->addWidget(fine);
    hboxLayout->addWidget(unitafr);
    hboxLayout->addWidget(fine);
    hboxLayout->addWidget(sommalineafr);
    hboxLayout->addWidget(fine);
    hboxLayout->addWidget(moveup);
    hboxLayout->addWidget(fine);
    hboxLayout->addWidget(eliminalinea);
    hboxLayout->addWidget(fine);
    gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);
    connect( moveup, SIGNAL( clicked() ),this, SLOT( MoveUpActuals()));
    connect( eliminalinea, SIGNAL( clicked() ),this, SLOT( RemoveActuals()));
    connect(fquant, SIGNAL(valueChanged(double)), this, SLOT(CalcSumLine()));
    connect(unitafr, SIGNAL(valueChanged(double)), this, SLOT(CalcSumLine()));
    connect(fatextline, SIGNAL(textChanged(QString)), this, SLOT(CalcSumLine()));
    connect(fatextline, SIGNAL(editingFinished()), this, SLOT(CalcSumLine()));
}

void ItemLine::MoveUpActuals()
{
    emit MoveUp(chain);
}

void ItemLine::RemoveActuals()
{
    emit RemoveItems(chain);
    delete this;
}


void ItemLine::setNumerate( int pos )
{
    chain = pos;
    if (chain > 9) {
    capitolo->setText(QString(" ID %1 ").arg(QString::number(pos))); 
    } else {
    capitolo->setText(QString(" ID 0%1 ").arg(QString::number(pos))); 
    }
}

void ItemLine::setTitle( QString title )
{ 
    fatextline->setText(title);
}
void ItemLine::setQuant( double menge )
{
    fquant->setValue(menge);
    /*
    int value () const
    void setValue ( int val )
    */
}
void ItemLine::setUnitprice( double unitario )
{
    unitafr->setValue(unitario);
}

void ItemLine::CalcSumLine()
{
   dicituratitolo = fatextline->text();
   menge = fquant->value();
   afranchi = unitafr->value();
   sommaline = menge * afranchi;
   sommalineafr->setValue(sommaline);
   emit RecalCalc();
}
