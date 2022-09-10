#-------------------------------------------------
#
# Project created by QtCreator 2021-12-20T15:19:15
#
#-------------------------------------------------

#buildDir = $$PWD/build/win32
#TARGET_ARCH = x86

buildDir = $$PWD/build/win64
TARGET_ARCH = x64

QT       += gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IndControl
RC_FILE = $$PWD/res/app.rc
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

include(./src/util/utils.pri)
include(./src/component/component.pri)
include(./src/qtview/qtview.pri)
include(./src/protocol/protocol.pri)

INCLUDEPATH += $$PWD/uimain
#LIBS += -lUser32

HEADERS += \
        $$PWD/uimain/CanOpenUI.h \
        $$PWD/uimain/Conf.h \
        $$PWD/uimain/ControlMain.h \
        uimain/OpenGLForm.h

SOURCES += \
        $$PWD/uimain/CanOpenUI.cpp \
        $$PWD/uimain/Conf.cpp \
        $$PWD/uimain/main.cpp \
        $$PWD/uimain/ControlMain.cpp \
        uimain/OpenGLForm.cpp

FORMS += \
        $$PWD/uimain/CanOpenUI.ui \
        uimain/OpenGLForm.ui
        $$PWD/uimain/SponWindow.ui

RESOURCES += \
        $$PWD/res/resource.qrc

DESTDIR = $$buildDir/bin
MOC_DIR = $$buildDir/temp
RCC_DIR = $$buildDir/temp
OBJECTS_DIR = $$buildDir/temp
UI_DIR = $$PWD/uimain

#DLLDESTDIR = $$[QT_INSTALL_BINS]

# Default rules for deployment.
unix:   IndControl.files=
win32:  IndControl.files= \
                    $$PWD/3rd/$$TARGET_ARCH/pgsql/lib/*
#                    $$PWD/3rd/$$TARGET_ARCH/emqtt/lib/*.dll


IndControl.path=$${DESTDIR}
INSTALLS += IndControl

#message($$LIBS)

message("INSTALLS SUCCESSFUL.....")
