#pragma once

/*==============================================================================
Implements multiple text log files
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Log
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initializes the log files facility.

   void reset    ();
   bool openFile       (int aLogNum,char* aFileName);
   bool openFileAppend (int aLogNum,char* aFileName);

   void closeFile      (int aLogNum);
   void closeAllFiles ();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Write to log file

   void write (int aLogNum, const char* aFormat, ...);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

