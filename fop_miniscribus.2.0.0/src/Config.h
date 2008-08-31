#ifndef MINISCRIBUS_CONFIG_H
#define MINISCRIBUS_CONFIG_H

#include "xslt_convert.h"
#include <QDomElement>
#include <stdio.h>
#include <math.h>
////////////#define M_PI 3.14159265  <math.h> !!!!
#define _PARSE_DEBUG_FOP_  1
#define _DRAWMODUS_WEB_ 0

////////////////#define _LOGGERON_APPS_  1    /* disble log comment here */



#define FOPIMAGEDIR\
             QString("Pictures/")

#define _MAINICONSIZE_  18

#define _DEFAULT_FONT_POINT_SIZE_  10

#define _IMAGE_PICS_ITEM_ 100

#define _LINK_COLOR_ \
             QColor("#dc0000")
             
             
#define _BOTTOM_VIEW_SPACE_RESERVE_  250

/* signature version to hidden on fop documents */
static const qreal ApplicationsVersionFopConvert = 1.001;
static const int BookMarkInternalID = 23402;


static const qreal HEADERFOOTER_ZINDEX = 0.6666666;
static const qreal STARTEND_ZINDEX = 0.55555555;


static const qreal MINIMUMZINDEXFLOATING= 0.7;
static const qreal MAXIMUMZINDEXFLOATING= 999.1;

static const qreal NEWLAYERZINDEXFLOATING= 1.1;

static const qreal NEWLAYERWI= 350;
static const qreal NEWLAYERHI= 40;


static const qreal MINIMUMJAVAVERSION= 1.6;



#define SCRIBEVERSION  "2.0.0"


#define _APPLICATIONS_NAME_ \
               QString( "MiniScribus Scribe (beta) version %1 ").arg(SCRIBEVERSION) 

#define _NOTITLEONDOC_ \
             QString("MiniScribus ver. %1 - Untitled Document").arg(SCRIBEVERSION)


#define NEWLAYERCOLORSET \
             QColor("turquoise")


#define AbsoluteLayerRemoveHashID  87686

#define _APPSCACHE_ \
             QString("%1/.miniscribe/").arg(QDir::homePath())
             
#define _OOOV1FILE_ \
             QString("%1styleoo11.xsl").arg(_APPSCACHE_)
#define _OOOV2FILE_ \
             QString("%1styleoo24.xsl").arg(_APPSCACHE_)
             
#define _MASTERLINK_ \
             QString("http://code.google.com/p/fop-miniscribus/")    /* real txt palette */



               
#define _BREACKLINECOMMENT_  "Hack to replace br fop Empty paragraph version 1.001. Target to display similar to Apache fop"
         

static const int InterSpace = 15;  /* distance from page 1 to 2 */
static const int SliderTopHeight = 28;
static const int SliderLeftHeight = 28;
static const int MaximumPages = 99;
static const int BorderShadow = 5;

static const qreal FrameMarginBottomBugDraw = 2.8;

static const int DocumentBugSpace = 15;

static const int ObjectName = 853;
static const int FlashReferenceID = 322;


static const int TextFloatObjectName = 20;


static const int FooterHeaderPadding = 2;
static const int FooterHeaderMaxBlocks = 3;

#define _PAGE_NUMERATION_ \
             QString("#Page#")



#define _BG_CHAR_ALPHA_  425
#define _TXT_CHAR_ALPHA_  424


#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)
#define POINT_TO_PI(pi) ((pi)/12)
#define POINT_TO_DD(dd) ((dd)/154.08124)
#define POINT_TO_CC(cc) ((cc)/12.840103)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
#define PI_TO_POINT(pi) ((pi)*12)
#define DD_TO_POINT(dd) ((dd)*154.08124)
#define CC_TO_POINT(cc) ((cc)*12.840103)


typedef enum
{  
  XHTML = 100,   /* normal html tag */
  FOP = 200,    /* XSL-FO tag <br/> tag find hack */
  OOOD = 300    /* import from OpenOffice */
} FileHandlerType;


typedef enum
{  
  FLAT = 1,   /* flat normal qtextdocument */
  PAGES = 2,    /* paginate display pages */
} DisplayModus;








/* correct codex from xml file read only first line */
static inline QString RecoveryBreackLineParagraph()
{
        if (ApplicationsVersionFopConvert == 1.001) {
         return QString("2.001%");
        } else {
         return QString("2%");
        }
}

static inline bool AllowtoBreack( const QDomElement e )
{
        const qreal founditx = e.attribute("speak-numeral").toDouble();
        if (ApplicationsVersionFopConvert == founditx) {
         return true;
        } else {
         return false;
        }
}

static inline QStringList KnowMimeFile()
{
 QStringList support;
 support << "fo" << "fop" << "page" "fop.gz" << "fo.gz" << "sxw" << "stw" << "odt" << "ott" << "htm" << "html"; 
 return support;
}
               
               
static inline QString FileFilterHaving()
{
  QString filter;
  filter = "";
  filter += QString( "FOP file" ) + " (*.fop *.fop.gz *.fo *.fo.gz *.xml);;";
  filter+= QString( "MiniScribus binary stream file" ) + " (*.page);;"; 
  filter+= QString( "OpenOffice 1.1 file format" ) + " (*.sxw *.stw);;"; 
  filter+= QString( "OpenOffice 2.4 file format" ) + " (*.odt *.ott);;"; 
  filter+= QString( "XHTML file format" ) + " (*.htm *.html);;"; 
  //////filter += ")";
  return filter;
}
  
        
               




#endif 




