#ifndef IPWMCONTROLLER_H
#define IPWMCONTROLLER_H

class IPwmController
{

public:
    void virtual SetPwmFreq(unsigned short freq) {}
    void virtual SetPwm(unsigned char channel, double fillFactor) {}
    void virtual InitPwm() {}

};

#endif // IPWMCONTROLLER_H
