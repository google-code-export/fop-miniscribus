#ifndef FAPANEL_H
#define FAPANEL_H
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <math.h>
#include <QGraphicsScene>
#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QBrush>
#include <QPixmap>
#include <QColor>
#include <QPalette> 
#include <QGraphicsTextItem>
#include <QMenu>
#include "graphicsscene.h"
#include "fop_handler.h"
#include "floatdiagram.h"


class FAPanel : public QGraphicsView
{
     Q_OBJECT
//
public:
FAPanel(QWidget * parent);
void SetMenuExpandet( QMenu *SuperiorMenu );
void PaintPage( Fop_Handler * onOpen , QString topdffile = "");
void SaveOn( const QString savefile );
inline void SetDB( PageDB *dbin ) { 
    db = dbin;
    FillData();
}
inline QRectF GetmarginPage() { return margin; }
inline void SetmarginPage( QRectF ma ) {
   /////////// margin = ma; 
    setter.setValue("MyPageMargin",ma);
    if (db->connect()) {
        db->page(CurrentPrinterPage)->setmargin(ma);
    }
    SwapPaperFormat(CurrentPrinterPage);
}
inline void SetChessGrid( QPixmap eg ) { 
    chessgrid = eg; 
    ReDrawAll();
}
inline int PageFormat() { return CurrentPrinterPage; }
qreal MostUp();   /* zindex max */
qreal MostDown();  /* zindex min */
QMenu *MenuFromParent;  /* context menu to childs */
Fop_Handler *fopOpenXmlfile;
Fop_Handler *fopSaveXmlfile;
QWidget *BigParent;
QRectF margin;
QString Current_file_from_Main_to_PDF;  /* direct to pfd from other friend application */
protected:
    void wheelEvent (QWheelEvent * event);
    void scaleView (qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void CurrentResetNow();
    /* universal print */
    void MakePrintChoise( QPrinter::OutputFormat form , QPrinter::Orientation  orient = QPrinter::Portrait , const QString fileName = QString() , QPrinter::ColorMode  colmod = QPrinter::Color );
    void MakePageformat();
    void UpdatePageSumms();
    int NeedetPageSumm( qreal fromY );

/* variable */
QGraphicsScene *scene;
QFont workfont;
QList<FloatDiagram*> items;
QList<PageSetup*> FormatoPagina;
QRectF CurrentPageRect;
QRectF PrinterPageRectFix;
QStringList TargetInternalLink;
int CurrentPrinterPage;
FloatDiagram *CurrentActive;
QPrinter::PageSize  PrintZise;
QPixmap chessgrid;
QPixmap PageWidgedsBG;
int LASTIDTOUCH;
int PageSumm;
QSettings setter;
PageDB *db;
private:
    void FillData();
signals:
    void TotalPage(int);
    void SetPagePrintIndex(int);
    void PaintButton(FloatDiagram*);
    void OnRotate(int);
public slots:
    void RemoveItem();
    void sceneScaleChanged(const QString &scale);
    void ReDrawAll();
    void bringToFront();
    void sendToBack();
    void BigEdit();
    void LockItem();
    void UnLockItem();
    void AppendLayerInside();
    void MakefilePrintPdf();
    void OpenDesktop( QUrl loc );
    void SwapPaperFormat( int index );
    void RemoveOnePage();
    void AppendOnePage();
    /* print */
    void NormalPrint();
    void PrintPostcript();
    void PrePareToprintPaint();
    void ResetFromToprintPaint();
    void SetBackColor( QColor col );
    void backgroundColorTriggered();
    /* clone layer */
    void AppendCurrenttoClone();   /* active floating clone to new ... on top Zvalue */
    void RotateCurrentActive( int deeg );
    void SceneRasterClear();
    void NewDocumentSet();
    void RotateReset();
    void WorksOn(QGraphicsItem * item , qreal zindex );   /* the most on top zindex from selected */
    void ExtractImages();
    void PrintPdfCurrentMainFile();
    void FontSetting();
    void PageFontSetting();
    void HtmlExport();
    

};
//
#endif // FAPANEL_H

