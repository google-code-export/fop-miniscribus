#ifndef FO_READER_H
#define FO_READER_H


#include "Fo_Format.h"

#ifdef _HAVING_NEW_TEXTDOCUMENT_
#include "FillCache.h"
#endif



namespace ApacheFop
{



//
class Fo_Reader : public Fo_Format
{
    Q_OBJECT
//
public:
    Fo_Reader( const QString readfile = 0 , QObject *parent = 0 );
    ~Fo_Reader();
    void LoadFopFile( const QString readfile );
    QTextDocument *document()
    {
        return Qdoc;
    }
    inline M_PageSize PSize()
    {
        return PageSize ;
    }
    QMap<int,RichDoc> layers()
    {
        return layerList;
    }
    inline QStringList urls() { return Remote_Image_List; }
    void appendPageParaBreak( QTextCursor Cinline , bool fullsize = true );
protected:
    int doc_cur;
    int table_cur;
    qreal layerYCurrentRead;
    bool oldMiniScribusFormat;
    qreal readerFopVersionDocument;
    void read();
    void RootFramePaint( const QDomElement e );
    bool FoBlockTagPaint( const QDomElement e , QTextCursor Cursor  );  /* fo:block + fo:character + link */

    bool InlineBlockLoop( const QDomElement e , QTextCursor Cinline ,  bool skipborder = false );

    bool FoChildBlockPaint( const QDomElement e , bool recursive = false );   /* fo:block child is fo:block */
    bool FoBlockContainerPaint( const QDomElement e , QTextCursor Cinline ); /*  fo:block-container  */

    bool FoTableTagPaint( const QDomElement e  , QTextCursor Cursor ); /* fo:table  */
    bool FoTableCellLoop( const QDomElement e  , QTextTableCell  cell  ,  QTextCursor Cursor , qreal maxhight = 0. );  /* cell inside loop */

    bool FoListUlTagPaint( const QDomElement e  );   /* fo:list-item */
    bool InlineSpanning( const QDomElement e , const QDomElement parentup  , QTextCursor Cinline , bool HandleSpace = true );
    bool FoFloatingContainerPaint( const QDomElement e , const QDomElement parentup );  /* fo:float*/

    void DidplayUnknowTag( const QDomElement e  , QTextCursor Cursor );
    bool FoLeaderPaint( const QDomElement e  , QTextCursor Cursor );


    void FootNoteSave( const QDomElement e  , QTextCursor Cursor );

    bool RunningFirstFrame( QTextCursor Cursor );


    /////////bool FoDrawSvgInline( const QDomElement e  );  /* only root tag */
    bool FoDrawSvgInline( const QDomElement e  , QTextCursor Cursor  ); /* only tree down */

    bool FoDrawImage(  const QDomElement e , QTextCursor Cursor );

    void FrameDomIterator(  QDomNode node ,  QTextCursor Cinline  );
    void MoveEndDocNow();
    void DocRootFrameDefault();
    void PaintMessage(  QTextCursor Curs , const QString msg );
    
    
    bool placeNewAbsoluteLayer( const QDomElement e );
    

private:
    QStringList Remote_Image_List;
    int Current_Block_Tree_Level;
    QString file;
    QFileInfo finfo;
    QDir read_dir;
    StreamFop *device;
    QDomDocument Xdoc;
    QTextCursor Tcursor;
    QTextDocument *Qdoc;
    M_PageSize PageSize;
    qreal Docwidth;
    int ImageCount;
    int LayerCount;
    QDomElement  trlastelemnt;
    QMap<int,RichDoc> layerList;
    QVector<QTextLength> LastTableCON;
    #ifdef _HAVING_NEW_TEXTDOCUMENT_
    ///////NetCacheSwap *netswap;
    #endif

signals:

public slots:

};

}



#ifdef _HAVING_NEW_TEXTDOCUMENT_



#else


namespace OpenOffice {
    
  enum OOTYPE {
    DOC_1_VERSION = 11,
    DOC_2_VERSION = 20, 
    DOC_UNKNOW = 100
  };
  
  
  /*
  
  usage
  
  OO_Xslt *oohandler = new OO_Xslt( const QString filedoc ,DOC_1_VERSION);
  
  */
  
#define XT_XMLFILE \
             QString("%1data.xml").arg(_GSCACHE_)
  
#define XT_STYLEFILE \
             QString("%1style.xsl").arg(_GSCACHE_)
  
  
#define XT_STREAMFILE \
             QString("%1stream.xml").arg(_GSCACHE_)
  
  
class OO_Xslt : public QObject
{
     Q_OBJECT
//
public:
   OO_Xslt( const QString filedoc , OOTYPE doc );
   void OpenFile( const QString filedoc , OOTYPE doc);
   QTextDocument  *Document() { return qdoc; }
   inline QMap<QString,QByteArray> imagelist() { return ooimage; }
   inline QMap<QString,QByteArray> fileziplist() { return ooFile; }

private:
    QTextDocument *qdoc;
    QStringList filelist;
    QMap<QString,QByteArray> ooimage; 
    QMap<QString,QByteArray> ooFile; 
    bool havingdoc;
    void convertXslt(OOTYPE doc);
    
signals:
public slots:

};


}






#endif









//
#endif // FO_READER_H

