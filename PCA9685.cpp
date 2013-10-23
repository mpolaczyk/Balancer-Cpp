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

void PCA9685::InitPwm()
{
    ConfigurePwm(40, 1000, 4095, 15);
    WriteToRegister(_REG_RW_MODE, 0x00); // Reset
}

void PCA9685::ConfigurePwm(unsigned short minFreq, unsigned short maxFreq, unsigned short maxValue, unsigned char maxChannel)
{
    _minFreq = minFreq;
    _maxFreq = maxFreq;
    _maxValue = maxValue;
    _maxChannel = maxChannel;
}

void PCA9685::SetPwmFreq(unsigned short freq)
{
    if (freq < _minFreq) { throw posix_error("Frequency cannot be less than minimum value."); }
    if (freq > _maxFreq) { throw posix_error("Frequency cannot be greater than maximum value."); }

    char prescale = static_cast<char>(floor(25000000 / 4096 / freq -1 + 0.5));
    unsigned char oldMode = ReadFromRegister(_REG_RW_MODE);

    WriteToRegister(_REG_RW_MODE, (oldMode & 0x7f) | 0x10); // Sleep
    WriteToRegister(_REG_RW_PRESCALER, prescale);
    WriteToRegister(_REG_RW_MODE, oldMode);

    usleep(0.005 * 1000000); // 0.5 ms

    WriteToRegister(_REG_RW_MODE, oldMode | 0x80);
}

void PCA9685::SetPwm(unsigned char channel, double fillFactor)
{
    if (channel > _maxChannel) { throw posix_error("Channel canot be greater than maximum value."); }
    if (fillFactor > 100) { throw posix_error("Fill factor [%] must be <= 100 and >= 0."); }

    unsigned short value = static_cast<unsigned short>(((double)_maxValue / 100) * fillFactor);

    WriteToRegister(_REG_RW_CH0_L_ON + 4 * channel, 0);
    WriteToRegister(_REG_RW_CH0_H_ON + 4 * channel, 0);
    WriteToRegister(_REG_RW_CH0_L_OFF + 4 * channel, value & 0xff);
    WriteToRegister(_REG_RW_CH0_H_OFF + 4 * channel, value >> 8);
}
