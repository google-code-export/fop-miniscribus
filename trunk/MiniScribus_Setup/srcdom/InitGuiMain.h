#ifndef GUI_MAIN_H
#define GUI_MAIN_H


#include "GraphicsView.h"
#include "DataStructures.h"
#include "GraphicsItemEdit.h"
#include "PageXmlHandlerCms.h"
#include "helpers.h"




//
class Gui_Main : public QMainWindow
{
     Q_OBJECT
//
public:
  Gui_Main( QWidget *parent = 0 );

protected:
    void OpenPanels();
//////void closeEvent( QCloseEvent *e );
private:
    bool event(QEvent *ev);
   int modus;
/* tool bar setup */
    QAction *swapedit;
    QToolBar *toolBar_pages;
    QToolBar *toolBar_0;
    QToolBar *toolBar_1;
    QToolBar *toolBar_2;
    QToolBar *toolBar_3;
    QToolBar *toolBar_4;
    //////////////GraphicsView *panel;
    QWidget *xysett;
    QDoubleSpinBox *xxpos;
    QDoubleSpinBox *yypos;
    QDoubleSpinBox *layhi;
    QDoubleSpinBox *laywi;
    QString unitas;
    QSettings setter;
    QWidget *scalesett;
    QComboBox *comboBox;
    enum { NumComandButton = 13 };
    Pbutton *tbutton[NumComandButton];
    
    /* tool bar setup */
    QAction *actionAttributeSetter;
    QAction *actionXmlCheckers;
    
    /* current page handler */  
    PageXmlHandler *Opage;
    /* page small attributes */
    PageFirstTree *Pattribut_tree;
    /* panel to send page attributes */
    QDockWidget *pattributes;
    QDockWidget *xmlcontroll;
    QLabel *AttLabel;
    XMLTextEdit *xml_editor;
    
    /* status */
    QStatusBar *status; 
    QWidget *pstatus;  
    QHBoxLayout *hboxLayout;
    QHBoxLayout *hboxLayout2;
   QLabel *statuslabel;
   QLabel *statuslabel_1;
   DigitalClock *clocke;  
   QLineEdit *searchgui;
   QGridLayout *stgrid;
   QCheckBox *boolemode;
    /* status */

    WorkPanel *work;
    
     
    
 QSize avaiable;
 void MainUpdate();
 void Load_Toolbar( const int modus);
 void Load_Connector();
signals:

public slots:
  void Msg( const QString txt );
  void update_attributes();

};
//
#endif // GUI_MAIN_H

