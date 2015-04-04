/**
 *  @file digits.cpp
 *
 *  7 segment digital indicator class implementation.
 *  Copyright (c) Anton Gusev aka AHTOXA
 **/

#include "stm32.h"
#include "digits.h"
#include "textbuf.h"

/**
 * Constructor
 */
DigitalIndicator::DigitalIndicator() :
		HC595_(), dot_(false)
{
}

/**
 * Convert character to 7-segment code.
 */
uint8_t DigitalIndicator::CharToCode(uint8_t c)
{
	static const uint8_t digits[] =
	{ DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7,
			DIGIT_8, DIGIT_9 };

	if ('0' <= c && c <= '9')
	{
		c -= '0';
		return digits[c];
	}
	if (c == 'c')
		return DIGIT_C;
	if (c == 'b')
		return DIGIT_B;
	if (c == 'r')
		return DIGIT_R;
	return 0;
}

/**
 * Set display brightness.
 */
void DigitalIndicator::SetBrightness(int val)
{
	// check value
	if (val > MAX_BRIGHTNESS)
		val = MAX_BRIGHTNESS;
	else if (val < 0)
		val = 0;

	// store brightness in battery-backed RAM
	PWR->CR |= PWR_CR_DBP;
	BKP->DR2 = val;
	PWR->CR &= ~PWR_CR_DBP;

	// adjust refresh steps
	refreshSteps_ = 20 - val;
}

/**
 * Set display text.
 * @param data: text to display. Should be at least 4 chars long.
 */
void DigitalIndicator::SetText(char const* data)
{
	for (int i = 0; i < DIGIT_COUNT; ++i)
		arr_[i] = CharToCode(data[i]);
}

/**
 * Refresh display (perform step of dynamic indication)
 */
void DigitalIndicator::Refresh()
{
	if (pos_ < 4)
	{
		// select current digit (low level)
		HC595_[0] = ~(1 << pos_);
		// put current digit
		HC595_[1] = arr_[pos_];
		// draw dots (connected as 5th digit)
		if (GetDot() && (pos_ == 0))
			HC595_[0] &= ~(1 << 4);
	}
	else // turn off all digits (adjust brightness)
	{
		HC595_[1] = 0;
		HC595_[0] = 0xFF;
	}
	HC595_.Write();
	if (++pos_ >= refreshSteps_)
		pos_ = 0;
}
