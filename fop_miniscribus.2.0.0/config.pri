
message(Need QT Version 4.4.1)


##### flag from compiler ??? 

DEFINES += OPENGLINITYES
LIB_EXTENSION = a   ### compiler default

win32-msvc*{
LIB_EXTENSION = lib  ### compiler option
DEFINES += VISUALSTUDIOCC_  QT_NO_OPENGL
message(win32 Visual Studio 2005 Compiler setting --------------------------------------------------) 
}

win32-g++{
DEFINES += MINGWCC_
message(win32 MINGW Compiler setting ----------------------------------------------------------------) 
}




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
######INCLUDEPATH += $$BUILD_TREE_PATH/modules/apng
INCLUDEPATH += $$BUILD_TREE_PATH/modules/tidy
INCLUDEPATH += $$BUILD_TREE_PATH/modules/zip
INCLUDEPATH += $$BUILD_TREE_PATH/modules/ooo
INCLUDEPATH += $$BUILD_TREE_PATH/modules/xslt

DEPENDPATH += $$BUILD_TREE_PATH/include
########DEPENDPATH += $$BUILD_TREE_PATH/modules/apng
DEPENDPATH += $$BUILD_TREE_PATH/modules/tidy
DEPENDPATH += $$BUILD_TREE_PATH/modules/zip
DEPENDPATH += $$BUILD_TREE_PATH/modules/ooo
DEPENDPATH += $$BUILD_TREE_PATH/modules/xslt

!include( ./config_xslt.pri ) {
message( "../config_xslt.pri not found" )
}











