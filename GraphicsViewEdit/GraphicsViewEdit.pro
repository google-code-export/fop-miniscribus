######################################################################
# Automatically generated by qmake (2.01a) ven 2. mag 19:45:15 2008
######################################################################

TEMPLATE = app
TARGET = Layeredit
DEPENDPATH += . include
INCLUDEPATH += . include

DESTDIR	+= ./


QT += xml
QT += network
QT += svg 

CONFIG   += qt warn_off release
LANGUAGE	= C++

MOC_DIR	= build/.moc
RCC_DIR	= build/.rcc
OBJECTS_DIR = build/.obj

# Input
HEADERS += GraphicsItemEdit.h \
           GraphicsScene.h \
           GraphicsView.h \
           include/DataStructures.h \
           include/getmargin.h \
           include/mimedataeditor.h \
           include/mounttextprivate.h \
           include/pics_structure.h \
           include/richdoc_structure.h
SOURCES += GraphicsItemEdit.cpp \
           GraphicsScene.cpp \
           GraphicsView.cpp \
           main.cpp \
           include/getmargin.cpp \
           include/mimedataeditor.cpp \
           include/mounttextprivate.cpp \
           include/pics_structure.cpp \
           include/richdoc_structure.cpp
RESOURCES += osApplication.qrc
