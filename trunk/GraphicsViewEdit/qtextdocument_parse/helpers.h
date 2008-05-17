#ifndef HELPERS_H
#define HELPERS_H

#include "GraphicsView.h"
#include "DataStructures.h"
#include "GraphicsItemEdit.h"


/* remove a file */
static inline bool qt_unlink(QString fullFileName)
{
    if (fullFileName.size() > 0) {
    QFile f( fullFileName );
	if ( f.exists(fullFileName) ) {
       if (f.remove()) {
        return true;
       }
	}
    }
return false;
}

/* write a file to utf-8 format */
static inline bool fwriteutf16(QString fullFileName,QString xml)
{
    if (fullFileName.contains("/", Qt::CaseInsensitive)) {
    QString ultimacartellaaperta = fullFileName.left(fullFileName.lastIndexOf("/"))+"/";
    QDir dira(ultimacartellaaperta);
    if ( dira.mkpath(ultimacartellaaperta) ) { } else {
    return false;
    }
    }
    
        QTextCodec *codecx;
        codecx = QTextCodec::codecForMib(106);
        QFile f( fullFileName );
        if ( f.open( QFile::WriteOnly | QFile::Text ) )
        {
        QTextStream sw( &f );
        sw.setCodec(codecx);
        sw << xml;
        f.close();
        return true;
        }
        return false;
}




/* remove dir recursive */
static inline void DownDir_RM(const QString d)
{
   QDir dir(d);
   if (dir.exists())
   {
      const QFileInfoList list = dir.entryInfoList();
      QFileInfo fi;
      for (int l = 0; l < list.size(); l++)
      {
         fi = list.at(l);
         if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
            DownDir_RM(fi.absoluteFilePath());
         else if (fi.isFile())
         {
            qt_unlink(fi.absoluteFilePath());
         } 
        
      }
      dir.rmdir(d);
   }
}





class WorkPanel : public QWorkspace
{
    Q_OBJECT
//
public:
 WorkPanel( QWidget * parent = 0 );
protected:
    void paintEvent(QPaintEvent *e);
private:
   //////QPixmap panello;
signals:
public slots:
};

class DigitalClock : public QLabel
{
    Q_OBJECT

public:
    DigitalClock(QWidget *parent = 0) {
    setContentsMargins (0,9,0,20);
      
    setAlignment ( Qt::AlignRight ); 
    //////////setSegmentStyle(Filled); 
    /////setSegmentStyle ( QLCDNumber::Flat );
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(100);
    showTime();
    }

private slots:
void showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss.zzz");
    if ((time.second() % 2) == 0) {
        text[2] = '.';
        text[5] = '.';
    }
    
    setText(text);
}
};



class Pbutton : public QToolButton
{
    Q_OBJECT
//
public:
 Pbutton( QString txt , QWidget * parent = 0 )
:QToolButton( parent )
{
  setText(txt);
}
void SetAction( QAction * action )
{
 setDefaultAction(action);
 setToolTip(action->text());
 
 setStatusTip ( action->text() );
 setText(action->text());
 setIconSize(QSize(18,18));
}

signals:
public slots:
};







  
  
  
#endif // HELPERS_H

