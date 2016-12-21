/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <windows.h> 
#include <stdio.h> 

#include "risThreadsProcess.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

static int mTimerPeriod = 10;

void setProcessTimerResolution(int aTimerPeriod)
{
   mTimerPeriod = aTimerPeriod;
}

int  getProcessTimerResolution()
{
   return mTimerPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void enterProcessHigh()
{
   // Set process priority class
   int status = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
   // Set process timer resolution to one millisecond
   timeBeginPeriod(mTimerPeriod);
}

//******************************************************************************

void exitProcess()
{
   timeEndPeriod(mTimerPeriod);
}

}//namespace
}//namespace

#if 0
int tPriorityClass = GetPriorityClass(GetCurrentProcess());
printf("GetPriorityClass %08X\n", tPriorityClass);
#endif
