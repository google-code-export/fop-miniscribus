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

#if QT_VERSION >= 0x040400
#include <QGraphicsProxyWidget>
#endif


class GraphicsScene : public QGraphicsScene
{
     Q_OBJECT
    
public:
GraphicsScene( QObject * parent = 0 );
void clear();
void storno();
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


//
#endif // GRAPHICSSCENE_H

