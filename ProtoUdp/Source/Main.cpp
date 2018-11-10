#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "procoNetworkThread.h"

#include "MainInit.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   main_initialize(argc,argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   ProtoComm::gNetworkThread = new ProtoComm::NetworkThread;
   ProtoComm::gNetworkThread->launchThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   TS::print(0, "ThreadInfo");
   ProtoComm::gNetworkThread->showThreadInfo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute user command line executive, wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(tExec);
   delete tExec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program threads.

   ProtoComm::gNetworkThread->shutdownThread();
   delete ProtoComm::gNetworkThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
// return 0;

   printf("press enter\n");
   getchar();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

