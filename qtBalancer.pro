TEMPLATE = app
CONFIG += console
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    I2CDevice.cpp \
    L3GD20.cpp

HEADERS += \
    bitops.h \
    I2CDevice.h \
    exceptions.h \
    L3GD20.h

