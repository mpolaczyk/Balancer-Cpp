#include <iostream>
#include "L3GD20.h"

using namespace std;

int main()
{
    L3GD20 *gyro = new L3GD20(0x6b, "/dev/i2c-1"); // Means: /dev/i2c-1

    std::cout << "DeviceId:";
    printBinary(gyro->Get_DeviceId_Value());

    delete gyro;

    return 0;
}

