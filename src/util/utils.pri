INCLUDEPATH += $$PWD
#INCLUDEPATH += $$PWD \
#               $$PWD/../../3rd/$$TARGET_ARCH/emqtt/include \
#               $$PWD/../../3rd/$$TARGET_ARCH/pgsql/include

#LIBS += -L$$PWD/../../3rd/$$TARGET_ARCH/emqtt/lib -lpaho-mqtt3a -lpaho-mqtt3c \
#        -L$$PWD/../../3rd/$$TARGET_ARCH/pgsql/lib -llibpq

#message($$INCLUDEPATH)
#message($$LIBS)

HEADERS += \
    $$PWD/UTypes.h \
    $$PWD/FormatTimes.h \
    $$PWD/ConvertUtil.h \
    $$PWD/UWLog.h \
    $$PWD/JSONUtil.h \
    $$PWD/MyDownloader.h \
    $$PWD/ShellProcess.h \
    $$PWD/NetToolHelper.h
#    $$PWD/MQTTUtil.h \
#    $$PWD/IOTUtil.h \
#    $$PWD/PGSQLDriveHelper.h \

SOURCES += \
    $$PWD/FormatTimes.cpp \
    $$PWD/ConvertUtil.cpp \
    $$PWD/UWLog.cpp \
    $$PWD/JSONUtil.cpp \
    $$PWD/MyDownloader.cpp \
    $$PWD/ShellProcess.cpp \
    $$PWD/NetToolHelper.cpp
#    $$PWD/PGSQLDriveHelper.cpp \
#    $$PWD/MQTTUtil.cpp \
#    $$PWD/IOTUtil.cpp \






