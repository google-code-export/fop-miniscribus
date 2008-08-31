#ifndef _COMMAND_STORAGE_H_
#define _COMMAND_STORAGE_H_

class QAction;

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>

/*

Command storage instance to close all current action static to
fast report on toolbar or other ......

*/



/*
command to reload if cursor change.!
*/
typedef enum {
	
	D_NONE				= 0,
	D_SEPARATOR				= 1,
	D_SUBMENUS  = 5001,
	TXTM_COPY		= 10,
	TXTM_PASTE		= 11,
	TXTM_SELECTALL		= 12,
	TXTM_REDO		= 13,
	TXTM_UNDO		= 14,
	TXTM_CUT		= 15,
	TXT_COLOR			= 400,
	TXT_BG_COLOR	= 401,
	TXT_BOLD			= 402,
	TXT_UNDERLINE			= 2600,
	TXT_STRIKOUT			= 404,
	TXT_OVERLINE			= 405,
	TXT_NOBREAKLINE		= 406,
	TXT_SPAN_FONTS		= 1650,
    TXT_ITALIC		= 2601,
    FONT_LETTER_SPACING  = 1204,
	LAYER_BG = 500,
    BLOCK_BGCOLOR = 707,
    BLOCK_ALIGN_LEFT = 1200,
    BLOCK_ALIGN_RIGHT = 1201,
    BLOCK_ALIGN_CENTER = 1202,
    BLOCK_ALIGN_JUSTIFY = 1203,
    BLOCK_MARGINS = 1500,
	LAYER_BORDER_COLOR = 502,
	FRAME_BGCOLOR			= 407,
	TABLE_BGCOLOR			= 408,
	TABLE_FORMATS			= 409,
	TABLE_CELLBGCOLOR			= 410,
	TABLE_APPENDCOOL			= 411,
	TABLE_APPENDROW			= 412,
	TABLE_REMCOOL		= 414,
	TABLE_REMROW		= 415,
	TABLE_MERGECELL		= 416,
	TABLE_COOLWIDHT		= 417,
    INTERNAL_BOOKMARK		= 8050
} DynamicCommandID;

/*
Static action no checked no icon swap 
live forever on application instance!
*/

typedef enum {
	S_NONE				= 0,
	S_SEPARATOR				= 1,
	S_SUBMENUS  = 5000,
	/* open doc */
	DOC_SAVE_FOP		= 2,
	DOC_SAVE_RTF		= 3,
	DOC_SAVE_HTML		= 4,
	DOC_SAVE_PAGE		= 5,
	DOC_SAVE_TXT		= 6,
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
	TXT_GLOBALFONTS = 400,
	FRAME_PARAMS			= 450,
	PARA_BREACK_PAGE_POLICY			= 451,
	
	LINK_TEXT = 1702,
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
    NEW_LAYER_NEW			= 515,
	INSERT_TABLE = 700,
	INSERT_FRAME = 701,
	INSERT_IMAGE = 702,
	SHOW_SOURCE_HTML = 703,
	SHOW_SOURCE_SCRIBE = 704,
    SHOW_SOURCE_FOP = 705,
	OPEN_PAGE_CHUNK = 858,
	MARGIN_CURRENT_ELEMENT			= 950,
	/* Page group layer */
	
	ID_ABOUT_QT			= 1403
} StaticCommandID;


/* command from absolute Layer */

typedef enum {
	
	F_NONE				= 0,
	F_SEPARATOR				= 1,
	F_SUBMENUS  = 5001,
    F_REMLAYER  = 7455,
	FTXTM_COPY		= 10,
	FTXTM_PASTE		= 11,
	FTXTM_SELECTALL		= 12,
	FTXTM_REDO		= 13,
	FTXTM_UNDO		= 14,
	FTXTM_CUT		= 15,
	FTXT_COLOR			= 400,
    PLAY_SOURCE_LAYER		= 1690,
	FTXT_BG_COLOR	= 401,
        FBLOCK_BGCOLOR			= 707,

        FBLOCK_ALIGN_LEFT = 1200,
        FBLOCK_ALIGN_RIGHT = 1201,
        FBLOCK_ALIGN_CENTER = 1202,
        FBLOCK_ALIGN_JUSTIFY = 1203,
        FBLOCK_MARGINS = 1500,
        ZINDEX_MAX = 1700,
        ZINDEX_MIN = 1701,
        FLINK_TEXT = 1702,
	FTXT_BOLD			= 402,
    FTXT_UNDERLINE			= 2600,
    FTXT_ITALIC             = 2601,
	FTXT_STRIKOUT			= 404,
	FTXT_OVERLINE			= 405,
	FTXT_NOBREAKLINE		= 406,
	FTXT_FONTS		= 407,
	FLAYER_BG = 500,
        FFONT_LETTER_SPACING  = 1204,
	FLAYER_BORDER_COLOR = 502,
        FINSERT_TABLE = 700,
	FFRAME_BGCOLOR			= 407,
	FTABLE_BGCOLOR			= 408,
	FTABLE_FORMATS			= 409,
	FTABLE_CELLBGCOLOR			= 410,
	FTABLE_APPENDCOOL			= 411,
	FTABLE_APPENDROW			= 412,
	FTABLE_REMCOOL		= 414,
	FTABLE_REMROW		= 415,
	FTABLE_MERGECELL		= 416,
	FTABLE_COOLWIDHT		= 417
} AbsCommandID;









struct DinamicCmd {
	DinamicCmd() {
		id = D_NONE;
	}
	
	DinamicCmd(DynamicCommandID Id, bool e , bool s ,  QString Name, QIcon Icon, 
	          QKeySequence Seq, QObject* Reciever, const QString& Slot , bool f = true ) {
		id = Id;
		name = Name;
		icon = Icon;
		shortcut = Seq;
		reciever = Reciever;
		slot = Slot;
		checkaBle_ = e;
		enables = f;
		status = s;
	}
	
	DynamicCommandID id;
	QString name;
	bool checkaBle_;
	bool status;
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



struct AbsoluteCmd {
	AbsoluteCmd() {
		id = F_NONE;
	}
	
	AbsoluteCmd(AbsCommandID Id, bool e , bool s ,  QString Name, QIcon Icon, 
	          QKeySequence Seq, QObject* Reciever, const QString& Slot , bool f = true ) {
		id = Id;
		name = Name;
		icon = Icon;
		shortcut = Seq;
		reciever = Reciever;
		slot = Slot;
		checkaBle_ = e;
		enables = f;
		status = s;
	}
	
	AbsCommandID id;
	QString name;
	bool checkaBle_;
	bool status;
	bool enables;
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
  void registerCommand_F(const AbsoluteCmd&);
  void recordmainaction(const StaticCmd&);
  QAction* actS(StaticCommandID);
  QAction* actD(DynamicCommandID);
  QAction* actF(AbsCommandID);
  QAction* actM(StaticCommandID);
  inline void clearS() { Scmd_.clear(); }
  inline void clearD() { Dcmd_.clear(); }
  inline void clearF() { Fcmd_.clear(); }
  inline void clearM() { Maincmd_.clear(); }
  inline int countD() { return Dcmd_.size(); }
  inline int countS() { return Scmd_.size(); }
  inline int countF() { return Fcmd_.size(); }
  inline int countM() { return Maincmd_.size(); }

private:
	CommandStorage() { }
	static CommandStorage* st_;
    QMap<AbsCommandID, QAction*> Fcmd_;
	QMap<StaticCommandID, QAction*> Scmd_;
	QMap<DynamicCommandID, QAction*> Dcmd_;
    QMap<StaticCommandID, QAction*> Maincmd_;
};

#endif
