win32 {
    INCLUDEPATH += $$PWD
    LIBS +=

    #message($$INCLUDEPATH)
    #message($$LIBS)

    HEADERS += \
    $$PWD/ProtocolXly.h \
    $$PWD/ProtocolWebJson.h

    SOURCES += \
    $$PWD/ProtocolXly.cpp \
    $$PWD/ProtocolWebJson.cpp
}

HEADERS += \
    $$PWD/ModBusTcp.h

SOURCES += \
    $$PWD/ModBusTcp.cpp
