#ifndef MINISCRIBUS_CONFIG_H
#define MINISCRIBUS_CONFIG_H

#include "xslt_convert.h"
#include <QDomElement>
#include <stdio.h>
#include <math.h>
////////////#define M_PI 3.14159265  <math.h> !!!!
#define _PARSE_DEBUG_FOP_  1

#define _DEFAULT_FONT_POINT_SIZE_  10


#define _BOTTOM_VIEW_SPACE_RESERVE_  250

/* signature version to hidden on fop documents */
static const qreal ApplicationsVersionFopConvert = 1.001;


#define _APPLICATIONS_NAME_ \
               QString( "MiniScribus Scribe (markup language)") 


#define _APPLICATIONS_NAME_ \
               QString( "MiniScribus Scribe (markup language)") 
               
#define _BREACKLINECOMMENT_  "Hack to replace br fop Empty paragraph version 1.001. Target to display similar to Apache fop"
               
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
               
               
static inline QString FileFilterHaving()
{
  QString filter;
  filter = "";
  filter += QString( "MiniScribus file" ) + " (*.fop *.fop.gz);;";
  filter += QString( "Apache Fop file" ) + " (*.fo *.fo.gz);;"; 
  filter+= QString( "MiniScribus binary stream file" ) + " (*.page);;"; 
  filter+= QString( "OpenOffice 1.1 file format" ) + " (*.sxw);;"; 
  filter+= QString( "OpenOffice 2.4 file format" ) + " (*.odt);;"; 
  filter+= QString( "XHTML file format" ) + " (*.htm *.html);;"; 
  //////filter += ")";
  return filter;
}
  
        
               


#include "Image_Page_Struct_Mime.h" 
#include "Basic_Mime.h"    /* static int space and mesures */
#include "Tools_Basic.h"
#include "XML_Editor.h"



#endif 




