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



const int TextTypeFloating = QGraphicsItem::UserType + TextFloatObjectName;


class TextMount;
class TextLayer : public QObject, public QGraphicsRectItem
{
   Q_OBJECT 

public:
    
    TextLayer( QGraphicsItem *parent = 0 );
    ~TextLayer();
    int type() const {return TextTypeFloating;}
    QRectF boundingRect() const;
    QString PageName();
    QTextDocument *document();
    void setDocument( const QTextDocument * document , FileHandlerType Type = FOP );
    void appendLayer( QMap<int,RichDoc> floatingelement  );
    QTextCursor textCursor();
    void SwapPageModel( M_PageSize e );
    
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

private:
    
  AbsoluteLayer *Aheader;
  AbsoluteLayer *Afooter;
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
  void autocursorchange();
  void absolutecursorchange();
public slots:
    void SceneReload();
    void ensureVisible( const QRectF areas );
    void updatearea( const QRect areas );
    void cursor_wake_up();
    void cursor_stop_it();
    void PageSizeReload();
    void Append_Layer();

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

