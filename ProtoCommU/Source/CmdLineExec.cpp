
#include "stdafx.h"

#include "procoSettings.h"
#include "procoMsg.h"
#include "procoMsgHelper.h"

#include "procoNetworkThread.h"

#include "CmdLineExec.h"

using namespace ProtoComm;

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}
void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("SHUTDOWN"))  executeShutdown (aCmd);
   if (aCmd->isCmd("TP"))        ProtoComm::gNetworkThread->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("TX"))        executeTx   (aCmd);
   if (aCmd->isCmd("GO1"))       executeGo1  (aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2  (aCmd);
   if (aCmd->isCmd("GO3"))       executeGo3  (aCmd);
   if (aCmd->isCmd("GO4"))       executeGo4  (aCmd);
   if (aCmd->isCmd("GO5"))       executeGo5  (aCmd);
   if (aCmd->isCmd("GO6"))       executeGo6  (aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeShutdown (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTx (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tMsgType= aCmd->argInt(1);

   switch (tMsgType)
   {
      case 1:
      {
         ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
         MsgHelper::initialize(tMsg);
         gNetworkThread->sendMsg(tMsg);
         break;
      }
      case 5:
      {
         ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
         MsgHelper::initialize(tMsg);
         gNetworkThread->sendMsg(tMsg);
         break;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1 (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->sendTestMsg();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::StatusRequestMsg* tMsg = new ProtoComm::StatusRequestMsg;

   gNetworkThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::TestMsg* tTxMsg = new ProtoComm::TestMsg;
   ProtoComm::TestMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ProtoComm::TestMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataMsg* tTxMsg = new ProtoComm::DataMsg;
   ProtoComm::DataMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ProtoComm::DataMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataRecord* tTxMsg = new ProtoComm::DataRecord;
   ProtoComm::DataRecord* tRxMsg = new ProtoComm::DataRecord;
   MsgHelper::initialize(tTxMsg);

   tBuffer.putToBuffer((Ris::ByteContent*)tTxMsg);
   printf("Buffer1 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tBuffer.rewind();
   printf("Buffer2 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tBuffer.getFromBuffer((Ris::ByteContent*)tRxMsg);
   printf("Buffer3 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
   MsgHelper::initialize(tMsg);

   gNetworkThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::gSettings.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

