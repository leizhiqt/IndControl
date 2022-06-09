win32 {
    INCLUDEPATH += $$PWD
    LIBS += -lws2_32

    #message($$INCLUDEPATH)
    #message($$LIBS)

    HEADERS += \
    $$PWD/QTcpComponentl.h \
    $$PWD/WTcpClient.h \
    $$PWD/WTcpServer.h

    SOURCES += \
    $$PWD/QTcpComponentl.cpp \
    $$PWD/WTcpClient.cpp \
    $$PWD/WTcpServer.cpp
}

