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

static bool AllowtoBreack( const QDomElement e )
{
        const qreal founditx = e.attribute("speak-numeral").toDouble();
        if (ApplicationsVersionFopConvert == founditx) {
         return true;
        } else {
         return false;
        }
}
               
               
               
               


#include "Image_Page_Struct_Mime.h" 
#include "Basic_Mime.h"    /* static int space and mesures */
#include "Tools_Basic.h"
#include "XML_Editor.h"



#endif 




