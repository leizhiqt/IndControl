win32 {
#    INCLUDEPATH += $$PWD $$PWD/../../ $$PWD/../../include/emqtt D:/pgsql/include
#    LIBS += -L$$PWD/../../lib/emqtt/x64 -lpaho-mqtt3a -lpaho-mqtt3c

    INCLUDEPATH += $$PWD \
                    $$PWD/../../include/emqtt \
                    e:/pgsql/include
#    LIBS += -L$$PWD/../../lib/emqtt/x64 -lpaho-mqtt3a -lpaho-mqtt3c \
#            -Le:/pgsql/lib -Le:/pgsql/bin -llibpq


    LIBS += -lws2_32
    #message($$INCLUDEPATH)
    #message($$LIBS)

    HEADERS += \
        $$PWD/JSONUtil.h \
#        $$PWD/MQTTUtil.h \
        $$PWD/MyDownloader.h \
        $$PWD/ShellProcess.h \
        $$PWD/IOTUtil.h \
        $$PWD/NetToolHelper.h \
#        $$PWD/PGSQLDriveHelper.h
        $$PWD/FormatTimes.h \
        $$PWD/UWLog.h $$PWD/TcpClientUtil.h

    SOURCES += \
        $$PWD/JSONUtil.cpp \
#        $$PWD/MQTTUtil.cpp \
        $$PWD/MyDownloader.cpp \
        $$PWD/ShellProcess.cpp \
        $$PWD/IOTUtil.cpp \
        $$PWD/NetToolHelper.cpp \
#        $$PWD/PGSQLDriveHelper.cpp
        $$PWD/FormatTimes.cpp \
        $$PWD/UWLog.cpp $$PWD/TcpClientUtil.cpp
}

SOURCES += \
    $$PWD/WTcpSocket.cpp

