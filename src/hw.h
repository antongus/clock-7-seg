#ifndef HW_H_INCLUDED
#define HW_H_INCLUDED

#include "pin.h"

typedef Pin<'B', 8, 'L'> PinUp;
typedef Pin<'B', 9, 'L'> PinDown;

#include "rtc.h"
typedef RtcModule<> Rtc;
extern Rtc rtc;

#include "kbd.h"
typedef Keyboard<8> kbd_t;
extern kbd_t kbd;

#include "digits.h"
extern DigitalIndicator digits;

enum
{
	BUTTON_UP = 0x01,
	BUTTON_DOWN = 0x02
};

template <int buf_size>
uint16_t Keyboard<buf_size>::ReadInput()
{
	uint16_t ret = 0;
	if (PinUp::Signalled()) ret |= BUTTON_UP;
	if (PinDown::Signalled()) ret |= BUTTON_DOWN;
	return ret;
}

#endif // HW_H_INCLUDED
