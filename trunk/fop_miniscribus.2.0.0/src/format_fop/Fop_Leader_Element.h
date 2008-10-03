#ifndef FOP_LEADER_H
#define FOP_LEADER_H

#include <QtGui>
#include <QtCore>
#include <QPixmap>
#include <QSvgRenderer>
#include <QTextDocumentFragment>
#include <QTextCharFormat>
#include <QAbstractTextDocumentLayout> // for QTextObjectInterface
#include <QDomDocument>
#include "Config.h"
#include "BasicFoConfig.h"



static const int LeaderNummer = 854;
static const int FootNoteNummer = 880;

class FopLeader
{
public:
    enum { MAGICNUMBER = 0xFFAAFFAA, VERSION = 854 };
    FopLeader();
    void read( const QDomElement e , qreal pagewidth );
    QByteArray SaveElement( const QDomElement e ); /* save original element */
    void RestoreOn( QDomElement appender );
    QColor ColorFromFoString( QString focolor );
    inline operator QVariant() const {
    return QVariant::fromValue(*this);
    }
    QString Name() const;
    QPixmap DummyImage();
    inline FopLeader& operator=( const FopLeader& d )
    {
      bg = d.bg;
      area = d.area;
      ELoriginal = d.ELoriginal;
      visibility = d.visibility;
      leaderpattern = d.leaderpattern;
			format = d.format;
      return *this;
    }
    inline bool makeimage()   /* question hideen true false */
    {
        if (visibility == 1) {
        return true;
        } else {
        return false;
        }
    }

QStringList display;
QStringList pattern;
QString name;
/* save param */    
QRectF area;
int visibility; ////  	visible | hidden | collapse | inherit
int leaderpattern;  ////// leader-pattern =   space | rule | dots | use-content | inherit
QColor bg;
QColor co;
QByteArray ELoriginal;
QTextFrameFormat format; 
};

Q_DECLARE_METATYPE(FopLeader);






     








#endif // FOP_LEADER_H

