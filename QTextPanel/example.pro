# #####################################################################
# Automatically generated by qmake (2.01a) lun 28. lug 11:03:47 2008
# #####################################################################
TEMPLATE = app
TARGET = example
DEPENDPATH += .
INCLUDEPATH += .
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
OBJECTS_DIR = build/.obj
win32 { 
    BUILD_PATH = $$PWD
    message(BUILD PATH $$BUILD_PATH)
}
DESTDIR += ./
QT += xml \
    network \
    svg
CONFIG += qt \
    release \
    warn_on \
    console
contains(QT_CONFIG, opengl):QT += opengl
macx:# ########## only on work to view debug output
CONFIG -= app_bundle

#Require at least Qt 4.4.1
QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0) 
QT_VER_MIN = $$member(QT_VERSION, 1) 
QT_VER_PAT = $$member(QT_VERSION, 2) 

lessThan(QT_VER_MAJ, 4) | lessThan(QT_VER_MIN, 4) | lessThan(QT_VER_PAT, 1) {
   error(QTextPanel requires Qt 4.4.1 or newer. Version $$[QT_VERSION] was detected.)
}

# Input
HEADERS += qtextpanel.h \
    panel.h \
    config.h \
    qtextpanelimage.h \
    qtextpanelcontrol.h \
    qtextpanellayercontrol.h \
    qtextpanelmargin.h \
    qtextpanellayer.h \
    qtextpanelmime.h \
    qtextpaneldata.h \
    qtextpaneltable.h
SOURCES += qtextpanel.cpp \
    panel.cpp \
    qtextpanelimage.cpp \
    qtextpanelcontrol.cpp \
    qtextpanellayercontrol.cpp \
    qtextpanellayer.cpp \
    qtextpanelmargin.cpp \
    main.cpp \
    qtextpanelmime.cpp \
    qtextpaneldata.cpp \
    qtextpaneltable.cpp

RESOURCES += example.qrc       
