#ifndef PANEL_H
#define PANEL_H

#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QWidget>

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif

#include "qtextpanel.h"

class Panel : public QFrame
{
	Q_OBJECT

	private:
		void FillPaperSize();
		bool NotPaperUpdate;
		uint tievents;
		QLabel *label;
		QComboBox *PortraitPaper;
		//////QComboBox *LandscapePaper;
		QToolButton *resetButton;
		QSlider *zoomSlider;
		QSlider *rotateSlider;
		QToolButton *printButton;
		QToolButton *openGlButton;
		QToolButton *headerButton;
		QToolButton *footerButton;

	private slots:
		void resetView();
		void setResetButtonEnabled();
		void setupMatrix();
		void displayTop();
		void zoomIn();
		void zoomOut();
		void SceneChange();
		void toggleOpenGL();
		void PaperSwap(const int index);

	public:
		Panel(QWidget *parent = 0);
		QTextPanel *textPanel;
};

#endif //PANEL_H
