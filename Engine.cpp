#include "Engine.h"
#include "bitops.h"
#include "math.h"
#include <iostream>
#include "IPwmController.h"

using namespace::std;

Engine::Engine(IPwmController* pwmController)
{
    _pwmController = pwmController;

    Stop();
}

Engine::~Engine()
{
    Stop();
    delete _pwmController;
}

void Engine::Init(unsigned char fwPwmChannel, unsigned char rwPwmChannel)
{
    _fwPwmChannel = fwPwmChannel;
    _rwPwmChannel = rwPwmChannel;

    _pwmController->InitPwm();

    Stop();
}

void Engine::SetSpeedNorm(char percentValue)
{
    if(percentValue > 0)
    {
        _pwmController->SetPwm(_fwPwmChannel, 0);
        _pwmController->SetPwm(_rwPwmChannel, (unsigned char)percentValue);
    }
    else
    {
        _pwmController->SetPwm(_fwPwmChannel, (unsigned char)(-percentValue));
        _pwmController->SetPwm(_rwPwmChannel, 0);
    }
}

void Engine::Stop()
{
    _pwmController->SetPwm(_fwPwmChannel, 0);
    _pwmController->SetPwm(_rwPwmChannel, 0);
}

