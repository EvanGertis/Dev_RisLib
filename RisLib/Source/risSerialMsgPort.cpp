/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>

#include "prnPrint.h"

#include "risSockets.h"
#include "risSerialMsgPort.h"

namespace Ris
{
namespace Serial
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
SerialMsgPort::SerialMsgPort()
{
   mTxBuffer = Sockets::memAlloc(BufferSize);
   mTxLength=0;
   mRxBuffer = Sockets::memAlloc(BufferSize);
   mRxLength=0;
   mTxMsgCount=0;
   mRxMsgCount=0;
   mValidFlag=false;
   mRxMessageParser=0;
   mTxMessageParser=0;
}
//******************************************************************************
SerialMsgPort::~SerialMsgPort()
{
   Sockets::memFree(mTxBuffer);
   Sockets::memFree(mRxBuffer);
}
//******************************************************************************
// Configure the socket

void SerialMsgPort::configure(
     int   aPortNumber,
     char* aPortSetup)
{
   SerialPort::initialize(aPortNumber, aPortSetup);
   mTxMsgCount=0;
   mRxMsgCount=0;

}

void SerialMsgPort::configure(
   BaseMessageParserCreator* aMessageParserCreator)
{
   if (mRxMessageParser == 0) mRxMessageParser = aMessageParserCreator->createNew();
   if (mTxMessageParser == 0) mTxMessageParser = aMessageParserCreator->createNew();
}

//******************************************************************************
// Reconfigure the socket

void SerialMsgPort::reconfigure()
{
   mTxMsgCount=0;
   mRxMsgCount=0;
}
//******************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket

bool SerialMsgPort::doSendMsg(ByteContent* aTxMsg)
{
   if (!aTxMsg) return false;

   bool tRet=false;

   // Mutex
   mTxMutex.get();

   // Message parser processing
   mTxMessageParser->processBeforeSend(aTxMsg);

   // Byte buffer, constructor takes address and size
   ByteBuffer tBuffer(mTxBuffer,BufferSize);  
   mTxMessageParser->configureByteBuffer(&tBuffer);

   // Copy transmit message to buffer
   tBuffer.putToBuffer(aTxMsg);

   // Delete the message
   DecreaseResource(aTxMsg);

   // Transmit the buffer
   int tLength=tBuffer.getLength();
   tRet = doSend(tBuffer.getBaseAddress(),tLength)!=0;

   mTxLength=tLength;
   mTxMsgCount++;

   // Mutex
   mTxMutex.put();

   if (!tRet)
   {
      Prn::print(Prn::SocketRun,0, "ERROR SerialMsgPort::doSendMsg FAIL");
   }

   return true;
}
//******************************************************************************
// This receives data from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool SerialMsgPort::doRecvMsg (ByteContent*& aRxMsg)
{
   //-------------------------------------------------------------------------
   // Initialize
   aRxMsg=0;
   bool tRet=false;
   int tStatus=0;

   // Byte buffer, constructor takes address and size
   ByteBuffer tBuffer(mRxBuffer,BufferSize);  
   mRxMessageParser->configureByteBuffer(&tBuffer);

   // Header length
   int tHeaderLength = mRxMessageParser->getHeaderLength();

   //-------------------------------------------------------------------------
   // Read the message header into the receive buffer

   tRet = receiveData(mRxBuffer,tHeaderLength,tStatus)!=0;
// Prn::print(Prn::SocketRun,Prn::Run4, "doRecv1 %d %d",tStatus,mError);

   // Guard
   // If bad status then return false.

   if (!tRet || tStatus<=0)
   {
      return false;
   }

   // Set the buffer length
   tBuffer.setLength(tHeaderLength);

   //--------------------------------------------------------------
   // Copy from the receive buffer into the message parser object
   // and validate the header

   mRxMessageParser->extractMessageHeaderParms(&tBuffer);

   // If the header is not valid then error
   if (!mRxMessageParser->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun,0, "ERROR doRecv1 INVALID HEADER");
      for (int i=0;i<40;i++) 
      {
         Prn::print(0,0, "%02d %02x",i,0xff&(unsigned)mRxBuffer[i]);
      }
      return false;
   }

   //-------------------------------------------------------------------------
   // Read the message payload into the receive buffer

   int tPayloadLength = mRxMessageParser->mPayloadLength;

   tRet=receiveData(&mRxBuffer[tHeaderLength],tPayloadLength,tStatus)!=0;
// Prn::print(Prn::SocketRun,Prn::Run4, "doRecv2 %d %d %d",mStatus,mError,tPayloadLength);

   // Guard
   // If bad status then return false.

   if (!tRet || tStatus<=0)
   {
      Prn::print(Prn::SocketRun,0, "ERROR doRecv2 INVALID RECV");
      return false;
   }

   // Set the buffer length
   tBuffer.setLength(mRxMessageParser->mMessageLength);

   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // extract the message from the byte buffer into a new message
   // object and return it.

   tBuffer.rewind();
   aRxMsg = mRxMessageParser->makeFromByteBuffer(&tBuffer);

   // Test for errors and return.
   // If the pointer is zero then message is bad
   if (aRxMsg==0)
   {
      tStatus=tBuffer.getError();
      Prn::print(Prn::SocketRun,0, "ERROR doRecv3 bad message %d",tStatus);
      return false;
   }

   mRxMsgCount++;
   return true;
}

}//namespace
}//namespace
