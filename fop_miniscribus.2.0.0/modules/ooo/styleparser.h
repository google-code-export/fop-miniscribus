/***************************************************************************
 *   Copyright (C) 2006 by Tobias Koenig <tokoe@kde.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
 
 

#ifndef OOO_STYLEPARSER_H
#define OOO_STYLEPARSER_H

#include "formatproperty.h"
#include "oo_document.h"
#include <QLocale>
#include <QPixmap>   /* find dpi actual to faktor 72 */

/* 
  default translate units to 72 DPI find difference 
  faktor to real DPI !
  #define INCH_TO_POINT(inch) ((inch)*72.0)  
  #include <QPixmap>
 */
 
/* fix from Peter Hohl to make image running on dpi unit! */
static inline qreal DPIactualDiff()
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    int hi = qMax(pixmap.logicalDpiY(),72);
    int mi = qMin(pixmap.logicalDpiY(),72);
    if (hi == mi) {
    return 1; /* no difference increase or degrease 1:1 running 72 DPI  */
    }
    const qreal precision = 0.09741;   /* gimp precision to image */
    ////////return 1;   /* 1:1 to test */
    int diff = hi - mi;
    qreal increments = 1.;
    if (mi == 72) {  //////  more >>>
    return increments + (diff / 100.0) + precision;
    } else {   ///////////// down <<<
    return increments - (diff / 100.0) + precision;
    }
}
#define PDIFIX(ii) ((ii)*DPIactualDiff())


class QDomDocument;
class QDomElement;

namespace OOO {

class Document;
class StyleInformation;

class StyleParser
{
  public:
    StyleParser( const Document *document, const QDomDocument &domDocument, StyleInformation *styleInformation );

    bool parse();

    static double convertUnit( const QString& );

  private:
    bool parseContentFile();
    bool parseStyleFile();
    bool parseMetaFile();

    bool parseDocumentCommonAttrs( QDomElement& );
    bool parseFontFaceDecls( QDomElement& );
    bool parseStyles( QDomElement& );
    bool parseMasterStyles( QDomElement& );
    bool parseAutomaticStyles( QDomElement& );

    StyleFormatProperty parseStyleProperty( QDomElement& );
    ParagraphFormatProperty parseParagraphProperty( QDomElement& );
    TableColumnFormatProperty parseTableColumnProperty( QDomElement& );
    TableCellFormatProperty parseTableCellProperty( QDomElement& );
    TextFormatProperty parseTextProperty( QDomElement& );
    PageFormatProperty parsePageProperty( QDomElement& );
    ListFormatProperty parseListProperty( QDomElement& );
    const qreal LogicalDPI_Y;
    const Document *mDocument;
    const QDomDocument &mDomDocument;
    StyleInformation *mStyleInformation;
    bool mMasterPageNameSet;
};

}

#endif
