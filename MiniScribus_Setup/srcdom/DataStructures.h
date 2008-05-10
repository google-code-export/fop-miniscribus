#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QtGui>
#include <QtCore>
#include <QToolButton>

#include <stdlib.h>
#if defined Q_WS_WIN
#include <windows.h>
#include <ddeml.h>
#endif
#include <QtCore>
#include <QSettings>
#include <QApplication>

#include <QPixmap>

#define _DRAWMODUS_WEB_ 1
#define _IMAGE_PICS_ITEM_ 100


#define _GSCACHE_ \
             QString("%1/.ghosti_CACHE/").arg(QDir::homePath())    /* gs tmp */
             
#include "xmlhighlighter.h"
#include "richdoc_structure.h"
#include "pics_structure.h"
#include "swisslang_structure.h"
#include "ModelSwap.h"
#include "metric_convert.h"
#include "GhostFunction.h"

#include <QSvgRenderer>


static inline QPixmap RenderPixmapFromSvgByte(  QByteArray streams ) 
{
        QSvgRenderer  svgRenderer( streams );
        QPixmap pix(  svgRenderer.defaultSize() );
        pix.fill(Qt::transparent);
        QPainter paint(&pix);
        svgRenderer.render(&paint);
        return pix;
}



static inline qreal Metric( const QString datain )
{
  return FopInt(datain);
}
static inline qreal ToUnit(qreal unit , const QString unita )
{
  return Pointo(unit,unita);
}

/* write a file to utf-8 format */
static inline bool fwriteutf8(QString fullFileName,QString xml)
{
    if (fullFileName.contains("/", Qt::CaseInsensitive)) {
    QString ultimacartellaaperta = fullFileName.left(fullFileName.lastIndexOf("/"))+"/";
    QDir dira(ultimacartellaaperta);
    if ( dira.mkpath(ultimacartellaaperta) ) { } else {
    return false;
    }
    }
    
        QTextCodec *codecx;
        codecx = QTextCodec::codecForMib(106);
        QFile f( fullFileName );
        if ( f.open( QFile::WriteOnly | QFile::Text ) )
        {
        QTextStream sw( &f );
        sw.setCodec(codecx);
        sw << xml;
        f.close();
        return true;
        }
        return false;
}







static inline QPixmap BruschChess( qreal chesswidth )  
{
    QPixmap m_tile = QPixmap(chesswidth,chesswidth);
    qreal piece = chesswidth / 2;
    m_tile.fill(Qt::white);
    QPainter pt(&m_tile);
    QColor color(230, 230, 230);
    pt.fillRect(0, 0, piece, piece, color);
    pt.fillRect(piece, piece, piece, piece, color);
    pt.end();
    return m_tile;
}











////////ToUnit





















#endif // DATASTRUCTURES_H
