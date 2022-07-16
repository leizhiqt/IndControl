#QT       += core gui network
#CONFIG += c++11

#QT += network websockets

#TARGET = wsserver
#TEMPLATE = lib

#INCLUDEPATH += $$PWD
#DEPENDPATH += $$PWD

#message($$QMAKE_CXX)
#message($$TARGET)          # 生成目标的名字
#message($$DESTDIR)         # 目标输出文件路径
#message($$PWD)             # 当前文件(.pro或.pri)所在的路径
#message($$OUT_PWD)         # Makefile生成的路径
#message($$_PRO_FILE_)      # pro项目文件(带路径)
#message($$_PRO_FILE_PWD_)  # pro项目文件所在的路径

#!win32:QT +=
#win32:LIBS +=

#message($$PWD)

win32 {
    INCLUDEPATH += $$PWD
    LIBS +=

    #message($$INCLUDEPATH)
    #message($$LIBS)

    INCLUDEPATH += \
            $$PWD/CANopenNode \
            $$PWD/CANopenNode/example

    SOURCES += \
            $$PWD/CANopenNode/example/CO_driver_blank.c \
            $$PWD/CANopenNode/example/CO_storageBlank.c \
            $$PWD/CANopenNode/301/CO_ODinterface.c \
            $$PWD/CANopenNode/301/CO_NMT_Heartbeat.c \
            $$PWD/CANopenNode/301/CO_HBconsumer.c \
            $$PWD/CANopenNode/301/CO_Emergency.c \
            $$PWD/CANopenNode/301/CO_SDOserver.c \
            $$PWD/CANopenNode/301/CO_TIME.c \
            $$PWD/CANopenNode/301/CO_SYNC.c \
            $$PWD/CANopenNode/301/CO_PDO.c \
            $$PWD/CANopenNode/303/CO_LEDs.c \
            $$PWD/CANopenNode/305/CO_LSSslave.c \
            $$PWD/CANopenNode/storage/CO_storage.c \
            $$PWD/CANopenNode/CANopen.c \
            $$PWD/CANopenNode/example/OD.c \
            $$PWD/CANopenNode/example/main_blank.c

    HEADERS += \
                $$PWD/CANopenNode/CANopenQThread.h
#                $$PWD/CANopenNode/example/CO_driver_blank.h \
#                $$PWD/CANopenNode/example/CO_storageBlank.h \
#                $$PWD/CANopenNode/301/CO_ODinterface.h \
#                $$PWD/CANopenNode/301/CO_NMT_Heartbeat.h \
#                $$PWD/CANopenNode/301/CO_HBconsumer.h \
#                $$PWD/CANopenNode/301/CO_Emergency.h \
#                $$PWD/CANopenNode/301/CO_SDOserver.h \
#                $$PWD/CANopenNode/301/CO_TIME.h \
#                $$PWD/CANopenNode/301/CO_SYNC.h \
#                $$PWD/CANopenNode/301/CO_PDO.h \
#                $$PWD/CANopenNode/303/CO_LEDs.h \
#                $$PWD/CANopenNode/305/CO_LSSslave.h \
#                $$PWD/CANopenNode/storage/CO_storage.h \
#                $$PWD/CANopenNode/CANopen.h \
#                $$PWD/CANopenNode/example/OD.h
    SOURCES += \
        $$PWD/CANopenNode/CANopenQThread.cpp
 }




