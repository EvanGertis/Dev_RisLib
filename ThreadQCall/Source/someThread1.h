#ifndef _SOMETHREAD1_H_
#define _SOMETHREAD1_H_

/*==============================================================================
File: someThread1.h
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

class  Thread1 : public Ris::Threads::BaseQCallThreadEx
{
public:
   typedef Ris::Threads::BaseQCallThreadEx BaseClass;

   Thread1();

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
       
#ifdef _SOMETHREAD1_CPP_
          Thread1* gThread1;
#else
   extern Thread1* gThread1;
#endif

//******************************************************************************
}//namespace


#endif

