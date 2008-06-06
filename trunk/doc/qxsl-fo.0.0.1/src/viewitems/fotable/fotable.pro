TEMPLATE = 	lib

QT += 		xml core gui

CONFIG += 	staticlib \
		debug \
		warn_on
    
    DESTDIR	= ./
    
SOURCES +=  \
    fotabledoc.cpp \
    fotableview.cpp \
    fotablecelldoc.cpp \
    fotablerowdoc.cpp \
    fotablebodydoc.cpp \
    fotableheaderdoc.cpp \
    fotablecellview.cpp \
    fotablerowview.cpp

HEADERS +=  \
    fotabledoc.h \
    fotableview.h \
    fotablecelldoc.h \
    fotablerowdoc.h \
    fotablebodydoc.h \
    fotableheaderdoc.h \
    fotablecellview.h \
    fotablerowview.h \
    fotablebodyview.h


INCLUDEPATH += ../.. \
  .. \
  ../../nodefo \
  ../../viewitemsabs \
  ../../focreator

