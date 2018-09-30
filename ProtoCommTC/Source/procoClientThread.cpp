/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoSettings.h"

#define  _PROCOCLIENTTHREAD_CPP_
#include "procoClientThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ClientThread::ClientThread()
{
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mTimerPeriod = 1000;

   // Initialize qcalls.
   mSessionQCall.bind(this, &ClientThread::executeSession);
   mRxMsgQCall.bind(this, &ClientThread::executeRxMsg);

   // Initialize variables.
   mTcpMsgClientThread = 0;
   mMonkeyCreator.configure(gSettings.mMyAppNumber);
   mTPFlag = false;
   mStatusCount1 = 0;
   mStatusCount2 = 0;
}

ClientThread::~ClientThread()
{
   delete mTcpMsgClientThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void ClientThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setRemoteIp(gSettings.mTcpServerIPAddress, gSettings.mTcpServerPort);
   tSettings.mMonkeyCreator = &mMonkeyCreator;
   tSettings.mClientSessionQCall = mSessionQCall;
   tSettings.mRxMsgQCall = mRxMsgQCall;

   // Create the child thread with the settings.
   mTcpMsgClientThread = new Ris::Net::TcpMsgClientThread(tSettings);

   // Launch the child thread.
   mTcpMsgClientThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void  ClientThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::threadExitFunction");

   // Shutdown the child thread.
   mTcpMsgClientThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a session is established or disestablished (when this client connects or
// disconnects to the server). It maintains session state variables. When
// a connection is established it sends a FirstMessage to the server to
// inform it of it's identity.

void ClientThread::executeSession (bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::ThreadRun1, "ClientThread::notifyForSession CONNECTED");

      // Transmit a FirstMessage to the server to inform it of who this 
      // client is.
      FirstMessageMsg* msg = new FirstMessageMsg;
      sendMsg(msg);
   }
   else
   {
      Prn::print(Prn::ThreadRun1, "ClientThread::notifyForSession DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a message is received. It process the received messages. It calls one of
// the specific receive message handlers.

void ClientThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   if(!aMsg) return;

   BaseMsg* tMsg = (BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case MsgIdT::cTestMsg :
         processRxMsg((TestMsg*)tMsg);
         break;
      case MsgIdT::cStatusRequestMsg :
         processRxMsg((StatusRequestMsg*)tMsg);
         break;
      case MsgIdT::cStatusResponseMsg :
         processRxMsg((StatusResponseMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg %d",tMsg->mMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg

void ClientThread::processRxMsg(TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg_TestMsg");
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusRequestMsg

void ClientThread::processRxMsg(StatusRequestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun2, "ClientThread::processRxMsg_StatusRequestMsg %d",mStatusCount1++);

   if (true)
   {
      StatusResponseMsg* tTxMsg = new StatusResponseMsg;
      sendMsg(tTxMsg);
   }

   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - StatusResponseMsg

void ClientThread::processRxMsg(StatusResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun2, "ClientThread::processRxMsg_StatusResponseMsg %d",aMsg->mCode1);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ClientThread::sendMsg (BaseMsg* aTxMsg)
{
   mTcpMsgClientThread->sendMsg(aTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ClientThread::sendTestMsg()
{
   TestMsg* msg = new TestMsg;
   msg->mCode1=201;
 
   mTcpMsgClientThread->sendMsg(msg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// executeOnTimer, base class overload.

void ClientThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
   Prn::print(Prn::ThreadRun3, "ClientThread::executeOnTimer %d", aTimerCount);

   // Send a status request message.
   StatusRequestMsg* tMsg = new StatusRequestMsg;
   tMsg->mCode1 = aTimerCount;
   sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace