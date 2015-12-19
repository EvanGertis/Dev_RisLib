#ifndef _RISBYTEBUFFER_H_
#define _RISBYTEBUFFER_H_
/*==============================================================================

ByteContent and ByteBuffer classes.

ByteBuffer is a smart pointer that addresses a fixed section of memory.

ByteContent is a base class for something that can be copied to/from a
ByteBuffer.

These classes are intended to support the construction of message classes.
The idea here is that each message is encapsulated by a class that inherits
from ByteContent and supplies a member function that allows it to be copied
to/from a ByteBuffer, which can be transported via some communications
medium. All copies are done in network order. 

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a base class for classes that can be copied to/from ByteBuffers.
// Each inheriting class supplies a copyToFrom member function that can be
// called by ByteBuffer put and get functions to copy it.

class ByteContent;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsualtes byte buffers. It represents memory sections. It
// provides member functions to do some simple memory management for a buffer.
// It provides primitive data type put and get operations that allow primitives
// to be copied to/from a byte buffer in network order. It also provides "copy"
// member functions that either put to a buffer or get from a buffer, based on
// a copy direction flag.
//
// This class maintains a buffer base pointer, which contains the base address
// of the buffer, a buffer working pointer, which contains an addresss within
// the buffer where the next put or get operation will be performed. It also
// maintains a buffer length, which contains the number of bytes that have
// been put into the buffer.

class  ByteBuffer
{
public:
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Members

   //---------------------------------------------------------------------------
   // Pointer members.
   // The base pointer contains the address of the beginning of the
   // buffer. The working pointer contains the address in the buffer
   // to which the next put or get operation will operate. The
   // working length contains the number of bytes that have been put
   // into the buffer. The max length is maxlength of the buffer,
   // it is the size allocated for the buffer

   char*  mBaseBytes;
   int    mWorkingIndex;
   int    mWorkingLength;
   int    mMaxLength;

   //---------------------------------------------------------------------------
   // Copy direction. This specifies the direction of copy operations.
   // Copy to does a put, copy from does a get.

   static const int cCopyTo   = 0;
   static const int cCopyFrom = 1;
   int mCopyDirection;

   //---------------------------------------------------------------------------
   // Copy direction. This specifies the direction of copy operations.
   // Copy to does a put, copy from does a get.

   static const int cNoError          = 0;
   static const int cBufferOverflow   = 1;
   static const int cBadPointer       = 2;
   int mError;

   //---------------------------------------------------------------------------
   // Byte swapping. This determines if copy operations do byte swapping. The 
   // default for little endian machines is to not do byte swapping and the
   // default for big endian machines is to do byte swapping.

   bool mByteSwapping;

   //---------------------------------------------------------------------------
   // Memory allocation code, itdetermines if the destructor does a memFree.

   int mMemAllocCode;

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Constructors.

   // Default constructor        does not do a memory alloc operation.
   ByteBuffer ();

   // Allocates heap memory of given size  
   // This constructor           does do a memory alloc operation.
   // It's associated destructor does do a memory free  operation.
   ByteBuffer (int aSize);

   // Assigns pre-allocated memory
   // This constructor           does not do a memory alloc operation.
   // It's associated destructor does not do a memory free  operation.
   ByteBuffer (char* aAddress,int aSize);

   // Deallocates any allocated memory.
  ~ByteBuffer ();
   
   //---------------------------------------------------------------------------
   // Heap memory, if allocated then destructor frees it

   void  memAlloc (int aSize);
   void  memFree  ();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Pointer operations.
   // reset sets the working pointer to the base address and
   // sets the length to zero.
   // rewind sets the working pointer to the base address
   // forward advances the working pointer and the length, if the
   // copy direction is for a put operation.
   // advance advances the working pointer and the length

   void  reset();
   void  rewind();
   void  forward     (int aByteSize=1);
   void  advance     (int aByteSize=1);
   void  fillZero    (int aByteSize=1);

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Buffer position

   // Get the address of the start of the buffer
   char*  getBaseAddress ();

   // Set the buffer working index.
   void  setPosition (int aIndex);

   // Get the buffer working index.
   int getPosition ();

   // Get the buffer address at the working index.
   char* getPositionC ();
   void* getPositionV ();

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Length, Error, Direction.

   //---------------------------------------------------------------------------
   // Buffer maximum size

   int getMaxLength();

   //---------------------------------------------------------------------------
   // Buffer length. This is the number of bytes that have
   // actually been put into the buffer, the actual data length.

   void setLength(int aValue);
   int  getLength();

   //---------------------------------------------------------------------------
   // Error codes.

   void setError(int aValue);
   int  getError();

   //---------------------------------------------------------------------------
   // Copy direction.
   //
   // this determines the direction of copy operations
   // copy to does a put, copy from does a get.

   void setCopyTo ();
   void setCopyFrom ();

   bool isCopyTo ();
   bool isCopyFrom ();

   //---------------------------------------------------------------------------
   // This sets the network order of the buffer and whether or not
   // byte swapping happens.
   //
   // If network order is true then bytes are copied into the 
   // buffer in network order, big endian. If false then little
   // endian.
   //
   // If network order is true and the machine is little endian
   // then byte swapping happens. If netwok order is true and
   // the machine is big endian then byte swapping does not 
   // happen. The other two cases follow.
   //
   // The default is non network order.

   void setNetworkOrder (bool aNetworkOrder);

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Copy operations

   //---------------------------------------------------------------------------
   // copy does either a copy to the buffer or a copy from the buffer
   //
   // These either put an item to the byte buffer or get an
   // item from the byte buffer, based on the value of the
   // buffer copy direction flag.

   void copy  (unsigned char*      aValue);
   void copy  (unsigned short*     aValue);
   void copy  (unsigned int*       aValue);
   void copy  (unsigned long long* aValue);
   void copy  (char*               aValue);
   void copy  (short*              aValue);
   void copy  (int*                aValue);
   void copy  (long long*          aValue);
   void copy  (float*              aValue);
   void copy  (double*             aValue);
   void copy  (bool*               aValue);

   void copyS (unsigned char*      aString);
   void copyS (char*               aString);

   //---------------------------------------------------------------------------
   // Copy an object that inherits from ByteContent to/from a byte buffer.

   void copy          (ByteContent* content);
   void putToBuffer   (ByteContent* content);
   void getFromBuffer (ByteContent* content);
};

//******************************************************************************
}//namespace
#endif
