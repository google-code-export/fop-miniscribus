#include "gui_main.h"




Gui_Main::Gui_Main( QWidget* parent )
	: QMainWindow( parent ),framerate(1000),timeline(0),RRunning(false)
{
	setupUi( this );
	devimg = new Foto_DD();
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(devimg);
	Movie_Label = new MoviePlayer(this);
	stackedWidget->addWidget(Movie_Label);
	/////////setCaptureEnabled(true);  /* test run */
	stackedWidget->setCurrentIndex(0);
}

void Gui_Main::movie_end()
{
	stackedWidget->setCurrentIndex(0);
}


void Gui_Main::on_actionOpen_mng_activated()
{
	stackedWidget->setCurrentIndex(1);
  Movie_Label->open();
}

void Gui_Main::on_grabstart_clicked()
{
	/* modus */
	StartRead();
}

void Gui_Main::on_grabstop_clicked()
{
	StopRead();
}



void Gui_Main::setCaptureEnabled(bool enable)
{
	RRunning = enable;
	
    if (enable && framerate > 32) {
        FrameTimeLine.start(framerate,this);
		}  else {
        FrameTimeLine.stop();
		}
}

QImage Gui_Main::CatScreen()
{
   QDesktopWidget *desk = qApp->desktop();
   QPixmap desktopscreen = QPixmap::grabWindow(desk->screen()->winId());
	 QPixmap small =  desktopscreen.scaledToWidth(100);
	 devimg->paint( small );
	 scrollArea->setWidget(devimg);
	 delete &desktopscreen;
   return small.toImage();
}

void Gui_Main::StartRead()
{
	  if (RRunning) {
			StopRead();
			return;
		}
		
	  QString filename = QFileDialog::getSaveFileName(this, "Save animation", "animation.mng", "*.mng");
	  if (filename.size() > 0) {
    animation = new AnimationWriter(filename,"MNG");
    animation->setFrameRate(framerate);
    animation->appendFrame(CatScreen());
		setCaptureEnabled(true);
		}
}


void  Gui_Main::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == FrameTimeLine.timerId()) {
        timeline++;
			  qDebug() << "### timeline  " << timeline;
			  animation->appendFrame(CatScreen());
			  /////animation->appendBlankFrame();
    }
}


void Gui_Main::StopRead()
{
	setCaptureEnabled(false);
	///////animation->appendBlankFrame();
	//////animation->close();
	delete animation;
	animation = 0;
}













