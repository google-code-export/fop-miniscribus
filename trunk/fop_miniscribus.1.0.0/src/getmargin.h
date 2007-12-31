#ifndef GETMARGIN_H
#define GETMARGIN_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>

#include "BasicConfig.h"
#include "main.h"


#include "ui_getmargin.h"
/*#include "main.h"*/
//
/*  Save file as getmargin.h  */
/*  incomming class name GetMargin */
//   public QObject | public QHttp | public QThread                                    // 
#include <QPointer>
//
class GetMargin : public QDialog, public Ui::GetMargin
{
     Q_OBJECT
//
public:
GetMargin( QWidget* = 0 );
inline QRectF Get_margin() { return current; }
inline void  Set_margin( const QRectF n ) {
/* QRectF ( qreal x, qreal y, qreal width, qreal height )   GoPoint(,unitas) */
    current = n; 
    Genable = false;
    /* save internal on point */
a1 = n.x() ;
a2 = n.y();
a3 = n.width();
a4 = n.height();
    
    //////qDebug() << "### incomming -> " << n;
    doubleSpinBox_4->setValue ( Pointo(n.x(),unitas) ); /* top */
    doubleSpinBox_2->setValue ( Pointo(n.y(),unitas) );   /* right*/
    doubleSpinBox->setValue (  Pointo(n.width(),unitas) );   /* bottom */
    doubleSpinBox_3->setValue ( Pointo(n.height(),unitas) );  /* left */
    Genable = true;
    ///////UnitChange("pt"); 
/*marge->Set_margin( QRectF (fo.topMargin(), fo.rightMargin() ,fo.bottomMargin() , fo.leftMargin() ) ); */    
}
protected:
QString one;
qreal a1;
qreal a2;
qreal a3;
qreal a4;
bool Genable;
int nummer;
QSettings setter;
QString unitas;
QRectF current;
void closeEvent( QCloseEvent* );
private:
 ////////GetMargin( QWidget* = 0 );
 /////static QPointer<GetMargin> _self;
 void Load_Connector();
signals:

public slots:
   void UnitChange(const QString unite );
   void UpdateVars();
};
//
#endif // GETMARGIN_H

