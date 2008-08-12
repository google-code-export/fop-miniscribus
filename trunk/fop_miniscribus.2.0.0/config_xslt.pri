

win32 {
    INCLUDEPATH += C:/Programming/libs/include
    DEPENDPATH += C:/Programming/libs/include
    LIBS   += -LC:/Programming/libs/lib/ -lxslt
    LIBS   += -LC:/Programming/libs/lib/ -lxml2
    LIBS   += -LC:/Programming/libs/lib/ -liconv
}


unix {
INCLUDEPATH += /opt/local/include
INCLUDEPATH += /opt/local/include/libxml2
INCLUDEPATH += /opt/local/include/libxslt
INCLUDEPATH += /opt/local/include/libxml2/libxml
LIBS += -lxml2 -lxslt
}

mac {
INCLUDEPATH += /opt/local/include
INCLUDEPATH += /opt/local/include/libxml2
INCLUDEPATH += /opt/local/include/libxslt
INCLUDEPATH += /opt/local/include/libxml2/libxml
LIBS += -lxml2 -lxslt
}