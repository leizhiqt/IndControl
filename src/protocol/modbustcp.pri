
INCLUDEPATH += $$PWD $$PWD/modbustcp
LIBS += -L$$PWD/modbustcp -lmodbus -lws2_32

INCLUDEPATH += \
        $$PWD \
        $$PWD/modbustcp

HEADERS += \
        $$PWD/modbustcp/ModbusTcp.h

SOURCES += \
    $$PWD/modbustcp/ModBusTcp.cpp

