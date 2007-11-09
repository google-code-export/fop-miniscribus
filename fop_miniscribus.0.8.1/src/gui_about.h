#ifndef GUI_ABOUT_H
#define GUI_ABOUT_H
//
//
/*  Save file as gui_about.h  */
/*  incomming class name Gui_About */
#include <QCloseEvent>
#include <QPointer>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>





#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

class Ui_Gui_About
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *hboxLayout1;
    QPushButton *okButton;

    void setupUi(QDialog *Gui_About)
    {
    if (Gui_About->objectName().isEmpty())
        Gui_About->setObjectName(QString::fromUtf8("Gui_About"));
    QSize size(629, 391);
    size = size.expandedTo(Gui_About->minimumSizeHint());
    Gui_About->resize(size);
    QPalette palette;
    QBrush brush(QColor(0, 0, 0, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    QBrush brush1(QColor(236, 233, 216, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush1);
    QBrush brush2(QColor(255, 255, 255, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Light, brush2);
    QBrush brush3(QColor(241, 239, 226, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
    QBrush brush4(QColor(172, 168, 153, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
    QBrush brush5(QColor(157, 155, 144, 255));
    brush5.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Active, QPalette::Text, brush);
    palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
    palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Active, QPalette::Base, brush2);
    palette.setBrush(QPalette::Active, QPalette::Window, brush2);
    QBrush brush6(QColor(113, 111, 100, 255));
    brush6.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
    QBrush brush7(QColor(49, 106, 197, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Highlight, brush7);
    palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
    QBrush brush8(QColor(0, 0, 255, 255));
    brush8.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Link, brush8);
    QBrush brush9(QColor(255, 0, 255, 255));
    brush9.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
    QBrush brush10(QColor(232, 232, 232, 255));
    brush10.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
    palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
    palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
    palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
    palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Link, brush8);
    palette.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
    palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
    QBrush brush11(QColor(118, 116, 108, 255));
    brush11.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush11);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
    QBrush brush12(QColor(245, 244, 235, 255));
    brush12.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush12);
    palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush11);
    palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush11);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush7);
    palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::Link, brush8);
    palette.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
    palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
    Gui_About->setPalette(palette);
    Gui_About->setModal(true);
    gridLayout = new QGridLayout(Gui_About);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    gridLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 1, 1, 1);


    gridLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding), 2, 0, 1, 1);

    hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    hboxLayout->setMargin(0);
#endif
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

    hboxLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    label_3 = new QLabel(Gui_About);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setPixmap(QPixmap(QString::fromUtf8(":/img/about_as_image.png")));

    hboxLayout->addWidget(label_3);

    label = new QLabel(Gui_About);
    label->setObjectName(QString::fromUtf8("label"));
    label->setPixmap(QPixmap());

    hboxLayout->addWidget(label);

    label_2 = new QLabel(Gui_About);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setPixmap(QPixmap());

    hboxLayout->addWidget(label_2);


    hboxLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));


    gridLayout->addLayout(hboxLayout, 1, 0, 1, 2);

    hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout1->setSpacing(6);
#endif
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));

    hboxLayout1->addItem(new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum));

    okButton = new QPushButton(Gui_About);
    okButton->setObjectName(QString::fromUtf8("okButton"));

    hboxLayout1->addWidget(okButton);


    gridLayout->addLayout(hboxLayout1, 3, 0, 1, 2);


    retranslateUi(Gui_About);
    QObject::connect(okButton, SIGNAL(clicked()), Gui_About, SLOT(accept()));

    QMetaObject::connectSlotsByName(Gui_About);
    } // setupUi

    void retranslateUi(QDialog *Gui_About)
    {
    Gui_About->setWindowTitle(QApplication::translate("Gui_About", "Dialog", 0, QApplication::UnicodeUTF8));
    label_3->setText(QString());
    label->setText(QString());
    label_2->setText(QString());
    okButton->setText(QApplication::translate("Gui_About", "OK", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Gui_About);
    } // retranslateUi

};

namespace Ui {
    class Gui_About: public Ui_Gui_About {};
} // namespace Ui










//
class Gui_About : public QDialog, public Ui::Gui_About
{
    Q_OBJECT
//
public: Gui_About( QWidget* = 0 )
{
 setupUi( this );
}
protected:
private:
signals:

public slots:

};
//
#endif // GUI_ABOUT_H

