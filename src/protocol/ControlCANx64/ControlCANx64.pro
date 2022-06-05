QT       += core gui network
CONFIG += c++11
CONFIG -= debug_and_release

#QT += network websockets

TARGET = CANExample
TEMPLATE = app

DESTDIR = $$PWD/build/temp
MOC_DIR = $$PWD/build/temp
RCC_DIR = $$PWD/build/temp
OBJECTS_DIR = $$PWD/build/temp
UI_DIR = $$PWD

#INCLUDEPATH += $$PWD
#DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD

LIBS += -L$$PWD/kerneldlls -lControlCAN

HEADERS += \
    $$PWD/src/ControlCAN.h \
    $$PWD/src/ControCANx64.h

SOURCES += \
        $$PWD/src/ControCANx64.cpp

#FORMS += \
#        mainwindow.ui

#RESOURCES += \
#    resource.qrc



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

#message($$QMAKE_CXX)
#message($$TARGET)          # 生成目标的名字
#message($$DESTDIR)         # 目标输出文件路径
#message($$PWD)             # 当前文件(.pro或.pri)所在的路径
#message($$OUT_PWD)         # Makefile生成的路径
#message($$_PRO_FILE_)      # pro项目文件(带路径)
#message($$_PRO_FILE_PWD_)  # pro项目文件所在的路径

message("INSTALLS SUCCESSFUL.....")


