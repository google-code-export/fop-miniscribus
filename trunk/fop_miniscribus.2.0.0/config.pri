
message(Need QT Version 4.4.1)


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

DEPENDPATH += $$BUILD_TREE_PATH/include
DEPENDPATH += $$BUILD_TREE_PATH/modules/apng
DEPENDPATH += $$BUILD_TREE_PATH/modules/tidy
DEPENDPATH += $$BUILD_TREE_PATH/modules/zip
DEPENDPATH += $$BUILD_TREE_PATH/modules/ooo


!include( ./config_xslt.pri ) {
message( "../config_xslt.pri not found" )
}











