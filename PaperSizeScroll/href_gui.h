#ifndef HREF_GUI_H
#define HREF_GUI_H
#include <QStringList>
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
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

class Ui_Href_Gui
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *vboxLayout1;
    QLineEdit *text_href;
    QComboBox *target_href;
    QComboBox *otext_href;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem1;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *Href_Gui)
    {
    Href_Gui->setObjectName(QString::fromUtf8("Href_Gui"));
    Href_Gui->resize(QSize(420, 140).expandedTo(Href_Gui->minimumSizeHint()));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(Href_Gui->sizePolicy().hasHeightForWidth());
    Href_Gui->setSizePolicy(sizePolicy);
    Href_Gui->setMinimumSize(QSize(420, 140));
    Href_Gui->setMaximumSize(QSize(420, 140));
    gridLayout = new QGridLayout(Href_Gui);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(9);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(spacerItem, 1, 0, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    label = new QLabel(Href_Gui);
    label->setObjectName(QString::fromUtf8("label"));
    label->setTextFormat(Qt::PlainText);

    vboxLayout->addWidget(label);

    label_2 = new QLabel(Href_Gui);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setTextFormat(Qt::PlainText);

    vboxLayout->addWidget(label_2);

    label_3 = new QLabel(Href_Gui);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setTextFormat(Qt::PlainText);

    vboxLayout->addWidget(label_3);


    hboxLayout->addLayout(vboxLayout);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    text_href = new QLineEdit(Href_Gui);
    text_href->setObjectName(QString::fromUtf8("text_href"));
    vboxLayout1->addWidget(text_href);
    
    otext_href = new QComboBox(Href_Gui);
    otext_href->setObjectName(QString::fromUtf8("otext_href"));
    otext_href->setEditable (true);

    vboxLayout1->addWidget(otext_href);


    target_href = new QComboBox(Href_Gui);
    target_href->setObjectName(QString::fromUtf8("target_href"));

    vboxLayout1->addWidget(target_href);


    hboxLayout->addLayout(vboxLayout1);


    gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem1 = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);

    okButton = new QPushButton(Href_Gui);
    okButton->setObjectName(QString::fromUtf8("okButton"));

    hboxLayout1->addWidget(okButton);

    cancelButton = new QPushButton(Href_Gui);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    hboxLayout1->addWidget(cancelButton);


    gridLayout->addLayout(hboxLayout1, 2, 0, 1, 1);

    retranslateUi(Href_Gui);
  

    QMetaObject::connectSlotsByName(Href_Gui);
    } // setupUi

    void retranslateUi(QDialog *Href_Gui)
    {
    Href_Gui->setWindowTitle(QApplication::translate("Href_Gui", "Url / www", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Href_Gui", "Text:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Href_Gui", "Url:", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("Href_Gui", "Target / Name:", 0, QApplication::UnicodeUTF8));
    target_href->clear();
    target_href->addItem(QApplication::translate("Href_Gui", "_top", 0, QApplication::UnicodeUTF8));
    target_href->addItem(QApplication::translate("Href_Gui", "_self", 0, QApplication::UnicodeUTF8));
    target_href->addItem(QApplication::translate("Href_Gui", "_blank", 0, QApplication::UnicodeUTF8));
    target_href->addItem(QApplication::translate("Href_Gui", "_main", 0, QApplication::UnicodeUTF8));
    target_href->addItem(QApplication::translate("Href_Gui", "_menu", 0, QApplication::UnicodeUTF8));
    target_href->addItem(QApplication::translate("Href_Gui", "#name", 0, QApplication::UnicodeUTF8));
    okButton->setText(QApplication::translate("Href_Gui", "OK", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("Href_Gui", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Href_Gui);
    } // retranslateUi

};

namespace Ui {
    class Href_Gui: public Ui_Href_Gui {};
} // namespace Ui











//
/*  Save file as href_gui.h  */
/*  Class Href_Gui Created on Fri Jun 2 11:13:27 CEST 2006  */
//
#include <QPointer>
#include <QStringList>

/* typedef QMap<int, QStringList> Userconf;*/
    
class Href_Gui : public QDialog, public Ui::Href_Gui
{
	Q_OBJECT
	//
public:
    Href_Gui( QWidget* = 0 );
	static Href_Gui* self( QWidget* = 0 );
    inline QComboBox *linker() { return otext_href; }
    QStringList GetUserConfig();
    static QPointer<Href_Gui> _self;
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	
	
    QStringList hrefconfisuser;

	//
 public slots:
     void Acceptvars();
     void reject();
     void urlChanged( const int index );

};
//
#endif // HREF_GUI_H


