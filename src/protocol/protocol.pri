win32 {
    INCLUDEPATH += $$PWD
    LIBS +=

    #message($$INCLUDEPATH)
    #message($$LIBS)

    HEADERS += \
    $$PWD/ProtocolXly.h

    SOURCES += \
    $$PWD/ProtocolXly.cpp
}
