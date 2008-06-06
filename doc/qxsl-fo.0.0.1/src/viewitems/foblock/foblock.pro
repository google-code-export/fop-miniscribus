TEMPLATE = 	lib

QT += 		xml core gui

CONFIG += 	staticlib \
		debug \
		warn_on

DESTDIR	= ./

SOURCES +=  	\
		foblockdoc.cpp \
 		foblockview.cpp \
 		textcontentdoc.cpp \
 		textcontentview.cpp \
  		rowinblockview.cpp
HEADERS += 	\
		foblockdoc.h \
 		foblockview.h \
 		textcontentdoc.h \
 		textcontentview.h \
 		rowinblockview.h



INCLUDEPATH += ../.. \
  ../fotable \
  .. \
  ../../nodefo \
  ../../viewitemsabs \
  ../xsl-fo \
  ../focreator \
  ../commands \
  fotable

