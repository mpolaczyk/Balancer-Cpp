#include "L3GD20.h"
#include "bitops.h"
#include <algorithm>
#include <numeric>
#include <time.h>
#include <sys/time.h>

L3GD20::L3GD20(const unsigned char address, const char *deviceFile)
 : I2CDevice(address, deviceFile)
{
    gain = 1;
    maxX = 0; meanX = 0; minX = 0;
    maxY = 0; meanY = 0; minY = 0;
    maxZ = 0; meanZ = 0; minZ = 0;
}

L3GD20::~L3GD20()
{

}

void L3GD20::Init()
{
    FullScaleDps scale = Get_FullScale(false);
    switch(scale)
    {
        case FullScaleDps::TwoHundredFifty:
            gain = 0.0085;
            break;
        case FullScaleDps::FiveHundred:
            gain = 0.0175;
            break;
        case FullScaleDps::TwoThousand:
            gain = 0.07;
            break;
    }
}

unsigned char L3GD20::Get_DeviceId()
{
    return ReadFromRegister(_REG_R_WHO_AM_I);
}

void L3GD20::Calibrate(bool print)
{
    CalibrateX(print);
    CalibrateY(print);
    CalibrateZ(print);
}

void L3GD20::CalibrateX(bool print)
{
    if(print) { std::cout << "\r\nCalibrating axis X, please do not move sensor...\r\n"; };

    int buff[20];
    for(int i = 0; i < 20; i ++)
    {
        while(!Get_DataAvailableX()) { usleep(1000); }
        buff[i] = Get_RawOutX();
    }
    minX = *std::min_element(buff, buff + 20);
    if(print) { std::cout << "Min x: " << minX; };
    maxX = *std::max_element(buff, buff + 20);
    if(print) { std::cout << "Max x: " << maxX; };
    meanX = std::accumulate(buff, buff+20,0)/20;
    if(print) { std::cout << "Mean x: " << meanX; };
}

void L3GD20::CalibrateY(bool print)
{
    if(print) { std::cout << "\r\nCalibrating axis Y, please do not move sensor...\r\n"; };

    int buff[20];
    for(int i = 0; i < 20; i ++)
    {
        while(!Get_DataAvailableY()) { usleep(1000); }
        buff[i] = Get_RawOutY();
    }
    minY = *std::min_element(buff, buff + 20);
    if(print) { std::cout << "Min y: " << minY; };
    maxY = *std::max_element(buff, buff + 20);
    if(print) { std::cout << "Max y: " << maxY; };
    meanY = std::accumulate(buff, buff+20,0)/20;
    if(print) { std::cout << "Mean y: " << meanY; };
}

void L3GD20::CalibrateZ(bool print)
{
    if(print) { std::cout << "\r\nCalibrating axis Z, please do not move sensor...\r\n"; };

    int buff[20];
    for(int i = 0; i < 20; i ++)
    {
        while(!Get_DataAvailableZ()) { usleep(1000); }
        buff[i] = Get_RawOutZ();
    }
    minZ = *std::min_element(buff, buff + 20);
    if(print) { std::cout << "Min z: " << minZ; };
    maxZ = *std::max_element(buff, buff + 20);
    if(print) { std::cout << "Max z: " << maxZ; };
    meanZ = std::accumulate(buff, buff+20,0)/20;
    if(print) { std::cout << "Mean z: " << meanZ; };
}

int L3GD20::Get_RawOutX()
{
    unsigned char l = ReadFromRegister(_REG_R_OUT_X_L);
    unsigned char h = ReadFromRegister(_REG_R_OUT_X_H);
    return (short)(h << 8 | l);
}

int L3GD20::Get_RawOutY()
{
    unsigned char l = ReadFromRegister(_REG_R_OUT_Y_L);
    unsigned char h = ReadFromRegister(_REG_R_OUT_Y_H);
    return (short)((h << 8) | l);
}

int L3GD20::Get_RawOutZ()
{
    unsigned char l = ReadFromRegister(_REG_R_OUT_Z_L);
    unsigned char h = ReadFromRegister(_REG_R_OUT_Z_H);
    return (short)((h << 8) | l);
}

double L3GD20::Get_CalOutX()
{
    return (Get_RawOutX() - meanX) * gain;
}

double L3GD20::Get_CalOutY()
{
    return (Get_RawOutY() - meanY) * gain;
}

double L3GD20::Get_CalOutZ()
{
    return (Get_RawOutZ() - meanZ) * gain;
}

bool L3GD20::Get_DataAvailableX()
{
    return ReadFromRegisterWithMask(_REG_R_STATUS_REG, _MASK_STATUS_REG_ZYXDA) == 1
        && ReadFromRegisterWithMask(_REG_R_STATUS_REG, _MASK_STATUS_REG_XDA) == 1;
}

bool L3GD20::Get_DataAvailableY()
{
    return ReadFromRegisterWithMask(_REG_R_STATUS_REG, _MASK_STATUS_REG_ZYXDA) == 1
        && ReadFromRegisterWithMask(_REG_R_STATUS_REG, _MASK_STATUS_REG_YDA) == 1;
}

bool L3GD20::Get_DataAvailableZ()
{
    return ReadFromRegisterWithMask(_REG_R_STATUS_REG, _MASK_STATUS_REG_ZYXDA) == 1
        && ReadFromRegisterWithMask(_REG_R_STATUS_REG, _MASK_STATUS_REG_ZDA) == 1;
}

void L3GD20::PrintConfig()
{
    // Device Id
    std::cout << "DeviceId: ";
    printBinary(Get_DeviceId());
    std::cout << "\r\n";

    // Power mode
    std::cout << "Power mode: ";
    Get_PowerMode(true);
    std::cout << "\r\n";

    // Full scale
    std::cout << "Full scale: ";
    Get_FullScale(true);
    std::cout << "\r\n";

    // Axis enabled
    Get_AxisEnbaled(L3GD20::Axis::X, true);
    std::cout << "\r\n";
    Get_AxisEnbaled(L3GD20::Axis::Y, true);
    std::cout << "\r\n";
    Get_AxisEnbaled(L3GD20::Axis::Z, true);
    std::cout << "\r\n";

    // Band width and data rate
    std::cout << "Band width: ";
    std::cout << Get_BandWidth();
    std::cout << "\r\n";
    std::cout << "Data rate: ";
    std::cout << Get_DataRate();
    std::cout << "\r\n";

    std::cout << "Frequency cut off: ";
    std::cout << Get_HighPassFrequencyCutOff();
    std::cout << "\r\n";
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



