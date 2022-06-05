QT       += core gui network
CONFIG += c++11

#QT += network websockets

TARGET = canopen
TEMPLATE = app

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

#CANOPEN_SRC = $$PWD/CANopenNode
#DRV_SRC = $$PWD/CANopenNode/example
#APPL_SRC = $$PWD/CANopenNode/example

message("INSTALLS SUCCESSFUL.....")


INCLUDEPATH += $$PWD
LIBS += -lUser32


    INCLUDEPATH = \
            $$PWD/CANopenNode \
            $$PWD/CANopenNode/example

    SOURCES = \
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

#HEADERS += \
##        mainwindow.h \
#    ControlMain.h

#SOURCES += \
#        mainwindow.cpp \
#        main.cpp \
#    ControlMain.cpp

#SOURCES += \
#        main.cpp

#FORMS += \
#        mainwindow.ui

#RESOURCES += \
#    resource.qrc

DESTDIR = $$PWD/build/bin
MOC_DIR = $$PWD/build/temp
RCC_DIR = $$PWD/build/temp
OBJECTS_DIR = $$PWD/build/temp
UI_DIR = $$PWD

#DLLDESTDIR = $$[QT_INSTALL_BINS]

# Default rules for deployment.
#unix:   IotService.files=
#win32:  IotService.files= \
#                    #$$PWD/lib/spon/*.dll $$PWD/lib/spon/*.ini \
#                    #$$PWD/lib/mysql-win32/*.dll $$PWD/lib/mysql-win32/*.pdb \
#                    $$PWD/lib/pgsql/* \
#                    $$PWD/lib/emqtt/x64/*.dll $$PWD/lib/emqtt/x64/*.pdb\
#                    $$PWD/tools

#IotService.path=$${DESTDIR}
#INSTALLS += IotService

message("INSTALLS SUCCESSFUL.....")



#message($$PWD)

#win32 {
#    INCLUDEPATH += $$PWD
#    LIBS +=

#    #message($$INCLUDEPATH)
#    #message($$LIBS)

#    HEADERS += \
##        $$PWD/Command.h \
###        $$PWD/CommandSDK.h \
##        $$PWD/CommandProtocol.h \
##        $$PWD/BDServer.h

#    SOURCES += \
##        $$PWD/Command.cpp \
##        $$PWD/CommandSDK.cpp \
##        $$PWD/CommandProtocol.cpp \
##        $$PWD/BDServer.cpp

#    INCLUDE_DIRS = \
#            -I$(DRV_SRC) \
#            -I$(CANOPEN_SRC) \
#            -I$(APPL_SRC)

#    SOURCES = \
#            $(DRV_SRC)/CO_driver_blank.c \
#            $(DRV_SRC)/CO_storageBlank.c \
#            $(CANOPEN_SRC)/301/CO_ODinterface.c \
#            $(CANOPEN_SRC)/301/CO_NMT_Heartbeat.c \
#            $(CANOPEN_SRC)/301/CO_HBconsumer.c \
#            $(CANOPEN_SRC)/301/CO_Emergency.c \
#            $(CANOPEN_SRC)/301/CO_SDOserver.c \
#            $(CANOPEN_SRC)/301/CO_TIME.c \
#            $(CANOPEN_SRC)/301/CO_SYNC.c \
#            $(CANOPEN_SRC)/301/CO_PDO.c \
#            $(CANOPEN_SRC)/303/CO_LEDs.c \
#            $(CANOPEN_SRC)/305/CO_LSSslave.c \
#            $(CANOPEN_SRC)/storage/CO_storage.c \
#            $(CANOPEN_SRC)/CANopen.c \
#            $(APPL_SRC)/OD.c \
#            $(DRV_SRC)/main_blank.c
#}


