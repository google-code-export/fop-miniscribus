#include "gui_display.h"
//
/*  Save file as gui_display.cpp  */
/*  Class Gui_Display Created on Sun Jun 18 16:49:52 CEST 2006  */
//
#include <QCloseEvent>
//
QPointer<Gui_Display> Gui_Display::_self = 0L;
//
Gui_Display* Gui_Display::self( QWidget* parent )
{
	if ( !_self )
		_self = new Gui_Display( parent );
	return _self;
}
//
Gui_Display::Gui_Display( QWidget* parent )
	: QDialog( parent )
{
	setupUi( this );
    textliner = msgline->verticalScrollBar();
	textareatext = msg->verticalScrollBar();
    connect( textliner, SIGNAL(valueChanged(int)), textareatext, SLOT(setValue(int)));
    connect( textareatext, SIGNAL(valueChanged(int)), textliner, SLOT(setValue(int)));
    connect( msg, SIGNAL(textChanged()), this, SLOT(UpdateSource()));
}
//
void Gui_Display::UpdateSource()
{
    QString lineNumbers = "";
    int i;
    int numberOfLines = 0;
    QString sorgente = msg->toPlainText();
    numberOfLines = sorgente.count(QRegExp("\n"));
     for (i = 1; i <= numberOfLines+1; i++) {
        if (i < 999) {
        lineNumbers.append(QString::number(i)+"\n");
        } else {
        lineNumbers.append("#\n");  
        }
    }
    msgline->setPlainText(lineNumbers);
    msgline->toPlainText();
    
}







void Gui_Display::closeEvent( QCloseEvent* e )
{
	e->accept();
}
