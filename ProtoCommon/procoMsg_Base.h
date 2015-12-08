#ifndef _PROCOMSG_BASE_H_
#define _PROCOMSG_BASE_H_
/*==============================================================================
This file contains a set of classes that encapsulate the message set
that is used to communicate with Intranet. The messages are specified
in the IDD.

There is a class for each particular message in the set and there is a
base class that all of the messages inherit from.

These messages follow the ByteContent pattern, where they all inherit
from ByteContent so that they can be copied to/from ByteBuffers.

The base class is used to specify set membership, any inheriting class
is a member of the message set, and message objects can be referenced
anonymously via pointers to the the base class.

The base class provides a member function, makeFromByteBuffer, that
extracts particular messages from a byte buffer and returns a pointer
to the base class.

These messages all have the same common form, they all contain a
common message header. The base class has a Header member object that
encapsulates the header.

These are the messages:

Test
FirstMessage
StatusRequest
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risByteContent.h"
#include "risByteMessageParser.h"

namespace ProtoComm
{

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Message Definitions

   namespace MsgDefT
   {
      //******************************************************************************
      // Use this for a buffer size for these messages

      static const int cMsgBufferSize = 20000;

   }//namespace

    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    // This encapsualtes the message header.

   class Header : public Ris::ByteContent
   {
   public:
      // Constructor
      Header();
      void reset();

      //------------------------------------------------
      // Header Content

      int   mSyncWord1;
      int   mSyncWord2;
      int   mMessageIdentifier;
      int   mMessageLength;
      int   mSourceId;
      int   mDestinationId;

      // Header Content
      //------------------------------------------------

      // Header length
      static const int cLength = 24;

      //--------------------------------------------------------------------------
      // If the byte buffer is configured for put operations then this puts the
      // contents of the object into the byte buffer (it does a copy to, it
      // copies the object to the byte buffer).
      // If the byte buffer is configured for get operations then this gets the
      // contents of the object from the byte buffer (it does a copy from, it
      // copies the object from the byte buffer).
      // Copy To and Copy From are symmetrical.
      //--------------------------------------------------------------------------

      void copyToFrom(Ris::ByteBuffer* aBuffer);

      //--------------------------------------------------------------------------
      // For variable content messages, the message length cannot be known until
      // the entire message has been written to a byte buffer. Therefore, the 
      // message header cannot be written to a byte buffer until the entire
      // message has been written and the length is known.
      //
      // The procedure to write a message to a byte buffer is to skip over the 
      // buffer segment where the header is located, write the message payload
      // to the buffer, set the header message length based on the now known
      // payload length, and write the header to the buffer.
      //
      // These are called explicitly by inheriting messages at the
      // beginning and end of their copyToFrom's to manage the headers.
      // For "get" operations, headerCopyToFrom "gets" the header and
      // headerReCopyToFrom does nothing. For "put" operations,
      // headerCopyToFrom stores the buffer pointer and advances past where the
      // header will be written and headerReCopyToFrom "puts" the header at the
      // stored position. Both functions are passed a byte buffer pointer to
      // where the copy is to take place. Both are also passed a ByteContent
      // pointer to where they can get and mMessageType
      // which they transfer into and out of the headers.
      //--------------------------------------------------------------------------

      void headerCopyToFrom   (Ris::ByteBuffer* aBuffer, Ris::ByteContent* aParent);
      void headerReCopyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteContent* aParent);

      //---------------------------------------------------------------------------
      // These are set by headerCopyToFrom and used by headerReCopyToFrom,
      // for "put" operations.Theyt contain the buffer position and length of
      // where the headerReCopyToFrom will take place, which should be
      // where headerCopyToFrom was told to do the copy.

      int mInitialPosition;
      int mInitialLength;

   };

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This encapsualtes a message header.
   //******************************************************************************
   // This is a message parser that can be used by code that receives messages
   // into byte buffers and then extracts them into message objects that live
   // on the heap.

   class BaseMsg;

   class MessageParser : public Ris::BaseMessageParser
   {
   public:
      //-------------------------------------------------------
      // Configure:

      void configure(int aSourceId);

      //-------------------------------------------------------
      // Base class overloads:

      // Return a contant header length
      int  getHeaderLength();

      // Return a contant max buffer size
      int getMaxBufferSize();

      // Extract message header parameters from a buffer and validate them
      bool extractMessageHeaderParms(Ris::ByteBuffer* aBuffer);

      // Create a new message based on a message type
      Ris::ByteContent* createMessage(int aMessageType);

      // Clone a new message from an existing message
      BaseMsg* cloneMessage(BaseMsg* aMsg);

      // Preprocess a message before it is sent
      void processBeforeSend(Ris::ByteContent* msg);

      //-------------------------------------------------------
      // Data:

      int mSourceId;
   };
   //******************************************************************************
   // This is a message parser creator. It defines a method that creates a new
   // message parser. It is used by transmitters and receivers to create new
   // instances of message parsers.

   class MessageParserCreator : public  Ris::BaseMessageParserCreator
   {
   public:
      // Constructor
      MessageParserCreator();

      // Members
      void configure(int aSourceId);
      int  mSourceId;

      // Base class overload, creates a new message parser and sets some of its 
      // member variables.
      Ris::BaseMessageParser* createNew();
   };

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Base message class.
   // All particular messages for this message set inherit from this.

   class BaseMsg : public Ris::ByteContent
   {
   public:
      //--------------------------------------------------------------------------
      // Constructor

      BaseMsg();

      //---------------------------------------------------------------------------
      // Message Header 

      Header mHeader;

   };

}//namespace
#endif

