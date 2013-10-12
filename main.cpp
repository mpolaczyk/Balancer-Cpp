#include <iostream>
#include "L3GD20.h"

using namespace std;

int main()
{
    L3GD20 *gyro = new L3GD20(0x6b, "/dev/i2c-1"); // Means: /dev/i2c-1

    std::cout << "DeviceId:\r\n";
    printBinary(gyro->Get_DeviceId_Value());
    std::cout << "\r\n";

    delete gyro;

    return 0;
}

