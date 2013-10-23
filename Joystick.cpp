#include "Joystick.h"
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>

//#include <sys/ioctl.h>

using namespace std;

Joystick::Joystick(const char * deviceFile)
{
    // Save values to private variables
    _deviceFile = *deviceFile;

    // Open device file
    //_handle = open(deviceFile, O_RDONLY);
    //if (_handle == -1)
    //{
    //    throw posix_error("Failed to open device");
    //}
    _stream.open(deviceFile, std::ios::in | std::ios::binary);

    for(char i = 0; i < _buttons; i++)
    {
        Button[i] = false;
    }

    for(char i = 0; i < _axises; i++)
    {
        Axis[i] = 0;
    }
}


Joystick::~Joystick()
{
    _stream.close();
}

void Joystick::Read()
{
    char buffer[8];
    _stream.read(buffer, 8);
    //cout << "Buffer: "; cout << "|";
    //printBinary(buffer[0]); cout << "|";
    //printBinary(buffer[1]); cout << "|";
    //printBinary(buffer[2]); cout << "|";
    //printBinary(buffer[3]); cout << "|";
    //printBinary(buffer[4]); cout << "|";
    //printBinary(buffer[5]); cout << "|";
    //printBinary(buffer[6]); cout << "|";
    //printBinary(buffer[7]); cout << "|";
    //cout << endl << flush;

    // Check Buttons and axises
    if (buffer[6] == _TYPE_AXIS)
    {
        short value =  (short)((buffer[4] << 8) | buffer[5]);
        Axis[buffer[7]] = value;
        return;
    }
    else if (buffer[6] == _TYPE_BUTTON)
    {
        Button[buffer[7]] = (buffer[4] == _STATE_PRESSED);
        return;
    }
    // or configuration
    else if (buffer[6] == _MODE_CONFIGURATION | _TYPE_AXIS)
    {
        // TODO: Add axis to dictionary
        cout << "Axis configuration bytes..." << endl;
        return;
    }
    else if (buffer[6] == _MODE_CONFIGURATION | _TYPE_BUTTON)
    {
        // TODO: Add button to dictionary
        cout << "Button configuration bytes..." << endl;
        return;
    }
}
