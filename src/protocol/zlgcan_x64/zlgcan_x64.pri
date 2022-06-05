INCLUDEPATH += $$PWD $$PWD/src
LIBS += -L$$PWD -L$$PWD/kerneldlls -lzlgcan

HEADERS += \
    $$PWD/src/CANzlg.h

SOURCES += \
    $$PWD/src/CANzlg.cpp



