/**
*  @file app.cpp
*
*  Application class implementation.
*  Copyright © 2015 Anton B. Gusev aka AHTOXA
**/

#include "app.h"
#include "textbuf.h"

/**
 * Dynamic indication loop.
 */
OS_PROCESS void Application::RefreshLoop()
{
	int kbdCounter = 20;
	// restore brightness at startup
	digits_.SetBrightness(BKP->DR2);
	for (;;)
	{
		OS::sleep(1);

		// refresh LED every millisecond
		digits_.Refresh();

		// read keyboard every 20 milliseconds
		if (!--kbdCounter)
		{
			kbd_.Loop();
			kbdCounter = 20;
		}
	}
}

/**
 * Time update loop
 */
OS_PROCESS void Application::UpdateLoop()
{
	for (;;)
	{
		OS::sleep(100);
		UpdateTime();
	}
}

/**
 * User interface loop.
 */
OS_PROCESS void Application::UserInterfaceLoop()
{
	int repeats = 0;
	const int FAST_LEVEL = 10;

	ButtonUp::Mode(INPUTPULLED);
	ButtonUp::PullUp();
	ButtonDown::Mode(INPUTPULLED);
	ButtonDown::PullUp();
	for (;;)
	{
		int keyCode = kbd_.GetChar(10000);
		if (keyCode == -1)
			continue;
		if (keyCode == (BUTTON_UP | BUTTON_DOWN))
		{
			if (EditBrightness())
				EditCorrection();
			continue;
		}

		time_t t = (rtc_.ReadTime() / 60) * 60; // cut off seconds
		if (keyCode & BUTTON_REPEAT)
		{
			if (repeats < FAST_LEVEL)
				repeats++;
		}
		else
			repeats = 0;
		time_t step = repeats < FAST_LEVEL ? 60 : 5 * 60;
		keyCode &= ~BUTTON_REPEAT;
		if (keyCode == BUTTON_UP)
			SetTime(t + step);
		else if (keyCode == BUTTON_DOWN)
			SetTime(t - step);
		else if (keyCode == (BUTTON_UP | BUTTON_DOWN))
		{
			uint8_t b = digits_.GetBrightness() + 1;
			if (b > 16)
				b = 4;
			digits_.SetBrightness(b);
		}
	}
}

/**
 * Update time.
 * Lock mutex, then draw current time.
 * Mutex is used to block updates when user enters menu.
 */
void Application::UpdateTime()
{
	OS::TMutexLocker lock(displayMutex_);

	time_t t = rtc_.ReadTime();
	struct tm stm;
	localtime_r(&t, &stm);

	TextBuffer<5> tmpBuf;

	if (stm.tm_hour < 10)
		tmpBuf << '0';
	tmpBuf << stm.tm_hour;
	if (stm.tm_min < 10)
		tmpBuf << '0';
	tmpBuf << stm.tm_min;

	digits_.SetText(tmpBuf);
	digits_.SetColon(t & 1);
}

/**
 * Set time.
 */
void Application::SetTime(time_t t)
{
	TCritSect cs;
	rtc_.WriteTime(t);
}

/**
 * Edit brightness.
 * Lock mutex, then edit brightness with LEFT/RIGHT buttons.
 * Mutex is used to block time updates.
 * @return true if user pressed LEFT+RIGHT, false otherwise.
 */
bool Application::EditBrightness()
{
	OS::TMutexLocker lock(displayMutex_);

	for (;;)
	{
		// Display current brightness: "br:02"
		TextBuffer<5> tmpBuf;
		tmpBuf << "br";
		uint8_t brightness = digits_.GetBrightness();
		if (brightness < 10)
			tmpBuf << '0';
		tmpBuf << brightness;
		digits_.SetText(tmpBuf);
		digits_.SetColon(true);

		// get user input
		int keyCode = kbd_.GetChar(10000);
		if (keyCode == -1)
			return false;
		if (keyCode == (BUTTON_UP | BUTTON_DOWN))
			return true;
		keyCode &= ~BUTTON_REPEAT;
		if (keyCode == BUTTON_UP)
			digits_.SetBrightness(brightness + 1);
		else if (keyCode == BUTTON_DOWN)
			digits_.SetBrightness(brightness - 1);
	}
}

/**
 * Edit time correction.
 * Lock mutex, then edit correction with LEFT/RIGHT buttons.
 * Mutex is used to block time updates.
 * @return true if user pressed LEFT+RIGHT, false otherwise.
 */
bool Application::EditCorrection()
{
	OS::TMutexLocker lock(displayMutex_);

	for (;;)
	{
		// Display current time correction: "c002"
		TextBuffer<5> tmpBuf;
		tmpBuf << "c";
		uint8_t corr = rtc_.GetCorrection();
		if (corr < 100)
			tmpBuf << '0';
		if (corr < 10)
			tmpBuf << '0';
		tmpBuf << corr;
		digits_.SetText(tmpBuf);
		digits_.SetColon(false);

		int keyCode = kbd_.GetChar(10000);
		if (keyCode == -1)
			return false;
		if (keyCode == (BUTTON_UP | BUTTON_DOWN))
			return true;
		keyCode &= ~BUTTON_REPEAT;
		if (keyCode == BUTTON_UP)
			rtc_.SetCorrection(corr + 1);
		else if (keyCode == BUTTON_DOWN)
			rtc_.SetCorrection(corr - 1);
	}
}

