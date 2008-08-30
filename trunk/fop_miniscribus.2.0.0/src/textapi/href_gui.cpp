#include "href_gui.h"
//
/*  Save file as href_gui.cpp  */
/*  Class Href_Gui Created on Fri Jun 2 11:13:28 CEST 2006  */
//
#include <QCloseEvent>
//
QPointer<Href_Gui> Href_Gui::_self = 0L;
//
Href_Gui* Href_Gui::self( QWidget* parent )
{
	if ( !_self )
		_self = new Href_Gui( parent );
	return _self;
}
//
Href_Gui::Href_Gui( QWidget* parent )
	: QDialog( parent )
{
	setupUi( this );
    hrefconfisuser.clear(); 
    connect(okButton, SIGNAL(clicked()), this, SLOT(Acceptvars()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(otext_href, SIGNAL(currentIndexChanged(int)), this, SLOT(urlChanged(int)));




//////QComboBox *otext_href;

}

void Href_Gui::urlChanged( const int index )
{
   const int internals = otext_href->itemData(index).toInt();
   if (internals == 2) {
   target_href->setCurrentIndex(5);
   target_href->setDisabled(true);
   } else {
   target_href->setDisabled(false);
   }

}


void Href_Gui::Acceptvars()
{
    QString te = text_href->text();
    QString url = otext_href->currentText();
    QString target = target_href->itemText(target_href->currentIndex()); 
    if (te.size() < 1 or  url.size() < 1) {
        QMessageBox::warning( this, tr( "Error Text!" ),tr("Please set a valid url"));
        return;
    }
    hrefconfisuser.clear(); 
    hrefconfisuser.append(te);
    hrefconfisuser.append(url);
    hrefconfisuser.append(target);
    accept();
}
QStringList Href_Gui::GetUserConfig()
{
    return hrefconfisuser;
}
void Href_Gui::reject()
{
    hrefconfisuser.clear(); 
    close();
}
void Href_Gui::closeEvent( QCloseEvent* e )
{
    hrefconfisuser.clear();
    e->accept();
}
