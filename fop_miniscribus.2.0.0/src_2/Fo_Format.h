#ifndef FO_FORMATER_H
#define FO_FORMATER_H

#include "Config.h"
#include <QSvgRenderer>
#include <QTextBlockFormat>
#include <QDomElement>
#include <QTextTableCellFormat>
#include <QSvgRenderer>
#include "ziphandle.h"
#include "SessionManager.h"
#include "Fop_Leader_Element.h"

#include "Image_Page_Struct_Mime.h"


/*
  buffer all resource image normal + svg
  to be resave if need
  discovery page format A4 or else
  using namespace ApacheFop;
*/


extern inline QByteArray  OpenGzipOneFileByte( QString fileName )
{
    QByteArray input;
    gzFile file;
    file = gzopen (fileName.toUtf8().data(), "rb");
    if (!file)
    {
        QMessageBox::critical(0, "Error",QString("Can't open file %1").arg(fileName));
        return input;
    }

    char buffer[1024];
    while (int readBytes =  gzread (file, buffer, 1024))
    {
        input.append(QByteArray(buffer, readBytes));
    }
    gzclose(file);
    return input;
}



extern inline void DotList_Circle( QDomElement e )
{
    QString dot1 = QString("<fo:instream-foreign-object content-width=\"11pt\">"
                           "<svg:svg width=\"20\" height=\"20\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">"
                           "<svg:circle cx=\"10\" cy=\"10\" r=\"5\" stroke=\"red\" stroke-width=\"0.3\" fill=\"black\"/>"
                           "</svg:svg>"
                           "</fo:instream-foreign-object>");

    QDomDocument dot;
    dot.setContent(dot1,false);
    QDomElement dummyss = e.ownerDocument().createElement("fo:instream-foreign-object");
    QDomNamedNodeMap attlist = dot.documentElement().attributes();

    for (int i=0; i<attlist.count(); i++)
    {
        QDomNode nod = attlist.item(i);
        dummyss.setAttribute (nod.nodeName().toLower(),nod.nodeValue());
    }

    e.appendChild( dummyss );
    QDomNode child = dot.documentElement().firstChild();
    while ( !child.isNull() )
    {
        if ( child.isElement() )
        {
            dummyss.appendChild(e.ownerDocument().importNode(child,true).toElement());
        }
        child = child.nextSibling();
    }
}

extern inline QString capitalize( QString & s)
{
    return (s.isEmpty()) ? s : s.left(1).toUpper() + s.mid(1).toLower();
}


extern inline QString  foptrimmed( QString txt , const QString txttransform = QString("none") )
{


    txt.replace("&amp;", "&");   /*  qt4 toUtf8 dont replace && */
    /////txt.replace("\"","&quot;");
    ///////txt.replace("'", "&apos;");
    txt.replace("&lt;", "<");
    txt.replace("&gt;", ">");


    if (txttransform == QString("uppercase"))
    {
        txt = txt.toUpper();
    }
    else if (txttransform == QString("capitalize"))
    {
        txt = capitalize(txt);
    }
    else if (txttransform == QString("lowercase"))
    {
        txt = txt.toLower();
    }

    const int cosize = txt.size() - 1;

    bool lastspace = false;
    bool firstspace = false;
    if (txt.size() >= 1 )
    {
        if (txt.at(0) == QChar::Null || txt.at(0) == QChar(' ') | txt.at(0) ==  QChar::Nbsp )
        {
            firstspace = true;
        }
        if (txt.at(cosize) == QChar::Null || txt.at(cosize) == QChar(' ') | txt.at(cosize) ==  QChar::Nbsp )
        {
            lastspace  = true;
        }



        QString nwex = txt.simplified();
        if (firstspace)
        {
            nwex.prepend(QChar::Nbsp);  /////// QChar::Nbsp
        }
        if (lastspace)
        {
            nwex.append(QChar::Nbsp);  /////// QChar::Nbsp
        }

        return nwex;
    }
    else
    {
        return txt;
    }
}




/* set all margin to zero qt4 send 12 top 12 bottom by default */
static inline  QTextBlockFormat DefaultMargin( QTextBlockFormat rootformats = QTextBlockFormat() )
{
    rootformats.setBottomMargin(1);
    rootformats.setTopMargin(1);
    rootformats.setRightMargin(0);
    rootformats.setLeftMargin(0);
    rootformats.setAlignment( Qt::AlignLeft );
    return rootformats;
}

static inline  QTextCharFormat DefaultCharFormats( QTextCharFormat format = QTextCharFormat() )
{
    QFont userfont( QApplication::font() );
    userfont.setPointSize(_DEFAULT_FONT_POINT_SIZE_);
    format.setFont(userfont);
    format.setForeground(QBrush(Qt::black));
    return format;
}

static inline  QTextCharFormat PreFormatChar( QTextCharFormat format = QTextCharFormat() )
{
    format.setFont(QFont( "courier",_DEFAULT_FONT_POINT_SIZE_,-1,true) );
    format.setForeground(QBrush(Qt::darkBlue));
    format.setFontLetterSpacing(90);
    format.setBackground(QBrush(Qt::lightGray));
    return format;
}




static inline QPixmap RenderPixmapFromSvgByteFo(  QByteArray streams )
{
    QSvgRenderer  svgRenderer( streams );
    QPixmap pix(  svgRenderer.defaultSize() );
    pix.fill(Qt::transparent);
    QPainter paint(&pix);
    svgRenderer.render(&paint);
    return pix;
}



static inline  QTextCharFormat AlertCharFormats( QTextCharFormat format = QTextCharFormat() )
{
    QFont userfont( QApplication::font() );
    userfont.setPointSize(22);
    format.setFont(userfont);
    format.setFontOverline ( true );
    format.setForeground(QBrush(QColor(Qt::red)));
    format.setUnderlineStyle ( QTextCharFormat::SingleUnderline );
    return format;
}

static inline QString ImagesrcUrl( const QDomElement e )
{
    QString hrefimageplace = e.attribute( "src" ,"");

    if (hrefimageplace.startsWith("url(",Qt::CaseInsensitive))
    {
        hrefimageplace = hrefimageplace.right(hrefimageplace.size() - 4);
        hrefimageplace = hrefimageplace.left(hrefimageplace.size() - 1).trimmed();
    }
    if (hrefimageplace.startsWith("file:",Qt::CaseInsensitive))
    {
        return hrefimageplace.mid(5);///////// =  hrefimageplace.replace("file:","");
    }
    return hrefimageplace;
}







namespace ApacheFop
{

typedef enum
{
    TAG_NOT_KNOW = 404,
    PAGE_SETUP, /* fo:simple-page-master   */
    BLOCK_TAG,    /* fo:block  */
    TABLE_TAG,  /* fo:table */
    TABLE_FOOTER, /* table-footer  */
    TABLE_HEADER, /* table-header */
    TABLE_BODY, /* fo:table-body */
    TABLE_COL, /* fo:table-column */
    TABLE_ROW,   /* fo:table-row */
    TABLE_CELL, /* fo:table-cell */
    BLOCK_CONTAINER,  /* fo:block-container   floating layer */
    INLINE_STYLE,  /* fo:inline */
    INLINE_CONTAINER,  /* fo:inline-container */
    LINK_DOC,  /* fo:basic-link */
    IMAGE_SRC, /* fo:external-graphic   all supported qt4.4 image + tif */
    IMAGE_INLINE,  /* fo:instream-foreign-object  svg inline */
    LIST_BLOCK,   /* fo:list-block  ul/ol */
    LIST_ITEM,   /* fo:list-item  li */
    LIST_BODY,   /* fo:list-item-body */
    LIST_LABEL,   /* fo:list-item-label  */
    FIRST_LAST_TAG,      /* fo:root */
    INLINE_BR,     /* fo:br provisional line breack*/
    FOCHAR,      /* fo:character  */
    FOLEADER,      /* fo:leader  inline space  */
    FOOTNOTEBOTTOM,            /* fo:footnote - fo:footnote-body */
    FOPAGENRCITATION,        /*  fo:page-number-citation*/
    FLOATBLOCK   /* fo:float */
} FOPTAGSINT;

typedef enum
{
    DIV,
    TABLE,
    IMAGE,
    FONT,
    UNDEFINE
} UNITELEMENT;





class Fo_Format : public QObject
{
    Q_OBJECT


public:
    Fo_Format( QObject *parent = 0 );
    FOPTAGSINT FoTag( const QDomElement e );
    qreal Unit( const QString datain , UNITELEMENT element = DIV );  /* distance document */
    /* dom 2 qt */
    QTextBlockFormat TextBlockFormFromDom( const QDomElement e , QTextBlockFormat pf  );
    QTextListFormat TextListFromFromDom(const QDomElement e);
    QTextFrameFormat::BorderStyle StyleBorder( const QString Sborder );
    QTextFrameFormat::BorderStyle StyleBorder( const QDomElement e );
    QTextCharFormat GlobalCharFormat( const QDomElement e , QTextCharFormat f = QTextCharFormat() );
    QBrush GetFrameBorderColor( const QDomElement e );
    QTextFrameFormat PaintFrameFormat( const QDomElement e , QTextFrameFormat format );
    QColor ColorFromFoString( QString focolor );
    qreal TakeOnePadding( const QDomElement e );
    void FindMargin( const QDomElement e );
    bool IsAbsoluteLayer( const QDomElement e );
    QPixmap RenderSvg(  const QDomElement e  ,  const QString nameresource );
    Qt::Alignment TagAlignElement(const QDomElement e );
    qreal TakeOneMargin( const QDomElement e , bool largespace = true );
    qreal isBorder( const QDomElement e );
    QTextLength BlockMesure( const QDomElement e );
    QRect BlockRect( const QDomElement e , qreal largespace = 0.); /* on td table cell && frame !! */
    qreal TrLineMaxHight( const QDomElement e , const QVector<QTextLength> constraints );
    qreal ColumnLarge( const QVector<QTextLength> constraints , int atcool );
    QDomDocument DomelenentToString ( const QDomElement e , const QString msg );
    
    QStringList attributeList(const QDomElement e);
    QRectF MarginPage;

    /* back qt 2 dom */
    QString BorderStyleCss(QTextFrameFormat::BorderStyle style);
protected:

private:
    QMap<int, QString> ErnoMap;
    int LoopChar;
    int LoopBlock;
    int ErrorSumm;
};


}
//
#endif // FO_READER_H

