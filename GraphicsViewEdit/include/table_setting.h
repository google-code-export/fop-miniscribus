#ifndef TABLE_SETTING_H
#define TABLE_SETTING_H

#include <QStringList>
#include <QSettings>
#include <QActionGroup>
#include <QColorDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QTextCursor>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QFontDatabase>
#include <QTextBlockFormat>
#include <QTextListFormat>
#include <QTextFormat>
#include <QTextList>
#include <QTextCodec>
#include <QByteArray>
#include <QDebug>
#include <QSettings>
#include <QErrorMessage>
#include <QProcess>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>
#include <QInputDialog>
#include <QMenu>
#include <QTextLength>


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
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>


class Ui_TableFormat
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_5;
    QVBoxLayout *vboxLayout1;
    QSpinBox *largotable;
    QSpinBox *vborder;
    QSpinBox *tapadding;
    QSpinBox *taspacing;
    QToolButton *tablecolor;
    QComboBox *xalign;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *TableFormat)
    {
    if (TableFormat->objectName().isEmpty())
        TableFormat->setObjectName(QString::fromUtf8("TableFormat"));
    TableFormat->resize(205, 239);
    TableFormat->setMaximumSize(QSize(255, 250));
    gridLayout = new QGridLayout(TableFormat);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    groupBox = new QGroupBox(TableFormat);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    gridLayout1 = new QGridLayout(groupBox);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));

    vboxLayout->addWidget(label);

    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    vboxLayout->addWidget(label_2);

    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    vboxLayout->addWidget(label_4);

    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    vboxLayout->addWidget(label_3);

    label_6 = new QLabel(groupBox);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    vboxLayout->addWidget(label_6);

    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    vboxLayout->addWidget(label_5);


    hboxLayout->addLayout(vboxLayout);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    largotable = new QSpinBox(groupBox);
    largotable->setObjectName(QString::fromUtf8("largotable"));
    largotable->setMaximum(100);
    largotable->setValue(100);

    vboxLayout1->addWidget(largotable);

    vborder = new QSpinBox(groupBox);
    vborder->setObjectName(QString::fromUtf8("vborder"));
    vborder->setMaximum(10);

    vboxLayout1->addWidget(vborder);

    tapadding = new QSpinBox(groupBox);
    tapadding->setObjectName(QString::fromUtf8("tapadding"));
    tapadding->setMaximum(44);

    vboxLayout1->addWidget(tapadding);

    taspacing = new QSpinBox(groupBox);
    taspacing->setObjectName(QString::fromUtf8("taspacing"));
    taspacing->setMaximum(44);

    vboxLayout1->addWidget(taspacing);

    tablecolor = new QToolButton(groupBox);
    tablecolor->setObjectName(QString::fromUtf8("tablecolor"));
    tablecolor->setIconSize(QSize(22, 22));

    vboxLayout1->addWidget(tablecolor);

    xalign = new QComboBox(groupBox);
    xalign->setObjectName(QString::fromUtf8("xalign"));

    vboxLayout1->addWidget(xalign);


    hboxLayout->addLayout(vboxLayout1);


    gridLayout1->addLayout(hboxLayout, 0, 0, 1, 1);


    gridLayout->addWidget(groupBox, 0, 0, 1, 1);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    pushButton_2 = new QPushButton(TableFormat);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    hboxLayout1->addWidget(pushButton_2);

    pushButton = new QPushButton(TableFormat);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    hboxLayout1->addWidget(pushButton);


    gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);


    retranslateUi(TableFormat);
    QObject::connect(pushButton, SIGNAL(clicked()), TableFormat, SLOT(reject()));
    QObject::connect(pushButton_2, SIGNAL(clicked()), TableFormat, SLOT(accept()));

    QMetaObject::connectSlotsByName(TableFormat);
    } // setupUi

    void retranslateUi(QDialog *TableFormat)
    {
    TableFormat->setWindowTitle(QApplication::translate("TableFormat", "Table Format", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("TableFormat", "Table setting", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("TableFormat", "Table widht:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("TableFormat", "Border:", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("TableFormat", "Cellpadding:", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("TableFormat", "CellSpacing", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("TableFormat", "Background color:", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("TableFormat", "Align table:", 0, QApplication::UnicodeUTF8));
    largotable->setSuffix(QApplication::translate("TableFormat", " %", 0, QApplication::UnicodeUTF8));
    tablecolor->setText(QApplication::translate("TableFormat", "...", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("TableFormat", "OK", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("TableFormat", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(TableFormat);
    } // retranslateUi

};

namespace Ui {
    class TableFormat: public Ui_TableFormat {};
}

//
/*  Save file as table_setting.h  */
/*  Class Table_Setting Created on Tue Sep 12 13:46:10 CEST 2006  */
//
#include <QPointer>
//
class Table_Setting : public QDialog, public Ui::TableFormat
{
	Q_OBJECT
	//
public:
	static Table_Setting* self( QWidget* = 0 );
    void SetFormat( QTextTable *existingtable );
    QTextTableFormat GetNewFormats();
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	Table_Setting( QWidget* = 0 );
	static QPointer<Table_Setting> _self;
    QTextTable *format_table;
    QTextTableFormat formtab;
    QTextLength trolengh;
    qreal xpadding;
    qreal xspacing;
    QBrush xbgcolor;
    int setfoundalign;
    int allineaesistente;
    qreal intborder;
    QColor normalcolor;
    QAction *TableColor;
    QPixmap pix;
	//
 public slots:
     void ChoisColor();
     void UpdateFormats();

};
//
#endif // TABLE_SETTING_H


