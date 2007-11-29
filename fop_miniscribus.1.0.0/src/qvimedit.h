#ifndef QVIMEDIT_H
#define QVIMEDIT_H
#include <QApplication>
#include <QtGui>
#include <QtCore> 
#include <QtGlobal>
#include <QtDebug>
#include <QDebug> 
#include <QtGui/QTextBrowser>
#include <QTextBrowser>
#include <QMimeData>
#include <QDir>
#include <QTextDocumentFragment>
#include "main.h"
#include <QCompleter>
#include "prefophandler.h"
#include "getmargin.h"

class QVimedit : public QTextBrowser
{
     Q_OBJECT
//
public:	 
    QVimedit( QWidget *parent = 0);
    QVariant ResourceBlock( const QString resourcename );
    void Linkers( QStringList l );
    inline TypImageContainer GetListBlock() { return ImageContainer; }
    void  AppendImageBlocks( TypImageContainer block );
    ~QVimedit();
    bool canInsertFromMimeData ( const QMimeData * source );
    void insertFromMimeData ( const QMimeData * source );
    QShortcut *shortcut0;
    QShortcut *shortcut1;
    QShortcut *shortcut2;
    QShortcut *shortcut3;
    QShortcut *shortcut4;
    QShortcut *shortcut5;
    QShortcut *shortcut6;
    QShortcut *shortcut7;
    QShortcut *shortcut8;
    QClipboard *ramclip;
    TypImageContainer ImageContainer;    /* savataggio immagini incollate !!!!!!!!!!!!!!!*/
    QMenu *createOwnStandardContextMenu();
inline void ResetScroll()
{
   if (LastScrolling !=0) {
      scroll->setValue(LastScrolling); 
   }
}
protected:
    int LastScrolling;
    QStringList InternalLinkAvaiable;
    QSettings setter;
    QScrollBar *scroll;
    QString ImageFilterHaving() const;
    void  mousePressEvent ( QMouseEvent * e );
    void  mouseReleaseEvent ( QMouseEvent * e );
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void mouseDoubleClickEvent ( QMouseEvent *e );
    qreal Get_Cell_Width( QTextTableFormat TableFormat , int position );
private:
    int numerobase;
    uint page;
    QString ImageCache;  /* from page unique */
    QString XHTML;
signals:
     void SaveStreamer();
     void ActualFormat(QTextCharFormat);
     void DDclick();
public slots:
    void SaveCurrentDoc();
    void CachePosition( int lastscroll );  /* memory last scroll position */
    void Editablemodus( bool modus );
    void RContext( const QPoint & pos );
    void RemoveCoolByCursorPosition();
    void RemoveRowByCursorPosition();
    void AppendTableRows();
    void AppendTableCools();
    void SetTableCellColor();
    void MergeCellByCursorPosition();
    void SetColumLarge();
    void ClipbordNewdata();
    void CreateanewTable();
    void SetTextBlockMargin();
    void InsertImageonCursor();
    void MakeHrefLink();




};
//
#endif // QVIMEDIT_H

