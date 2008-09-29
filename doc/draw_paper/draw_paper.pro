TEMPLATE = app
TARGET = xx
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += qt debug warn_off console
DESTDIR	+= ./
win32-msvc*{
CONFIG += embed_manifest_exe
}
SOURCES += main.cpp