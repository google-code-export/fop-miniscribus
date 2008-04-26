#ifndef MIMEDATAEDITOR_H
#define MIMEDATAEDITOR_H


#include <QPointer>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QTextDocument>
#include <QTextCursor>
#include <QPointer>
#include <qfont.h>
#include <qpainter.h>
#include <qevent.h>
#include <qdebug.h>
#include <qmime.h>
#include <qdrag.h>
#include <qclipboard.h>
#include <qmenu.h>
#include <qstyle.h>
#include <qtimer.h>
#include "qtextdocument.h"
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QApplication>
#include <QSvgRenderer>
#include <QWidget>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>
#include <QTextLayout>
#include <QTextList>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QTextBlockFormat>
#include <QTextDocumentFragment>
#include <QMimeData>
#include <QTextDocumentFragment>



class QTextEditMimeData : public QMimeData
{
public:
    inline QTextEditMimeData(const QTextDocumentFragment &aFragment) : fragment(aFragment) {}

    virtual QStringList formats() const;
protected:
    virtual QVariant retrieveData(const QString &mimeType, QVariant::Type type) const;
private:
    void setup() const;

    mutable QTextDocumentFragment fragment;
};










//
#endif // MIMEDATAEDITOR_H

