#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H
//
#include "ui_gui_display.h"
//
/*  Save file as gui_display.h  */
/*  Class Gui_Display Created on Sun Jun 18 16:49:52 CEST 2006  */
//


#include <QPointer>
#include <QWidget>
#include <QString>
#include <QScrollBar>
#include <QTextCursor>

class Gui_Display : public QDialog, public Ui::Gui_Display
{
	Q_OBJECT
	//
public:
	static Gui_Display* self( QWidget* = 0 );
	//
protected:
	void closeEvent( QCloseEvent* );
	//
private:
	Gui_Display( QWidget* = 0 );
	static QPointer<Gui_Display> _self;
    QScrollBar *textliner;
	QScrollBar *textareatext;
	//
 public slots:
     void UpdateSource();

};
//
#endif // GUI_DISPLAY_H


