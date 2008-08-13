#ifndef MINISCRIBUS_CONFIG_H
#define MINISCRIBUS_CONFIG_H


#include <stdio.h>
#include <math.h>
////////////#define M_PI 3.14159265  <math.h> !!!!
#define _PARSE_DEBUG_FOP_  1

#define _DEFAULT_FONT_POINT_SIZE_  10


#define _BOTTOM_VIEW_SPACE_RESERVE_  250

#define _APPLICATIONS_NAME_ \
               QString( "MiniScribus Scribe (markup language)") 


#include "xslt_convert.h"
#include "Image_Page_Struct_Mime.h" 
#include "Basic_Mime.h"    /* static int space and mesures */
#include "Tools_Basic.h"
#include "XML_Editor.h"



#endif 




