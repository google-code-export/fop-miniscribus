TEMPLATE = app
TARGET = xx
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += qt debug warn_off console
DESTDIR	+= ./
win32-msvc*{
CONFIG += embed_manifest_exe
}

QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0) 
QT_VER_MIN = $$member(QT_VERSION, 1) 
QT_VER_PAT = $$member(QT_VERSION, 2) 

lessThan(QT_VER_MAJ, 4) | lessThan(QT_VER_MIN, 5) {

} else {
QT += xmlpatterns
}



SOURCES += main.cpp