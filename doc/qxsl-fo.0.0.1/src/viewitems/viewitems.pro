TEMPLATE = 	lib

QT += 		xml core gui

CONFIG += 	staticlib \
		debug \
		warn_on
		
		DESTDIR	= ./

SOURCES +=  	\
		fobodyregiondoc.cpp \
		fobodyregionview.cpp \
		foborderregiondoc.cpp \
		foborderregionview.cpp \
 		foblockcontainerdoc.cpp \
  		foblockcontainerview.cpp \
		foexternalgraphicdoc.cpp \
		foexternalgraphicview.cpp \
    		pageview.cpp \
		pagedoc.cpp \
		marginsdoc.cpp\
		marginsview.cpp 

HEADERS += 	\
		fobodyregiondoc.h \
		fobodyregionview.h \
		foborderregiondoc.h \
		foborderregionview.h \
 		foblockcontainerdoc.h \
 		foblockcontainerview.h \
		foexternalgraphicdoc.h \
		foexternalgraphicview.h \
		pageview.h \
		pagedoc.h \
		marginsdoc.h \ 
		marginsview.h

LIBS += ../commands/libcommands.a

TARGETDEPS += ../commands/libcommands.a

INCLUDEPATH +=	.. \
		fotable \
		foblock \
		../nodefo \
		../viewitemsabs \
		../focreator \
		../commands

