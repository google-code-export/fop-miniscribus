
CONFIG += libxslt libxml2 libiconv
### libexiv2

win32 {

############### Mingw Compiler ###########################################
##################Define Path from external libs #########################
LIB_EXTERN_PATH = C:/Programming/libs


message(MINGW LIB_EXTENSION ->  libname.$$LIB_EXTENSION)

INCLUDEPATH += $$LIB_EXTERN_PATH/include
DEPENDPATH += $$LIB_EXTERN_PATH/include
    
  contains(CONFIG, libxslt) {
    INCLUDEPATH += $$LIB_EXTERN_PATH/libxslt
    LIBS += $$LIB_EXTERN_PATH/lib/libxslt.$$LIB_EXTENSION
    TARGETDEPS += $$LIB_EXTERN_PATH/lib/libxslt.$$LIB_EXTENSION
  }
  contains(CONFIG, libxml2) {
    INCLUDEPATH += $$LIB_EXTERN_PATH/include/libxml2
    LIBS += $$LIB_EXTERN_PATH/lib/libxml2.$$LIB_EXTENSION
    TARGETDEPS += $$LIB_EXTERN_PATH/lib/libxml2.$$LIB_EXTENSION
  }
  contains(CONFIG, libiconv) {
    INCLUDEPATH += $$LIB_EXTERN_PATH/include
    LIBS += $$LIB_EXTERN_PATH/lib/libiconv.$$LIB_EXTENSION
    TARGETDEPS += $$LIB_EXTERN_PATH/lib/libiconv.$$LIB_EXTENSION
  }
  contains(CONFIG, libexiv2) {
    INCLUDEPATH += $$LIB_EXTERN_PATH/include/exiv2
    LIBS += $$LIB_EXTERN_PATH/lib/libexiv2.$$LIB_EXTENSION
    TARGETDEPS += $$LIB_EXTERN_PATH/lib/libexiv2.$$LIB_EXTENSION
  }
  LIBS += -lws2_32  
}


unix {
INCLUDEPATH += /usr/include/libxslt
INCLUDEPATH += /usr/include/libxml2/libxml
INCLUDEPATH += /usr/include/libxml2
LIBS += -lxml2 -lxslt
}




mac {
INCLUDEPATH += /opt/local/include
INCLUDEPATH += /opt/local/include/libxml2
INCLUDEPATH += /opt/local/include/libxslt
INCLUDEPATH += /opt/local/include/libxml2/libxml
LIBS += -lxml2 -lxslt
}









