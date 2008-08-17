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




















#endif


