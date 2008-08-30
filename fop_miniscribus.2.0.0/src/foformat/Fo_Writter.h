#ifndef FOPWRITER_H
#define FOPWRITER_H


#include "Fo_Format.h"
#include "SessionManager.h"
/* session file !!! */
#include <QGraphicsRectItem>
#include "Layer_Absolute_Floating.h"


typedef enum
{  
  BORDERALL = 100,
  BORDERTOP = 4,
  BORDERLEFT = 2,
  BORDERBOTTOM = 1,
  BORDERRIGHT = 3,
  BORDERNONE = 0
} LINERPOS;


class FoBorder
{
  public:
  enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 1 };
  FoBorder()
  {
  rgb = "rgb(0,0,0)";
  style = "solid";
  wo = BORDERALL;
  point = "0.09pt";
  }
  
  void Change( QString rgbcolor , QString pp = QString("1pt") , QString fostyle = QString("solid"))
  {
    rgb = rgbcolor;
    point = pp;
    style = fostyle;
  }
  
  void SetBorder(  QDomElement element )
  {
  if (FopInt(point) < 0.1) {
  return;
  }
  QStringList allb;
  const int stopon = (int)wo;
  allb << "border-bottom-" << "border-left-" << "border-right-" << "border-top-";
     for (int i = 0; i < allb.size(); ++i)  {
       /* qt can make only one border alround */
       if (wo == BORDERALL) {
       element.setAttribute(allb.at(i)+"style",style); 
       element.setAttribute(allb.at(i)+"width",point); 
       element.setAttribute(allb.at(i)+"color",rgb);
       } else if (wo == BORDERTOP && i == 0 ) {
         /* grep -future- */
       }
     }
  }
  QString point;
  QString style;
  QString rgb;
  LINERPOS wo;
};


Q_DECLARE_METATYPE(FoBorder);


namespace ApacheFop {

typedef enum
{  
  NOTSTYLE = 404,
  FOP_APACHE,
  FOP_MINISCRIBE
} LEVEL;



class FopDom : public QObject
{
     Q_OBJECT
    
    
    
public:
FopDom( QTextDocument * docin , M_PageSize page , LEVEL e = FOP_APACHE );  
QDomDocument absoluteLayer( AbsoluteLayer *e );
~FopDom();
inline QDomDocument Foxml() { return dom; }
private:
void SetDoc( QTextDocument * docin , M_PageSize page , LEVEL e /*FOP_APACHE*/  );
inline qreal maxim() { return maxtower; }
inline QDomDocument DocFop() { return dom; }
QDomElement bookMarkTree( const QByteArray xmlt );
inline QDomDocument asDom() { return dom; }
void FrameLoop( QTextFrame::Iterator frameIt , QDomElement appender );  /* root frame ! */
void HandleFrameInline( QTextFrame::Iterator frameIt , QDomElement appender );  /* inline child frame/frame ! */
void HandleTable( QTextTable  * childTable , QDomElement appender );
void HandleBlock( QTextBlock  para  , QDomElement appender ); 
void HandleFragment( QTextFragment fr  , QDomElement appender , bool pref );
void ParaFormat( QDomElement e , QTextBlockFormat bf );
void PaintLastBlockformat( QDomElement e , QTextCharFormat bf  );
qreal maxtower;
void PaintFrameFormat ( QDomElement e , QTextFrameFormat bf  );
QString BorderStyleCss(QTextFrameFormat::BorderStyle style);

void ListUlLiSymbol( QDomElement appender , QTextList *list );
void SendBreakLine( QDomElement appender , bool blockcreate = false );  /* force line break <br> ! */


qreal LineHightCurrent(  QTextBlockFormat bf , QTextCharFormat cf  );

/* write attributes */
void PaintFopBlockFormat( QDomElement e , QTextBlockFormat bf );
void TextAlignment(Qt::Alignment align , QDomElement e );
void PaintFopCharFormat( QDomElement e , QTextCharFormat bf  );
/* write attributes */

bool ColorValid( const QBrush paintcolor ) const;
bool ColorValid( const QColor c ) const;


QString PageName;
qreal LINEHIGHT_CURRENT;
QTextDocument *doc;

QDomElement ulblock;  /* internal block ul to make or remove ! */
QDomElement NodeFooter();


int sumblox;
QDomDocument dom;
int BigframeProcessing;
M_PageSize m_page;
  LEVEL styler;
  QTextFrame  *RootDocFrame;  /* general qtexframe base */
  QString FilterAttribute( QDomElement element , QString attribute );
  QString ColorFopString( const QBrush paintcolor ) const;
  QString ColorFopString( const QColor c ) const;
  QString TranslateTextLengh( const QTextLength unit );
  
  
  void HumanRead( QDomElement appender , QString longtext );
signals:
  void DomReady();
public slots:

};



};

#endif // FopDom_H

