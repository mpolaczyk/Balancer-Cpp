#ifndef _I2CDEVICE_H
#define _I2CDEVICE_H

#include "bitops.h"
#include "exceptions.h"
#include <stdint.h>

class I2CDevice
{

public:
    I2CDevice(const unsigned char address, const char * deviceFile);
	~I2CDevice();

protected:
    void WriteToRegister(unsigned char address, unsigned char value);
    void WriteToRegisterWithMask(unsigned char address, unsigned char mask, unsigned char value);
    unsigned char ReadFromRegister(unsigned char address);
    unsigned char ReadFromRegisterWithMask(unsigned char address, unsigned char mask);
	
private:
    int _handle;
    unsigned char _address;
    char _deviceFile;

};

#endif
