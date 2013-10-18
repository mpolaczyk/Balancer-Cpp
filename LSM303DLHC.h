#ifndef LSM303DLHC_H
#define LSM303DLHC_H

#include "I2CDevice.h"

class LSM303DLHC : public I2CDevice
{

public:
    LSM303DLHC(const unsigned char address, const char * deviceFile);
    ~LSM303DLHC();
    void Init();
    void PrintConfig();

    double Get_CalOutX();
    double Get_CalOutY();
    double Get_CalOutZ();

    int Get_RawOutX();
    int Get_RawOutY();
    int Get_RawOutZ();

    bool Get_DataAvailableX();
    bool Get_DataAvailableY();
    bool Get_DataAvailableZ();

    long Get_DataRate();
    void Set_DataRate(long value);

    enum PowerMode { Normal = 0, LowPower = 1 };
    void Set_PowerMode(PowerMode mode);
    PowerMode Get_PowerMode(bool print);

    enum Axis { X = 1, Y = 2, Z = 4 };
    void Set_AxisEnabled(Axis axis, bool value);
    bool Get_AxisEnabled(Axis axis, bool print);

    enum FullScaleN { TwoG, FourG, EightG, SixteenG };
    void Set_FullScale(FullScaleN value);
    FullScaleN Get_FullScale(bool print);

private:
    double gain;

    static const unsigned char _REG_RW_CTRL_REG1_A            = 0x20;      // Control register 1
    static const unsigned char _REG_RW_CTRL_REG4_A            = 0x23;      // Control register 4
    static const unsigned char _REG_R_STATUS_REG_A            = 0x27;      // Status register
    static const unsigned char _REG_R_OUT_X_L_A               = 0x28;      // Output X LSB
    static const unsigned char _REG_R_OUT_X_H_A               = 0x29;      // Output X MSB
    static const unsigned char _REG_R_OUT_Y_L_A               = 0x2a;      // Output Y LSB
    static const unsigned char _REG_R_OUT_Y_H_A               = 0x2b;      // Output Y MSB
    static const unsigned char _REG_R_OUT_Z_L_A               = 0x2c;      // Output Z LSB
    static const unsigned char _REG_R_OUT_Z_H_A               = 0x2d;      // Output Z MSB



    static const unsigned char _MASK_CTRL_REG1_A_ODR          = 0xf0;      // Data rate selection
    static const unsigned char _MASK_CTRL_REG1_A_LPen         = 0x08;      // Low power mode enable
    static const unsigned char _MASK_CTRL_REG1_A_Zen          = 0x04;      // Z axis enabled
    static const unsigned char _MASK_CTRL_REG1_A_Yen          = 0x02;      // Y axis enabled
    static const unsigned char _MASK_CTRL_REG1_A_Xen          = 0x01;      // X axis enabled
    static const unsigned char _MASK_CTRL_REG4_A_FS           = 0x30;      // Full scale selection +/-[g]
    static const unsigned char _MASK_STATUS_REG_ZYXDA    = 0x08;      // Z, Y and X axis data available
    static const unsigned char _MASK_STATUS_REG_ZDA      = 0x04;      // Z axis data available
    static const unsigned char _MASK_STATUS_REG_YDA      = 0x02;      // Y axis data available
    static const unsigned char _MASK_STATUS_REG_XDA      = 0x01;      // X axis data available



};

#endif // LSM303DLHC_H
