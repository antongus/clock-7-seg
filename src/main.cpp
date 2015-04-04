#include "hw.h"
#include "processes.h"

TLedRefreshProcess LedRefreshProcess;
TUpdateTimeProcess UpdateTimeProcess;
TInputProcess InputProcess;

int main()
{
	OS::run();
}

namespace OS
{

template<>
OS_PROCESS void TLedRefreshProcess::exec()
{
	for (;;)
	{
		digits.Refresh();
		sleep(1);
	}
}

template<>
OS_PROCESS void TUpdateTimeProcess::exec()
{
	for (;;)
	{
		sleep(100);
		digits.UpdateTime();
	}
}

namespace
{

void SetTime(time_t t)
{
	TCritSect cs;
	rtc.WriteTime(t);
}

}

template<>
OS_PROCESS void TInputProcess::exec()
{
	int repeats = 0;
	const int FAST_LEVEL = 10;

	PinUp::Mode(INPUTPULLED);
	PinUp::PullUp();
	PinDown::Mode(INPUTPULLED);
	PinDown::PullUp();
	for (;;)
	{
		sleep(20);
		kbd.Loop();
		if (kbd.Keypressed())
		{
			int ret = kbd.GetChar();
			time_t t = (rtc.ReadTime() / 60) * 60; // cut off seconds
			if (ret & BUTTON_REPEAT)
			{
				if (repeats < FAST_LEVEL)
					repeats++;
			}
			else
				repeats = 0;
			time_t step = repeats < FAST_LEVEL ? 60 : 5 * 60;
			ret &= ~BUTTON_REPEAT;
			if (ret == BUTTON_UP)
				SetTime(t + step);
			else if (ret == BUTTON_DOWN)
				SetTime(t - step);
			else if (ret == (BUTTON_UP | BUTTON_DOWN))
			{
				uint8_t b = digits.GetBrightness() + 1;
				if (b > 16)
					b = 4;
				digits.SetBrightness(b);
			}
		}
	}
}

} // namespace OS
