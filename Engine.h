#ifndef ENGINE_H
#define ENGINE_H

#include "IPwmController.h"

class Engine
{
public:
    Engine(IPwmController* pwmController);
    ~Engine();
    void Init(unsigned char fwPwmChannel, unsigned char rwPwmChannel);
    void SetSpeedNorm(char percentValue);
    void Stop();
private:
    IPwmController* _pwmController;
    unsigned char _fwPwmChannel;
    unsigned char _rwPwmChannel;

};

#endif // ENGINE_H
