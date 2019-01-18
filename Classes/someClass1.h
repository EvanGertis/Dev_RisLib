#ifndef _SOMECLASS1_H_
#define _SOMECLASS1_H_

/*==============================================================================
File: someClass1.h
Description:
QCall test Class class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "prnPrint.h"

namespace Some
{

   //******************************************************************************
//******************************************************************************
//******************************************************************************
class ParentClass1
{
public:
   void parent1()
	{
      Prn::print(0,"Parent1");
	}

   virtual void parent2()
	{
      Prn::print(0,"Parent2");
	}
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class Traits1
{
public:
   Traits1();
   static void print1();
   void print2();
   int mCode;
};

template <class Traits>
class Class1 : public ParentClass1
{
public:

   void test1()
	{
	   Traits::print1();
	}

   void test2()
	{
	   mTraits.print2();
	}

   Traits mTraits;

   void parent2()
	{
      Prn::print(0,"Class1::Parent2");
	}
};


//******************************************************************************
}//namespace


#endif

