#ifndef QTEXTPANELCONTROL_H
#define QTEXTPANELCONTROL_H

#include "qtextpanelmime.h"
#include "qtextpaneldata.h"

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>


#include <limits.h>


#define _INTERNAL_LINKS_IDS_ \
             QString("doc_id")

typedef enum
{
	XHTML = 100,   /* normal html tag */
	FOP = 200,    /* XSL-FO tag <br/> tag find hack */
	OOO = 300    /* import from OpenOffice */
} FileHandlerType;


typedef enum
{
	FLAT = 1,   /* flat normal qtextdocument */
	PAGES = 2,    /* paginate display pages */
} DisplayModus;

static const int BookMarkInternalID = 23465;


class ScribeParser
{
   public:
	   typedef enum
	   {
		   Psave,
		   Plinker
	   } ScribeParseModus;

   private:
	   QDomDocument *document;
	   QTextDocument *textDocument;
	   QTextCursor helper_cursor;
	   ScribeParseModus modus;
	   QStringList Internal_Destination_Links;
	   QStringList Clicks_Destination_Links;

	   void MemoonBlock(QTextCursor c ,  QVariant data , const int id);

	   void processFrame(QDomElement appender ,  QTextFrame *frame);
	   void processBlock(QDomElement appender ,  QTextBlock   block);
	   void processFragment(QDomElement appender ,  QTextFragment  fragment);
	   void processTable(QDomElement appender , QTextTable *table);
	   void processTableCell(QDomElement appender , QTextTableCell cell);

   public:
	   ScribeParser(QTextDocument *doc, ScribeParseModus e = Plinker);
	   ~ScribeParser();
	   inline QStringList internals() {return Internal_Destination_Links;}
	   inline QStringList destinations() {return Clicks_Destination_Links;}
	   inline QDomDocument *dom() {return document;}
};

/* TODO QTextPanelControl needs to be formatted */
class QTextPanelControl : public QObject
{
	Q_OBJECT

	private:
	   /* not possible to change if launch */
	   DisplayModus Modus;
	   QStringList  HubBlockids();
	   QPair<int,int> RangeSelection;
	   void StartDragOperation();

	   QBasicTimer trippleClickTimer;
	   QBasicTimer dragClickTimer;

	   QTextCharFormat LastCharFormat;

	   qreal Get_Cell_Width(QTextTableFormat TableFormat , int position);
	   bool IsSelfPlacePaste();
	   /* events */
	   ////////////QPointF trapos( const QPointF &pos );   /* translate position from space */
	   void timerEvent(QTimerEvent *event);

   public:
	   int CurrentSessionID;
	   bool WorkREvent;
	   QClipboard *clipboard;
	   bool overwriteMode;  /* overwrite on write by cursor  */
	   bool PlayCursorMode;
	   bool edit_enable;
	   bool cursorOn;
	   bool cursortime;
	   uint timeline;
	   uint eventline;
	   bool cursorIsFocusIndicator;
	   bool DragFill;

	   QPointF PointPositionOnDoc;

	   int OnPageClick;
	   uint SceneTimerLine;
	   QPointF LastReleasePoint;

	   /* cursor selection */
	   int selectionStartPosition;
	   int cursor_position;
	   int currentPageNumber;
	   QLineF CursorDrawLine;
	   QTextFrameFormat root_format;
	   qreal LeftBorderPadding;
	   qreal BaseTextCursorBottom;
	   QLineF BlinkedcursorDraw;
	   QWidget *Gwi;  /* event widged from  !!!!!!!!!!!!!!!!!!!!!! */
	   bool isBottomBlock;   /* if cursor at last block from page ???  */
	   bool isTopBlock;   /* if cursor at first block from page ???  */
	   bool FullDocSelect;
	   /* core data */
	   //////////QAbstractTextDocumentLayout *layout;
	   QTextDocument *_d;
	   QTextCursor controlTextCursor;
	   FileHandlerType Op;

	   QRect OverloadRectUpdate;
	   QSettings setter;

	   /* core data */
	   /* document params */
	   qreal Page_Width;
	   qreal Page_Height;
	   QRectF Page_Edit_Rect;
	   QRectF ALL_Page_Edit_Rect;
	   int PageTotal;

	   void ResetClickTimer();
	   /* draw item */
	   QBasicTimer cursorTimeLine;  /* blink cursor time line */

      /* keyboard swap on depending format */
	   explicit QTextPanelControl(DisplayModus _modus_);
	   QTextDocument *document();
	   QTextCursor textCursor();

	   void insertPixmap(QPixmap p);    /* on cursor insert */
	   void RegisterImage(SPics e , bool insert);
	   void ImageonCursor(const QString file);
	   QString ImageFilterHaving() const;

	   void ClearCurrentSelection();
	   bool CheckedAlign(const int a);    /* checkable case of alignment qaction*/

	   void gotoNextTableCell();
	   void gotoPreviousTableCell();

	   QLineF ViewBlinkedCursorLine();

	   void InsertMimeDataOnCursor(const QMimeData *md);
	   bool AllowedPosition(const QPointF inpos);

	   /* event on incomming */
	   void Controller_keyPressEvent(QKeyEvent * e);
	   void Controller_keyReleaseEvent(QKeyEvent * e);
	   bool procesevent(QEvent *e);

	   bool getoverwriteMode() const;
	   void SwapOverwriteMode();
	   void SwapCursorMode();  /* same as hide blink cursor */

	   QRectF CurrentBlockRect();  /* on view !not on document !!!!!! */

	   QRectF boundingRect();  /* all page and spacer to x y 00 */
	   QRectF GroupboundingRect();  /* + shadow space */

	   inline bool Edit_On() {return cursorTimeLine.isActive();} /* same as cursor blink true / false */
	   void setBlinkingCursorEnabled(bool enable);

	   /* function */
	   QPointF PageIndexTopLeft(const int index);

	   QLineF BlinkCursorLine(); /* on doc coordinate */
	   QTextLine currentTextLine(const QTextCursor &cursor);
	   void CursorMovetoPosition(const QPointF &pos);     /* inside document not view !!!! */
	   qreal SlicedPage(const int page);
	   void FrameHandler();
	   QTextTableCell CellOnPosition(const int posi);
	   QMimeData *createMimeDataFromSelection();


	   QPointF traposin(const QPointF &pos);

	   /* internal event */
	   bool cursorMoveKeyEvent(QKeyEvent *e);
	   void repaintCursor(bool allrect = false);
	   void BaseMousePressEvent(const  QPointF posi , const QGraphicsSceneMouseEvent *epress);
	   void BaseMoveEvent(const int cursorpos ,  QPointF moveposition);
	   void BaseMouseReleaseEvent(const  QPointF posi , Qt::MouseButton button);
	   void BaseDoubleClickEvent(const  QPointF position , const QGraphicsSceneMouseEvent * event);

   public slots:
	   /* internal and frineds */
	   void ChangeFormatDoc(bool e = true);
	   /* internal and frineds */

	   void int_clipboard_new();
	   void cursorPosition(const QTextCursor curs);
	   void EnsureVisibleCursor();
	   void LayoutRepaint(const QRectF docrect);
	   void redir_update(QRectF area);
	   void in_image(int id);    /* remote image incomming */


	   void deleteSelected();
	   void cut();
	   void paste();
	   void copy();
	   void undo();
	   void showhtml();
	   void redo();
	   void selectAll();
	   void InsertImageonCursor();

	   void UnderlineText();
	   void StrickText();
	   void OverlineText();
	   void BoldText();
	   void FontText();
	   void LinkText();
	   void ParaBGcolor();  /* paragraph bg color */

	   void MaketextAlign();

	   void BGcolor();  /* fragment span */
	   void TXcolor();

	   void FosInsertFrame();
	   void SetFrameBGColor();

	   void CreateanewTable();

	   void MaketableColorBG();
	   void UpdateTableFormat();
	   void SetTableCellColor();
	   void AppendTableCools();
	   void AppendTableRows();
	   void RemoveCoolByCursorPosition();
	   void RemoveRowByCursorPosition();
	   void MergeCellByCursorPosition();
	   void SetColumLarge();

	   void SetElementMargin();
	   void SetParaMargin();
	   void FontsLetterSpacing();
	   void SwapUnbrekableBlock();

   signals:
	   void q_cursor_newPos();  /* +++  swap reformat qmenu from session */
	   void q_visible(QRectF);
	   void q_update(QRect);
	   void q_pageupdate();
	   void q_startDrag(QPointF);
	   void q_update_scene();  /* page changes ++ or -- */

};




//////////////class CachePainter
/* auto floating formating text !!!!!!  */
class ScribePage : public QTextPanelControl
{
	Q_OBJECT

	private slots:
		void SessionUserInput(int position = 0 , int charsRemoved = 0, int charsAdded  = 0);
		void PageUpdate();
		void ParaBlockPageBreackPolicyInsert();
        void cstatus( const int currentdraw , const int tot );
        void docCache( QPicture img );

	public:
		explicit ScribePage(PanelPageSize e);
		void setDocument(const QTextDocument * document , FileHandlerType Type = FOP , bool oncache =  true );
        void setCacheDocument(const QTextDocument * document );
		inline PanelPageSize  Model() const {return PAGE_MODEL;}
		void paint(QPainter * painter , const QStyleOptionGraphicsItem *option , QWidget *widget);
		void SwapPageModel(PanelPageSize e);
		bool AllowedPosition(const QPointF inpos);
		QRectF GroupboundingRect();
        void startCache();
		QRectF boundingRect();
		void DrawPage(const int index  , QPainter * painter );
        void paintPage(const int index  , QPainter * painter , bool printer = false );
		QPointF PageIndexTopLeft(const int index);
		PanelPageSize PAGE_MODEL;
        QPicture allPageCache;
};



/* absolute layer position or  header or footer formating text !!!!!!  */
class LayerText : public QTextPanelControl
{
	Q_OBJECT

	public:
		explicit LayerText();
		void SetRect(const QRectF re);
		void paint(QPainter * painter , const QStyleOptionGraphicsItem *option , QWidget *widget);
		void setDocument(const QTextDocument * document , FileHandlerType Type = FOP);
};



class CachePainter : public QThread
{
    Q_OBJECT
public:
    CachePainter( QObject *creator , PanelPageSize p ,  const QTextDocument * d );
protected:
    void run();
signals:
     void cstatus(int,int);  ///////page x from tot 
     void cgenerator(QPicture);
private:
    QObject* receiver;
    PanelPageSize page;
    QTextDocument *doc;
};



#endif // QTEXTPANELCONTROL_H
