/*******************************************************************************
 * class GraphicsScene
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/


#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

static const int ObjectNameEditor = 400;   /* normal layer div */
#include "mounttextprivate.h"
#include <QGraphicsScene>

#include <QLabel>
#include <QPixmap>
#include <QSize>

#if QT_VERSION >= 0x040400
#include <QGraphicsProxyWidget>
#endif

class QResizeEvent;

class PreviewLabel : public QWidget
{
    Q_OBJECT

public:
    PreviewLabel(QWidget *parent = 0);
    void setPixmap(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap panello;
};



class GraphicsScene : public QGraphicsScene
{
     Q_OBJECT
    
public:
GraphicsScene( QObject * parent = 0 );
void clear();
void storno();
QList<QGraphicsItem *> l_items();
GraphicsScene( const QRectF & sceneRect, QObject * parent = 0 );
GraphicsScene( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 );
void setSceneRect( qreal x, qreal y, qreal w, qreal h );
void setSceneRect(  const QRectF & rect );
QGraphicsItem *ItemTop( const QPointF incomming );
void clearSelection();
void allclear();
qreal zmax();
qreal zmin();
#if QT_VERSION >= 0x040400
QGraphicsProxyWidget *addWidget(QWidget * widget, Qt::WindowFlags wFlags = Qt::Popup );
#endif
bool WakeUp( const QPointF incomming );

protected:
    qreal minimumH;
    QRectF Area;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * e );
    void mousePressEvent( QGraphicsSceneMouseEvent * e );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    
private:
signals:
   void nullitem();  /* QPointF */
   void SelectOn( QGraphicsItem * item , qreal zindex );  /* QPointF */
public slots:
    void reload();
    void TopFocus();
    void remid( const int id );

};



#include "ui_previewdialogbase.h"



class QTreeWidgetItem;
typedef QList<QTreeWidgetItem *> StyleItems;

Q_DECLARE_METATYPE(StyleItems);

class PreviewDialog : public QDialog, private Ui::PreviewDialogBase
{
    Q_OBJECT

public:
    PreviewDialog( QGraphicsScene *sceneprint );
    enum { SmallPreviewLength = 200, LargePreviewLength = 400 };
    bool isSelected(int index);
signals:
    //////void pageRequested(int index, QPainter &painter, QPrinter &printer);

protected:
    void resizeEvent(QResizeEvent *);
    void RenderFaktor();
    void setupComboBoxes();
    void paintItem(QTreeWidgetItem *item, int index);
  
public slots:
    void on_paperSizeCombo_activated(int index);
    void on_paperOrientationCombo_activated(int index);
    void on_pageList_currentItemChanged();
    void addPage();
    void setNumberOfPages(int count);
    void reject();
    void accept();
private:
    ///////void paintItem(QTreeWidgetItem *item, int index);
    void paintPreview(QPixmap &pixmap, int index );
    int ActualMaxPreview();
    QRectF Rscene;
    QRectF Paper_Rect;
    QRectF rectScenePiece;
    QPushButton *pdfButton;
    PreviewLabel *previewLabel;
    QGraphicsScene *scene;
    QPrinter *print;
    qreal faktor_print;
    qreal faktor_scene;
    bool canceled;
    int currentPage;  /* loop count */
    int pageCount;   /* summ pages */
};




















//
#endif // GRAPHICSSCENE_H

