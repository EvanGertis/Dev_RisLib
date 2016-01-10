#ifndef _SOMETIMERTHREAD_H_
#define _SOMETIMERTHREAD_H_

/*==============================================================================
File: someThread1.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTimerThread.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TimerThread : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   // Constructor
   TimerThread();

   // Base class overloads
   void executeOnTimer(int aTimeCount);

   void showThreadInfo(int aTimeCount);

   // Members
   int mTimerFrequency;

};

//******************************************************************************
// Global instance

#ifdef _SOMETIMERTHREAD_CPP_
          TimerThread* gTimerThread;
#else
   extern TimerThread* gTimerThread;
#endif

//******************************************************************************
}//namespace


#endif

