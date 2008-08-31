
#include "Layer_CommandStorage.h"
#include "Config.h"
#include "Tools_Basic.h"

//	Qt
#include <QtGui/QAction>

CommandStorage* CommandStorage::instance() {
	if (st_ == 0)
		st_ = new CommandStorage();

	return st_;
}





void CommandStorage::recordmainaction(const StaticCmd& cmd) {
	StaticCommandID id = cmd.id;

	if (Scmd_.contains(id))
		delete Scmd_[id];
	
	QString keya = QString(); /* not Shortcut */
	if (!cmd.shortcut.isEmpty()) {
		keya = QString(" ") + cmd.shortcut.toString(QKeySequence::NativeText);
	}
    
    #if defined _LOGGERON_APPS_
    QString logger = QString(cmd.name + keya + " / " + cmd.reciever->objectName() + " / " +cmd.slot);
    fwriteAppend(_APPSCACHE_ + "command.dat",logger+" StaticCommandID\n");
    #endif
    
	QAction* action = new QAction(cmd.icon, cmd.name + keya, 0);
	action->setShortcut(cmd.shortcut);
	action->setData(id);
	action->setIcon(cmd.icon);
	QObject::connect(action, SIGNAL(triggered()), cmd.reciever, qPrintable(cmd.slot));
	Maincmd_[id] = action;
}


void CommandStorage::registerCommand_S(const StaticCmd& cmd) {
	StaticCommandID id = cmd.id;

	if (Scmd_.contains(id))
		delete Scmd_[id];
	
	QString keya = QString(); /* not Shortcut */
	if (!cmd.shortcut.isEmpty()) {
		keya = QString(" ") + cmd.shortcut.toString(QKeySequence::NativeText);
	}
    
    #if defined _LOGGERON_APPS_
    QString logger = QString(cmd.name + keya + " / " + cmd.reciever->objectName() + " / " +cmd.slot);
    fwriteAppend(_APPSCACHE_ + "command.dat",logger+" StaticCommandID\n");
    #endif
    
	QAction* action = new QAction(cmd.icon, cmd.name + keya, 0);
	action->setShortcut(cmd.shortcut);
	action->setData(id);
	action->setIcon(cmd.icon);
	QObject::connect(action, SIGNAL(triggered()), cmd.reciever, qPrintable(cmd.slot));
	Scmd_[id] = action;
}

void CommandStorage::registerCommand_D(const DinamicCmd& cmd) {
	DynamicCommandID id = cmd.id;

	if (Dcmd_.contains(id))
		delete Dcmd_[id];
	
	QString keya = QString(); /* not Shortcut */
	if (!cmd.shortcut.isEmpty()) {
		keya = QString(" ") + cmd.shortcut.toString(QKeySequence::NativeText);
	}
    
    #if defined _LOGGERON_APPS_
    QString logger = QString(cmd.name + keya + " / " + cmd.reciever->objectName() + " / " +cmd.slot);
    fwriteAppend(_APPSCACHE_ + "command.dat",logger+" DynamicCommandID\n");
    #endif
    
    
	QAction* action = new QAction(cmd.icon, cmd.name + keya, 0);
	action->setShortcut(cmd.shortcut);
	action->setData(id);
	if (cmd.checkaBle_)  {
		action->setCheckable ( true );
		action->setChecked( cmd.status );
	}
	action->setEnabled ( cmd.enables );
	action->setIcon(cmd.icon);
	QObject::connect(action, SIGNAL(triggered()), cmd.reciever, qPrintable(cmd.slot));
	Dcmd_[id] = action;
}

void CommandStorage::registerCommand_F(const AbsoluteCmd& cmd) {
	AbsCommandID id = cmd.id;

	if (Fcmd_.contains(id))
		delete Fcmd_[id];
    
    
	
	QString keya = QString(); /* not Shortcut */
	if (!cmd.shortcut.isEmpty()) {
		keya = QString(" ") + cmd.shortcut.toString(QKeySequence::NativeText);
	}
    
    
    #if defined _LOGGERON_APPS_
    QString logger = QString(cmd.name + keya + " / " + cmd.reciever->objectName() + " / " +cmd.slot);
    fwriteAppend(_APPSCACHE_ + "command.dat",logger+"  AbsCommandID\n");
    #endif
    
	QAction* action = new QAction(cmd.icon, cmd.name + keya, 0);
	action->setShortcut(cmd.shortcut);
	action->setData(id);
	if (cmd.checkaBle_)  {
		action->setCheckable ( true );
		action->setChecked( cmd.status );
	}
	action->setEnabled ( cmd.enables );
	action->setIcon(cmd.icon);
	QObject::connect(action, SIGNAL(triggered()), cmd.reciever, qPrintable(cmd.slot));
	Fcmd_[id] = action;
}


QAction* CommandStorage::actS(StaticCommandID id) {
	return Scmd_[id];
}

QAction* CommandStorage::actM(StaticCommandID id) {
	return Maincmd_[id];
}


QAction* CommandStorage::actD(DynamicCommandID id) {
	return Dcmd_[id];
}

QAction* CommandStorage::actF(AbsCommandID id) {
	return Fcmd_[id];
}



CommandStorage* CommandStorage::st_ = 0;



















































