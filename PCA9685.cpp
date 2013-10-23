#include "PCA9685.h"
#include "bitops.h"
#include "math.h"
#include <iostream>

using namespace::std;

PCA9685::PCA9685(const unsigned char address, const char * deviceFile)
 : I2CDevice(address, deviceFile)
{

}

PCA9685::~PCA9685()
{

}

void PCA9685::Init()
{
    WriteToRegister(_REG_RW_MODE, 0x00); // Reset
}

void PCA9685::PrintConfig()
{
    cout << endl << "Mode: ";
    printBinary(ReadFromRegister(_REG_RW_MODE));

    cout << endl << "Prescaler: ";
    printBinary(ReadFromRegister(_REG_RW_PRESCALER));

    cout << endl << "On L: ";
    printBinary(ReadFromRegister(_REG_RW_CH0_L_ON));
    cout << endl << "On H: ";
    printBinary(ReadFromRegister(_REG_RW_CH0_H_ON));
    cout << endl << "Off L: ";
    printBinary(ReadFromRegister(_REG_RW_CH0_L_OFF));
    cout << endl << "Off H: ";
    printBinary(ReadFromRegister(_REG_RW_CH0_H_OFF));

}

void PCA9685::SetPWMFreq(unsigned short freq)
{
    char prescale = static_cast<char>(floor(25000000 / 4096 / freq -1 + 0.5));
    cout << "Prescale: ";
    printBinary(prescale);
    cout << endl;

    unsigned char oldMode = ReadFromRegister(_REG_RW_MODE);
    cout << "old mode: ";
    printBinary(oldMode);
    cout << endl;

    cout << "Sleep: ";
    printBinary((oldMode & 0x7f) | 0x10);
    cout << endl;

    WriteToRegister(_REG_RW_MODE, (oldMode & 0x7f) | 0x10); // Sleep
    WriteToRegister(_REG_RW_PRESCALER, prescale);
    WriteToRegister(_REG_RW_MODE, oldMode);

    usleep(0.005 * 1000000); // 0.5 ms

    cout << "New mode: ";
    printBinary(oldMode | 0x80);
    cout << endl;
    WriteToRegister(_REG_RW_MODE, oldMode | 0x80);
}

void PCA9685::SetPWM(unsigned char channel, unsigned short on, unsigned short off)
{
    if (channel > 15) { throw posix_error("Channel must be between 0 and 15."); }
    if (on > 4095) { throw posix_error("On must be between 0 and 4096."); }
    if (off > 4095) { throw posix_error("Off must be between 0 and 4096."); }

    WriteToRegister(_REG_RW_CH0_L_ON + 4 * channel, on & 0xff);
    WriteToRegister(_REG_RW_CH0_H_ON + 4 * channel, on >> 8);
    WriteToRegister(_REG_RW_CH0_L_OFF + 4 * channel, off & 0xff);
    WriteToRegister(_REG_RW_CH0_H_OFF + 4 * channel, off >> 8);
}
