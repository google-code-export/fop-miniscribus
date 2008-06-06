TEMPLATE = lib

QT += xml core gui

CONFIG += 	staticlib \
		debug \
		warn_on
    
    DESTDIR	= ./

SOURCES +=  	\
  	objectfosequencedview.cpp \
  	objectfosequenceddoc.cpp \
  	objectfosimpledoc.cpp \
	objectfosimpleview.cpp \
	objectfoview.cpp \
	objectfodoc.cpp \
  	objectfocontainerdoc.cpp \
  	objectfocontainersimpledoc.cpp \
  	objectfocontainersimpleview.cpp \
  	objectfocontainerview.cpp \
  	absobjectfosimpledoc.cpp 

HEADERS +=  	\
  	objectfosequenceddoc.h \
  	objectfosequencedview.h \
  	objectfosimpledoc.h \
	objectfosimpleview.h  \
	objectfoview.h \
	objectfodoc.h \
  	objectfocontainerdoc.h \
  	objectfocontainersimpledoc.h \
  	objectfocontainersimpleview.h \
  	objectfocontainerview.h \
  	absobjectfosimpledoc.h

LIBS += ../commands/libcommands.a

TARGETDEPS += ../commands/libcommands.a

INCLUDEPATH += .. \
  ../viewitems/fotable \
  ../viewitems/foblock \
  ../viewitems \
  ../nodefo \
  ../focreator \
  ../commands

