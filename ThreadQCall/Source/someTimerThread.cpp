/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someShare.h"
#include "someThread1.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

TimerThread::TimerThread()
{
   // Set base class thread priority

   // Set timer period
   int mTimerFrequency = 50;
   BaseClass::mTimerPeriod = 1000 / mTimerFrequency;

   gShare.mTimeMarker.initialize(5*mTimerFrequency);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void TimerThread::executeOnTimer(int aTimeCount)
{
   if (aTimeCount < mTimerFrequency) return;

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TESTQ %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
         gShare.mTimeMarker.mStatistics.mEndOfPeriod = false;
         return;
   }

   gShare.mTimeMarker.doStart();
   gThread1->mC101QCall.invoke(aTimeCount);
}

}//namespace