#ifndef _PORTABLECALLS_H_
#define _PORTABLECALLS_H_

/*==============================================================================
These calls are intended to be portable across different
software development environments and operating systems.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************

   //---------------------------------------------------------------------------
   // Ticks are in milliseconds

   void portableSleep(int aTicks);

   //---------------------------------------------------------------------------
   // This returns a pointer to the settings folder.
   // Use it like this:
   //
   //    char tFilePath[200];
   //    strcpy(tFilePath,Ris::portableGetSettingsDir());
   //    strcat(tFilePath,"GSettings.txt");

   const char* portableGetSettingsDir();

   //---------------------------------------------------------------------------
   // This returns a pointer to the home folder.
   // Use it like this:
   //
   //    char tFilePath[200];
   //    strcpy(tFilePath,Ris::portableGetHomeDir());
   //    strcat(tFilePath,"Data.txt");

   const char* portableGetHomeDir();

   //---------------------------------------------------------------------------
   // This returns a pointer to the current working directory.

   char* portableGetCurrentWorkingDir();

   //---------------------------------------------------------------------------
   // Wait for key pressed

   void  portableWaitForKbhit();
   void  portableAbortWaitForKbhit();
   bool  portableKbhit(void);

   //---------------------------------------------------------------------------
   // Get system time, nanoseconds

   long long int portableGetHiResCounter();
   long long int portableGetHiResFrequency();

   //---------------------------------------------------------------------------
   // strupr

   void portableStrupr(char* aString);

   //---------------------------------------------------------------------------
   // Console title

   void portableSetConsoleTitle(char* aTitle);

}//namespace

#endif

