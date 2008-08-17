#ifndef QTEXTMANIPULATOR_H
#define QTEXTMANIPULATOR_H

#include <QThread>
#include "config.h"
#include "qtextpanelmime.h"
#include <QtCore>
#include <QtGui>

class ComposeDoc : public QThread
{
    Q_OBJECT
public:
    ComposeDoc( QObject *creator , MountPage p );
protected:
    void run();
signals:
     void generator(RichDoc);
     void cstatus(int,int); /* page x from tot x*/
private:
    QObject* receiver;
    MountPage page;
};



/*
class CachePainter : public QThread
{
    Q_OBJECT
public:
    CachePainter( QObject *creator , PanelPageSize p ,  QTextDocument * d );
protected:
    void run();
signals:
     void cstatus(int,int);  page x from tot 
     void cgenerator(QPicture);
private:
    QObject* receiver;
    MountPage page;
};
*/















#endif


