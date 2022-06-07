win32 {
    INCLUDEPATH += $$PWD
    LIBS +=

    #message($$INCLUDEPATH)
    #message($$LIBS)

    HEADERS += \
    $$PWD/QTcpComponentl.h \
    $$PWD/WTcpSocket.h

    SOURCES += \
    $$PWD/QTcpComponentl.cpp \
        $$PWD/WTcpSocket.cpp
}

