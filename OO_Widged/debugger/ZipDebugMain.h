#ifndef ZIPDEBUGMAIN_H
#define ZIPDEBUGMAIN_H
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "ui_main.h"
#include "OOReader.h"                                 // 
#include <QPointer>
#include "OOReader.h"


class ZipDebugMain : public QMainWindow, public Ui::ZipDebugMain
{
     Q_OBJECT
//
public:
     ZipDebugMain( QWidget* = 0 );
protected:
    void drawTab( QByteArray dat ,  const QString file );
    void drawDoc( QTextDocument *doc );
    bool eventFilter(QObject *obj, QEvent *event);
private:
    OOReader *Ooo;
    QTextEdit *t;
    XMLTextEdit *de;
signals:

public slots:
    void on_openzip_triggered();
    void openFile( const QString file );
    void drawDoc();
    void onRead( int now ,int tot );

private slots:


};
//
#endif // ZIPDEBUGMAIN_H

