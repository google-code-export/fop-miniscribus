#ifndef SCRIBE_PARSER_QTEXTDOCUMENT_H
#define SCRIBE_PARSER_QTEXTDOCUMENT_H


#include "Config.h"
#include "SessionManager.h"
#include <limits.h>

/* Parser qtextdocument to save on other format */

class ScribeParser {
public:
  
typedef enum
{  
  Psave,
  Plinker
} ScribeParseModus;

    ScribeParser( QTextDocument * doc , ScribeParseModus e  = Plinker );
    inline QStringList internals() { return Internal_Destination_Links; }
    inline QStringList destinations() { return Clicks_Destination_Links; }
    inline QDomDocument *dom() { return document; }
    ~ScribeParser();
  private:
QDomDocument *document;
QTextDocument *textDocument;
QTextCursor helper_cursor;
ScribeParseModus modus;
QStringList Internal_Destination_Links;
QStringList Clicks_Destination_Links;
  
void MemoonBlock( QTextCursor c ,  QVariant data , const int id );
void processFrame(QDomElement appender ,  QTextFrame *frame);
void processBlock( QDomElement appender ,  QTextBlock   block );
void processFragment( QDomElement appender ,  QTextFragment  fragment );
void processTable( QDomElement appender , QTextTable *table);
void processTableCell( QDomElement appender , QTextTableCell cell );
};






#endif 


