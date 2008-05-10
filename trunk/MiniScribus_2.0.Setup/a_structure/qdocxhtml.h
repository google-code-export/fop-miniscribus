#ifndef QDOCXHTML_H
#define QDOCXHTML_H

#include "QmyDom.h"
#include "bestqtidy.h"
#include "pics_structure.h"
#include "metric_convert.h"








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
void HandleFrame( QTextFrame  * Tframe , QDomElement appender ); 
void HandleTable( QTextTable  * childTable , QDomElement appender );
void HandleBlock( QTextBlock  para  , QDomElement appender ); 
void ParaFormat( QDomElement e , QTextBlockFormat bf );
void PaintCharFormat( QDomElement e , QTextCharFormat bf  );
void PaintLastBlockformat( QDomElement e , QTextCharFormat bf  );
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











#endif // QDOCXHTML_H

