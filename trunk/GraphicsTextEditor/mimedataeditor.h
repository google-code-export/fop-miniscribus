#ifndef MIMEDATAEDITOR_H
#define MIMEDATAEDITOR_H





#define _TXT_CHAR_ALPHA_ 200
#define _BG_CHAR_ALPHA_ 201
#define _TXT_CHAR_SPACING_ 300 /* css letter-spacing:0.1em */

#define _IMAGE_PICS_ITEM_ 100


#define _MAXIEDITWI_  2000;   /* image maximum widht */


#include <QMessageBox>
#include <QInputDialog>
#include <QColorDialog>
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

#include "getmargin.h"


static inline  QIcon createColorToolButtonIcon(const QString &imageFile,QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);
    return QIcon(pixmap);
}

static inline  QIcon createColorIcon( QColor color )
{
    QPixmap pixmap(50, 50);
    pixmap.fill(color);
    return QIcon(pixmap);
}


class Layoutpainter : public QObject
{
     Q_OBJECT
//
public:
Layoutpainter( QObject *parent );
~Layoutpainter();
QTextCursor textCursor();
QAction *actionBold;
QAction *actionItalic;
QAction *actionUnderline;
QAction *actionStricktext;
QAction *actionOverline;
QAction *actionBackColor;
QAction *actionTextColor;
QAction *actionBlockMargin;

QAction *actionstretchfont;
QAction *actionAlignLeft,
        *actionAlignCenter,
        *actionAlignRight,
        *actionAlignJustify;

QActionGroup *grp;

QTextCursor C_cursor;
QTextDocument *_d;
QRectF line_rect_out;
QMenu *TextMenu( QWidget * inparent );
QMenu *BlockMenu( QWidget * inparent );
void ComposeAction();
QString ImageFilterHaving() const;
signals:
  void updateRequest(QRectF);
public slots:
 void NewCharformat(QTextCursor cursor);
 /* on qmenu */
 /* txt op */
 void BoldText();
 void ItalicText();
 void UnderlineText();
 void OverlineText();
 void StrickText();
 void BGcolor();
 void TXcolor();
 void StretchText();
 void MakealignmentChanged(Qt::Alignment a);

 /* block op */
 void  SetTextBlockMargin();
 void MaketextAlign(QAction *a);

};

////////////Q_DECLARE_METATYPE(Layoutpainter); 


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

