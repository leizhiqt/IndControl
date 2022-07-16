win32 {
    INCLUDEPATH += $$PWD
    LIBS +=

    #message($$INCLUDEPATH)
    #message($$LIBS)

    HEADERS += \
    $$PWD/QTcpComponentl.h \
    $$PWD/WTcpClient.h \
    $$PWD/WTcpServer.h \
    $$PWD/WebSocket.h

    SOURCES += \
    $$PWD/QTcpComponentl.cpp \
    $$PWD/WTcpClient.cpp \
    $$PWD/WTcpServer.cpp \
    $$PWD/WebSocket.cpp
}

