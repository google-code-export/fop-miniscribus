#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>
#include <QDebug>
#include <math.h>


#define _DRAWMODUS_WEB_ 0
#define FOPIMAGEDIR "Pictures/"
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
#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())    /* gs tmp */
#define _DUMMY_SPACE_DOCUMENT_ \
             QString("dummy_space_please")
#define _LINK_COLOR_ \
             QColor("#dc0000")
#define _INTERNAL_LINK_COLOR_ \
             QColor(Qt::darkMagenta)
#define _DEFAULT_TXT_COLOR_ \
             QColor("#000088")   /* cursor color palette from text now is cursor color */
#define _DOCUMENT_TXT_COLOR_ \
             QColor("#717171")    /* real txt palette */
#define _MASTERLINK_ \
             QString("http://code.google.com/p/fop-miniscribus/")    /* real txt palette */
/* set your monitor to 72 DPI on go to */
/* set your monitor to 92 DPI on go to */
/*      http://www.unitconversion.org/unit_converter/typography.html            */

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














//
#endif // CONFIG_H

