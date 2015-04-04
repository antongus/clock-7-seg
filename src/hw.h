/**
*  @file hw.h
*
*  Project hardware definitions.
*  Copyright © 2015 Anton B. Gusev aka AHTOXA
**/

#ifndef HW_H_INCLUDED
#define HW_H_INCLUDED

/**
 * Keyboard buttons
 */
#include "pin.h"

typedef Pin<'B', 8, 'L'> ButtonUp;
typedef Pin<'B', 9, 'L'> ButtonDown;

/**
 * Real-time clock module
 */
#include "rtc.h"
typedef RtcModule<> Rtc;

/**
 * Keyboard
 */
#include "kbd.h"
typedef Keyboard<8> Kbd;

enum
{
	BUTTON_UP = 0x01,
	BUTTON_DOWN = 0x02
};

template <int buf_size>
uint16_t Keyboard<buf_size>::ReadInput()
{
	uint16_t ret = 0;
	if (ButtonUp::Signalled()) ret |= BUTTON_UP;
	if (ButtonDown::Signalled()) ret |= BUTTON_DOWN;
	return ret;
}

#endif // HW_H_INCLUDED
