/*******************************************************************************
 * class GetMargin
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/


#include "getmargin.h"
//
/*  Save file as getmargin.cpp  */
/*  incomming class name GetMargin */
//
#include <QCloseEvent>
//

GetMargin::GetMargin( QWidget* parent )
	: QDialog( parent )
{
	setupUi( this );
    Genable = false;
    unitas = setter.value("CurrentRunningUnit").toString();
    if (unitas.size() < 1) {
    unitas = "pt";
    comboBox->setCurrentIndex(3);
    } else {
    comboBox->setCurrentIndex(comboBox->findText(unitas));
    UnitChange(unitas); 
    }
    current = QRectF ( 0, 0, 0, 0 ); 
	connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(UnitChange(QString)));
    ///////connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
    /////connect(doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
    ////////connect(doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
    //////////////connect(doubleSpinBox_4, SIGNAL(valueChanged(double)), this, SLOT(UpdateVars()));
    connect(pushButton, SIGNAL(clicked()),this, SLOT(UpdateVars()));
    
    Genable = true;
}


void GetMargin::UnitChange(const QString unite ) 
{
    unitas = unite;
    setter.setValue("CurrentRunningUnit",unitas);
    doubleSpinBox->setSuffix(QString(" %1.").arg(unitas)); 
    doubleSpinBox_2->setSuffix(QString(" %1.").arg(unitas)); 
    doubleSpinBox_3->setSuffix(QString(" %1.").arg(unitas)); 
    doubleSpinBox_4->setSuffix(QString(" %1.").arg(unitas)); 
    
    doubleSpinBox_4->setValue ( Pointo(a1,unitas) ); /* top */
    doubleSpinBox_2->setValue ( Pointo(a2,unitas) );   /* right*/
    doubleSpinBox->setValue (  Pointo(a3,unitas) );   /* bottom */
    doubleSpinBox_3->setValue ( Pointo(a4,unitas) );  /* left */
    
    
}

void GetMargin::UpdateVars() 
{
    if (!Genable) {
    return;
    }
    
    const qreal TopBB = ToPoint(doubleSpinBox_4->value(),unitas);
    a1 = TopBB;
    const qreal RightBB = ToPoint(doubleSpinBox_2->value(),unitas);
    a2 = RightBB;
    const qreal BottomBB = ToPoint(doubleSpinBox->value(),unitas);
    a3 = BottomBB;
    const qreal LeftBB = ToPoint(doubleSpinBox_3->value(),unitas);
    a4 = LeftBB;
    current = QRectF ( TopBB, RightBB ,BottomBB , LeftBB );  /* css sequense */
    /*
    doubleSpinBox_4->setValue ( current.x() );  top 
    doubleSpinBox_2->setValue ( current.y() ); 
    doubleSpinBox->setValue ( current.width() );    bottom 
    doubleSpinBox_3->setValue ( current.height() );  left
    */
    /* QRectF ( qreal x, qreal y, qreal width, qreal height )  */
}

void GetMargin::closeEvent( QCloseEvent* e )
{
    UpdateVars();
	e->accept();
}

/* Q_ASSERT ( bool test )  */ 

/* QString one;  toLower() toUpper()  split(",")  endsWith() startsWith()  QByteArray QString::toAscii() */
/* int nummer; */ 
/* QStringList list;  join();  */ 
/* QUrl url; */ 
/* QByteArray byteArray; */ 
/* bool having; */ 
/* QDomDocument doc;*/ 
/* QDomElement root;*/ 
