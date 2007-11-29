#ifndef WYMSINGTXT_H
#define WYMSINGTXT_H
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QColorDialog>
#include <QTextFrame>
#include <QTextList>
#include "qvimedit.h"
#include <QGraphicsTextItem>
#include <QTextFrameFormat>

#include "ui_wymtext.h"

#include "getmargin.h"

static inline bool file_put_contents_append(QString fullFileName,QString xml)
{
    QString data = xml+"\n";
    QFile f( fullFileName );
	if ( f.open( QFile::Append | QFile::Text ) )
	{
		QTextStream sw( &f );
		sw << data;
		f.close();
		return true;
	}
	return false;
    
}

//
/*  Save file as wymsingtxt.h  */
/*  incomming class name WymsingTxt */
//   public QObject | public QHttp | public QThread                                    // 
#include <QPointer>
//
class WymsingTxt : public QDialog, public Ui::WymsingTxt
{
     Q_OBJECT

public:
WymsingTxt( QRectF recter , QColor sfondo , QColor margincolor , qreal borders , int rotate , QTextDocument *item , TypImageContainer block , QGraphicsItem  * parent );
void SetInternalLink( QStringList l );
protected:
    QTextDocument *tdoc;
    QTextCursor c;
    QGraphicsTextItem *destimage;
    QTextFrameFormat rootformat;
    QTextCursor lastcursor;
    QTextImageFormat nowimage;
    QTextBlock textblocc;
    QTextTable  *nowtable;
    QFont workfont;
    bool GoPaintTable;
    bool loadetConnection;
      QAction *actionAlignLeft,
        *actionAlignCenter,
        *actionAlignRight,
        *actionAlignJustify,
        *actionTextColor,
        *actionBGColor,
        *actionUndo,
        *actionPscreen,
        *actionSave,
        *sourceTidy,
        *TableColorize,
        *actionRedo,
        *actionCut,
        *actionCopy,
        *actionCleanertext,
        *actionPaste;
void closeEvent( QCloseEvent* );
void LoadButtonControll();
void ReadNowTable( QTextTable * table );
void ResettingTableControll();
QIcon createColorIcon(QColor color);
    QColor BGColor;
    QColor MarginColor;
    qreal BorderDicks;
    QRectF InitValue;
    int AlphaColor;
    QSettings setter;
    QTextFrameFormat DocFrameFormings;
private:
    QPixmap currentim;
    void CatchPixmapCurrent( const QString  resname );
    QString CurrentResname;
signals:
   void RetocValues(QRectF,qreal,QColor,QColor,QTextDocument*,TypImageContainer); 
   void TransnRotation(int);
public slots:
    void SetMarginColor();
    void SetBGColor();
    void UpdateVars();
    void SaveNewFormat();
    void MaketextAlign(QAction *a);
    void MaketextColor();
    void currentCharFormatChanged(const QTextCharFormat &format);
    void PlaceNowFontChanged(const QFont &f);
    void MakealignmentChanged(Qt::Alignment a);
    void TextStyle(int styleIndex);
    void MakeTextBold();
    void WakeUpFont();
    void TableImageSetup();
    void MaketableColorBG();
    void UpdateCurrentTable();
    void UpdateCurrentImage();
    void ImageToClipbord();
    void SaveCurrentImageStream();
    void RotateFloat( int rotaz );
    void CurrentParamMargin();

};
//
#endif // WYMSINGTXT_H

