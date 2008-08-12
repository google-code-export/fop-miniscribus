#ifndef FO_READER_H
#define FO_READER_H


#include "Fo_Format.h"

namespace ApacheFop {
    
    
class StreamFop
{
  public:
  StreamFop() 
  :d(new QBuffer())
  {
  d->open(QIODevice::ReadWrite);
  start();
  }
  ~StreamFop()
  {
   d->close();
  }
  bool clear()
  {
    d->write(QByteArray()); 
    return d->bytesAvailable() == 0 ? true : false;
  }
  void start() { 
    d->seek(0);
  }
  bool LoadFile( const QString file ) { 
    if (clear()) {
     QFile f(file); 
        if (f.exists()) {
          if (f.open(QFile::ReadOnly)) {
           d->write(f.readAll());
           f.close();
           start();
           return true;
          }
        }
    }
    return false;
  }
  bool PutOnFile( const QString file ) { 
      QFile f(file);
      start();
      if (f.open(QFile::WriteOnly)) {
        uint bi = f.write(d->readAll());
        f.close();
        start();
        return bi > 0 ? true : false;
      }
   return false; 
  }
  QDomDocument Dom() { return doc; }
  QBuffer *device() { return d; }
  bool isValid() { return doc.setContent(stream(),false,0,0,0);  }
  QByteArray stream() { return d->data(); }
  QDomDocument doc;
  QBuffer *d;
}; 


    
    

//
class Fo_Reader : public Fo_Format
{
     Q_OBJECT
//
public:
    Fo_Reader( const QString readfile = 0 , QObject *parent = 0 );
    void LoadFopFile( const QString readfile );
    QTextDocument *document() { return Qdoc; }
    inline M_PageSize PSize() { return PageSize ;}
    
protected:
    int doc_cur;
    int table_cur;
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
    void FoLeaderPaint( const QDomElement e  , QTextCursor Cursor );


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

    QVector<QTextLength> LastTableCON;
    
signals:

public slots:

};

}
//
#endif // FO_READER_H

