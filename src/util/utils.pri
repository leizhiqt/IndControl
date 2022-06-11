pgsqlDir = E:\pgsql-32

win32 {
#    INCLUDEPATH += $$PWD $$PWD/../../ $$PWD/../../include/emqtt D:/pgsql/include
#    LIBS += -L$$PWD/../../lib/emqtt/x64 -lpaho-mqtt3a -lpaho-mqtt3c

    INCLUDEPATH += $$PWD \
                   $$pgsqlDir/include

    LIBS += -LE:\IndControl\build\win32\bin -llibpq

    #message($$INCLUDEPATH)
    #message($$LIBS)

    HEADERS += \
        $$PWD/JSONUtil.h \
#        $$PWD/MQTTUtil.h \
        $$PWD/MyDownloader.h \
        $$PWD/ShellProcess.h \
        $$PWD/IOTUtil.h \
        $$PWD/NetToolHelper.h \
        $$PWD/PGSQLDriveHelper.h \
        $$PWD/FormatTimes.h \
        $$PWD/UWLog.h

    SOURCES += \
        $$PWD/JSONUtil.cpp \
#        $$PWD/MQTTUtil.cpp \
        $$PWD/MyDownloader.cpp \
        $$PWD/ShellProcess.cpp \
        $$PWD/IOTUtil.cpp \
        $$PWD/NetToolHelper.cpp \
        $$PWD/PGSQLDriveHelper.cpp \
        $$PWD/FormatTimes.cpp \
        $$PWD/UWLog.cpp
}

