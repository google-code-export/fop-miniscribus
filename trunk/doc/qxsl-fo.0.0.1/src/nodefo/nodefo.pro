TEMPLATE = lib

QT += xml core gui

CONFIG += 	staticlib \
		debug \
		warn_on
    
    DESTDIR	= ./

SOURCES +=  	\
  		attributefo.cpp \
  		attributefodata.cpp \
  		nodefo.cpp 

HEADERS +=  	\
  		attributefo.h \
  		attributefodata.h \
  		nodefo.h 

INCLUDEPATH += 	../viewitems \
		.. \
 		../commands \
 		../focreator \
 		../viewitems/fotable
		
LIBS += ../commands/libcommands.a

TARGETDEPS += ../commands/libcommands.a

