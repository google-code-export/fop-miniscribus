#ifndef PDOCCONFIG_H
#define PDOCCONFIG_H


#include <QtGui>
#include <QPair>
#include <QTextDocument>

static const qreal PAGEFAKTOR = 1.4;
static const qreal SCALING_STEEP = 0.05555555555;
static const qreal SLIDERMARGIN_TICK = 20.0;
static const qreal SLIDERSPACER = 2.5;
static const qreal SLIDERCURSORWI = 6.0;
static const qreal PAPERSPACE = 18.0;


static const qreal BorderShadow = 5.2;
static const int DefaultFonzSize = 10;

static const qreal SLIDERMARGIN_TICK_LEFT = SLIDERMARGIN_TICK + SLIDERSPACER;
static const qreal SLIDERMARGIN_TICK_TOTAL = SLIDERMARGIN_TICK + SLIDERSPACER + SLIDERSPACER;


typedef QPair<qreal,qreal> bodyMargins;
void paintWidged( QPainter *p , const QRectF rect , const QTransform trax );
void paintCursor( QPainter *p , const QRectF rect );
void paintShadow( QPainter *p , const QRectF rect );
void paintScale( QPainter *p , const QRectF rect , QPair<qreal,qreal> bodyMargins , const QTransform trax );



#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)


#define _SLIDERBGCOLOR_ \
             QColor("#ebe9ed")
#define _EDITORBGCOLOR_ \
             QColor("#999999")
#define _SHADOWCOLOR_ \
             QColor("#7b6868")





















//
#endif // PDOCCONFIG_H

