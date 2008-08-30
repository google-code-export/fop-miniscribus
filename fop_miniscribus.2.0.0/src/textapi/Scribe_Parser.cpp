#include "Scribe_Parser.h"

#include "Text_Api.h"
#include "Fo_Writter.h"
#include "Image_Page_Struct_Mime.h"


/*   Parser QTextDocument  */

using namespace ApacheFop;

ScribeParser::ScribeParser( QTextDocument *  doc  , ScribeParseModus e )
{
    textDocument = doc;
    resource.clear();
    ApiSession *sx = ApiSession::instance();
    M_PageSize pageformat = sx->CurrentPageFormat();
    modus = e;
    bool ContinueParser = true;
    if (e == PFopModus) {
    FopDom *foppi = new FopDom(doc,pageformat,FOP_APACHE);
    document = new QDomDocument();
    document->setContent (foppi->Foxml().toByteArray(1),false);
    foppi->deleteLater();
    ContinueParser = false;
    }
    
    Internal_Destination_Links.clear();
    if (ContinueParser) {
    QTextFrame *root = doc->rootFrame();
    helper_cursor = QTextCursor(doc); 
    QDomImplementation implementation;
    QDomDocumentType docType = implementation.createDocumentType("scribe-document", "scribe","www.trolltech.com/scribe");
    document = new QDomDocument(docType);
    QDomProcessingInstruction process = document->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    document->appendChild(process);
    QDomElement documentElement = document->createElement("document");
    document->appendChild(documentElement);
    if (root) {
    processFrame(documentElement,root);
    }
    
   }
    mimeDoc.Register(doc,resource,QString());
}



void ScribeParser::processFrame(QDomElement appender ,  QTextFrame *frame)
{
      //~ qDebug() << "### processFrame   ";

    QDomElement frameElement = document->createElement("frame");
    frameElement.setAttribute("begin", frame->firstPosition());
    frameElement.setAttribute("end", frame->lastPosition());
    appender.appendChild(frameElement);
    QTextFrame::iterator it;
    for (it = frame->begin(); !(it.atEnd()); ++it) {
        QTextFrame *childFrame = it.currentFrame();
        QTextBlock childBlock = it.currentBlock();
    
        if (childFrame) {
            if (QTextTable *table = qobject_cast<QTextTable *>(it.currentFrame())) {
            processTable(frameElement,table);
            } else {
            processFrame(frameElement, childFrame);
            }
        } else if (childBlock.isValid()) {
            processBlock(frameElement, childBlock);
        }
    }

}

///////////int QTextBlock::position () const

void ScribeParser::MemoonBlock( QTextCursor c ,  QVariant data , const int id )
{
      QTextBlockFormat bf = c.blockFormat();
      QString idmemo = bf.property(BookMarkInternalID).toString();
    
      if (idmemo.isEmpty()) {
           bf.setProperty (id,data);
           c.setBlockFormat(bf); 
      }
}


void ScribeParser::processBlock( QDomElement appender ,  QTextBlock   block )
{
    bool HavingIdBlock = false;
    const int idnumerate = block.blockNumber();
    QTextBlockFormat bf = block.blockFormat();
    QString idMemobockMarks = bf.property(BookMarkInternalID).toString();
    const QString blokstxt = Qt::escape(block.text());
    QString humantxt = Imagename(blokstxt.toUpper()); 
    humantxt.truncate(4);
    ApiSession *sx = ApiSession::instance();
    QString InternDestName = sx->validateUnique(QString("%1_").arg(idnumerate) + humantxt.leftJustified(15, '0'));
    
    if (!idMemobockMarks.isEmpty()) {
        InternDestName = idMemobockMarks;
        HavingIdBlock = true;
        Internal_Destination_Links.append(idMemobockMarks);
    }
    
    
    if (modus == Plinker && blokstxt.size() > 1 && InternDestName.size() > 4 ) {
        
          QTextCharFormat chformat = block.charFormat();
          if ( chformat.isAnchor() )  {
              
                helper_cursor.setPosition(block.position());
                helper_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
              
                QStringList dests = chformat.anchorNames();
                if (dests.size() > 0) {
                    QString aslink = dests.first();
                    if (!aslink.isEmpty()) {
                     MemoonBlock(helper_cursor,aslink,BookMarkInternalID);
                     HavingIdBlock = true;
                    }
                }
              
               for (int i = 0; i < dests.size(); ++i) {
                 Internal_Destination_Links.append(dests.at(i));
               }
          
          
       
          
          } else {
              
                 if (!HavingIdBlock) {
                    QStringList linker;
                    linker << InternDestName;
                    chformat.setAnchor(true);
                    chformat.setAnchorNames(linker);
                    chformat.setToolTip ( InternDestName );
                    helper_cursor.setCharFormat(chformat);
                    Internal_Destination_Links.append(InternDestName);
                    MemoonBlock(helper_cursor,InternDestName,BookMarkInternalID);
                 }
              
          }
    }


    QDomElement blockElement = document->createElement("block");
    blockElement.setAttribute("block_number",idnumerate);
    blockElement.setAttribute("idfop",InternDestName);
    blockElement.setAttribute("position", block.position());
    blockElement.setAttribute("length", block.length());
    appender.appendChild(blockElement);

    QTextBlock::iterator it;
    for (it = block.begin(); !(it.atEnd()); ++it) {
        QTextFragment fragment = it.fragment();
        if (fragment.isValid()) {
            processFragment(blockElement,fragment);
        }
    }
}

void ScribeParser::processFragment( QDomElement appender ,  QTextFragment  fragment )
{
     //~ qDebug() << "### processFragment   ";

     QTextCharFormat chformat = fragment.charFormat();
     if (chformat.isAnchor() && chformat.anchorNames().size() > 0 ) {
         
         
         
         
     }  else if ( chformat.isAnchor() && 
                 chformat.anchorHref().size() > 0 && 
                 !Clicks_Destination_Links.contains(chformat.anchorHref())) {
         Clicks_Destination_Links.append(chformat.anchorHref());
     }

    if (modus == Plinker ) {
          
          if ( chformat.isAnchor() )  {
                 QStringList dests = chformat.anchorNames();
                 for (int i = 0; i < dests.size(); ++i) {
                   Internal_Destination_Links.append(dests.at(i));
                 }
          }
    }
    
     if (chformat.isImageFormat()) {
     QTextImageFormat Pics = chformat.toImageFormat();
     const QString hrefadress = Pics.name();
         QDomElement imagen = document->createElement("imgage");
         imagen.setAttribute ("width",QString("%1").arg(Pics.width()));
         imagen.setAttribute ("height",QString("%1").arg(Pics.height()));
         imagen.setAttribute ("src",hrefadress);
         appender.appendChild(imagen);
         QVariant xx = textDocument->resource(QTextDocument::ImageResource,QUrl(hrefadress));
         QVariant xx1 = Pics.property(_IMAGE_PICS_ITEM_);
         
         SPics qtrecord;
         qtrecord.name = hrefadress;    
         QPixmap img = xx.value<QPixmap>();
         qtrecord.set_pics(img);
         
         if (!xx1.isNull()) {
            SPics pic = xx1.value<SPics>();
            pic.name = hrefadress; 
            resource.insert(hrefadress,pic); 
         } else {
            resource.insert(hrefadress,qtrecord); 
         }
     }

    QDomElement fragmentElement = document->createElement("fragment");
    appender.appendChild(fragmentElement);
    fragmentElement.setAttribute("length", fragment.length());
    fragmentElement.setAttribute("position", fragment.position());
    ///////blockElement.setAttribute("length", block.length());
    QDomText fragmentText = document->createTextNode(fragment.text());
    fragmentElement.appendChild(fragmentText);
}



void ScribeParser::processTable( QDomElement appender , QTextTable *table)
{
     //~ qDebug() << "### processTable   ";

    QDomElement element = document->createElement("table");

    for (int row = 0; row < table->rows(); ++row) {
        for (int column = 0; column < table->columns(); ++column) {
            QTextTableCell cell = table->cellAt(row, column);
            processTableCell(element, cell);
        }
    }
    appender.appendChild(element);
}


void ScribeParser::processTableCell( QDomElement appender , QTextTableCell cell )
{

      //~ qDebug() << "### processTableCell   ";

    QDomElement element = document->createElement("cell");
    element.setAttribute("row", cell.row());
    element.setAttribute("column", cell.column());
    
    QTextFrame::iterator it;
    for (it = cell.begin(); !(it.atEnd()); ++it) {

        QTextFrame *childFrame = it.currentFrame();
        QTextBlock childBlock = it.currentBlock();

        if (childFrame) {
            processFrame(element, childFrame);
        } else if (childBlock.isValid()) {
            processBlock(element, childBlock);
        }
    }
    appender.appendChild(element);
}




 
ScribeParser::~ScribeParser()
{
}




/* image get remote */


LoadGetImage::LoadGetImage( int nr , QUrl url_send  )
        : QHttp(url_send.host(),QHttp::ConnectionModeHttp ,80)
{
    url = url_send;
    cid = nr;
    setHost(url_send.host() , 80);
}

void LoadGetImage::Start()
{
    const QString METHOD =  "GET";
    const QString agent = QString("Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");
    header.setRequest(METHOD,url.path(),1,1);
    header.setValue("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");
    header.setValue("Host",url.host());
    header.setValue("User-Agent",agent);
    connect(this, SIGNAL(done(bool)), this , SLOT(ImageReady(bool)));
    Http_id = request(header,0,0);
}

void LoadGetImage::ImageReady( bool error )
{
    if (!error)
    {
        resultimage.loadFromData(readAll());
        if (!resultimage.isNull())
        {
            emit take(cid);
        }
    }
}


void Gloader::Setting( QObject *parent , int id , QUrl url_send )
{
    receiver = parent;
    cid = id;
    url = url_send;
    setTerminationEnabled(true);
}

void Gloader::run()
{
    Rhttp = new LoadGetImage(cid,url);
    connect(Rhttp, SIGNAL(take(int)), receiver , SLOT(in_image(int)));
    Rhttp->Start();
    exec();
}


/*   GraphicsScene   QGraphicsScene  */




GraphicsScene::GraphicsScene( QObject * parent )
        : QGraphicsScene( parent ),bridge(0)
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}
GraphicsScene::GraphicsScene(  const QRectF & sceneRect, QObject * parent )
        : QGraphicsScene( sceneRect,  parent ),bridge(0)
{
    setSceneRect( sceneRect );
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

void GraphicsScene::SetVisibleArea( const QRectF area )
{
    emit MakeVisible(area);
}

/*
void GraphicsScene::dragMoveEvent ( QGraphicsSceneDragDropEvent * e )
{
    return QGraphicsScene::dragMoveEvent(e);
}
*/


AbsoluteLayer *GraphicsScene::itemAbsolute( QGraphicsItem * item )
{
    AbsoluteLayer *layer = NULL;
    if (layer = static_cast<AbsoluteLayer *>(item) ) {
       return  layer;
    } else {
      return  layer;
    }
    
}

void GraphicsScene::clear()
{
    clearSelection();
    QGraphicsScene::clear();
    storno();
}

void GraphicsScene::clearSelection()
{
    QGraphicsScene::clearSelection();
    update(sceneRect());
}

void GraphicsScene::storno()
{
    bridge = 0;
    emit SelectOn(bridge);
}

void GraphicsScene::removeIdItem( const int idchunk )
{
    
    qDebug() << "### RotateLayer " << idchunk;
    
    const QList<QGraphicsItem*> items =  QGraphicsScene::items();
    QList<QGraphicsItem*> subLevelItems;
    /* loop all elemenst */
    foreach(QGraphicsItem* item, items)
    {
        subLevelItems << item;
        
        if (item->parentItem())
        {
            QList<QGraphicsItem *> des = item->childItems();
            foreach(QGraphicsItem* de,des)
            {
                subLevelItems << de;
            }
        }
    }
    
    foreach(QGraphicsItem* it, subLevelItems) {
        const QString chunk = it->data(AbsoluteLayerRemoveHashID).toString();
        if (chunk.size() > 3) {
            delete it;
        }
    } 
}

qreal GraphicsScene::zmax()
{
    qreal mValue = 0.00;
    const QList<QGraphicsItem*> items =  QGraphicsScene::items();
    QList<QGraphicsItem*> subLevelItems;
    /* loop all elemenst */
    foreach(QGraphicsItem* item, items)
    {
        mValue = qMax(item->zValue(),mValue);
        if (item->parentItem())
        {
            QList<QGraphicsItem *> des = item->childItems();
            foreach(QGraphicsItem* de,des)
            {
                subLevelItems << de;
            }
        }
    }
    foreach(QGraphicsItem* it, subLevelItems)
    {
        mValue = qMax(it->zValue(),mValue);
    }
    /* loop all elemenst */
    qDebug() << "### sc maxi ---------------------------------------------------------" << mValue;

    return mValue;
}

qreal GraphicsScene::zmin()
{
    qreal mValue = zmax();
    const QList<QGraphicsItem*> items =  QGraphicsScene::items();
    QList<QGraphicsItem*> subLevelItems;
    /* loop all elemenst */
    foreach(QGraphicsItem* item, items)
    {
        mValue = qMin(item->zValue(),mValue);
        if (item->parentItem())
        {
            QList<QGraphicsItem *> des = item->childItems();
            foreach(QGraphicsItem* de,des)
            {
                subLevelItems << de;
            }
        }
    }
    foreach(QGraphicsItem* it, subLevelItems)
    {
        mValue = qMin(it->zValue(),mValue);
    }
    /* loop all elemenst */
    qDebug() << "### sc minimum ---------------------------------------------------" << mValue;

    return mValue;
}



/* filter only item a top Zindex /  zValue */
bool GraphicsScene::WakeUp( const QPointF incomming )
{
    QList<QGraphicsItem *>  listing = QGraphicsScene::items(incomming);
    qreal thebest = 0;  /* zindex on front */
    for (int o=0;o<listing.size();o++)
    {
        listing[o]->setSelected(false);
        listing[o]->setFlag(QGraphicsItem::ItemIsSelectable,false);
        thebest = qMax(listing[o]->zValue(),thebest);
    }
    ///////qDebug() << "### thebest->" << thebest;
    /* activate item at top z-index zValue  /  not the down not visible!!!! */
    for (int e=0;e<listing.size();e++)
    {
        if (listing[e]->zValue() == thebest)
        {
            listing[e]->setFlag(QGraphicsItem::ItemIsSelectable,true);
            listing[e]->setSelected(true);
            bridge = listing[e];
            emit SelectOn(listing[e]);
            return true;
        }
    }
    emit SelectOn(bridge);
    return false;
}

void GraphicsScene::keyPressEvent(QKeyEvent *e )
{
    //////////////qDebug() << "### keyPressEvent scene ...";
    QGraphicsScene::keyPressEvent(e);
}

void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))
    {
        event->setAccepted ( true ) ;
        clearSelection();
    }
    else
    {
        storno();
    }
    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))
    {
        event->setAccepted ( true ) ;
        clearSelection();
    }
    else
    {
        storno();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
void GraphicsScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    if (WakeUp(QPointF(event->scenePos().x(),event->scenePos().y())))
    {
        event->setAccepted ( true ) ;
        clearSelection();
    }
    else
    {
        storno();
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}


void GraphicsScene::setSceneRect(  const QRectF & rect )
{
    ///////Area = rect;
    QGraphicsScene::setSceneRect(rect);
}




















