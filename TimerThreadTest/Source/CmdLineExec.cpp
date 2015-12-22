
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "CmdLineExec.h"

#include "risThreads.h"
#include "risTimeMarker.h"
#include "risPortableCalls.h"

#include "uviewTimeMarker.h"

#include "someTimerThread.h"
using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("TP"    ))  executeTP      (aCmd);
   if(aCmd->isCmd("T5"    ))  executeTest5   (aCmd);
   if(aCmd->isCmd("M1"    ))  executeMarker1 (aCmd);
   if(aCmd->isCmd("M2"    ))  executeMarker2 (aCmd);
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   gTimerThread->mTPFlag=aCmd->argBool(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest5(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1000);
   aCmd->setArgDefault(2,10);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeMarker1(Ris::CmdLineCmd* aCmd)
{
   Ris::TimeMarker tMarker;
   tMarker.doBegin();
   Ris::portableSleep(1000);
   tMarker.doEnd();
   Prn::print(0,"%s",tMarker.asString2());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeMarker2(Ris::CmdLineCmd* aCmd)
{
   long long tFreq  = Ris::portableGetHiResFrequency();
   long long tCount = Ris::portableGetHiResCounter();
   Prn::print(0,"%lld",tFreq);
   Prn::print(0,"%lld",tCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeMarker3(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"executeMarker3");
   UView::TimeMarker tMarker;
   tMarker.doStart();
   Ris::portableSleep(1000);
   tMarker.doStop();
   Prn::print(0,"ScaleFactorUS     %8.3f",tMarker.mScaleFactorUS);
   Prn::print(0,"TimeDifferenceUS  %8.3f",tMarker.mTimeDifferenceUS);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1 (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,100000);
   int tCount = aCmd->argInt(1);

   UView::TimeMarker tMarker;
   tMarker.initialize(tCount);

   while(true)
   {
      tMarker.doStart();
      tMarker.doStop();
      if (tMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, " %10.3f  %10.3f  %10.3f  %10.3f",
      tMarker.mStatistics.mMean,
      tMarker.mStatistics.mStdDev,
      tMarker.mStatistics.mMinX,
      tMarker.mStatistics.mMaxX);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2 (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,100000);
   int tCount = aCmd->argInt(1);

   UView::TimeMarker tMarker;
   tMarker.initialize(tCount);

   Ris::Threads::MutexSemaphore tMutex;

   while(true)
   {
      tMarker.doStart();

      tMutex.lock();
      tMutex.unlock();

      tMarker.doStop();
      if (tMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, " %10.3f  %10.3f  %10.3f  %10.3f",
      tMarker.mStatistics.mMean,
      tMarker.mStatistics.mStdDev,
      tMarker.mStatistics.mMinX,
      tMarker.mStatistics.mMaxX);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3 (Ris::CmdLineCmd* aCmd)
{
}



