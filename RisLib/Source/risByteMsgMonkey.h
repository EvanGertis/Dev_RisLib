#pragma once
/*==============================================================================
ByteContent message monkey.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for a message creator. Inheriting classes 
// are used to create messages for a specific message set.

class BaseMsgCreator
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Create a new message, based on a message type.
   virtual Ris::ByteContent* createMsg(int aMessageType) = 0;

   // Destroy a message, default deletes the message.
   virtual void destroyMsg(Ris::ByteContent* aMsg);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for a message monkey. It can be used
// by code that receives messages into byte buffers such that the message
// classes don't have to be visible to the receiving code. Inheriting classes
// provide all of the details that are needed by receiving code to receive and
// extract messages, as opposed to having the message classes being visible
// to the receiving code. Likewise for the transmitting code.

class BaseMsgMonkey
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Message header parameters, these are common to all message headers.
   // They are extracted from an actual received message header. In some
   // form, all message headers contain these parameters. These are extracted
   // from a byte buffer.
   int  mHeaderLength;
   int  mMessageLength;
   int  mMessageType;
   int  mPayloadLength;
   bool mHeaderValidFlag;

   // Endian network order.
   bool mNetworkOrder;

   // Message creator, this must be set by the inheritor.
   BaseMsgCreator* mMsgCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BaseMsgMonkey(BaseMsgCreator* aCreator);
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, header processing.

   // Extract message header parameters from a buffer and validate them
   // Returns true if the header is valid
   virtual bool extractMessageHeaderParms(Ris::ByteBuffer* aBuffer)=0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, message processing.

   // Preprocess a message before it is sent.
   virtual void processBeforeSend(Ris::ByteContent* aMsg){};

   // Copy a message to a byte buffer.
   void putMsgToBuffer (Ris::ByteBuffer* aBuffer,Ris::ByteContent* aMsg);

   // Copy a message from a byte buffer.
   // The header parms must be extracted prior to calling this.
   Ris::ByteContent* getMsgFromBuffer (Ris::ByteBuffer* aBuffer);

   // Copy a message from a byte buffer.
   // This first extracts the header parms.
   Ris::ByteContent* makeMsgFromBuffer (Ris::ByteBuffer* aBuffer);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, buffer management.

   // Return a contant header length.
   virtual int getHeaderLength()=0;

   // Return a contant max buffer size.
   virtual int getMaxBufferSize()=0;

   // Endianess for buffers associated with the monkey.
   // If true then the messages will be sent in network order,
   // big endian. If false, then little endian.
   void setNetworkOrder (bool aNetworkOrder);

   // Configure a byte buffer endianess.
   void configureByteBuffer(ByteBuffer* aBuffer);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, footer processing.

   // Validate a message footer by comparing the contents of the byte buffer
   // that the message was received into and the message itself. For example,
   // the byte buffer can contain the message bytes and the message can contain
   // a checksum and this function would calculate the checksum on the buffer
   // bytes and then compare it with the checksum stored in the message footer.
   virtual bool validateMessageFooter(Ris::ByteBuffer* aBuffer,Ris::ByteContent* aMsg)
   { return true; };
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for a message monkey creator. It defines
// a method that inheriting classes overload to create new message monkeys.
// It is used by transmitters and receivers to create new instances of message
// monkeys.

class BaseMsgMonkeyCreator
{
public:
   virtual BaseMsgMonkey* createMonkey() = 0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

