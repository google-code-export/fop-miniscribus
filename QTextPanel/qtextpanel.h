#ifndef QTEXTPANEL_H
#define QTEXTPANEL_H
#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>

#include "qtextpanelmime.h"
#include "qtextpaneldata.h"
#include <QGraphicsView>
#include "qtextpanellayercontrol.h"



#define _PARSE_DEBUG_FOP_  1
#define _DEFAULT_FONT_POINT_SIZE_  10
#define _BOTTOM_VIEW_SPACE_RESERVE_  50


static const int DefaultStartZoom = 220;

/**
 * This class controls the display of Scene and Views
 */
class QTextPanel : public QGraphicsView
{
	Q_OBJECT

	private:
		bool onPrintRender;

	protected:
		void pageClear();
		QPixmap chessGrid;
		void printSetup(bool printok);
		void keyPressEvent(QKeyEvent *e);
		void resizeEvent(QResizeEvent *event);
		QSettings setter;
        GraphicsScene *scene;

	public:
		QRectF boundingRect();
        QTextCursor textCursor();
        QTextDocument *document();
		QRectF rectToScene();
		QTextPanelLayerControl *BASE_TEXT;
		QTextPanel(QWidget * parent  = 0);
		~QTextPanel();
		void setHeaderActive(bool active) {BASE_TEXT->setHeaderActive(active); scene->update();}
		void setFooterActive(bool active) {BASE_TEXT->setFooterActive(active); scene->update();}

	public slots:
		void displayTop();
		void viewDisplay(const QRectF area);
		void swapPaper();
        void forceResize();

	signals:
		void sceneSwap();
        void newPageFormatin();
};

#endif // QTEXTPANEL_H
