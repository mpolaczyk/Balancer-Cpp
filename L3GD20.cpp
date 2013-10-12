#include "L3GD20.h"

L3GD20::L3GD20(const unsigned char address, const char *deviceFile)
 : I2CDevice(address, deviceFile)
{

}

L3GD20::~L3GD20()
{

}

unsigned char L3GD20::Get_DeviceId_Value()
{
    return ReadFromRegister(_REG_R_WHO_AM_I);
}
