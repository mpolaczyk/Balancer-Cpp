#include "I2CDevice.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

I2CDevice::I2CDevice(const unsigned char address, const char * deviceFile)
{
    _address = address;
    _deviceFile = *deviceFile;
    _handle = open(deviceFile, O_RDWR);
	if (_handle == -1)
	{
		throw posix_error("Failed to open device");
	}
	if(ioctl(_handle, I2C_SLAVE, _address) == -1)
	{
		throw posix_error("Failed to set an address");
	}
}

I2CDevice::~I2CDevice()
{
	close(_handle);
}

void I2CDevice::WriteToRegister(unsigned char address, unsigned char mask, unsigned char value)
{
    unsigned char currentValue = ReadFromRegister(address, mask);
    unsigned char newValue = setValueUnderMask(currentValue, value, mask);
    int result = i2c_smbus_write_byte_data(_handle, address, newValue);
    if (result == -1)
    {
        throw posix_error("Unable to write data to I2C bus.");
    }
}

unsigned char I2CDevice::ReadFromRegister(unsigned char address, unsigned char mask)
{
    int value = i2c_smbus_read_byte_data(_handle, address);
    if (value == -1)
    {
        throw posix_error("Unable to read data from I2C bus.");
    }
    return getValueUnderMask(static_cast<unsigned char>(value), mask);
}
	
