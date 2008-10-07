#ifndef LAYEREXTENDED_H
#define LAYEREXTENDED_H

#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "Layer_CommandStorage.h"
#include "Config.h"
#include "SessionManager.h" 
#include "Layer_Absolute_Floating.h"
#include "Text_Api.h"
#include "Scribe_Parser.h"


const int TextTypeFloating = QGraphicsItem::UserType + TextFloatObjectName;


class TextMount;
class TextLayer : public QObject, public QGraphicsRectItem
{
   Q_OBJECT 

public:
    
    TextLayer( QGraphicsItem *parent = 0 );
    ~TextLayer();
    enum { Type = UserType + 1 };
    int type() const {return Type;}
    QRectF boundingRect() const;
    PageDoc binaryPageFormat();   /* binary format qtextdocumet + images no bookmark no page size format or header/footer  */
    QDomDocument *fopPagedoc();   /* xsl-fo xml format all option inside */
    QString PageName();
    QTextDocument *document();
    void setDocument( const QTextDocument * document , FileHandlerType Type = FOP );
    void appendLayer( QMap<int,RichDoc> floatingelement  );
    QTextCursor textCursor();
    void SwapPageModel( M_PageSize e );
    /* load remote image or resource inline on documents */
    void loadDocs( QStringList remoteurls );
    
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool sceneEvent(QEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event); 
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void keyPressEvent( QKeyEvent * event );
    void keyReleaseEvent ( QKeyEvent * event );
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );
    void inputMethodEvent ( QInputMethodEvent * event );
    /* layer floating */
    void SetupHeaderFooter();
    void childAreaCheck();  
    /* check absolute if 
    outside area append a null page breack policy */

private:
  uint lastEventCursorSend;
  AbsoluteLayer *Aheader;
  AbsoluteLayer *Afooter;
  AbsoluteLayer *Astartreg;
  AbsoluteLayer *Aendreg;
  int PageRecords;
  QRectF LastRect;
  bool ContextOpen;
  TextMount *dev;
  QRectF LastUpdateRequest;
  QRectF LastVisibleRequest;
  void MakeDinamicCommand();
  void MakeActionHere();


signals:
  void pageCountChange();
  void autocursorchange(bool);   /* true on auto text / false on absolute */
  void statusMsg(QString);
  void inBookmark(QStringList);
public slots:
    void SceneReload();
    void ensureVisible( const QRectF areas );
    void updatearea( const QRect areas );
    void cursor_wake_up();
    void cursor_stop_it();
    void PageSizeReload();
    void Append_Layer();
    void showSource();

private slots:
 void bookmarkRecord();
 void fillresource( QUrl uri );
 void appStatus( const QString msg );

/*
    void deleteSelected();
    void cut();
    void paste();
    void copy();
    void undo();
    void showhtml();
    void redo();
    void selectAll();
    void InsertImageonCursor();
*/ 

};

Q_DECLARE_METATYPE(TextLayer *)


class TextMount: public QObject
{
    Q_OBJECT 
    
public:
   TextMount();
   ScribePage *txtControl() const;
   TextLayer *q;
   private:
   mutable ScribePage *device;
   
};

Q_DECLARE_METATYPE(TextMount *)
















//
#endif // LAYEREXTENDED_H

