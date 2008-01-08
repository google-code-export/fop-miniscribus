
/* qt zlib dir visual studio MS */
#if defined Q_WS_WIN
#include "../src/3rdparty/zlib/zconf.h"
#endif

#ifndef Q_WS_WIN
#include "zconf.h"
#endif