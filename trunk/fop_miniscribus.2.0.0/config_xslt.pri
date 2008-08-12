

win32 {
    INCLUDEPATH += C:/Programming/libs/include
    DEPENDPATH += C:/Programming/libs/include
    LIBS   += -LC:/Programming/libs/lib/ -lxslt
    LIBS   += -LC:/Programming/libs/lib/ -lxml2
    LIBS   += -LC:/Programming/libs/lib/ -liconv
}

unix {
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include
    LIBS += -lxslt -lxml2 -liconv

}

macx {
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include
    LIBS += -lxslt -lxml2 -liconv
}