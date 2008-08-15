#ifndef FOPCOLORFORMAT_H
#define FOPCOLORFORMAT_H


#include <QtCore>
#include <QDebug>
#include <QCoreApplication>
#include <QColor>
#include <QMap>
#include <QPixmap>

#define _USELISTAPACHECOLOR_  1

/*

    QTextEdit t;
    FopColor *fc = new FopColor();
    qDebug() << "### load color   ";
    QStringList items = fc->fopListColor();
    for (int i = 0; i < items.size(); ++i)  {
        const QString name = items.at(i);
        QTextCursor c = t.textCursor();
        QPixmap e =  fc->createColorMap(name);
        t.document()->addResource(QTextDocument::ImageResource,QUrl(name),e);
        QTextImageFormat format;
        format.setName( name );
        format.setHeight ( e.height() );
        format.setWidth ( e.width() );
        format.setToolTip(name);
        t.textCursor().insertImage( format );
    }

    t.show();
*/




/*  grab color from fop format ;
    alpha color only on rgb(11,11,11,110)  last


*/


class FopColor
{

public:
    
typedef enum
{  
  DarkColor = 100,
  LightColor = 200,
  Transparent = 300
} AlternateColor;
///static 
FopColor( const QString colorchunk = QString() , FopColor::AlternateColor col = Transparent );
QColor foColor( const QString colorchunk , FopColor::AlternateColor col = DarkColor );
QPixmap createColorMap( const QString colorchunk );
/////  static to retun name human readable ? 
inline QStringList fopListColor() { return fopcolorNames; }
inline QColor color() { return currentcolor; }
inline int size() { return fopcolorNames.count(); }
protected:
   QMap<QString,QColor> foplist;   /* forever clear only construct */
   QMap<QString,QColor> avaiablelist; 
   QStringList fopcolorNames;
private:
    QColor alternateColor( FopColor::AlternateColor col );
   QColor currentcolor;
   void record( const QString colorchunk , QColor item );

};








#endif







