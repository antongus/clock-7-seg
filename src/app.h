/**
*  @file app.h
*
*  Application class.
*  Copyright © 2015 Anton B. Gusev aka AHTOXA
**/


#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "hw.h"
#include "digits.h"
#include <scmRTOS.h>

class Application
{
public:
	OS_PROCESS void RefreshLoop();
	OS_PROCESS void UpdateLoop();
	OS_PROCESS void UserInterfaceLoop();
private:
	DigitalIndicator digits_;
	Rtc rtc_;
	Kbd kbd_;
	OS::TMutex displayMutex_;

	void UpdateTime();
	void SetTime(time_t t);
	bool EditBrightness();
	bool EditCorrection();
};

#endif // APP_H_INCLUDED
