#include <iostream>
#include <map>

#include "L3GD20.h"

using namespace std;

int main()
{
    L3GD20 *gyro = new L3GD20(0x6b, "/dev/i2c-1");

    // Device Id
    std::cout << "DeviceId: ";
    printBinary(gyro->Get_DeviceId());
    std::cout << "\r\n";

    // Power mode
    gyro->Set_PowerMode(L3GD20::PowerMode::Normal);
    std::cout << "Power mode: ";
    gyro->Get_PowerMode(true);
    std::cout << "\r\n";

    // Full scale
    gyro->Set_FullScale(L3GD20::FullScaleDps::TwoHundredFifty);
    std::cout << "Full scale: ";
    gyro->Get_FullScale(true);
    std::cout << "\r\n";

    // Axis enabled
    gyro->Set_AxisEnabled(L3GD20::Axis::X, true);
    gyro->Set_AxisEnabled(L3GD20::Axis::Y, true);
    gyro->Set_AxisEnabled(L3GD20::Axis::Z, true);
    gyro->Get_AxisEnbaled(L3GD20::Axis::X, true);
    std::cout << "\r\n";
    gyro->Get_AxisEnbaled(L3GD20::Axis::Y, true);
    std::cout << "\r\n";
    gyro->Get_AxisEnbaled(L3GD20::Axis::Z, true);
    std::cout << "\r\n";

    // Band width and data rate
    gyro->Set_DataRateAndBandWidth(95, 12.5);
    std::cout << "Band width: ";
    std::cout << gyro->Get_BandWidth();
    std::cout << "\r\n";
    std::cout << "Data rate: ";
    std::cout << gyro->Get_DataRate();
    std::cout << "\r\n";

    std::cout << "Frequency cut off: ";
    std::cout << gyro->Get_HighPassFrequencyCutOff();
    std::cout << "\r\n";

    delete gyro;

    return 0;
}

