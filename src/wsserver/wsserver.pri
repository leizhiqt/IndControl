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


