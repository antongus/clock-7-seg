/**
 *  @file digits.h
 *
 *  7 segment digital indicator class.
 *  Copyright (c) Anton Gusev aka AHTOXA
 **/

#ifndef DIGITS_H_INCLUDED
#define DIGITS_H_INCLUDED

#include <stdint.h>
#include "hc595.h"

class DigitalIndicator
{
	enum
	{
		SEG_A =	(0x01 << 2),
		SEG_B =	(0x01 << 3),
		SEG_C =	(0x01 << 5),
		SEG_D =	(0x01 << 6),
		SEG_E =	(0x01 << 7),
		SEG_F =	(0x01 << 1),
		SEG_G =	(0x01 << 0),
		SEG_H =	(0x01 << 4),
		DOT =	SEG_H
	};

	enum
	{
		DIGIT_0 =  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F        ),
		DIGIT_1 =  (        SEG_B | SEG_C                                ),
		DIGIT_2 =  (SEG_A | SEG_B         | SEG_D | SEG_E         | SEG_G),
		DIGIT_3 =  (SEG_A | SEG_B | SEG_C | SEG_D                 | SEG_G),
		DIGIT_4 =  (        SEG_B | SEG_C                 | SEG_F | SEG_G),
		DIGIT_5 =  (SEG_A         | SEG_C | SEG_D         | SEG_F | SEG_G),
		DIGIT_6 =  (SEG_A         | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
		DIGIT_7 =  (SEG_A | SEG_B | SEG_C                                ),
		DIGIT_8 =  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
		DIGIT_9 =  (SEG_A | SEG_B | SEG_C | SEG_D         | SEG_F | SEG_G),
		DIGIT_C =  (                        SEG_D | SEG_E         | SEG_G),
		DIGIT_B =  (                SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
		DIGIT_R =  (                                SEG_E         | SEG_G),
	};

	enum { DIGIT_COUNT = 4 };
	enum { MAX_BRIGHTNESS = 16 };
public:
	DigitalIndicator();
	void SetText(char const* data);
	const uint8_t& operator[](const int index) { return arr_[index]; }
	void SetColon(bool val) { dot_ = val; }
	bool GetDot() { return dot_; }
	void Refresh();
	void SetBrightness(int val);
	int GetBrightness() { return 20 - refreshSteps_; }

private:
	Hc595<> HC595_;
	bool dot_;
	int refreshSteps_;
	int pos_;
	uint8_t arr_[DIGIT_COUNT];

	static const uint8_t digits[];
	uint8_t CharToCode(uint8_t c);
};

#endif // DIGITS_H_INCLUDED
