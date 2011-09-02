#ifndef GUI_ABOUT_H
#define GUI_ABOUT_H
//
#include "ui/ui_gui_about.h"
//
/*  Save file as gui_about.h  */
/*  Class Gui_About Created on Mon Apr 24 15:37:04 CEST 2006  */
//
#include <QPointer>
//
class Gui_About : public QDialog, public Ui::Gui_About
{
	Q_OBJECT
	//
public:
	static Gui_About* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	Gui_About( QWidget* = 0 );
	static QPointer<Gui_About> _self;
	//
/* public slots: */

};
//
#endif // GUI_ABOUT_H


