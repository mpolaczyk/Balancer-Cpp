#include "I2CDevice.h"
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


I2CDevice::I2CDevice(const unsigned char address, const char * deviceFile)
{
    // Save values to private variables
    _address = address;
    _deviceFile = *deviceFile;

    // Open device file
    _handle = open(deviceFile, O_RDWR);
	if (_handle == -1)
	{
		throw posix_error("Failed to open device");
	}

    // Define slave address
    if(-1 == ioctl(_handle, I2C_SLAVE, _address))
	{
		throw posix_error("Failed to set an address");
	}
}


I2CDevice::~I2CDevice()
{
	close(_handle);
}


void I2CDevice::WriteToRegister(unsigned char address, unsigned char value)
{
    // Overwrite all register with a given value
    int result = i2c_smbus_write_byte_data(_handle, address, value);
    if (result == -1)
    {
        throw posix_error("Unable to write data to I2C bus.");
    }
}


void I2CDevice::WriteToRegisterWithMask(unsigned char address, unsigned char mask, unsigned char value)
{
    // Read current value of whole register
    unsigned char currentValue = ReadFromRegister(address);

    // Set new value under mask
    unsigned char newValue = setValueUnderMask(currentValue, value, mask);

    // Overwrite all register with new data (rememeber that only bits under mask were changed)
    int result = i2c_smbus_write_byte_data(_handle, address, newValue);
    if (result == -1)
    {
        throw posix_error("Unable to write data to I2C bus.");
    }
}


unsigned char I2CDevice::ReadFromRegister(unsigned char address)
{
    // Read whole register
    int value = i2c_smbus_read_byte_data(_handle, address);
    if (value == -1)
    {
        throw posix_error("Unable to read data from I2C bus.");
    }
    return static_cast<unsigned char>(value);
}


unsigned char I2CDevice::ReadFromRegisterWithMask(unsigned char address, unsigned char mask)
{
    // Read whole register
    int value = i2c_smbus_read_byte_data(_handle, address);
    if (value == -1)
    {
        throw posix_error("Unable to read data from I2C bus.");
    }

    // Return value under mask
    return getValueUnderMask(static_cast<unsigned char>(value), mask);
}
