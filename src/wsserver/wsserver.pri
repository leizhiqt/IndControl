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

    HEADERS += \
        $$PWD/Command.h \
#        $$PWD/CommandSDK.h \
        $$PWD/CommandProtocol.h \
        $$PWD/BDServer.h

    SOURCES += \
#        $$PWD/Command.cpp \
#        $$PWD/CommandSDK.cpp \
        $$PWD/CommandProtocol.cpp \
        $$PWD/BDServer.cpp
}


