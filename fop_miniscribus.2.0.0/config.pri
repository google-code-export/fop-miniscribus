
message(Need QT Version 4.4.1)


##### flag from compiler ??? 

DEFINES += OPENGLINITYES
LIB_EXTENSION = a   ### compiler default



win32-msvc {
LIB_EXTENSION = lib  ### compiler option
DEFINES += VISUALSTUDIOCC_
error(Visual studio xslt lib not setting on pro file) 
}

win32-g++{
DEFINES += MINGWCC_
###############error(xslt lib not setting to mingw compiler on pro file) 
}


CONFIG += libxslt libxml2 libiconv

win32 {
    ########## window no pwd  #############
    BUILD_TREE_PATH = $$PWD
    message(Window path  $$BUILD_TREE_PATH)
}


unix {
    BUILD_TREE_PATH = $$PWD
    message(Unix path  $$BUILD_TREE_PATH)
}

macx {
    BUILD_TREE_PATH = $$PWD
    message(Mac path $$BUILD_TREE_PATH)
}

LIB_TREE_PATH = $$BUILD_TREE_PATH/lib
message(Libs install dir  $$LIB_TREE_PATH)

INCLUDEPATH += $$BUILD_TREE_PATH/include
INCLUDEPATH += $$BUILD_TREE_PATH/modules/apng
INCLUDEPATH += $$BUILD_TREE_PATH/modules/tidy
INCLUDEPATH += $$BUILD_TREE_PATH/modules/zip
INCLUDEPATH += $$BUILD_TREE_PATH/modules/ooo
INCLUDEPATH += $$BUILD_TREE_PATH/modules/xslt

DEPENDPATH += $$BUILD_TREE_PATH/include
DEPENDPATH += $$BUILD_TREE_PATH/modules/apng
DEPENDPATH += $$BUILD_TREE_PATH/modules/tidy
DEPENDPATH += $$BUILD_TREE_PATH/modules/zip
DEPENDPATH += $$BUILD_TREE_PATH/modules/ooo
DEPENDPATH += $$BUILD_TREE_PATH/modules/xslt


!include( ./config_xslt.pri ) {
message( "../config_xslt.pri not found" )
}











