#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include "L3GD20.h"
#include "LSM303DLHC.h"
#include "PCA9685.h"
#include "Engine.h"
#include "Joystick.h"

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

void JoystickDemo()
{
    Joystick* joy = new Joystick("/dev/input/js0");

    while(1)
    {
        joy->Read();

        cout << "Buttons: |";
        cout << joy->Button[0] << "|";
        cout << joy->Button[1] << "|";
        cout << joy->Button[2] << "|";
        cout << joy->Button[3] << "|";
        cout << joy->Button[4] << "|";
        cout << joy->Button[5] << "|";
        cout << joy->Button[6] << "|";
        cout << joy->Button[7] << "|";
        cout << joy->Button[8] << "|";
        cout << joy->Button[9] << "|";
        cout << endl << flush;

        cout << "Axises: |";
        cout << joy->Axis[0] << "|";
        cout << joy->Axis[1] << "|";
        cout << joy->Axis[2] << "|";
        cout << joy->Axis[3] << "|";
        cout << joy->Axis[4] << "|";
        cout << joy->Axis[5] << "|";
        cout << endl << flush;

        usleep(0.001*1000000);
    }
}

void EngineDemo()
{
    PCA9685* pwm = new PCA9685(0x40, "/dev/i2c-1");
    Engine* engineL = new Engine(pwm);
    Engine* engineR = new Engine(pwm);

    engineL->Init(15, 14);
    engineR->Init(13, 12);

    double alphaMax = 360 * DEG_TO_RAD;
    double dalpha = 1 * DEG_TO_RAD;

    while(1)
    {
        for (double alpha = 0; alpha < alphaMax; alpha += dalpha)
        {
            engineL->SetSpeedNorm(static_cast<double>(100*sin(alpha)));

            engineR->SetSpeedNorm(static_cast<double>(100*sin(alpha)));

            usleep(0.001*1000000);
        }
        engineL->Stop();
        sleep(2);
    }


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
    JoystickDemo();

    return 0;
}

