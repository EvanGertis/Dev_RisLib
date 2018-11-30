#pragma once

/*==============================================================================
InputReader Services.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides global program console i/o facility.

class InputReader
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 400;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Handle.
   HANDLE mInputHandle;
   INPUT_RECORD mInputBuffer[128];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Input string.
   char mInputString[cMaxStringSize];
   int mInputLength;

   // Output string.
   char mOutputString[cMaxStringSize];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   InputReader();
   void resetVariables();

   // Initialize.
   void initialize();
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Run test loop.
   void doTestLoop1();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _CONSTRINGREADER_CPP_
          InputReader gInputReader;
#else
   extern InputReader gInputReader;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

