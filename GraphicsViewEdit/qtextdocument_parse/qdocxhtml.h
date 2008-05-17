#ifndef QDOCXHTML_H
#define QDOCXHTML_H


#include "xmlhighlighter.h"
#include "QmyDom.h"
#include "bestqtidy.h"
#include "DataStructures.h"
#include "pics_structure.h"
#include "richdoc_structure.h"

#include "cmspage_structure.h"
#include "helpers.h"





class QdocXhtml : public QObject
{
     Q_OBJECT
public:
  typedef enum
{  
  STYLEUNKNOW = 404,
  STYLESIMPLE,
  STYLEMEDIUM,
  STYLEFULL
} LEVEL;

QdocXhtml();
QdocXhtml( QTextDocument * docin , LEVEL e = STYLESIMPLE );
void SetDoc( QTextDocument * docin , LEVEL e = STYLESIMPLE );
void SetHtml( QString html , LEVEL e = STYLESIMPLE );
QString Format( QString html );
~QdocXhtml();
inline QString Docxml() { return stream; }
void FrameLoop( QTextFrame::Iterator frameIt , QDomElement appender );
void HandleTable( QTextTable  * childTable , QDomElement appender );
void HandleBlock( QTextBlock  para  , QDomElement appender ); 
void ParaFormat( QDomElement e , QTextBlockFormat bf );
void PaintCharFormat( QDomElement e , QTextCharFormat bf  );
void PaintLastBlockformat( QDomElement e , QTextCharFormat bf  );

void PaintFrameFormat ( QDomElement e , QTextFrameFormat bf  );
QString BorderStyleCss(QTextFrameFormat::BorderStyle style);
QString TranslateTextLengh( QTextLength unit );
QTextDocument *doc;
QDomDocument dom;
QTidy *cleaner;
int BigframeProcessing;
private:
  LEVEL styler;
  QString stream;
  QString FilterAttribute( QDomElement element , QString attribute );
signals:
    
public slots:

};




class Html_Transformer : public QObject
{
     Q_OBJECT
//
public:
  Html_Transformer();
  Html_Transformer( QMap<int,RichDoc> divs  );
  void work( QMap<int,RichDoc> divs );
  QDomDocument xml_document();
  bool Conform();
  inline QString text() { return xml; }
protected:
  QString WebSource( RichDoc edoc );
  QString xml;
private:
  int passage;
signals:
public slots:
   void Syntaxcheck();
   QString source();

};










#endif // QDOCXHTML_H

