#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>

#include "prnPrint.h"
#include "CmdLineExec.h"
#include "TableTestParms.h"

//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
void CmdLineExec::reset()
{
}

//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   gTableTestParms.reset();
   gTableTestParms.readSection("default");
   gTableTestParms.show();
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

