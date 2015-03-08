#ifndef _CMDLINEEXEC_H_
#define _CMDLINEEXEC_H_

#include "risCmdLineExec.h"

//******************************************************************************
// specific command line sequencer
//******************************************************************************
class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

  CmdLineExec();

  void reset();
  void execute(Ris::CmdLineCmd* aCmd);

  void executeTP      (Ris::CmdLineCmd* aCmd);
  void executeTest5   (Ris::CmdLineCmd* aCmd);
  void executeMarker1 (Ris::CmdLineCmd* aCmd);
  void executeMarker2 (Ris::CmdLineCmd* aCmd);
  void executeMarker3 (Ris::CmdLineCmd* aCmd);

};
//******************************************************************************
#endif

