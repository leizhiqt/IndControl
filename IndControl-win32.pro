#-------------------------------------------------
#
# Project created by QtCreator 2021-12-20T15:19:15
#
#-------------------------------------------------

buildDir = $$PWD/build/win32

QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IndControl
RC_FILE = app.rc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG += c++11 console
CONFIG += c++11
CONFIG -= debug_and_release

#include(./src/example/example.pri)

include(./src/util/utils.pri)
include(./src/component/component.pri)

include(./src/wsserver/wsserver.pri)
#include(./src/protocol/protocol.pri)

#include(./src/protocol/modbustcp.pri)
#include(./src/protocol/zlgcan_x64/zlgcan_x64.pri)
#include(./src/protocol/zlgcan_x86/zlgcan_x86.pri)

#include(./src/plugs/plugs.pri)
include(./src/qtview/qtview.pri)

INCLUDEPATH += $$PWD
#LIBS += -lUser32

HEADERS += \
    CanOpenUI.h \
#        mainwindow.h \
    Conf.h \
    ControlMain.h \
    UIReadThread.h

SOURCES += \
    CanOpenUI.cpp \
#        mainwindow.cpp \
    Conf.cpp \
    UIReadThread.cpp \
        main.cpp \
    ControlMain.cpp

FORMS += \
 \#        mainwindow.ui
    CanOpenUI.ui

RESOURCES += \
    resource.qrc

DESTDIR = $$buildDir/bin
MOC_DIR = $$buildDir/temp
RCC_DIR = $$buildDir/temp
OBJECTS_DIR = $$buildDir/temp
UI_DIR = $$PWD

#DLLDESTDIR = $$[QT_INSTALL_BINS]

# Default rules for deployment.
#unix:   IndControl.files=
#win32:  IndControl.files= \
#                    #$$PWD/lib/spon/*.dll $$PWD/lib/spon/*.ini \
#                    #$$PWD/lib/mysql-win32/*.dll $$PWD/lib/mysql-win32/*.pdb \
##                    $$PWD/lib/pgsql/* \
##                    $$PWD/lib/emqtt/x64/*.dll $$PWD/lib/emqtt/x64/*.pdb\
##                    $$PWD/tools
#                     $$PWD/lib/pgsql/*

IndControl.path=$${DESTDIR}
INSTALLS += IndControl

message($$LIBS)

message("INSTALLS SUCCESSFUL.....")
