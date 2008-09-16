######################################################################
# Automatically generated by qmake (2.01a) mar 16. set 18:29:25 2008
######################################################################

TEMPLATE = app
TARGET = OasisDemo
DEPENDPATH += . ../oasis
INCLUDEPATH += . ../oasis


QT += xml network
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
OBJECTS_DIR = build/.obj
DESTDIR += ../

CONFIG   +=  qt release warn_off console

LIBS   += -lz
win32:LIBS	+= -luser32

win32:RC_FILE = win.rc

# Input
HEADERS += textedit.h \
           ../oasis/FoColorName.h \
           ../oasis/GZipReader.h \
           ../oasis/GZipWriter.h \
           ../oasis/OOFormat.h \
           ../oasis/OOReader.h \
           ../oasis/XML_Editor.h
SOURCES += main.cpp \
           textedit.cpp \
           ../oasis/FoColorName.cpp \
           ../oasis/GZip.cpp \
           ../oasis/OOFormat.cpp \
           ../oasis/OOReader.cpp \
           ../oasis/XML_Editor.cpp
RESOURCES += textedit.qrc

