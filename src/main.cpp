/**
*  @file main.cpp
*
*  Main function, application and process objects.
*  Copyright © 2011-2015 Anton B. Gusev aka AHTOXA
**/

#include "hw.h"
#include "processes.h"
#include "app.h"

Application app;

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
	app.RefreshLoop();
}

template<>
OS_PROCESS void TUpdateTimeProcess::exec()
{
	app.UpdateLoop();
}

template<>
OS_PROCESS void TInputProcess::exec()
{
	app.UserInterfaceLoop();
}

} // namespace OS
