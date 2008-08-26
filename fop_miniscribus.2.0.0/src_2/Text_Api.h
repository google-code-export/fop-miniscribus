#ifndef TXTAPI_H
#define TXTAPI_H

#include "Config.h"
#include "SessionManager.h"
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <limits.h>
#include "Scribe_Parser.h"


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









class TextProcessor : public QObject
{
     Q_OBJECT

public:
 
/* keyboard swap on depending format */







/* keyboard swap on depending format */

  explicit TextProcessor( DisplayModus _modus_ );
  QTextDocument *document();
  QTextCursor textCursor();

  void insertPixmap( QPixmap p );  /* on cursor insert */
  void RegisterImage( SPics e , bool insert );
  void ImageonCursor( const QString file );
  QString ImageFilterHaving() const;

  void ClearCurrentSelection();
  bool CheckedAlign( const int a );  /* checkable case of alignment qaction*/
  
  void gotoNextTableCell();
  void gotoPreviousTableCell();

  QLineF ViewBlinkedCursorLine();
  


  void InsertMimeDataOnCursor( const QMimeData *md );
  bool AllowedPosition( const QPointF inpos );

  /* event on incomming */
  void Controller_keyPressEvent ( QKeyEvent * e );
  void Controller_keyReleaseEvent ( QKeyEvent * e );
  bool procesevent( QEvent *e );
  
  bool getoverwriteMode() const;
  void SwapOverwriteMode();
  void SwapCursorMode();  /* same as hide blink cursor */

  QRectF CurrentBlockRect();  /* on view !not on document !!!!!! */
  
  
  QRectF boundingRect();  /* all page and spacer to x y 00 */
  QRectF GroupboundingRect();  /* + shadow space */
  
  inline bool Edit_On() { return cursorTimeLine.isActive(); }  /* same as cursor blink true / false */
  void setBlinkingCursorEnabled( bool enable );

  
  
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

  /* function */
  QPointF PageIndexTopLeft( const int index  );
  
  QLineF BlinkCursorLine(); /* on doc coordinate */
  QTextLine currentTextLine(const QTextCursor &cursor);
  void CursorMovetoPosition( const QPointF &pos );   /* inside document not view !!!! */
  qreal SlicedPage( const int page );
  void FrameHandler();
  QTextTableCell CellOnPosition( const int posi );
  QMimeData *createMimeDataFromSelection();
  

  QPointF traposin( const QPointF &pos );

  /* internal event */
  bool cursorMoveKeyEvent(QKeyEvent *e);
  void repaintCursor( bool allrect = false );
  void BaseMousePressEvent( const  QPointF posi , const QGraphicsSceneMouseEvent *epress );
  void BaseMoveEvent( const int cursorpos ,  QPointF moveposition );
  void BaseMouseReleaseEvent( const  QPointF posi , Qt::MouseButton button ); 
  void BaseDoubleClickEvent( const  QPointF position , const QGraphicsSceneMouseEvent * event ); 


  /* cursor selection */
  int position_selection_start;
  int cursor_position;
  int Current_Page_Nr;
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
  QTextCursor C_cursor;
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
  private:
  qreal Get_Cell_Width( QTextTableFormat TableFormat , int position ); 
  QPair<int,int> RangeSelection;
  bool IsSelfPlacePaste();
  void StartDragOperation();
  
  QBasicTimer trippleClickTimer;
  QBasicTimer dragClickTimer;
  
  QTextCharFormat LastCharFormat;
  /* events */
  ////////////QPointF trapos( const QPointF &pos );   /* translate position from space */
  void timerEvent(QTimerEvent *event);

signals:
  void q_cursor_newPos();  /* +++  swap reformat qmenu from session */
  void q_visible(QRectF);
  void q_update(QRect);
  void q_pageupdate();
  void q_startDrag(QPointF);
  void q_update_scene();  /* page changes ++ or -- */
  
public slots:
  
  /* internal and frineds */
  void ChangeFormatDoc( bool e = true );
  /* internal and frineds */

  void int_clipboard_new();
  void cursorPosition( const QTextCursor curs );
  void EnsureVisibleCursor();
  void LayoutRepaint( const QRectF docrect );
  void redir_update( QRectF area );
  void in_image( int id );  /* remote image incomming */


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
  
  

private:
  /* not possibel to change if launch */
DisplayModus Modus;
QStringList  HubBlockids();

};



////////class TextProcessor;
/* line 1805 */
class ScribePage : public TextProcessor
{
    Q_OBJECT
//
public:
  explicit ScribePage( M_PageSize e );
  void setDocument ( const QTextDocument * document , FileHandlerType Type = FOP );
  inline M_PageSize  Model() const { return PAGE_MODEL; }
  void paint(QPainter * painter , const QStyleOptionGraphicsItem *option , QWidget *widget   );
  void SwapPageModel( M_PageSize e );
  bool AllowedPosition( const QPointF inpos );
  QRectF GroupboundingRect();
  QRectF boundingRect();
  void DrawPage( const int index  , QPainter * painter , const int cursorpage );
  QPointF PageIndexTopLeft( const int index  );
  M_PageSize PAGE_MODEL;
private:
  

private slots:
  void SessionUserInput( int position = 0 , int charsRemoved = 0, int charsAdded  = 0);
  void PageUpdate();
  void ParaBlockPageBreackPolicyInsert();


};




class LayerText : public TextProcessor
{
    Q_OBJECT
//
public:
explicit LayerText();
void SetRect ( const QRectF re );
void paint(QPainter * painter , const QStyleOptionGraphicsItem *option , QWidget *widget   );
void setDocument ( const QTextDocument * document , FileHandlerType Type = FOP );
protected:
private:
private slots:
  
};





//
#endif // TXTAPI_H

