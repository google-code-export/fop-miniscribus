
#include "Layer_CommandStorage.h"

//	Qt
#include <QtGui/QAction>

CommandStorage* CommandStorage::instance() {
	if (st_ == 0)
		st_ = new CommandStorage();

	return st_;
}

void CommandStorage::registerCommand(const Command& cmd) {
	CommandID id = cmd.id;

	if (cmds_.contains(id))
		delete cmds_[id];
	
	QString keya = QString(); /* not Shortcut */
	if (!cmd.shortcut.isEmpty()) {
		keya = QString(" ") + cmd.shortcut.toString(QKeySequence::NativeText);
	}
	QAction* action = new QAction(cmd.icon, cmd.name + keya, 0);
	action->setShortcut(cmd.shortcut);
	action->setData(id);
	action->setIcon(cmd.icon);
	QObject::connect(action, SIGNAL(triggered()), cmd.reciever, qPrintable(cmd.slot));
	cmds_[id] = action;
}

QAction* CommandStorage::action(CommandID id) {
	return cmds_[id];
}

CommandStorage* CommandStorage::st_ = 0;
