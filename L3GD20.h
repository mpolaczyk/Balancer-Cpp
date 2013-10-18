#ifndef L3GD20_H
#define L3GD20_H

#include "I2CDevice.h"

class L3GD20 : public I2CDevice
{

public:
    L3GD20(const unsigned char address, const char * deviceFile);
    ~L3GD20();
    void Init();
    unsigned char Get_DeviceId();
    void PrintConfig();

    enum PowerMode { PowerDown = 0, Sleep = 1, Normal = 2 };
    void Set_PowerMode(PowerMode mode);
    PowerMode Get_PowerMode(bool print);

    enum FullScaleDps { TwoHundredFifty = 0, FiveHundred = 1, TwoThousand = 2 };
    void Set_FullScale(FullScaleDps value);
    FullScaleDps Get_FullScale(bool print);

    enum Axis { X = 1, Y = 2, Z = 4 };
    void Set_AxisEnabled(Axis axis, bool value);
    bool Get_AxisEnabled(Axis axis, bool print);

    int Get_DataRate();
    float Get_BandWidth();
    void Set_DataRateAndBandWidth(int dataRate, float bandWidth);

    void Set_HighPassFrequencyCutOff(float freq);
    float Get_HighPassFrequencyCutOff();

    void Calibrate(bool print);
    void CalibrateX(bool print);
    void CalibrateY(bool print);
    void CalibrateZ(bool print);

    int Get_RawOutX();
    int Get_RawOutY();
    int Get_RawOutZ();
    double Get_CalOutX();
    double Get_CalOutY();
    double Get_CalOutZ();

    bool Get_DataAvailableX();
    bool Get_DataAvailableY();
    bool Get_DataAvailableZ();

private:
    double gain;
    int maxX;
    int meanX;
    int minX;
    int maxY;
    int meanY;
    int minY;
    int maxZ;
    int meanZ;
    int minZ;

    static const unsigned char _REG_R_WHO_AM_I            = 0x0f;      // Device identification register
    static const unsigned char _REG_RW_CTRL_REG1          = 0x20;      // Control register 1
    static const unsigned char _REG_RW_CTRL_REG2          = 0x21;      // Control register 2
    static const unsigned char _REG_RW_CTRL_REG3          = 0x22;      // Control register 3
    static const unsigned char _REG_RW_CTRL_REG4          = 0x23;      // Control register 4
    static const unsigned char _REG_RW_CTRL_REG5          = 0x24;      // Control register 5
    static const unsigned char _REG_RW_REFERENCE          = 0x25;      // Reference value for interrupt generation
    static const unsigned char _REG_R_OUT_TEMP            = 0x26;      // Output temperature
    static const unsigned char _REG_R_STATUS_REG          = 0x27;      // Status register
    static const unsigned char _REG_R_OUT_X_L             = 0x28;      // X-axis angular data rate LSB
    static const unsigned char _REG_R_OUT_X_H             = 0x29;      // X-axis angular data rate MSB
    static const unsigned char _REG_R_OUT_Y_L             = 0x2a;      // Y-axis angular data rate LSB
    static const unsigned char _REG_R_OUT_Y_H             = 0x2b;      // Y-axis angular data rate MSB
    static const unsigned char _REG_R_OUT_Z_L             = 0x2c;      // Z-axis angular data rate LSB
    static const unsigned char _REG_R_OUT_Z_H             = 0x2d;      // Z-axis angular data rate MSB
    static const unsigned char _REG_RW_FIFO_CTRL_REG      = 0x2e;      // Fifo control register
    static const unsigned char _REG_R_FIFO_SRC_REG        = 0x2f;      // Fifo src register
    static const unsigned char _REG_RW_INT1_CFG_REG       = 0x30;      // Interrupt 1 configuration register
    static const unsigned char _REG_R_INT1_SRC_REG        = 0x31;      // Interrupt source register
    static const unsigned char _REG_RW_INT1_THS_XH        = 0x32;      // Interrupt 1 threshold level X MSB register
    static const unsigned char _REG_RW_INT1_THS_XL        = 0x33;      // Interrupt 1 threshold level X LSB register
    static const unsigned char _REG_RW_INT1_THS_YH        = 0x34;      // Interrupt 1 threshold level Y MSB register
    static const unsigned char _REG_RW_INT1_THS_YL        = 0x35;      // Interrupt 1 threshold level Y LSB register
    static const unsigned char _REG_RW_INT1_THS_ZH        = 0x36;      // Interrupt 1 threshold level Z MSB register
    static const unsigned char _REG_RW_INT1_THS_ZL        = 0x37;      // Interrupt 1 threshold level Z LSB register
    static const unsigned char _REG_RW_INT1_DURATION      = 0x38;      // Interrupt 1 duration register

    static const unsigned char _MASK_CTRL_REG1_Xen        = 0x01;      // X enable
    static const unsigned char _MASK_CTRL_REG1_Yen        = 0x02;      // Y enable
    static const unsigned char _MASK_CTRL_REG1_Zen        = 0x04;      // Z enable
    static const unsigned char _MASK_CTRL_REG1_PD         = 0x08;      // Power-down
    static const unsigned char _MASK_CTRL_REG1_BW         = 0x30;      // Bandwidth
    static const unsigned char _MASK_CTRL_REG1_DR         = 0xc0;      // Output data rate
    static const unsigned char _MASK_CTRL_REG2_HPCF       = 0x0f;      // High pass filter cutoff frequency
    static const unsigned char _MASK_CTRL_REG2_HPM        = 0x30;      // High pass filter mode selection
    static const unsigned char _MASK_CTRL_REG3_I2_EMPTY   = 0x01;      // FIFO empty interrupt on DRDY/INT2
    static const unsigned char _MASK_CTRL_REG3_I2_ORUN    = 0x02;      // FIFO overrun interrupt on DRDY/INT2
    static const unsigned char _MASK_CTRL_REG3_I2_WTM     = 0x04;      // FIFO watermark interrupt on DRDY/INT2
    static const unsigned char _MASK_CTRL_REG3_I2_DRDY    = 0x08;      // Date-ready on DRDY/INT2
    static const unsigned char _MASK_CTRL_REG3_PP_OD      = 0x10;      // Push-pull / Open-drain
    static const unsigned char _MASK_CTRL_REG3_H_LACTIVE  = 0x20;      // Interrupt active configuration on INT1
    static const unsigned char _MASK_CTRL_REG3_I1_BOOT    = 0x40;      // Boot status available on INT1
    static const unsigned char _MASK_CTRL_REG3_I1_Int1    = 0x80;      // Interrupt enabled on INT1
    static const unsigned char _MASK_CTRL_REG4_SIM        = 0x01;      // SPI Serial interface selection
    static const unsigned char _MASK_CTRL_REG4_FS         = 0x30;      // Full scale selection
    static const unsigned char _MASK_CTRL_REG4_BLE        = 0x40;      // Big/little endian selection
    static const unsigned char _MASK_CTRL_REG4_BDU        = 0x80;      // Block data update
    static const unsigned char _MASK_CTRL_REG5_OUT_SEL    = 0x03;      // Out selection configuration
    static const unsigned char _MASK_CTRL_REG5_INT_SEL    = 0xc0;      // INT1 selection configuration
    static const unsigned char _MASK_CTRL_REG5_HPEN       = 0x10;      // High-pass filter enable
    static const unsigned char _MASK_CTRL_REG5_FIFO_EN    = 0x40;      // Fifo enable
    static const unsigned char _MASK_CTRL_REG5_BOOT       = 0x80;      // Reboot memory content
    static const unsigned char _MASK_STATUS_REG_ZYXOR     = 0x80;      // Z, Y, X axis overrun
    static const unsigned char _MASK_STATUS_REG_ZOR       = 0x40;      // Z axis overrun
    static const unsigned char _MASK_STATUS_REG_YOR       = 0x20;      // Y axis overrun
    static const unsigned char _MASK_STATUS_REG_XOR       = 0x10;      // X axis overrun
    static const unsigned char _MASK_STATUS_REG_ZYXDA     = 0x08;      // Z, Y, X data available
    static const unsigned char _MASK_STATUS_REG_ZDA       = 0x04;      // Z data available
    static const unsigned char _MASK_STATUS_REG_YDA       = 0x02;      // Y data available
    static const unsigned char _MASK_STATUS_REG_XDA       = 0x01;      // X data available
    static const unsigned char _MASK_FIFO_CTRL_REG_FM     = 0xe0;      // Fifo mode selection
    static const unsigned char _MASK_FIFO_CTRL_REG_WTM    = 0x1f;      // Fifo treshold - watermark level
    static const unsigned char _MASK_FIFO_SRC_REG_FSS     = 0x1f;      // Fifo stored data level
    static const unsigned char _MASK_FIFO_SRC_REG_EMPTY   = 0x20;      // Fifo empty bit
    static const unsigned char _MASK_FIFO_SRC_REG_OVRN    = 0x40;      // Overrun status
    static const unsigned char _MASK_FIFO_SRC_REG_WTM     = 0x80;      // Watermark status
    static const unsigned char _MASK_INT1_CFG_ANDOR       = 0x80;      // And/Or configuration of interrupt events
    static const unsigned char _MASK_INT1_CFG_LIR         = 0x40;      // Latch interrupt request
    static const unsigned char _MASK_INT1_CFG_ZHIE        = 0x20;      // Enable interrupt generation on Z high
    static const unsigned char _MASK_INT1_CFG_ZLIE        = 0x10;      // Enable interrupt generation on Z low
    static const unsigned char _MASK_INT1_CFG_YHIE        = 0x08;      // Enable interrupt generation on Y high
    static const unsigned char _MASK_INT1_CFG_YLIE        = 0x04;      // Enable interrupt generation on Y low
    static const unsigned char _MASK_INT1_CFG_XHIE        = 0x02;      // Enable interrupt generation on X high
    static const unsigned char _MASK_INT1_CFG_XLIE        = 0x01;      // Enable interrupt generation on X low
    static const unsigned char _MASK_INT1_SRC_IA          = 0x40;      // Int1 active
    static const unsigned char _MASK_INT1_SRC_ZH          = 0x20;      // Int1 source Z high
    static const unsigned char _MASK_INT1_SRC_ZL          = 0x10;      // Int1 source Z low
    static const unsigned char _MASK_INT1_SRC_YH          = 0x08;      // Int1 source Y high
    static const unsigned char _MASK_INT1_SRC_YL          = 0x04;      // Int1 source Y low
    static const unsigned char _MASK_INT1_SRC_XH          = 0x02;      // Int1 source X high
    static const unsigned char _MASK_INT1_SRC_XL          = 0x01;      // Int1 source X low
    static const unsigned char _MASK_INT1_THS_H           = 0x7f;      // MSB
    static const unsigned char _MASK_INT1_THS_L           = 0xff;      // LSB
    static const unsigned char _MASK_INT1_DURATION_WAIT   = 0x80;      // Wait number of samples or not
    static const unsigned char _MASK_INT1_DURATION_D      = 0x7f;      // Duration of int1 to be recognized
};

#endif // L3GD20_H
