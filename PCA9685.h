#ifndef PCA9685_H
#define PCA9685_H

#include "I2CDevice.h"
#include "IPwmController.h"


class PCA9685 : protected I2CDevice, public IPwmController
{

public:
    PCA9685(const unsigned char address, const char * deviceFile);
    ~PCA9685();
    void InitPwm();
    void SetPwmFreq(unsigned short freq);
    void SetPwm(unsigned char channel, double fillFactor);

private:
    void ConfigurePwm(unsigned short minFreq, unsigned short maxFreq, unsigned short maxValue, unsigned char maxChannel);

    unsigned short _minFreq;
    unsigned short _maxFreq;
    unsigned short _maxValue;
    unsigned char _maxChannel;

    static const unsigned char _REG_R_SUBADDR1            = 0x02;    // ?
    static const unsigned char _REG_R_SUBADDR2            = 0x03;    // ?
    static const unsigned char _REG_R_SUBADDR3            = 0x04;    // ?
    static const unsigned char _REG_RW_MODE               = 0x00;    // Working mode
    static const unsigned char _REG_RW_PRESCALER          = 0xfe;    // Internal prescaller
    static const unsigned char _REG_RW_CH0_L_ON           = 0x06;    // Channel 0, LSB, rising slope moment
    static const unsigned char _REG_RW_CH0_H_ON           = 0x07;    // Channel 0, MSB, rising slope moment
    static const unsigned char _REG_RW_CH0_L_OFF          = 0x08;    // Channel 0, LSB, declining slope moment
    static const unsigned char _REG_RW_CH0_H_OFF          = 0x09;    // Channel 0; MSB, declining slope moment
    static const unsigned char _REG_RW_ALLCH_L_ON         = 0xFA;    // All channels, LSB, rising slope moment
    static const unsigned char _REG_RW_ALLCH_H_ON         = 0xFB;    // All channels, MSB, rising slope moment
    static const unsigned char _REG_RW_ALLCH_L_OFF        = 0xFC;    // All channels, LSB, declining slope moment
    static const unsigned char _REG_RW_ALLCH_H_OFF        = 0xFD;    // All channels, MSB, declining slope moment



};

#endif // PCA9685_H
