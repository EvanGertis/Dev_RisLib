#ifndef _PROGRAMTIME_H_
#define _PROGRAMTIME_H_
/*==============================================================================
Time since program start in seconds.
==============================================================================*/

#include <time.h>
#include "risPortableTypes.h"

//*****************************************************************************

namespace Ris
{
   // Read current time and subtract program start time.
   // This returns the current program time in seconds.
   double getCurrentProgramTime();

}//namespace
#endif

