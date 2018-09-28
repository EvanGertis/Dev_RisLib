/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risSockets.h"
#include "procoSettings.h"
#include "procoMsgHelper.h"

#define  _PROCONETWORKTHREAD_CPP_
#include "procoNetworkThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

NetworkThread::NetworkThread()
{
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mRxMsgQCall.bind(this, &NetworkThread::executeRxMsg);

   // Initialize variables.
   mUdpMsgThread = 0;
   mMonkeyCreator.configure(gSettings.mMyAppNumber);
   mTPFlag = false;
   mStatusCount1=0;
   mStatusCount2=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

NetworkThread::~NetworkThread()
{
   delete mUdpMsgThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.

void NetworkThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalIp  (gSettings.mMyUdpIPAddress,    gSettings.mMyUdpPort);
   tSettings.setRemoteIp (gSettings.mOtherUdpIPAddress, gSettings.mOtherUdpPort);
   tSettings.mMonkeyCreator = &mMonkeyCreator;
   tSettings.mRxMsgQCall = mRxMsgQCall;

   // Create the child thread with the settings.
   mUdpMsgThread = new Ris::Net::UdpMsgThread(tSettings);

   // Launch the child thread.
   mUdpMsgThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void  NetworkThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::threadExitFunction");

   // Shutdown the child thread.
   mUdpMsgThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mUdpMsgThread child thread. It is invoked when
// a message is received. It process the received messages.

void NetworkThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case ProtoComm::MsgIdT::cTestMsg :
         processRxMsg((ProtoComm::TestMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cStatusRequestMsg :
         processRxMsg((ProtoComm::StatusRequestMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cStatusResponseMsg :
         processRxMsg((ProtoComm::StatusResponseMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cDataMsg :
         processRxMsg((ProtoComm::DataMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxMsg ??? %d",tMsg->mMessageType);
         delete tMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg.

void NetworkThread::processRxMsg(ProtoComm::TestMsg*  aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusRequestMsg

void NetworkThread::processRxMsg(ProtoComm::StatusRequestMsg* aMsg)
{
   if (true)
   {
      ProtoComm::StatusResponseMsg* tMsg = new ProtoComm::StatusResponseMsg;
      mUdpMsgThread->sendMsg(tMsg);
   }

   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusResponseMsg

void NetworkThread::processRxMsg(ProtoComm::StatusResponseMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg

void NetworkThread::processRxMsg(ProtoComm::DataMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child socket thread.

void NetworkThread::sendMsg (ProtoComm::BaseMsg* aMsg)
{
   mUdpMsgThread->sendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child socket thread.

void NetworkThread::sendTestMsg()
{
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
 
   mUdpMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Periodic timer execution, base class overload.

void NetworkThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;

   // Send a test message.
   ProtoComm::TestMsg* tx = new ProtoComm::TestMsg;
   tx->mCode1 = aTimerCount;
   mUdpMsgThread->sendMsg(tx);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace