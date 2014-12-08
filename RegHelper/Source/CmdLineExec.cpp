
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "prnPrint.h"

#include "regHelper.h"

#include "CmdLineExec.h"

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
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
}

//******************************************************************************
void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
    unsigned int   tT32 = 0;
    unsigned short tT16 = 0;
    unsigned char  tT08 = 0;

    Reg::writeBitAtMask32(&tT32, 1, 1);
    Reg::writeBitAtMask16(&tT16, 2, 1);
    Reg::writeBitAtMask08(&tT08, 4, 1);

    printf("%d %08X\n", Reg::readBitAtMask32(&tT32, 1), tT32);
    printf("%d %04X\n", Reg::readBitAtMask16(&tT16, 2), tT16);
    printf("%d %02X\n", Reg::readBitAtMask08(&tT08, 4), tT08);
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
    unsigned int   tT32 = 0;
    unsigned short tT16 = 0;
    unsigned char  tT08 = 0;

    Reg::writeBit32(&tT32, 0, 1);
    Reg::writeBit16(&tT16, 1, 1);
    Reg::writeBit08(&tT08, 2, 1);

    printf("%d %08X\n", Reg::readBit32(&tT32, 0), tT32);
    printf("%d %04X\n", Reg::readBit16(&tT16, 1), tT16);
    printf("%d %02X\n", Reg::readBit08(&tT08, 2), tT08);
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
    unsigned int   tT32 = 0x000D8000;

    printf("%08X %08X\n", Reg::readBits32(&tT32, 16, 3), tT32);
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

