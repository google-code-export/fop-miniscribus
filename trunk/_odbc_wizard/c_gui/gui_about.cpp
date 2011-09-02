#include "gui_about.h"
//
/*  Save file as gui_about.cpp  */
/*  Class Gui_About Created on Mon Apr 24 15:37:04 CEST 2006  */
//
#include <QCloseEvent>
//
QPointer<Gui_About> Gui_About::_self = 0L;
//
Gui_About* Gui_About::self( QWidget* parent )
{
	if ( !_self )
		_self = new Gui_About( parent );
	return _self;
}
//
Gui_About::Gui_About( QWidget* parent )
	: QDialog( parent )
{
	setupUi( this );
}
//
void Gui_About::closeEvent( QCloseEvent* e )
{
	e->accept();
}


