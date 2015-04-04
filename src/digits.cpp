/**
*  @file digits.cpp
*
*  Created on: 20.02.2012
*  Copyright (c) Anton Gusev aka AHTOXA
**/

#include "digits.h"
#include "ftoa.h"
#include "util.h"
#include "hw.h"
#include "textbuf.h"
#include <string.h>

DigitalIndicator::DigitalIndicator()
	: HC595_()
	, dot_(false)
{
	SetBrightness(BKP->DR2);
}

uint8_t DigitalIndicator::c_v(uint8_t c)
{
static const uint8_t digits[] =
	{ DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9 };
	if ('0' <= c && c <= '9')
	{
		c -= '0';
		return digits[c];
	}
	return 0;
}

void DigitalIndicator::SetValue(double value, int precision)
{
	char buf[12];
	uint8_t len = DIGIT_COUNT;

	// convert float to string
	ftoa(value, buf, precision);
	// add left spaces to fit width (and +1 to dot)
	pad(buf, len+1, ' ');

	// loop from end of string
	for (int i = strlen(buf)-1; i >= 0; i--)
	{
		int dot = false;
		// skip dot
		if (buf[i] == '.')
		{
			dot = true;
			i--;
		}
		uint8_t b = c_v(buf[i]);
		if (dot) b |= DOT;
		len--;
		arr_[len] = b;
		if (!len) break;
	}
}

void DigitalIndicator::SetBrightness(uint8_t val)
{
	if (val < 4) val = 4;
	if (val > 16) val = 16;
	brightness_ = val;
	BKP->DR2 = val;
}


void DigitalIndicator::UpdateTime()
{
    time_t t = rtc.ReadTime();
	struct tm stm;
	localtime_r(&t, &stm);

    TextBuffer<5> tmpBuf;

    if (stm.tm_hour < 10) tmpBuf << '0';
    tmpBuf << stm.tm_hour;
    if (stm.tm_min < 10) tmpBuf << '0';
    tmpBuf << stm.tm_min;

    arr_[0] = c_v(tmpBuf[0]);
    arr_[1] = c_v(tmpBuf[1]);
    arr_[2] = c_v(tmpBuf[2]);
    arr_[3] = c_v(tmpBuf[3]);
    SetDot(t & 1);
}

void DigitalIndicator::Refresh()
{
	if (pos_ < 4)
	{
		// select current digit (low level)
		HC595_[0] = ~(1 << pos_);
		// put current digit
		HC595_[1] = arr_[pos_];
		// draw dots (connected as 5th digit)
		if (GetDot() && (pos_ == 0)) HC595_[0] &= ~(1 << 4);
	}
	else
	{
		HC595_[1] = 0;
		HC595_[0] = 0xFF;
	}
	HC595_.Write();
	if (++pos_ >= GetBrightness()) pos_ = 0;
}
