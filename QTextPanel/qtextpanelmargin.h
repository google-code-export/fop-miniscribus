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



#ifndef QTEXTPANELMARGIN_H
#define QTEXTPANELMARGIN_H
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>



#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>


#include "qtextpanelmime.h"

class Ui_GetMargin
{
public:
	QGridLayout *gridLayout;
	QVBoxLayout *vboxLayout;
	QGroupBox *groupBox;
	QGridLayout *gridLayout1;
	QHBoxLayout *hboxLayout;
	QVBoxLayout *vboxLayout1;
	QSpacerItem *spacerItem;
	QLabel *label_2;
	QDoubleSpinBox *doubleSpinBox_3;
	QSpacerItem *spacerItem1;
	QVBoxLayout *vboxLayout2;
	QLabel *label;
	QDoubleSpinBox *doubleSpinBox_4;
	QSpacerItem *spacerItem2;
	QLabel *label_4;
	QDoubleSpinBox *doubleSpinBox;
	QVBoxLayout *vboxLayout3;
	QSpacerItem *spacerItem3;
	QLabel *label_3;
	QDoubleSpinBox *doubleSpinBox_2;
	QSpacerItem *spacerItem4;
	QHBoxLayout *hboxLayout1;
	QLabel *label_5;
	QSpacerItem *spacerItem5;
	QComboBox *comboBox;
	QHBoxLayout *hboxLayout2;
	QSpacerItem *spacerItem6;
	QPushButton *pushButton;
	QPushButton *pushButton_2;

	void setupUi(QDialog *GetMargin)
	{
		if (GetMargin->objectName().isEmpty())
		{
			GetMargin->setObjectName(QString::fromUtf8("GetMargin"));
		}
		GetMargin->resize(306, 207);
		GetMargin->setMaximumSize(QSize(310, 210));

		gridLayout = new QGridLayout(GetMargin);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

		vboxLayout = new QVBoxLayout();
		vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));

		groupBox = new QGroupBox(GetMargin);
		groupBox->setObjectName(QString::fromUtf8("groupBox"));

		gridLayout1 = new QGridLayout(groupBox);
		gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));

		hboxLayout = new QHBoxLayout();
		hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

		vboxLayout1 = new QVBoxLayout();
		vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));

		spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		vboxLayout1->addItem(spacerItem);

		label_2 = new QLabel(groupBox);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		vboxLayout1->addWidget(label_2);

		doubleSpinBox_3 = new QDoubleSpinBox(groupBox);
		doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
		doubleSpinBox_3->setDecimals(4);
		doubleSpinBox_3->setMinimum(-50);
		doubleSpinBox_3->setMaximum(100000);
		vboxLayout1->addWidget(doubleSpinBox_3);

		spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		vboxLayout1->addItem(spacerItem1);

		hboxLayout->addLayout(vboxLayout1);

		vboxLayout2 = new QVBoxLayout();
		vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));

		label = new QLabel(groupBox);
		label->setObjectName(QString::fromUtf8("label"));
		vboxLayout2->addWidget(label);

		doubleSpinBox_4 = new QDoubleSpinBox(groupBox);
		doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));
		doubleSpinBox_4->setDecimals(4);
		doubleSpinBox_4->setMinimum(-50);
		doubleSpinBox_4->setMaximum(100000);
		vboxLayout2->addWidget(doubleSpinBox_4);

		spacerItem2 = new QSpacerItem(20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);
		vboxLayout2->addItem(spacerItem2);

		label_4 = new QLabel(groupBox);
		label_4->setObjectName(QString::fromUtf8("label_4"));

		vboxLayout2->addWidget(label_4);

		doubleSpinBox = new QDoubleSpinBox(groupBox);
		doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
		doubleSpinBox->setDecimals(4);
		doubleSpinBox->setMinimum(-50);
		doubleSpinBox->setMaximum(100000);

		vboxLayout2->addWidget(doubleSpinBox);
		hboxLayout->addLayout(vboxLayout2);

		vboxLayout3 = new QVBoxLayout();
		vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
		spacerItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		vboxLayout3->addItem(spacerItem3);

		label_3 = new QLabel(groupBox);
		label_3->setObjectName(QString::fromUtf8("label_3"));
		vboxLayout3->addWidget(label_3);

		doubleSpinBox_2 = new QDoubleSpinBox(groupBox);
		doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
		doubleSpinBox_2->setDecimals(4);
		doubleSpinBox_2->setMinimum(-50);
		doubleSpinBox_2->setMaximum(100000);
		vboxLayout3->addWidget(doubleSpinBox_2);

		spacerItem4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		vboxLayout3->addItem(spacerItem4);

		hboxLayout->addLayout(vboxLayout3);

		gridLayout1->addLayout(hboxLayout, 0, 0, 1, 1);

		vboxLayout->addWidget(groupBox);

		hboxLayout1 = new QHBoxLayout();
		hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));

		label_5 = new QLabel(GetMargin);
		label_5->setObjectName(QString::fromUtf8("label_5"));
		hboxLayout1->addWidget(label_5);

		spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		hboxLayout1->addItem(spacerItem5);

		comboBox = new QComboBox(GetMargin);
		comboBox->setObjectName(QString::fromUtf8("comboBox"));

		hboxLayout1->addWidget(comboBox);

		vboxLayout->addLayout(hboxLayout1);

		hboxLayout2 = new QHBoxLayout();
		hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));

		spacerItem6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		hboxLayout2->addItem(spacerItem6);

		pushButton = new QPushButton(GetMargin);
		pushButton->setObjectName(QString::fromUtf8("pushButton"));
		hboxLayout2->addWidget(pushButton);

		pushButton_2 = new QPushButton(GetMargin);
		pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
		hboxLayout2->addWidget(pushButton_2);

		vboxLayout->addLayout(hboxLayout2);

		gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);

		retranslateUi(GetMargin);
		QObject::connect(pushButton, SIGNAL(clicked()), GetMargin, SLOT(accept()));
		QObject::connect(pushButton_2, SIGNAL(clicked()), GetMargin, SLOT(reject()));

		comboBox->setCurrentIndex(0);

		QMetaObject::connectSlotsByName(GetMargin);
	}

	void retranslateUi(QDialog *GetMargin)
	{
		GetMargin->setWindowTitle(QApplication::translate("GetMargin", "GetMargin Padding", 0, QApplication::UnicodeUTF8));
		groupBox->setTitle(QApplication::translate("GetMargin", "Margin/Padding", 0, QApplication::UnicodeUTF8));
		label_2->setText(QApplication::translate("GetMargin", "Left", 0, QApplication::UnicodeUTF8));
		label->setText(QApplication::translate("GetMargin", "Top", 0, QApplication::UnicodeUTF8));
		label_4->setText(QApplication::translate("GetMargin", "Bottom", 0, QApplication::UnicodeUTF8));
		label_3->setText(QApplication::translate("GetMargin", "Right", 0, QApplication::UnicodeUTF8));
		label_5->setText(QApplication::translate("GetMargin", "Unit:", 0, QApplication::UnicodeUTF8));
		pushButton->setText(QApplication::translate("GetMargin", "OK", 0, QApplication::UnicodeUTF8));
		pushButton_2->setText(QApplication::translate("GetMargin", "Close", 0, QApplication::UnicodeUTF8));

		doubleSpinBox_3->setSuffix(QApplication::translate("GetMargin", " pt.", 0, QApplication::UnicodeUTF8));
		doubleSpinBox_4->setSuffix(QApplication::translate("GetMargin", " pt.", 0, QApplication::UnicodeUTF8));
		doubleSpinBox->setSuffix(QApplication::translate("GetMargin", " pt.", 0, QApplication::UnicodeUTF8));
		doubleSpinBox_2->setSuffix(QApplication::translate("GetMargin", " pt.", 0, QApplication::UnicodeUTF8));

		comboBox->insertItems(0, QStringList()
		                      << QApplication::translate("GetMargin", "mm", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "cm", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "inch", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "pt", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "em", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "dm", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "pi", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "dd", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "cc", 0, QApplication::UnicodeUTF8)
		                      << QApplication::translate("GetMargin", "in", 0, QApplication::UnicodeUTF8)
		                     );
		Q_UNUSED(GetMargin);
	}
};

namespace Ui
{
	class GetMargin: public Ui_GetMargin {};
}

class GetMargin : public QDialog, public Ui::GetMargin
{
	Q_OBJECT

	private:
	////////GetMargin( QWidget* = 0 );
	/////static QPointer<GetMargin> _self;
		void Load_Connector();

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
		void closeEvent(QCloseEvent*);

	public:
		GetMargin(QWidget* = 0);
		inline QRectF Get_margin()
		{
			return current;
		}
		inline void  Set_margin(const QRectF n)
		{
			/* QRectF ( qreal x, qreal y, qreal width, qreal height )   GoPoint(,unitas) */
			current = n;
			Genable = false;
			/* save internal on point */
			a1 = n.x() ;
			a2 = n.y();
			a3 = n.width();
			a4 = n.height();

			//////qDebug() << "### incomming -> " << n;
			doubleSpinBox_4->setValue(pointTo(n.x(),unitas));    /* top */
			doubleSpinBox_2->setValue(pointTo(n.y(),unitas));      /* right*/
			doubleSpinBox->setValue(pointTo(n.width(),unitas));       /* bottom */
			doubleSpinBox_3->setValue(pointTo(n.height(),unitas));     /* left */
			Genable = true;
			///////UnitChange("pt");
			/*marge->Set_margin( QRectF (fo.topMargin(), fo.rightMargin() ,fo.bottomMargin() , fo.leftMargin() ) ); */
		}

	public slots:
		void UnitChange(const QString unite);
		void UpdateVars();
};

#endif // QTEXTPANELMARGIN_H
