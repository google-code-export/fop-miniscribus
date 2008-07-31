#ifndef _COMMAND_STORAGE_H_
#define _COMMAND_STORAGE_H_

class QAction;

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>


typedef enum {
	
	TXT_COLOR
	TXT_BG_COLOR
	
} DynamicCommandID;

/*
Static action no checked no icon swap 
live forever on application instance!
*/

typedef enum {
	ID_NONE				= 0,
	/* open doc */
	DOC_SAVE_FOP		= 1,
	DOC_SAVE_RTF		= 2,
	DOC_SAVE_HTML		= 3,
	DOC_SAVE_PAGE		= 4,
	DOC_SAVE_TXT		= 5,
	/* open doc */
	/* open open */
	DOC_OPEN_FOP		= 20,
	DOC_OPEN_HTML		= 22,
	DOC_OPEN_PAGE		= 23,
	DOC_OPEN_TXT		= 24,
	/* open open */
	/* create fop from xsl + xml  */
	DOC_COMPOSE_XSLT_FOP		= 8,
	/* scene clear same as new blank page */
	CLEAR_PAGE = 103,
	
	/* TXT group layer */
	TXT_FONTS = 400,
	
	
	/* TXT group layer */
	
	/* Page group layer */
	LAYER_BG = 500,
	LAYER_BORDER_WI = 501,
	LAYER_BORDER_COLOR = 502,
	LAYER_OPEN_FILE = 503,
	LAYER_SAVE_FILE = 504,
	LAYER_Z_UP = 505,
	LAYER_Z_DOWN = 506,
	LAYER_COPY_I = 507,
	LAYER_CLONE_I = 508,
	LAYER_REMOVE = 509,
	NEW_LAYER_ABS = 510,
	NEW_LAYER_OOO			= 511,
	PRINT_PAGE			= 512,
	PASTE_LAYER = 513,
	INSERT_TABLE
	INSERT_FRAME
	INSERT_IMAGE
	SHOW_SOURCE_HTML
	SHOW_SOURCE_FOP
	/* Page group layer */
	
	ID_ABOUT_QT			= 1403
} StaticCommandID;



struct Command {
	Command() {
		id = ID_NONE;
	}
	Command(CommandID Id, QString Name, QIcon Icon, QKeySequence Seq, QObject* Reciever, const QString& Slot) {
		id = Id;
		name = Name;
		icon = Icon;
		shortcut = Seq;
		reciever = Reciever;
		slot = Slot;
	}

	CommandID id;
	QString name;
	QIcon icon;
	QKeySequence shortcut;
	QObject* reciever;
	QString slot;
};

class CommandStorage {
public:
	static CommandStorage* instance();

	void registerCommand(const Command&);
	QAction* action(CommandID);
  inline void clear() { cmds_.clear(); }

private:
	CommandStorage() { }
	static CommandStorage* st_;
	QMap<CommandID, QAction*> cmds_;
};

#endif
