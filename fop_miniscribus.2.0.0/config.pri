


#Require at least Qt 4.4.1
QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0) 
QT_VER_MIN = $$member(QT_VERSION, 1) 
QT_VER_PAT = $$member(QT_VERSION, 2) 



lessThan(QT_VER_MAJ, 4) | lessThan(QT_VER_MIN, 4) {
    error(MiniScribus requires Qt 4.4.0 or newer. Version $$[QT_VERSION] was detected. optimum 4.5)
} else {
   message(..ok qt version..)
   message(Version $$[QT_VERSION] was detected)
   message(..ok qt version..)
}


DEFINES += OPENGLINITYES
LIB_EXTENSION = a

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
DEPENDPATH += $$BUILD_TREE_PATH/include

lessThan(QT_VER_MAJ, 4) | lessThan(QT_VER_MIN, 5) {
INCLUDEPATH += $$BUILD_TREE_PATH/modules/tidy
INCLUDEPATH += $$BUILD_TREE_PATH/modules/zip
INCLUDEPATH += $$BUILD_TREE_PATH/modules/ooo
INCLUDEPATH += $$BUILD_TREE_PATH/modules/xslt
DEPENDPATH += $$BUILD_TREE_PATH/modules/tidy
DEPENDPATH += $$BUILD_TREE_PATH/modules/zip
DEPENDPATH += $$BUILD_TREE_PATH/modules/ooo
DEPENDPATH += $$BUILD_TREE_PATH/modules/xslt
} else {
INCLUDEPATH += $$BUILD_TREE_PATH/modules/tidy
DEPENDPATH += $$BUILD_TREE_PATH/modules/tidy
}






!include( ./config_xslt.pri ) {
message( "../config_xslt.pri not found" )
}











