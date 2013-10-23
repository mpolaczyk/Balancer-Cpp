#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "bitops.h"
#include "exceptions.h"
#include <stdint.h>
#include <fstream>

using namespace std;

class Joystick
{

public:
    Joystick(const char * deviceFile);
    ~Joystick();
    void Read();

    bool Button[9];
    short Axis[5];

private:
    int _handle;
    ifstream _stream;
    char _deviceFile;

    static const char _buttons = 10;
    static const char _axises = 6;

    static const unsigned char _MODE_CONFIGURATION = 0x80;
    static const unsigned char _MODE_VALUE = 0x00;
    static const unsigned char _TYPE_AXIS = 0x02;
    static const unsigned char _TYPE_BUTTON = 0x01;
    static const unsigned char _STATE_PRESSED = 0x01;
    static const unsigned char _STATE_RELEASED = 0x00;
};

#endif // JOYSTICK_H
