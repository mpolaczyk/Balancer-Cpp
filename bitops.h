#ifndef _BITOPS_H
#define _BITOPS_H

#include <iostream>

// Checks if bit on given position is set
static inline bool checkBit(unsigned char value, unsigned char position)
{
	return (value & (1 << position)) == (1 << position);
}

// Sets bit under position given
static inline unsigned char setBit(unsigned char value, unsigned char position)
{
	return value | (1 << position);
}

// Clears bit under position given
static inline unsigned char clearBit(unsigned char value, unsigned char position)
{
    return value & ~(1 << position);
}

// Chenges state of bit under given position
static inline unsigned char flipBit(unsigned char value, unsigned char position)
{
    return value ^ (1 << position);
}

// Checks if all bits are set under mask
static inline bool checkBits(unsigned char value, unsigned char mask)
{
    return (value & mask) == mask;
}

// Sets all bits under mask
static inline unsigned char setBits(unsigned char value, unsigned char mask)
{
	return value | mask;
}

// Clears all bits under mask
static inline unsigned char clearBits(unsigned char value, unsigned char mask)
{
	return value & (~mask);
}

// Flips all bits under mask
static inline unsigned char flipBits(unsigned char value, unsigned char mask)
{
	return value ^ mask;
}

// Inserts value under mask. Example:
// currentValue        = 0b00100101
// valueToSetUnderMask = 0b00000111
// mask                = 0b11100000
// result              = 0b11100101
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

// Gets value from under mask. Example:
// currentValue = 0b01100100
// mask         = 0b11110000
// result       = 0b00000110
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
		
static inline int TwosComplementToByte(unsigned char value)
{
    if (value >= 0 && value <= 127) { return value; }
    else { return value - 256; }
}

// Prints binary representation to standard output
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
