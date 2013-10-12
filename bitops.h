#ifndef _BITOPS_H
#define _BITOPS_H

#include <iostream>

static inline bool checkBit(unsigned char value, unsigned char position)
{
	return (value & (1 << position)) == (1 << position);
}

static inline unsigned char setBit(unsigned char value, unsigned char position)
{
	return value | (1 << position);
}

static inline unsigned char clearBit(unsigned char value, unsigned char position)
{
    return value & ~(1 << position);
}

static inline unsigned char flipBit(unsigned char value, unsigned char position)
{
    return value ^ (1 << position);
}

static inline bool checkBits(unsigned char value, unsigned char mask)
{
    return (value & mask) == mask;
}

static inline unsigned char setBits(unsigned char value, unsigned char mask)
{
	return value | mask;
}

static inline unsigned char clearBits(unsigned char value, unsigned char mask)
{
	return value & (~mask);
}

static inline unsigned char flipBits(unsigned char value, unsigned char mask)
{
	return value ^ mask;
}

static inline unsigned char setValueUnderMask(unsigned char currentValue, unsigned char valueToSetUnderMask, unsigned char mask)
{
    unsigned char currentValueCleared = clearBits(currentValue, mask);
    char i = 0;
    while (mask % 2 == 0 && mask != 0x00)
	{
        mask = mask >> 1;
        i += 1;
	}
    return (valueToSetUnderMask << i) | currentValueCleared;
}

static inline unsigned char getValueUnderMask(unsigned char currentValue, unsigned char mask)
{
    unsigned char currentValueCleared = clearBits(currentValue, ~mask);
    char i = 0;
    while (mask % 2 == 0 && mask != 0)
	{
        mask = mask >> 1;
        i += 1;
	}
    return currentValueCleared >> i;

}

static inline unsigned char twosComplementToByte(unsigned char value)
{
    if (value >= 0 && value <= 127)
	{
        return value;
	}
    else
    {
		return value - 256;
	}
}
		
static inline void printBinary(const unsigned char value)
{
	for(int i = 7; i>=0;i--)
	{
		if(value &(1 << i))
		{
            std::cout << "1";
		}
		else
		{
            std::cout << "0";
		}
	}
}

#endif
