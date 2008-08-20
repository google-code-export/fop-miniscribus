#ifndef FO_READER_H
#define FO_READER_H


#include "Fo_Format.h"



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
protected:
    int doc_cur;
    int table_cur;
    bool oldMiniScribusFormat;
    void read();
    void RootFramePaint( const QDomElement e );
    bool FoBlockTagPaint( const QDomElement e , QTextCursor Cursor  );  /* fo:block + fo:character + link */

    bool InlineBlockLoop( const QDomElement e , QTextCursor Cinline ,  bool skipborder = false );

    bool FoChildBlockPaint( const QDomElement e , bool recursive = false );   /* fo:block child is fo:block */
    bool FoBlockContainerPaint( const QDomElement e , QTextCursor Cinline ); /*  fo:block-container  */

    bool FoTableTagPaint( const QDomElement e  ); /* fo:table  */
    bool FoTableCellLoop( const QDomElement e  , QTextTableCell  cell  , qreal maxhight = 0. );  /* cell inside loop */

    bool FoListUlTagPaint( const QDomElement e  );   /* fo:list-item */
    bool InlineSpanning( const QDomElement e , const QDomElement parentup  , QTextCursor Cinline , bool HandleSpace = true );
    bool FoFloatingContainerPaint( const QDomElement e , const QDomElement parentup );  /* fo:float*/

    void DidplayUnknowTag( const QDomElement e  , QTextCursor Cursor );
    bool FoLeaderPaint( const QDomElement e  , QTextCursor Cursor );


    void FootNoteSave( const QDomElement e  , QTextCursor Cursor );

    bool RunningFirstFrame( QTextCursor Cursor );


    bool FoDrawSvgInline( const QDomElement e  );  /* only root tag */
    bool FoDrawSvgInline( const QDomElement e  , QTextCursor Cursor  ); /* only tree down */

    bool FoDrawImage(  const QDomElement e );
    bool FoDrawImage(  const QDomElement e , QTextCursor Cursor );

    void FrameDomIterator(  QDomNode node ,  QTextCursor Cinline  );
    void MoveEndDocNow();
    void DocRootFrameDefault();
    void PaintMessage(  QTextCursor Curs , const QString msg );
    
    
    bool placeNewAbsoluteLayer( const QDomElement e );
    

private:
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
    QMap<int,RichDoc> layerList;
    QVector<QTextLength> LastTableCON;

signals:

public slots:

};

}
//
#endif // FO_READER_H

