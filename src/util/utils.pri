win32 {

INCLUDEPATH += $$PWD \
                   $$PWD/../../3rd/$$TARGET_ARCH/emqtt/include \
                   $$PWD/../../3rd/$$TARGET_ARCH/pgsql/include

    LIBS += -L$$PWD/../../3rd/$$TARGET_ARCH/emqtt/lib -lpaho-mqtt3a -lpaho-mqtt3c \
            -L$$PWD/../../3rd/$$TARGET_ARCH/pgsql/lib -llibpq

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

