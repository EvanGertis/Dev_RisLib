/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#define  _SOMECLASS4_CPP_
#include "someClass4.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

Class4A::Class4A()
{
   mCode1=101;
}

//******************************************************************************

void Class4A::show()
{
   Prn::print(0,"Class4A mCode1  %5d",mCode1);
}

void Class4A::constMethod() const
{

}

void Class4A::mutableMethod()
{

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

   
}//namespace