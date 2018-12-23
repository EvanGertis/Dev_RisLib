/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someThreadParms.h"

#define  _TESTQCALLTHREAD_CPP_
#include "someTestQCallThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TestQCallThread::TestQCallThread()
{
   // Set base class thread variables.
   BaseClass::setThreadName("TestQCall");
   BaseClass::setThreadPrintLevel(TS::PrintLevel(3, 3));
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mTest1QCall.bind(this, &TestQCallThread::executeTest1);
   mTest2QCall.bind(this, &TestQCallThread::executeTest2);
   mTest3QCall.bind(this, &TestQCallThread::executeTest3);
   mTest4QCall.bind(this, &TestQCallThread::executeTest4);

   // Initialize variables.
   mTPFlag = false;
   mStatusCount1=0;
   mStatusCount2=0;
}

TestQCallThread::~TestQCallThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running.

void TestQCallThread::threadInitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void  TestQCallThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void TestQCallThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
   Prn::print(Prn::View11, "StatusCount %10d", mStatusCount1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test qcall function. 

void TestQCallThread::executeTest1(int aTimeout)
{
   Prn::print(Prn::View11, "Test1 BEGIN");

   try
   {
      mNotify.setMaskOne("label1",1);
      mNotify.wait(aTimeout);
   }
   catch (int aException)
   {
      Prn::print(Prn::View11, "EXCEPTION TestQCallThread::executeTest1 %d %s", aException,mNotify.mException);
   }

   Prn::print(Prn::View11, "Test1 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test qcall function. 

void TestQCallThread::executeTest2(int aTimeout)
{
   Prn::print(Prn::View11, "Test2 BEGIN");

   try
   {
      mNotify.setMaskAny("label1", 2, 1, 2);
      mNotify.wait(aTimeout);
   }
   catch (int aCode)
   {
      Prn::print(Prn::View11, "EXCEPTION TestQCallThread::executeTest2 %d %s", aCode, mNotify.mException);
   }

   Prn::print(Prn::View11, "Test2 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test qcall function. 

void TestQCallThread::executeTest3(int aTimeout)
{
   Prn::print(Prn::View11, "Test3 BEGIN");

   try
   {
      mNotify.setMaskAll("label1", 2, 1, 2);
      mNotify.wait(aTimeout);
   }
   catch (int aCode)
   {
      Prn::print(Prn::View11, "EXCEPTION TestQCallThread::executeTest3 %d %s", aCode, mNotify.mException);
   }

   Prn::print(Prn::View11, "Test3 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test qcall function. 

void TestQCallThread::executeTest4(int aTimeout)
{
   Prn::print(Prn::View11, "Test4 BEGIN");

   try
   {
      Prn::print(Prn::View11, "Test4 Mark11");
      mNotify.setMaskOne("label1", 1);
      Prn::print(Prn::View11, "Test4 Mark12");
      mNotify.wait(aTimeout);
      Prn::print(Prn::View11, "Test4 Mark13");

      Prn::print(Prn::View11, "Test4 Mark21");
      mNotify.setMaskOne("label2", 2);
      Prn::print(Prn::View11, "Test4 Mark22");
      mNotify.wait(aTimeout);
      Prn::print(Prn::View11, "Test4 Mark23");
   }
   catch (int aCode)
   {
      Prn::print(Prn::View11, "EXCEPTION TestQCallThread::executeTest4 %d %s", aCode, mNotify.mException);
   }

   Prn::print(Prn::View11, "Test4 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace