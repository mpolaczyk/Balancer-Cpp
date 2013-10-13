#include <iostream>
#include <time.h>
#include <sys/time.h>

#include "L3GD20.h"

using namespace std;

int getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

int main()
{
    L3GD20 *gyro = new L3GD20(0x6b, "/dev/i2c-1");
    gyro->Set_DataRateAndBandWidth(95, 12.5);
    gyro->Set_AxisEnabled(L3GD20::Axis::X, true);
    gyro->Set_AxisEnabled(L3GD20::Axis::Y, true);
    gyro->Set_AxisEnabled(L3GD20::Axis::Z, true);
    gyro->Set_FullScale(L3GD20::FullScaleDps::TwoHundredFifty);
    gyro->Set_PowerMode(L3GD20::PowerMode::Normal);
    gyro->PrintConfig();
    gyro->Init();
    gyro->Calibrate(true);

    double x = 0;     // Angle around X axis [?]
    double dt = 0.02; // Delta time [s]

    while(1)
    {
        // Calculate integral
        x += (gyro->Get_CalOutX() * dt);

        // Print
        cout << x <<"\r\n";

        // Wait
        usleep(dt*100000);
    }

    delete gyro;

    return 0;
}

