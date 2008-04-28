#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QtGui>
#include <QtCore>

#include "pics_structure.h"
#include "richdoc_structure.h"


#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_PX(px) ((px)*1.3333333)   /* error dpi problem */
#define POINT_TO_MM(mm) ((mm)/2.83465058)
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)
#define POINT_TO_PI(pi) ((pi)/12)
#define POINT_TO_DD(dd) ((dd)/154.08124)
#define POINT_TO_CC(cc) ((cc)/12.840103)

#define PX_TO_POINT(px) ((px)/1.3333333)  /* error dpi problem */
#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
#define PI_TO_POINT(pi) ((pi)*12)
#define DD_TO_POINT(dd) ((dd)*154.08124)
#define CC_TO_POINT(cc) ((cc)*12.840103)


static inline qreal FopInt( const QString datain )
{
  QString ctmp = datain;
  const QString data = ctmp.replace(" ","").trimmed();
  //////////qDebug() << "### request unit data->" << datain << " size->" << datain.size();   
  qreal points = 0;
  if (data.size() < 1) {
  return points;
  }
  if (datain == "0") {
  return points;
  }
  if ( data.endsWith( "pt" ))  {
    points = data.left( data.length() - 2 ).toDouble();
  }  else if ( data.endsWith( "px" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = PX_TO_POINT( value );
  }  else if ( data.endsWith( "cm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = CM_TO_POINT( value );
  } else if ( data.endsWith( "em" ) ) {
    points = data.left( data.length() - 2 ).toDouble();
  } else if ( data.endsWith( "mm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = MM_TO_POINT( value );
  } else if ( data.endsWith( "dm" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = DM_TO_POINT( value );
  } else if ( data.endsWith( "in" ) ) {
    double value = data.left( data.length() - 2 ).toDouble();
    points = INCH_TO_POINT( value );
  } else if ( data.endsWith( "inch" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = INCH_TO_POINT( value );
  } else if ( data.endsWith( "pi" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = PI_TO_POINT( value );
  } else if ( data.endsWith( "dd" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = DD_TO_POINT( value );
  } else if ( data.endsWith( "cc" ) ) {
    double value = data.left( data.length() - 4 ).toDouble();
    points = CC_TO_POINT( value );
  } else {
    points = 0;
  }
  return points;
} 


static inline qreal Pointo( qreal unit , const QString unita )  {
    qreal ri = 0;
    if (unita == "cm") {
    ri = POINT_TO_CM( unit );
    } else if (unita == "pt") {
    ri = unit;
    } else if (unita == "mm") {
    ri = POINT_TO_MM( unit );
    } else if (unita == "px") {
    ri = POINT_TO_PX( unit );
    } else if (unita == "dm") {
    ri = POINT_TO_DM( unit );
    } else if (unita == "inch") {
    ri = POINT_TO_INCH( unit );
    } else if (unita == "pi") {
    ri = POINT_TO_PI( unit );
    } else if (unita == "dd") {
    ri = POINT_TO_DD( unit );
    }  else if (unita == "cc") {
    ri = POINT_TO_CC( unit );
    } else {
    ri = 10;
    }
    return ri;
}


static inline qreal ToPoint( qreal unit , const QString unita )  {
    qreal ri = 0;
    if (unita == "cm") {
    ri = CM_TO_POINT( unit );
    } else if (unita == "pt") {
    ri = unit;
    } else if (unita == "mm") {
    ri = MM_TO_POINT( unit );
    } else if (unita == "px") {
    ri = PX_TO_POINT( unit );
    } else if (unita == "dm") {
    ri = DM_TO_POINT( unit );
    } else if (unita == "inch") {
    ri = INCH_TO_POINT( unit );
    } else if (unita == "pi") {
    ri = PI_TO_POINT( unit );
    } else if (unita == "dd") {
    ri = DD_TO_POINT( unit );
    }  else if (unita == "cc") {
    ri = CC_TO_POINT( unit );
    } else {
    ri = 10;
    }
    return ri;
}








#endif // DATASTRUCTURES_H
