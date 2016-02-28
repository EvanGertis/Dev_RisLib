#ifndef _SOMEQCALLTHREAD2_H_
#define _SOMEQCALLTHREAD2_H_

/*==============================================================================
File: someQCallThread2.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsQCallThreadEx.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class  QCallThread2 : public Ris::Threads::BaseQCallThreadEx
{
public:
   typedef Ris::Threads::BaseQCallThreadEx BaseClass;

   QCallThread2();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------

   void executeOnTimer(int aTimeCount);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // QCall, 101:

   Ris::Threads::QCall1<int>   mC101QCall;
   void executeC101 (int aX=0);

   Ris::Threads::QCall1<int>   mC102QCall;
   void executeC102 (int aX=0);
};

//******************************************************************************
// Global instance
       
#ifdef _SOMEQCALLTHREAD2_CPP_
          QCallThread2* gQCallThread2;
#else
   extern QCallThread2* gQCallThread2;
#endif

//******************************************************************************
}//namespace


#endif

