#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include "L3GD20.h"
#include "LSM303DLHC.h"
#include "PCA9685.h"

#define PI 3.14159265
#define DEG_TO_RAD (PI/180)
#define RAD_TO_DEG (180/PI)

using namespace std;

int getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

void PWMDemo()
{
    PCA9685 *pwm = new PCA9685(0x40, "/dev/i2c-1");
    pwm->Init();


    pwm->PrintConfig();
    cout << endl;
    cout << endl;

    pwm->SetPWMFreq(1000);
    cout << endl;
    cout << endl;

    pwm->PrintConfig();
    cout << endl;
    cout << endl;

    double A = 360 * DEG_TO_RAD;
    double da = 1 * DEG_TO_RAD;
    double v = 0;
    double vv = 0;

    while(1)
    {
        for(double a = 0; a < A; a += da)
        {
            v = sin(a);
            vv = v * 4095;
            if(v>0)
            {
                pwm->SetPWM(15, 0, static_cast<unsigned short>(vv));
                pwm->SetPWM(14, 0, 0);
                pwm->SetPWM(13, 0, static_cast<unsigned short>(vv));
                pwm->SetPWM(12, 0, 0);
            }
            else
            {
                pwm->SetPWM(15, 0, 0);
                pwm->SetPWM(14, 0, static_cast<unsigned short>(-vv));
                pwm->SetPWM(13, 0, 0);
                pwm->SetPWM(12, 0, static_cast<unsigned short>(-vv));
            }
            sleep(0.5);
        }
    }

    //pwm->PrintConfig();
    //cout << endl;

}

void GyroDemo()
{
    L3GD20 *gyro = new L3GD20(0x6b, "/dev/i2c-1");
    gyro->Set_DataRateAndBandWidth(95, 12.5);
    gyro->Set_AxisEnabled(L3GD20::Axis::X, true);
    gyro->Set_AxisEnabled(L3GD20::Axis::Y, true);
    gyro->Set_AxisEnabled(L3GD20::Axis::Z, true);
    gyro->Set_FullScale(L3GD20::FullScaleDps::TwoThousand);
    gyro->Set_PowerMode(L3GD20::PowerMode::Normal);
    gyro->PrintConfig();
    gyro->Init();
    gyro->Calibrate(true);

    double x = 0;     // Angle around X axis [?]
    double y = 0;     // Angle around X axis [?]
    double z = 0;     // Angle around X axis [?]
    double dt = 0.02; // Delta time [s]

    while(1)
    {
        // Calculate integral
        x += (gyro->Get_CalOutX() * dt);
        y += (gyro->Get_CalOutY() * dt);
        z += (gyro->Get_CalOutZ() * dt);

        // Print
        cout << x << " " << y << " " << z << "\r\n";

        // Wait
        usleep(dt*1000000);
    }

    delete gyro;
}

void AccDemo()
{
    LSM303DLHC *acc = new LSM303DLHC(0x19, "/dev/i2c-1");
    acc->Set_AxisEnabled(LSM303DLHC::Axis::X, true);
    acc->Set_AxisEnabled(LSM303DLHC::Axis::Y, true);
    acc->Set_AxisEnabled(LSM303DLHC::Axis::Z, true);
    acc->Set_FullScale(LSM303DLHC::FullScaleN::TwoG);
    acc->Set_PowerMode(LSM303DLHC::PowerMode::Normal);
    acc->Set_DataRate(1344000);
    acc->PrintConfig();
    acc->Init();
    //acc->Calibrate();

    double x = 0;
    double y = 0;
    double z = 0;
    double dt = 0.1;

    while(1)
    {
        x = acc->Get_CalOutX();
        y = acc->Get_CalOutY();
        z = acc->Get_CalOutZ();

        cout << x << " " << y << " " << z << "\r\n";
        cout << atan2(z * DEG_TO_RAD, y * DEG_TO_RAD) * RAD_TO_DEG << "\r\n";
        cout << "\033[F" << "\033[F" << std::flush;

        usleep(dt*1000000);
    }

    delete acc;
}

int main()
{
    PWMDemo();

    return 0;
}

