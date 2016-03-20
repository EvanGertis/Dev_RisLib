#ifndef _LOGFILES_H_
#define _LOGFILES_H_

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
   bool openFile (int aLogNum,char* aFileName);
   void closeAllFiles ();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Write to log file

   void write (int aLogNum, const char* aFormat, ...);

}//namespace
#endif

