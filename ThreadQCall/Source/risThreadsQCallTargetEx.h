#ifndef _RISTHREADSQCALLTARGETEX_H_
#define _RISTHREADSQCALLTARGETEX_H_

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risLFBlockQueue.h"
namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseQCall;

class BaseQCallTargetEx
{
public:
   LFBlockQueue mCallQueue;

   virtual void postQCallAvailable()=0;
};


//******************************************************************************
}//namespace
}//namespace

#endif

