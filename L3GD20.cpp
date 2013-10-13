#include "L3GD20.h"

L3GD20::L3GD20(const unsigned char address, const char *deviceFile)
 : I2CDevice(address, deviceFile)
{

}

L3GD20::~L3GD20()
{

}

unsigned char L3GD20::Get_DeviceId()
{
    return ReadFromRegister(_REG_R_WHO_AM_I);
}

void L3GD20::Set_PowerMode(PowerMode mode)
{
    switch (mode)
    {
        case PowerMode::Normal:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_PD, 1);
            break;

        case PowerMode::Sleep:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_PD | _MASK_CTRL_REG1_Zen | _MASK_CTRL_REG1_Yen | _MASK_CTRL_REG1_Xen, 8);
            break;

        case PowerMode::PowerDown:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_PD, 0);
            break;
    }
}

L3GD20::PowerMode L3GD20::Get_PowerMode(bool print)
{
    unsigned char currentMode = ReadFromRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_PD | _MASK_CTRL_REG1_Xen | _MASK_CTRL_REG1_Yen | _MASK_CTRL_REG1_Zen);

    if (checkBit(currentMode, 3))
    {
        if (8 == currentMode)
        {
            if(print) { std::cout << "Sleep"; };
            return PowerMode::Sleep;
        }
        if(print) { std::cout << "Normal"; };
        return PowerMode::Normal;
    }
    if(print) { std::cout << "Power down"; };
    return PowerMode::PowerDown;
}

void L3GD20::Set_FullScale(FullScaleDps value)
{
    switch(value)
    {
        case FullScaleDps::TwoHundredFifty:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG4, _MASK_CTRL_REG4_FS, 0x00);
            break;
        case FullScaleDps::FiveHundred:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG4, _MASK_CTRL_REG4_FS, 0x01);
            break;
        case FullScaleDps::TwoThousand:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG4, _MASK_CTRL_REG4_FS, 0x02);
            break;
    }
}

L3GD20::FullScaleDps L3GD20::Get_FullScale(bool print)
{
    unsigned char value = ReadFromRegisterWithMask(_REG_RW_CTRL_REG4, _MASK_CTRL_REG4_FS);
    switch(value)
    {
        case 0:
            if(print) { std::cout << "250"; };
            return FullScaleDps::TwoHundredFifty;
            break;
        case 1:
            if(print) { std::cout << "500"; };
            return FullScaleDps::FiveHundred;
            break;
        case 2:
            if(print) { std::cout << "2000"; };
            return FullScaleDps::TwoThousand;
            break;
    }
}

void L3GD20::Set_AxisEnabled(Axis axis, bool value)
{
    switch(axis)
    {
        case Axis::X:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_Xen, static_cast<unsigned char>(value));
            break;
        case Axis::Y:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_Yen, static_cast<unsigned char>(value));
            break;
        case Axis::Z:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_Zen, static_cast<unsigned char>(value));
            break;
    }
}

bool L3GD20::Get_AxisEnbaled(Axis axis, bool print)
{
    bool ans;
    switch(axis)
    {
        case Axis::X:
            ans = static_cast<bool>(ReadFromRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_Xen));
            if(print) { std::cout << "Axis X: " << (ans ? "Enabled" : "Disabled"); };
            break;
        case Axis::Y:
            ans = static_cast<bool>(ReadFromRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_Yen));
            if(print) { std::cout << "Axis Y: " << (ans ? "Enabled" : "Disabled"); };
            break;
        case Axis::Z:
            ans = static_cast<bool>(ReadFromRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_Zen));
            if(print) { std::cout << "Axis Z: " << (ans ? "Enabled" : "Disabled"); };
            break;
    }
}

int L3GD20::Get_DataRate()
{
    unsigned char value = ReadFromRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_DR | _MASK_CTRL_REG1_BW);

    if (value >= 0 && value <= 1) { return 95; }
    else if (value >= 4 && value <= 7) { return 190; }
    else if (value >= 8 && value <= 11) { return 380; }
    else if (value >= 12 && value <= 15) { return 760; }
}

float L3GD20::Get_BandWidth()
{
    unsigned char value = ReadFromRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_DR | _MASK_CTRL_REG1_BW);

    if (value == 0) { return 12.5; }
    else if (value == 1) { return 25; }
    else if (value == 4) { return 12.5; }
    else if (value == 5) { return 25; }
    else if (value == 6) { return 50; }
    else if (value == 7) { return 70; }
    else if (value == 8) { return 20; }
    else if (value == 9) { return 25; }
    else if (value == 10) { return 50; }
    else if (value == 11) { return 100; }
    else if (value == 12) { return 30; }
    else if (value == 13) { return 35; }
    else if (value == 14) { return 50; }
    else if (value == 15) { return 100; }
}

void L3GD20::Set_DataRateAndBandWidth(int dataRate, float bandWidth)
{
    bool fail = false;
    unsigned char value;
    if (dataRate == 95)
    {
        if (bandWidth == 12.5) { value = 0x00; } else if (bandWidth == 25) { value = 0x01; } else { fail = true; }
    }
    else if (dataRate == 190)
    {
        if (bandWidth == 12.5) { value = 0x04; } else if (bandWidth == 25) { value = 0x05; } else if (bandWidth == 50) { value = 0x06; } else if (bandWidth == 70) { value = 0x07; } else { fail = true; }
    }
    else if (dataRate == 380)
    {
        if (bandWidth == 20) { value = 0x08; } else if (bandWidth == 25) { value = 0x09; } else if (bandWidth == 50) { value = 0x0a; } else if (bandWidth == 100) { value = 0x0b; } else { fail = true; }
    }
    else if (dataRate == 760)
    {
        if (bandWidth == 30) { value = 0x0c; } else if (bandWidth == 35) { value = 0x0d; } else if (bandWidth == 50) { value = 0x0e; } else if (bandWidth == 100) { value = 0x0f; } else { fail = true; }
    }
    else { fail = true; }

    if(!fail)
    {
        WriteToRegisterWithMask(_REG_RW_CTRL_REG1, _MASK_CTRL_REG1_DR | _MASK_CTRL_REG1_BW, value);
    }
    else
    {
        throw posix_error("Unknown values configuration.");
    }
}

float L3GD20::Get_HighPassFrequencyCutOff()
{
    unsigned char value = ReadFromRegisterWithMask(_REG_RW_CTRL_REG2, _MASK_CTRL_REG2_HPCF);
    int dataRate = Get_DataRate();

    if (value == 0)
    {
        if (dataRate == 95) { return 7.2; } else if(dataRate == 190) { return 13.5; } else if(dataRate  == 380) { return 27; } else if(dataRate  == 720) { return 51.4; }
    }
    else if (value == 1)
    {
        if (dataRate == 95) { return 3.5; } else if(dataRate  == 190) { return 7.2; } else if(dataRate  == 380) { return 13.5; } else if(dataRate  == 720) { return 27; }
    }
    else if (value == 2)
    {
        if (dataRate == 95) { return 1.8; } else if(dataRate  == 190) { return 3.5; } else if(dataRate  == 380) { return 7.2; } else if(dataRate  == 720) { return 13.5; }
    }
    else if (value == 3)
    {
        if (dataRate == 95) { return 0.9; } else if(dataRate  == 190) { return 1.8; } else if(dataRate  == 380) { return 3.5; } else if(dataRate  == 720) { return 7.2; }
    }
    else if (value == 4)
    {
        if (dataRate == 95) { return 0.45; } else if (dataRate == 190) { return 0.9; } else if (dataRate == 380) { return 1.8; } else if (dataRate == 760) { return 3.5; }
    }
    else if (value == 5)
    {
        if (dataRate == 95) { return 0.18; } else if(dataRate  == 190) { return 0.45; } else if(dataRate  == 380) { return 0.9; } else if(dataRate  == 720) { return 1.8; }
    }
    else if (value == 6)
    {
        if (dataRate == 95) { return 0.09; } else if(dataRate  == 190) { return 0.18; } else if(dataRate  == 380) { return 0.45; } else if(dataRate  == 720) { return 0.9; }
    }
    else if (value == 7)
    {
        if (dataRate == 95) { return 0.045; } else if(dataRate  == 190) { return 0.09; } else if(dataRate  == 380) { return 0.18; } else if(dataRate  == 720) { return 0.45; }
    }
    else if (value == 8)
    {
        if (dataRate == 95) { return 0.018; } else if(dataRate  == 190) { return 0.045; } else if(dataRate  == 380) { return 0.09; } else if(dataRate  == 720) { return 0.18; }
    }
    else if (value == 9)
    {
        if (dataRate == 95) { return 0.009; } else if(dataRate  == 190) { return 0.018; } else if(dataRate  == 380) { return 0.045; }
    }
}

void L3GD20::Set_HighPassFrequencyCutOff(float freq)
{
    bool fail = false;
    int dataRate = Get_DataRate();
    unsigned char value;

    if(freq == 51.4)
    {
        if(dataRate == 760) { value = 0; } else { fail = true; }
    }
    else if(freq == 27)
    {
        if(dataRate == 380) { value = 0; } else if(dataRate == 760) { value = 1; } else { fail = true; }
    }
    else if(freq == 13.5)
    {
        if(dataRate == 190) { value = 0; } else if(dataRate == 380) { value = 1; } else if(dataRate == 760) { value = 2; } else { fail = true; }
    }
    else if(freq == 7.2)
    {
        if(dataRate == 95) { value = 0; } else if(dataRate == 190) { value = 1; } else if(dataRate == 380) { value = 2; } else if(dataRate == 760) { value = 3; } else { fail = true; }
    }
    else if(freq == 3.5)
    {
        if(dataRate == 95) { value = 1; } else if(dataRate == 190) { value = 2; } else if(dataRate == 380) { value = 3; } else if(dataRate == 720) { value = 4; } else { fail = true; }
    }
    else if(freq == 1.8)
    {
        if(dataRate == 95) { value = 2; } else if(dataRate == 190) { value = 3; } else if(dataRate == 380) { value = 4; } else if(dataRate == 720) { value = 5; } else { fail = true; }
    }
    else if(freq == 0.9)
    {
        if(dataRate == 95) { value = 3; } else if(dataRate == 190) { value = 4; } else if(dataRate == 380) { value = 5; } else if(dataRate == 720) { value = 6; } else { fail = true; }
    }
    else if(freq == 0.45)
    {
        if(dataRate == 95) { value = 4; } else if(dataRate == 190) { value = 5; } else if(dataRate == 380) { value = 6; } else if(dataRate == 720) { value = 7; } else { fail = true; }
    }
    else if(freq == 0.18)
    {
        if(dataRate == 95) { value = 5; } else if(dataRate == 190) { value = 6; } else if(dataRate == 380) { value = 7; } else if(dataRate == 720) { value = 8; } else { fail = true; }
    }
    else if(freq == 0.09)
    {
        if(dataRate == 95) { value = 6; } else if(dataRate == 190) { value = 7; } else if(dataRate == 380) { value = 8; } else if(dataRate == 720) { value = 9; } else { fail = true; }
    }
    else if(freq == 0.045)
    {
        if(dataRate == 95) { value = 7; } else if(dataRate == 190) { value = 8; } else if(dataRate == 380) { value = 9; } else { fail = true; }
    }
    else if(freq == 0.018)
    {
        if(dataRate == 95) { value = 8; } else if(dataRate == 190) { value = 9; } else { fail = true; }
    }
    else if(freq == 0.009)
    {
        if(dataRate == 95) { value = 9; } else { fail = true; }
    }
    else { fail = true; }

    if(!fail)
    {
        WriteToRegisterWithMask(_REG_RW_CTRL_REG2, _MASK_CTRL_REG2_HPCF, value);
    }
    else
    {
        throw posix_error("Unknown values configuration.");
    }
}







