/*******************************************************************************
 * class Layoutpainter
 *******************************************************************************
 * Copyright (C) 2007 by Peter Hohl
 * e-Mail: ppkciz@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *******************************************************************************/

#ifndef MIMEDATAEDITOR_H
#define MIMEDATAEDITOR_H





#define _TXT_CHAR_ALPHA_ 200
#define _BG_CHAR_ALPHA_ 201
#define _TXT_CHAR_SPACING_ 300 /* css letter-spacing:0.1em */




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
#include <QKeySequence>


#define ACCEL_KEYL(k) "\t" + QString("Ctrl+" #k)

#include "DataStructures.h"
#include "getmargin.h"


class FrameStyler : public QWidget
{
    Q_OBJECT
//
public:
 FrameStyler( QTextFrame *base , QWidget *parent  )
 : QWidget( parent ),frame(base)
{
    QTextFrameFormat Ftf = frame->frameFormat();
 
    gridLayout = new QGridLayout(this);
    hboxLayout = new QHBoxLayout();
    vboxLayout = new QVBoxLayout();
    label = new QLabel(tr("Border style:"),this);
    vboxLayout->addWidget(label);
    label_3 = new QLabel(tr("Border color:"),this);
    vboxLayout->addWidget(label_3);
    label_2 = new QLabel(tr("Float:"),this);
    vboxLayout->addWidget(label_2);
    vboxLayout->addWidget(new QLabel(tr("Border width:"),this));
    vboxLayout->addWidget(new QLabel(tr("Padding:"),this));
     
 
 
    hboxLayout->addLayout(vboxLayout);
    vboxLayout1 = new QVBoxLayout();
    comboBox = new QComboBox(this); ///// style
    comboBox->addItem(tr("Border none"),QTextFrameFormat::BorderStyle_None);
    comboBox->addItem(tr("Border Dotted"),QTextFrameFormat::BorderStyle_Dotted);
    comboBox->addItem(tr("Border Double"),QTextFrameFormat::BorderStyle_Double);
    comboBox->addItem(tr("Border Solid"),QTextFrameFormat::BorderStyle_Solid);
    comboBox->addItem(tr("Border Dashed"),QTextFrameFormat::BorderStyle_Dashed);
    comboBox->addItem(tr("Border DotDash"),QTextFrameFormat::BorderStyle_DotDash);
    comboBox->addItem(tr("Border DotDotDash"),QTextFrameFormat::BorderStyle_DotDotDash);
    comboBox->addItem(tr("Border Groove"),QTextFrameFormat::BorderStyle_Groove);
    comboBox->addItem(tr("Border Ridge"),QTextFrameFormat::BorderStyle_Ridge);
    comboBox->addItem(tr("Border Inset"),QTextFrameFormat::BorderStyle_Inset);
    comboBox->addItem(tr("Border Outset"),QTextFrameFormat::BorderStyle_Outset);
    comboBox->setCurrentIndex(comboBox->findData(Ftf.borderStyle()));
    
    
    QPixmap pix(22, 22);
    vboxLayout1->addWidget(comboBox);
    comboBox_3 = new QComboBox(this); //// color 
    QStringList colorNames = QColor::colorNames();
    foreach (QString name, colorNames) {  
         pix.fill(QColor(name));
         comboBox_3->addItem(pix,QString("color %1").arg(name),QColor(name));
    }
    comboBox_3->setCurrentIndex(comboBox_3->findData(Ftf.borderBrush().color()));
    
    vboxLayout1->addWidget(comboBox_3);
    comboBox_2 = new QComboBox(this);   /// float
    comboBox_2->addItem(tr("Float inline"),QTextFrameFormat::InFlow);
    comboBox_2->addItem(tr("Float left"),QTextFrameFormat::FloatLeft);
    comboBox_2->addItem(tr("Float right"),QTextFrameFormat::FloatRight);
    
    comboBox_2->setCurrentIndex(comboBox->findData(Ftf.position()));
    
    vboxLayout1->addWidget(comboBox_2);
    /* border widht */
    borderwi = new QSpinBox(this);
    borderwi->setMaximum(20);
    borderwi->setValue(Ftf.border());
    vboxLayout1->addWidget(borderwi);
    
    paddingwi = new QSpinBox(this);
    paddingwi->setMaximum(50);
    paddingwi->setValue(Ftf.padding());
    vboxLayout1->addWidget(paddingwi);
    
    
    hboxLayout->addLayout(vboxLayout1);
    gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);
    QObject::connect(comboBox_3, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
    QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
    QObject::connect(comboBox_2, SIGNAL(currentIndexChanged(int)),this, SLOT(FormatUpdate()));
    QObject::connect(borderwi, SIGNAL(valueChanged(int)),this, SLOT(FormatUpdate()));
    QObject::connect(paddingwi, SIGNAL(valueChanged(int)),this, SLOT(FormatUpdate()));
    
  
}
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QVBoxLayout *vboxLayout1;
    QComboBox *comboBox;
    QComboBox *comboBox_3;
    QComboBox *comboBox_2;
    QSpinBox *borderwi;
    QSpinBox *paddingwi;
    
private:
    QTextFrame *frame;
signals:
public slots:
  
void FormatUpdate()
{
  QTextFrameFormat Ftf = frame->frameFormat();
  Ftf.setPosition(QTextFrameFormat::Position(comboBox_2->itemData(comboBox_2->currentIndex()).toInt()));
  Ftf.setBorderStyle(QTextFrameFormat::BorderStyle(comboBox->itemData(comboBox->currentIndex()).toInt()));
  Ftf.setBorderBrush(QBrush(comboBox_3->itemData(comboBox_3->currentIndex()).value<QColor>()));
  Ftf.setBorder(borderwi->value());
  Ftf.setPadding(paddingwi->value());  
 
  
 
  frame->setFrameFormat(Ftf);
}

};








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


class SpanBorder : public QObject, public QTextObjectInterface
{
	Q_OBJECT
 	Q_INTERFACES(QTextObjectInterface)
public:
 	SpanBorder(QObject* par = 0);
	QSizeF intrinsicSize(QTextDocument* doc, int posInDoc, const QTextFormat &fmt);
  QTextLine currentTextLine(const QTextCursor &cursor);
	void drawObject(QPainter* p, const QRectF &rect, QTextDocument* doc,
		              int posInDoc, 
                  const QTextFormat &fmt);
private:
};




class Layoutpainter : public QObject
{
     Q_OBJECT
//
public:
Layoutpainter( QObject *parent );
~Layoutpainter();
QTextCursor textCursor();
QAction *actionUndo;
QAction *actionRedo;
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
        *actionFonts,
        *actionAlignJustify;

QActionGroup *grp;
int cursor_position;
QTextCursor C_cursor;
QTextDocument *_d;
QRectF line_rect_out;
QMenu *TextMenu( QWidget * inparent );
QMenu *BlockMenu( QWidget * inparent );
QMenu *TableMenu( QWidget * inparent );
QMenu *FrameMenu( QWidget * inparent );
void ComposeAction();
QString ImageFilterHaving() const;
qreal Get_Cell_Width( QTextTableFormat TableFormat , int position ); 
signals:
  void updateRequest(QRectF);
public slots:
 //////////////void NewCharformat(QTextCursor cursor);
 /* on qmenu */
 /* txt op */
 void FontText();
 void BoldText();
 void ItalicText();
 void UnderlineText();
 void OverlineText();
 void StrickText();
 void BGcolor();
 void TXcolor();
 void StretchText();
 void MakealignmentChanged(Qt::Alignment a);
 void undo();
 void redo();
 /* block op */
 void  SetTextBlockMargin();
 void MaketextAlign(QAction *a);
 void cursorPosition( const QTextCursor curs );
 void FosInsertFrame();

 void MaketableColorBG();
 void MaketableBorder();
 void CreateanewTable();
 void SetTableCellColor();
 void AppendTableCools();
 void AppendTableRows();
 void RemoveRowByCursorPosition();
 void RemoveCoolByCursorPosition();
 void MergeCellByCursorPosition();
 void SetColumLarge();
 void SetFrameBGColor();
 void SetFrameMargin();

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

