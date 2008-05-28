#ifndef QDOCXHTML_H
#define QDOCXHTML_H


#include "xmlhighlighter.h"
#include "QmyDom.h"
#include "bestqtidy.h"
#include "DataStructures.h"
#include "pics_structure.h"
#include "richdoc_structure.h"






/* allowed char on file name image to save */
static inline bool CheckExtra(  QString stream )
{
    stream = stream.replace(" ","").trimmed();
    QStringList piece = stream.split("");
    QString normalchars = "abcdefghijklmnopqrstuvwxyz1234567890<>_|/\-=()%[]ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QStringList alist = normalchars.split("");
    
     for (int i = 0; i < piece.size(); ++i) {
           QString pin = piece.at(i);
           if (!alist.contains(pin)) {
            return true;
           }
       }
    
    return false;
}
/* log extra chars not abcdefghijklmnopqrstuvwxyz1234567890<>_-=()%[]ABCDEFGHIJKLMNOPQRSTUVWXYZ */
static inline bool LogFile(QString xml)
{
  return false;
  
  if (!CheckExtra(xml)) {
  return false;
  }
  
  QString data = xml+"\n";
  QFile f( "hotlog.txt" );
	if ( f.open( QFile::Append | QFile::Text ) )
	{
		QTextStream sw( &f );
    sw.setCodec(QTextCodec::codecForMib(106));
		sw << data;
		f.close();
		return true;
	}
	return false;
    
}







typedef enum
{  
  STYLEUNKNOW = 404,
  STYLESIMPLE,
  STYLEMEDIUM,
  STYLEFULL,
  STYLEOLDFORMAT = 500
} LEVEL;


class QdocXhtml : public QObject
{
     Q_OBJECT
public:
QdocXhtml();
QdocXhtml( QTextDocument * docin , LEVEL e = STYLESIMPLE , QString style = QString() );
void SetDoc( QTextDocument * docin , LEVEL e = STYLESIMPLE , QString style = QString() );
void SetHtml( QString html , LEVEL e = STYLESIMPLE );
QString Format( QString html );
~QdocXhtml();
inline qreal maxim() { return maxtower; }
inline QString Docxml() { return stream; }
void FrameLoop( QTextFrame::Iterator frameIt , QDomElement appender );
void HandleTable( QTextTable  * childTable , QDomElement appender );
void HandleBlock( QTextBlock  para  , QDomElement appender ); 
void ParaFormat( QDomElement e , QTextBlockFormat bf );
void PaintCharFormat( QDomElement e , QTextCharFormat bf  );
void PaintLastBlockformat( QDomElement e , QTextCharFormat bf  );
qreal maxtower;
void PaintFrameFormat ( QDomElement e , QTextFrameFormat bf  );
QString BorderStyleCss(QTextFrameFormat::BorderStyle style);
void TextAlignment(Qt::Alignment align , QDomElement e );
QString TranslateTextLengh( QTextLength unit );
QTextDocument *doc;
QDomDocument dom;
QTidy *cleaner;
int BigframeProcessing;
private:
  QStringList find;
  QMap<QString,QVariant> incss;
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
  inline qreal bigtower() { return maximumTower; }
  qreal maximumTower;
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

/*

    if (($asciiPos >= 240) && ($asciiPos <= 255)) {
            // 4 chars representing one unicode character
            $thisLetter = substr ($source, $pos, 4);
            $pos += 4;
        }
        else if (($asciiPos >= 224) && ($asciiPos <= 239)) {
            // 3 chars representing one unicode character
            $thisLetter = substr ($source, $pos, 3);
            $pos += 3;
        }
        else if (($asciiPos >= 192) && ($asciiPos <= 223)) {
            // 2 chars representing one unicode character
            $thisLetter = substr ($source, $pos, 2);
            $pos += 2;
        }
        else {
            // 1 char (lower ascii)
            $thisLetter = substr ($source, $pos, 1);
            $pos += 1;
        }

*/


static inline QString HTML2Filter(  QString stream )
{
  QdocXhtml *convert = new QdocXhtml();
  return convert->Format(stream);
}


static inline QString HTML3Filter(  QString stream )
{


  QMap<uint,QString> webchars;
  webchars.insert(8226,QString(" ")); /* • */
  webchars.insert(194,QString(" "));
  webchars.insert(13,QString(""));
  webchars.insert(10,QString(""));
  QdocXhtml *convert = new QdocXhtml();
  QString html = convert->Format(stream);  /* tidy try to clean */
  for (int i = 0; i < html.size(); ++i)   {
    
            QChar sign = html.at(i);
            const uint asciiPos = (uint)sign.unicode();
            bool replaceUnis = false;
             if ((asciiPos >= 240) && (asciiPos <= 255)) {
               replaceUnis = true;
             } else if ((asciiPos >= 224) && (asciiPos <= 239)) {
               replaceUnis = true;
             } else if ((asciiPos >= 192) && (asciiPos <= 223)) {
               replaceUnis = true;
             } else if ( asciiPos >7999 ) {
               replaceUnis = true;
             }
                       if (replaceUnis) {
                       /////////QString charU = QString("%1").arg(asciiPos);
                       ///////QByteArray preU = QByteArray();
                       //////const int prepender = qBound(1,5 - charU.size(),5);
                       ///////preU.fill('0',prepender);
                       ///////const QString UnicodeChar = QString("&#%2%1").arg(charU).arg(preU.data());
                       const QString UnicodeChar = QString("&#%1").arg(asciiPos);
                       webchars.insert(asciiPos,UnicodeChar);
                       }
                    
             
  }
   QMapIterator<uint,QString> i(webchars);
         while (i.hasNext()) {
             i.next();
             html = html.replace(QChar(i.key()),i.value());
          }
  return html;
          
}
















#endif // QDOCXHTML_H

