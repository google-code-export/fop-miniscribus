#ifndef QTEXTPANEL_H
#define QTEXTPANEL_H
#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>
#include "config.h"
#include "qtextpanelmime.h"
#include "qtextpaneldata.h"
#include <QGraphicsView>
#include "qtextpanellayercontrol.h"


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
        QTextPanelLayerControl *BASE_TEXT;
    ////////  void setDocument(const QTextDocument * document , FileHandlerType Type = FOP);
	public:
		QRectF boundingRect();
        QTextCursor textCursor();
        QTextDocument *document() const;
		QRectF rectToScene();
		inline QTextPanelLayerControl *editor() { return BASE_TEXT; }
		QTextPanel(QWidget * parent  = 0);
        void setMatrix ( const QMatrix & matri, bool combine = false );
		~QTextPanel();

	public slots:
		void displayTop();
		void viewDisplay(const QRectF area);
		void swapPaper();
        void forceResize();
		void setHeaderActive(bool active) {BASE_TEXT->setHeaderActive(active); scene->update();}
		void setFooterActive(bool active) {BASE_TEXT->setFooterActive(active); scene->update();}
        void newPageInit();  /* begin a new blank page */
        void stressTestPaint();
        void matrixExchange();

	signals:
		void sceneSwap();
        void newPageFormatin();
};

#endif // QTEXTPANEL_H
