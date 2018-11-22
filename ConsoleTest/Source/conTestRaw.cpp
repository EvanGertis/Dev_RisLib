/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <conio.h>
#include <ctype.h>

#include "tsThreadServices.h"

#include "conReadWrite.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Raw input and output.

int readOneRaw()
{
   return _getch_nolock();
}

void writeOneRaw(int aChar)
{
   _putchar_nolock(aChar);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run a test loop that prints raw key codes.

void doTestLoopRaw()
{
   Prn::print(Prn::View11, "doTestLoopRaw****************************");
   while (true)
   {
      int tChar = 0;
      tChar = readOneRaw();
      if (tChar != 'n')
      {
         writeOneRaw(tChar);
      }
      else
      {
         writeOneRaw(tChar);
      }

      Prn::print(Prn::View11, "char %d", tChar);
      if (tChar == 'z')
      {
         Prn::print(Prn::View11, "escape");
         break;
      }
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
