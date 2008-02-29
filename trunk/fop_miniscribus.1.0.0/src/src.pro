######################################################################
# Automatically generated by qmake (2.01a) dom 7. ott 20:47:33 2007
######################################################################

TEMPLATE = app
TARGET = MiniScribus
DEPENDPATH += . floating_box ui
INCLUDEPATH += . floating_box ui

##########TARGET = xx

DESTDIR	+= ../

win32:RC_FILE = win.rc
######CONFIG += console
CONFIG   += qt warn_off release


macx {
RC_FILE = zzz.icns
QTPLUGIN     += qmng qjpeg qgif qtiff
QMAKE_INFO_PLIST = Info.plist
QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
CONFIG+=x86 ppc
}


INCLUDEPATH +=  ../fop_lib ../fop_lib/zip
DEPENDPATH += ../fop_lib ../fop_lib/zip



LIBS   += -L../all_os_libs/ -lfop


QT += xml
QT += network
QT += svg 

MOC_DIR	= build/.moc
RCC_DIR	= build/.rcc
OBJECTS_DIR = build/.obj
#### bug on window 
unix:UI_DIR	= ui
macx:UI_DIR	= ui

unix {
TARGET = fopedit
BINDIR = /usr/bin
target.path = $$BINDIR
INSTALLS += target
}




# Input
HEADERS += fapanel.h \
           gui_about.h \
           main.h \
           qvimedit.h \
           getmargin.h \
           floating_box/floatdiagram.h \
           floating_box/graphicsscene.h \
           floating_box/wymsingtxt.h
FORMS += ui/gmain.ui ui/wymtext.ui ui/getmargin.ui
SOURCES += fapanel.cpp \
           main.cpp \
           qvimedit.cpp \
           getmargin.cpp \
           floating_box/floatdiagram.cpp \
           floating_box/graphicsscene.cpp \
           floating_box/wymsingtxt.cpp
RESOURCES += osApplication.qrc
