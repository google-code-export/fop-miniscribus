#ifndef HELPERS_H
#define HELPERS_H

#include "GraphicsView.h"
#include "DataStructures.h"
#include "GraphicsItemEdit.h"

class WorkPanel : public QWorkspace
{
    Q_OBJECT
//
public:
 WorkPanel( QWidget * parent = 0 );
protected:
    void paintEvent(QPaintEvent *e);
private:
   QPixmap panello;
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
    timer->start(10);
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

