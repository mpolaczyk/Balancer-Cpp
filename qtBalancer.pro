TEMPLATE = app
CONFIG += console
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    I2CDevice.cpp \
    L3GD20.cpp \
    LSM303DLHC.cpp \
    PCA9685.cpp \
    Engine.cpp \
    Joystick.cpp

HEADERS += \
    bitops.h \
    I2CDevice.h \
    exceptions.h \
    L3GD20.h \
    LSM303DLHC.h \
    PCA9685.h \
    Engine.h \
    IPwmController.h \
    Joystick.h

