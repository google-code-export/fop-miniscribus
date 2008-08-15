#ifndef QTEXTPANELLAYER_H
#define QTEXTPANELLAYER_H

#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "qtextpanelmime.h"
#include "qtextpaneldata.h"
#include "qtextpanelcontrol.h"

///////qDebug() << "###  width" << boundingRect().height();
/////qDebug() << "###  largo" << alto;

#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265

static const qreal MinimumHightLayer = 15.0;
static const qreal MinimumWhidhLayer = 15.0;

static const qreal MAXLargoTmp = 1000.0;

/* drag point to rotate and resize absolute layer */

class FWButton : public QObject, public QGraphicsItem
{
	Q_OBJECT

	private:
		QGraphicsItem *m_parent;
		QBrush m_brush;
		QPointF m_startPos;
		bool permission;
		QString txt;

	public:
		FWButton(QGraphicsItem * parent, const QBrush & brush , const QString msg);
		QRectF boundingRect() const;

		void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
		void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
		void mousePressEvent(QGraphicsSceneMouseEvent * event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
		void allow(bool e);
		void focusInEvent(QFocusEvent * event);
		void focusOutEvent(QFocusEvent * event);
		void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	Q_SIGNALS:
		void dragging(const QPointF & point);
		void reset();
		void operate(bool);
};

Q_DECLARE_METATYPE(FWButton *)

enum LAYERTYPE
{
	DIV_ABSOLUTE = 1000,
	DIV_HEADER = 2000,
	DIV_FOOTER = 3000
};

class AbsText;

class AbsoluteLayer : public QObject, public QGraphicsRectItem
{
	Q_OBJECT

	private:
		/* layer absolute params */
		QColor Background_Color;
		QColor Border_Color;

		QColor Border_Color_t;
		QColor Border_Color_b;
		QColor Border_Color_l;
		QColor Border_Color_r;
		qreal _border_top;
		qreal _border_bottom;
		qreal _border_left;
		qreal _border_right;

		QTextPanelData *panelData;

		LAYERTYPE layermods;
		int id;
		bool ContextOpen;
		FWButton  *Angle_1;
		FWButton  *Angle_2;
		FWButton  *Angle_4;
		bool OnMoveRects;
		qreal Rotate;
		QRectF lastUpdateRequest;
		AbsText *dev;

	private slots:
		void slotModpos_1(const QPointF posi);
		void slotResize_1(const QPointF posi);
		void slotRotate_1(const QPointF posi);
		void MoveActions(bool e);
		void updateArea(const QRect areas);
		void UpdateDots();
		void ShowInfos();

	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		bool sceneEvent(QEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
		void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		void keyPressEvent(QKeyEvent * event);
		void keyReleaseEvent(QKeyEvent * event);
		void focusInEvent(QFocusEvent * event);
		void focusOutEvent(QFocusEvent * event);
		void inputMethodEvent(QInputMethodEvent * event);

	public:
		AbsoluteLayer(QGraphicsItem *parent , LAYERTYPE layermodus = DIV_ABSOLUTE);
		~AbsoluteLayer();
		QRectF boundingRect() const;
		inline int Type() const
		{
			return layermods;
		}
		QRectF absoluteRect();
		QLineF LineTops();
		QLineF Diagonal();
		QImage LayerImage(const int pageNumber = 0);    /* draw header or footer */

		QTextDocument *document();
		void setDocument(const QTextDocument * doc , FileHandlerType Type = FOP);
		QTextCursor textCursor();

	public slots:
		void seTBack();
		void seTFront();
		void UpdatePageFormat();
		void RotateLayer(const int ro) ;
		void BackGroundColor();

	signals:
		void close_main_cursor();
		void pagesize_swap();

};

Q_DECLARE_METATYPE(AbsoluteLayer *)

class AbsText: public QObject
{
	Q_OBJECT

	private:
		mutable LayerText *device;

	public:
		AbsText();
		LayerText *txtControl() const;
		AbsoluteLayer *q;
};

Q_DECLARE_METATYPE(AbsText *)

#endif // QTEXTPANELLAYER_H
