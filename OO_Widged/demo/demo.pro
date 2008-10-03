######################################################################
# Automatically generated by qmake (2.01a) mar 16. set 18:29:25 2008
######################################################################

TEMPLATE = app
TARGET = Oasis
DEPENDPATH += . ../oasis
INCLUDEPATH += . ../oasis


QT += xml network
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
OBJECTS_DIR = build/.obj
DESTDIR += ../
##########console
CONFIG   +=  qt release warn_off

##########LIBS   += -lz
win32:LIBS	+= -luser32

win32:RC_FILE = win.rc


contains(CONFIG, static): {

       ######### on main.cpp use defined ########
       
       DEFINES += _COMPOSE_STATIC_
        exists($$[QT_INSTALL_PLUGINS]/imageformats/qjpeg.a) {
            QTPLUGIN += qjpeg
            DEFINES += _USE_qjpeg
        }
        exists($$[QT_INSTALL_PLUGINS]/imageformats/libqgif.a) {
            QTPLUGIN += qgif   
            DEFINES += _USE_qgif            
        }
        exists($$[QT_INSTALL_PLUGINS]/imageformats/libqmng.a) {
            QTPLUGIN += qmng   
            DEFINES += _USE_qmng            
        }
        exists($$[QT_INSTALL_PLUGINS]/imageformats/libqtiff.a) {
            QTPLUGIN += qtiff   
            DEFINES += _USE_qtiff            
        }
}




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


