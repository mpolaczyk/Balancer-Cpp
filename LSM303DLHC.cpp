#include "LSM303DLHC.h"
#include "bitops.h"

LSM303DLHC::LSM303DLHC(const unsigned char address, const char * deviceFile)
 : I2CDevice(address, deviceFile)
{

}

LSM303DLHC::~LSM303DLHC()
{

}

void LSM303DLHC::Init()
{

}

void LSM303DLHC::PrintConfig()
{
    // Axis enabled
    std::cout << "Axis X enabled: ";
    Get_AxisEnabled(LSM303DLHC::Axis::X, true);
    std::cout << "\r\n";
    std::cout << "Axis Y enabled: ";
    Get_AxisEnabled(LSM303DLHC::Axis::Y, true);
    std::cout << "\r\n";
    std::cout << "Axis Z enabled: ";
    Get_AxisEnabled(LSM303DLHC::Axis::Z, true);
    std::cout << "\r\n";

    // Full scale
    std::cout << "Full scale +/-G[N]: ";
    printBinary(Get_FullScale(true));
    std::cout << "\r\n";

    // Power mode
    std::cout << "Power mode: ";
    Get_PowerMode(true);
    std::cout << "\r\n";

}

int LSM303DLHC::Get_RawOutX()
{
    unsigned char l = ReadFromRegister(_REG_R_OUT_X_L_A);
    unsigned char h = ReadFromRegister(_REG_R_OUT_X_H_A);
    return (short)(h << 8 | l);
}

int LSM303DLHC::Get_RawOutY()
{
    unsigned char l = ReadFromRegister(_REG_R_OUT_Y_L_A);
    unsigned char h = ReadFromRegister(_REG_R_OUT_Y_H_A);
    return (short)((h << 8) | l);
}

int LSM303DLHC::Get_RawOutZ()
{
    unsigned char l = ReadFromRegister(_REG_R_OUT_Z_L_A);
    unsigned char h = ReadFromRegister(_REG_R_OUT_Z_H_A);
    return (short)((h << 8) | l);
}

void LSM303DLHC::Set_PowerMode(LSM303DLHC::PowerMode mode)
{
    switch (mode)
    {
        case PowerMode::Normal:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_LPen, 0);
            break;

        case PowerMode::LowPower:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_LPen, 1);
            break;
    }
}


LSM303DLHC::PowerMode LSM303DLHC::Get_PowerMode(bool print)
{
    unsigned char currentMode = ReadFromRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_LPen);
    switch(currentMode)
    {
        case 0:
            if(print) { std::cout << "Normal"; };
            return PowerMode::Normal;
        case 1:
            if(print) { std::cout << "Low power"; };
            return PowerMode::LowPower;
    }
}

void LSM303DLHC::Set_AxisEnabled(Axis axis, bool value)
{
    switch(axis)
    {
        case Axis::X:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_Xen, static_cast<unsigned char>(value));
            break;
        case Axis::Y:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_Yen, static_cast<unsigned char>(value));
            break;
        case Axis::Z:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_Zen, static_cast<unsigned char>(value));
            break;
    }
}

bool LSM303DLHC::Get_AxisEnabled(Axis axis, bool print)
{
    bool ans = false;
    switch(axis)
    {
        case Axis::X:
            ans = static_cast<bool>(ReadFromRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_Xen));
            if(print) { std::cout << "Axis X: " << (ans ? "Enabled" : "Disabled"); };
            break;
        case Axis::Y:
            ans = static_cast<bool>(ReadFromRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_Yen));
            if(print) { std::cout << "Axis Y: " << (ans ? "Enabled" : "Disabled"); };
            break;
        case Axis::Z:
            ans = static_cast<bool>(ReadFromRegisterWithMask(_REG_RW_CTRL_REG1_A, _MASK_CTRL_REG1_A_Zen));
            if(print) { std::cout << "Axis Z: " << (ans ? "Enabled" : "Disabled"); };
            break;
    }
    return ans;
}

bool LSM303DLHC::Get_DataAvailableX()
{
    return ReadFromRegisterWithMask(_REG_R_STATUS_REG_A, _MASK_STATUS_REG_ZYXDA) == 1
        && ReadFromRegisterWithMask(_REG_R_STATUS_REG_A, _MASK_STATUS_REG_XDA) == 1;
}

bool LSM303DLHC::Get_DataAvailableY()
{
    return ReadFromRegisterWithMask(_REG_R_STATUS_REG_A, _MASK_STATUS_REG_ZYXDA) == 1
        && ReadFromRegisterWithMask(_REG_R_STATUS_REG_A, _MASK_STATUS_REG_YDA) == 1;
}

bool LSM303DLHC::Get_DataAvailableZ()
{
    return ReadFromRegisterWithMask(_REG_R_STATUS_REG_A, _MASK_STATUS_REG_ZYXDA) == 1
        && ReadFromRegisterWithMask(_REG_R_STATUS_REG_A, _MASK_STATUS_REG_ZDA) == 1;
}

void LSM303DLHC::Set_FullScale(FullScaleN value)
{
    switch(value)
    {
        case FullScaleN::TwoG:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG4_A, _MASK_CTRL_REG4_A_FS, 0x00);
            break;
        case FullScaleN::FourG:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG4_A, _MASK_CTRL_REG4_A_FS, 0x01);
            break;
        case FullScaleN::EightG:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG4_A, _MASK_CTRL_REG4_A_FS, 0x02);
            break;
        case FullScaleN::SixteenG:
            WriteToRegisterWithMask(_REG_RW_CTRL_REG4_A, _MASK_CTRL_REG4_A_FS, 0x03);
            break;
    }
}

LSM303DLHC::FullScaleN LSM303DLHC::Get_FullScale(bool print)
{
    unsigned char value = ReadFromRegisterWithMask(_REG_RW_CTRL_REG4_A, _MASK_CTRL_REG4_A_FS);
    switch(value)
    {
        case 0:
            if(print) { std::cout << "2"; };
            return FullScaleN::TwoG;
            break;
        case 1:
            if(print) { std::cout << "4"; };
            return FullScaleN::FourG;
            break;
        case 2:
            if(print) { std::cout << "8"; };
            return FullScaleN::EightG;
            break;
        case 3:
            if(print) { std::cout << "16"; };
            return FullScaleN::SixteenG;
            break;
    }
}
