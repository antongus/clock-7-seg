/**
*  @file hc595.h
*
*  Created on: 19.02.2012
*  Copyright (c) Anton Gusev aka AHTOXA
**/

#ifndef HC595_H_
#define HC595_H_

#include "pin.h"

struct Hc595SampleProps
{
	typedef Pin<'A', 3, 'H'> CLK;     // pin 12 of 595
	typedef Pin<'A', 4, 'L'> LATCH;   // pin 11
	typedef Pin<'A', 5, 'H'> DATA;    // pin 14

	enum { CHIP_COUNT = 2 };
};

template <typename Props = Hc595SampleProps>
class Hc595
{
public:
	enum { CHIP_COUNT = Props::CHIP_COUNT };
private:
	typedef typename Props::CLK   CLK;
	typedef typename Props::LATCH LATCH;
	typedef typename Props::DATA  DATA;

public:
	Hc595()
	{
		CLK::Mode(OUTPUT); CLK::Off();
		LATCH::Mode(OUTPUT); LATCH::On();
		DATA::Mode(OUTPUT); DATA::Off();
	}
	void Write();
	uint8_t& operator[](const int index) { return buf_[index]; }
	uint8_t buf_[CHIP_COUNT];
};

template <typename Props>
void Hc595<Props>::Write()
{
	LATCH::On();
	for (unsigned i = 0; i < CHIP_COUNT; i++)
	{
		uint8_t byte = buf_[i];
		for (unsigned bit = 0; bit < 8; bit++)
		{
			if (byte & 0x80)
				DATA::On();
			else
				DATA::Off();
			CLK::Off();
			byte <<= 1;
			CLK::On();
		}
	}
	LATCH::Off();
}


#endif /* HC595_H_ */
