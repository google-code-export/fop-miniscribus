TEMPLATE = lib

QT += xml core gui

DESTDIR	= ./

CONFIG += 	staticlib \
		debug \
		warn_on
SOURCES += 	domitem.cpp \
		dommodel.cpp \
 	domitemattributesdoc.cpp
HEADERS += 	domitem.h \
		dommodel.h \
 	domitemattributesdoc.h
INCLUDEPATH += .. \
  ../viewitems \
  ../nodefo \
  ../viewitemsabs \
  ../xsl-fo

