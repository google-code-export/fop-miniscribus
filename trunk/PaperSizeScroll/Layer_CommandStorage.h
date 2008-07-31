#ifndef _COMMAND_STORAGE_H_
#define _COMMAND_STORAGE_H_

class QAction;

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>

/*
command to reload if cursor change.!
*/
typedef enum {
	
	D_NONE				= 0,
	TXTM_COPY		= 10,
	TXTM_PASTE		= 11,
	TXTM_SELECTALL		= 12,
	TXTM_REDO		= 13,
	TXTM_UNDO		= 14,
	TXTM_CUT		= 15,
	TXT_COLOR			= 400,
	TXT_BG_COLOR	= 401,
	TXT_BOLD			= 402,
	TXT_UNDERLINE			= 403,
	TXT_STRIKOUT			= 404,
	TXT_OVERLINE			= 405,
	TXT_NOBREAKLINE		= 406,
	LAYER_BG = 500,
	LAYER_BORDER_COLOR = 502,
	FRAME_BGCOLOR			= 407,
	TABLE_BGCOLOR			= 408,
	TABLE_CELLBGCOLOR			= 409
} DynamicCommandID;

/*
Static action no checked no icon swap 
live forever on application instance!
*/

typedef enum {
	S_NONE				= 0,
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
	FRAME_PARAMS			= 450,
	
	
	/* TXT group layer */
	
	/* Page group layer */
	LAYER_BORDER_WI = 501,
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
	INSERT_TABLE = 700,
	INSERT_FRAME = 701,
	INSERT_IMAGE = 702,
	SHOW_SOURCE_HTML = 703,
	SHOW_SOURCE_FOP = 704,
	/* Page group layer */
	
	ID_ABOUT_QT			= 1403
} StaticCommandID;

struct DinamicCmd {
	DinamicCmd() {
		id = D_NONE;
	}
	
	DinamicCmd(DynamicCommandID Id, bool e , QString Name, QIcon Icon, 
	          QKeySequence Seq, QObject* Reciever, const QString& Slot , bool f = true ) {
		id = Id;
		name = Name;
		icon = Icon;
		shortcut = Seq;
		reciever = Reciever;
		slot = Slot;
		checkaBle_ = e;
		enables = f;
	}
	
	DynamicCommandID id;
	QString name;
	bool checkaBle_;
	bool enables;
	QIcon icon;
	QKeySequence shortcut;
	QObject* reciever;
	QString slot;
};



struct StaticCmd {
	StaticCmd() {
		id = S_NONE;
	}
	
	StaticCmd(StaticCommandID Id, QString Name, QIcon Icon, QKeySequence Seq, QObject* Reciever, const QString& Slot) {
		id = Id;
		name = Name;
		icon = Icon;
		shortcut = Seq;
		reciever = Reciever;
		slot = Slot;
	}
	
	StaticCommandID id;
	QString name;
	QIcon icon;
	QKeySequence shortcut;
	QObject* reciever;
	QString slot;
};







class CommandStorage {
public:
	static CommandStorage* instance();

	void registerCommand_S(const StaticCmd&);
  void registerCommand_D(const DinamicCmd&);
	QAction* actS(StaticCommandID);
  QAction* actD(DynamicCommandID);
  inline void clearS() { Scmd_.clear(); }
	inline void clearD() { Dcmd_.clear(); }
	inline int countD() { return Dcmd_.size(); }
	inline int countS() { return Scmd_.size(); }

private:
	CommandStorage() { }
	static CommandStorage* st_;
	QMap<StaticCommandID, QAction*> Scmd_;
	QMap<DynamicCommandID, QAction*> Dcmd_;
};

#endif
