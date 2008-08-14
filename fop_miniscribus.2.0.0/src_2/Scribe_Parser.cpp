#include "Scribe_Parser.h"

#include "Text_Api.h"
#include "Fo_Writter.h"

/*   Parser QTextDocument  */

using namespace ApacheFop;

ScribeParser::ScribeParser( QTextDocument *  doc  , ScribeParseModus e )
{
    textDocument = doc;
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
      bf.setProperty (id,data);
      c.setBlockFormat(bf);

}


void ScribeParser::processBlock( QDomElement appender ,  QTextBlock   block )
{
    
    const int idnumerate = block.blockNumber();
    const QString blokstxt = Qt::escape(block.text());
    QString humantxt = Imagename(blokstxt.toUpper()); 
    humantxt.truncate(4);
    const QString InternDestName = QString("%1_").arg(idnumerate) + humantxt.leftJustified(10, '0');
    if (modus == Plinker && blokstxt.size() > 1 && InternDestName.size() > 9 ) {
          QTextCharFormat chformat = block.charFormat();
          if ( chformat.isAnchor() )  {
              QStringList dests = chformat.anchorNames();
               for (int i = 0; i < dests.size(); ++i) {
                 Internal_Destination_Links.append(dests.at(i));
               }
          
          
              if (dests.contains(InternDestName)) {
                 /* having insert all link and block bookmark */
                 Internal_Destination_Links.append(InternDestName);
             
              } else {
                dests << InternDestName;
                chformat.setAnchor(true);
                chformat.setAnchorNames(dests);
                chformat.setToolTip ( InternDestName );
                ////chformat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
                //////chformat.setForeground(QBrush(_LINK_COLOR_));
                Internal_Destination_Links.append(InternDestName);
                helper_cursor.setPosition(block.position());
                helper_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
                helper_cursor.setCharFormat(chformat);
                MemoonBlock(helper_cursor,InternDestName,BookMarkInternalID);

              }
          
          } else {
            QStringList linker;
            linker << InternDestName;
            chformat.setAnchor(true);
            chformat.setAnchorNames(linker);
            chformat.setToolTip ( InternDestName );
            /////chformat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
            /////chformat.setForeground(QBrush(_LINK_COLOR_));
            helper_cursor.setPosition(block.position());
            helper_cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            helper_cursor.setCharFormat(chformat);
            MemoonBlock(helper_cursor,InternDestName,BookMarkInternalID);
          }
    }


    QDomElement blockElement = document->createElement("block");
    blockElement.setAttribute("block_number",idnumerate);
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
         /////chformat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
         //////chformat.setForeground(QBrush(_INTERNAL_LINK_COLOR_));
         ////helper_cursor.setPosition(fragment.position() + fragment.length(),QTextCursor::KeepAnchor);
         ///////helper_cursor.setCharFormat(chformat);
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

