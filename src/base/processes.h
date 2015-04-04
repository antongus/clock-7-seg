/**
*  @file processes.h
*
*  Declarations for all processes in project
*
*  Created on: 11.04.2011
*  Copyright (c) 2010 by Anton Gusev aka AHTOXA
**/

#ifndef PROCESSES_H_INCLUDED
#define PROCESSES_H_INCLUDED

#include <scmRTOS.h>

typedef OS::process<OS::pr0, 600> TLedRefreshProcess;
typedef OS::process<OS::pr1, 600> TUpdateTimeProcess;
typedef OS::process<OS::pr2, 600> TInputProcess;

extern TLedRefreshProcess LedRefreshProcess;
extern TUpdateTimeProcess UpdateTimeProcess;
extern TInputProcess InputProcess;

#endif // PROCESSES_H_INCLUDED
