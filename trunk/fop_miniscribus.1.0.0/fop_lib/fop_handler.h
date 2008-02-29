#ifndef FOP_HANDLER_H
#define FOP_HANDLER_H
#include "prefophandler.h"


class PageSetup : public QObject
{
    Q_OBJECT
//
public:
 PageSetup(  const QString name , QRectF ret , int value )
{
  Na = name;
  Rect = ret;
  printerQtValue = value;
}
inline int Getid() { return printerQtValue; }
inline QString GetName() { return Na; }
inline QRectF Getrect() { return Rect; }
protected:
    QString Na;
    QRectF Rect;
    int printerQtValue;
};











/* ------------------------------ converter from fop to QTextDocument ----------------------------*/


class Fop_Handler : public PreFopHandler
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Peter Hohl" )
    Q_CLASSINFO( "link", "http://www.ciz.ch" )
    Q_CLASSINFO( "date", "2007/29/09" )
    Q_CLASSINFO( "version", "1.0.1a" )
    Q_CLASSINFO( "since", "1.0.1" )
    Q_CLASSINFO( "default_background", "#ffffff" )
    Q_CLASSINFO( "default_text", "#000000" )
    Q_CLASSINFO("Status", "Active")
    Q_CLASSINFO( "TagReference", "http://www.zvon.org/xxl/xslfoReference/Output/index.html" )
    
    
#define default_background "#ffffff" 
#define default_text "#000000" 
#define default_alphacolor "250" 
    
    
public:
    




      Fop_Handler( const  QString  file , bool modus , PageDB *dbin  , QWidget * parent = 0 );  /* file to open or save */
      ~Fop_Handler();
      inline QList<Fop_Layer*> GetLayerList() { return layerlist; }
      
      inline int ErrorSize() { return ErnoMap.size(); }
      inline TypErnoMap GetError() { return ErnoMap; }
      inline TypnotexistFontMap FontError() { return notexistFontMap; }
      
      
      
      inline int RemoteFileSize() { return RemoteImageMap.size(); }
      inline QString PaintPrinterName() const { return CurrentPrinterNameFormat; }
      /* write a new document */
      QDomDocument GetStructure( QRectF page , QRectF margin , QColor pagebg , const QString PaperName , int pagesum = 0 );  /* layer paint in dom */
      void AppendLayer( QDomElement e , QTextDocument * d );   /* append layer to doc writteln */
      void Writtelnend();     /* save writteln file end! */
      inline QRectF PageMargin() { return Pmargin; }
      inline int getPageSumms() const  { return Page_summer; }
      int Page_summer;
protected:
    /* write fop read QTextDocument and bring to file stream or to QDomDocument */
    void HandleFrame( QTextFrame  * Tframe , FRAME_TYP dd , QDomElement appender );
    void HandleBlock( QTextBlock  para  , QDomElement appender );   /* write paragraph to fop */
    void HandleTable( QTextTable  * childTable , QDomElement appender );  /* write table to fop */
    
    int BigframeProcessing;
    /* write fop read QTextDocument and bring to file stream or to QDomDocument */
    //////////QTextCursor Tcursor;  /* the layer write cursor */
    TypImageContainer RemoteImageMap;
    QRectF Pmargin;
    QStringList WaitList;
    QTextDocument *IncommingFromLayer;
    QFile *Fopdevice;
    QString Fopfile;
    QString ActualBackgroundColor;
    QFileInfo fopinfo;
    QString FopBaserRefDir;
    QString ImageFopBaserRefDir;
    QDomDocument doc;
    QDomDocument wdoc;
    QDomElement flowwrite;
    QTextCodec *codecx;
    bool canContinue;
    int LayerSum;
    int imagecounter;
    int remotefilecounter;
    QDomElement root;
    QDomElement page;
    QDomElement layout_master;
    QDomElement master;
    QList<Fop_Layer*> layerlist;
    QStringList layerNames;
    QString CurrentIdLayer;
    QWidget *BigParent;
    QString CurrentPrinterNameFormat;
    QTextCharFormat  targetAnchor;
private:
    void  OpenModus();
    QProgressDialog *dlg;
    /* help on cell */
    /* text doc format */
    int CursorStatementPosition;
    int ParaGraphCounter;
    QTextBlockFormat GetBlockFormat( const QDomElement e );
    QTextBlockFormat ParamFormat( const QDomElement e , QTextBlockFormat f = QTextBlockFormat()); /* paragraphs */
    QTextCharFormat GetCharFormat( const QDomElement e , QTextCharFormat f = QTextCharFormat() ); /* inline elements */
    /* text doc format */

    /* floating block x,y */
    void  ParserLayerFloating(  Fop_Layer * layer ,  const QDomElement e );
    void  ParserParagraph( const QDomElement e , QTextDocument * d , Fop_Layer * layer , int CursorPositon = 0 , bool firsttdcell = false );
    void  ParserTable( const QDomElement e , QTextDocument * d , Fop_Layer * layer , int CursorPositon = 0 );

    /* image and svg renders */
    /* http://vallemaggia.ch/multimedia/Cartina_VM.svgz */
    void ImageParserTag(  const QDomElement e ,  QTextDocument * d  , Fop_Layer * layer   , int CursorPositon = 0 );
    bool ObjectParserTag(  const QDomElement e ,  QTextDocument * d  , Fop_Layer * layer , int CursorPositon = 0 );
    QPixmap RenderSvg(  const QDomElement e  , Fop_Layer * layer , const QString nameresource );
    
    
    /* download remote image */
    void RegisterRemoteFile( const QString urlimage );
    void LoopOnTagRemoteFile( const QDomElement e );
    void GetFileatPos( int on );
    QVariant ResourceBlockRemoteImage( const QString urlimage );  /* image QByteArray  as key url */
    
signals:
    void ConnectList(bool);
public slots:
    void SaveSreamList( int nr , QByteArray data , QString url );
    void UpdateStatus( int sums  );
    void CheckRemoteImage();

};




/* ------------------------------ converter from fop to QTextDocument ----------------------------*/




//
#endif // FOP_HANDLER_H

