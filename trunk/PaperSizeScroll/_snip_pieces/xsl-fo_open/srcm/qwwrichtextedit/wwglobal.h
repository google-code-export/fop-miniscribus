//
// C++ Interface: wwglobal
//
// Description:
//
//
// Author: Witold Wysota <wysota@wysota.eu.org>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __WW_GLOBAL_H
#define __WW_GLOBAL_H

#include <QString>
#include <QColor>
#include "qwwcolorbutton.h"
#include "colormodel.h"

struct QEffects {
    enum Direction {
        LeftScroll  = 0x0001,
        RightScroll = 0x0002,
        UpScroll    = 0x0004,
        DownScroll  = 0x0008
    };

    typedef uint DirFlags;
};



#endif
