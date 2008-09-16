#ifndef PAGEFORMATDLG_H
#define PAGEFORMATDLG_H
#include "config.h"
#include <stdio.h>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "FoColorName.h"
#include "PageFormatGroup.h"
#include "ui_pageformatdlg.h"
#include "SessionManager.h"
                                
#include <QPointer>

class PageFormatDlg : public QDialog, public Ui::PageFUiDialog
{
     Q_OBJECT

public:
    
 static PageFormatDlg* self( QWidget* = 0 );
 void   fillFormat( M_PageSize e  );
 inline M_PageSize format() { return currentP; }
protected:
    QStringList l_colors;
    QMap<int,M_PageSize> original;
    void saveOriginal();
    FopColor *fopC;
    bool allowUpdate;
    qreal SessionBigerPaper;  /* big quadra to display it */
    void resizeEvent ( QResizeEvent * event );
    QVariant currentItemData(QComboBox *Box);
    void findeSetItemData(QComboBox *box  , QVariant e );
    void findeSetItemPen(QComboBox *box  , const QPen e );
private:
  QString unit;
  M_PageSize currentP;
 void DrawPageResult();

 void insertItemColorBox( const QString txt , const QColor qcol );
 void  fillColorBox();
 PageFormatDlg( QWidget* = 0 );
 static QPointer<PageFormatDlg> _self;
 //////////void Load_Connector();
signals:
public slots:
    void formatPageChange( const int index );
    void restoreOriginal();
    void recalcAll();

};
//
#endif // PAGEFORMATDLG_H

