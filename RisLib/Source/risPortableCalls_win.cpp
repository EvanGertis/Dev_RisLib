/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h> 
#include <direct.h> 
#include <string.h> 
#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <io.h>

#include "risPortableCalls.h"

namespace Ris
{

//******************************************************************************
// Regionals

bool rAbortWaitForKbhit=false;

//******************************************************************************
//******************************************************************************
//******************************************************************************

void portableSleep(int aTicks)
{
   Sleep(aTicks);  
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

const char* portableGetSettingsDir()
{
   return "C:\\Alpha\\Settings\\";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rProgramDir[400];

char* portableGetProgramDir()
{
   int tIndex = GetModuleFileName(NULL, rProgramDir, 400);

   while (--tIndex > 0)
   {
      if (rProgramDir[tIndex] == '\\')
      {
         rProgramDir[tIndex + 1] = 0;
         break;
      }
   }

   return rProgramDir;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rCurrentDir[400];

char* portableGetCurrentDir()
{
   GetCurrentDirectory(400,rCurrentDir);
   strcat(rCurrentDir,"\\");
   return rCurrentDir;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void portableWaitForKbhit()
{
   while(!kbhit())
   {
      if (rAbortWaitForKbhit) return;;
      Sleep(100);
   }
}

bool portableKbhit()
{
   return kbhit()!=0;
}


//******************************************************************************

long long int portableGetHiResCounter()
{
   LARGE_INTEGER tPerformanceCount;
   QueryPerformanceCounter(&tPerformanceCount);
   return tPerformanceCount.QuadPart;
}

long long int portableGetHiResFrequency()
{
   LARGE_INTEGER tPerformanceFrequency;
   QueryPerformanceFrequency(&tPerformanceFrequency);
   return tPerformanceFrequency.QuadPart;
}

//******************************************************************************
void portableStrupr(char* aString)
{
   strupr(aString);
}
//******************************************************************************

void portableSetConsoleTitle(char* aTitle)
{
   SetConsoleTitle(aTitle);
}

//******************************************************************************

bool portableFilePathExists (char* aFilePath)
{
   return  _access(aFilePath, 0) == 0;
}

}//namespace

