win32 {
    INCLUDEPATH += $$PWD

#    INCLUDEPATH += $$PWD $$PWD/../../ $$PWD/../../3rd/include
#    LIBS += -L$$PWD/../../3rd/lib -lDPSDK_Core -lDPSDK_Ext

    #message($$INCLUDEPATH)
    #message($$LIBS)
    HEADERS += \
        $$PWD/QTScreen.h \
        $$PWD/QTCamera.h \
#        $$PWD/QTPanTilt.h \
        $$PWD/QTTray.h \
        $$PWD/VideoWidget.h \
        $$PWD/ScreenshotWidget.h

    SOURCES += \
        $$PWD/QTScreen.cpp \
        $$PWD/QTCamera.cpp \
#        $$PWD/QTPanTilt.cpp \
        $$PWD/QTTray.cpp \
        $$PWD/VideoWidget.cpp \
        $$PWD/ScreenshotWidget.cpp
}


