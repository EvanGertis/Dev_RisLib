/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "risByteContent.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Here is a global flag that gets assigned a value at load time.
// If the machine that this executes on is little endian then it is true
// else it is false

   unsigned int gByteBuffer_ThisMachineIsLittleEndian_TestValue = 0x01020304;

   bool gByteBuffer_ThisMachineIsLittleEndian =
      *((char*)&gByteBuffer_ThisMachineIsLittleEndian_TestValue)==0x04;

// Here is a global flag that gets assigned a value at load time.
// It sets the default for byte swapping to non network order.

   bool gByteBuffer_DoByteSwapping = !gByteBuffer_ThisMachineIsLittleEndian;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructors

ByteBuffer::ByteBuffer ()
{
   mBasePtr=0;
   mWorkingPtr=0;
   mWorkingLength=0;                               
   mMaxLength=0;
   mMemAllocCode=0;
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (char* address,int size)
{
   mBasePtr=address;
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=size;
   mMemAllocCode=0;
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (int size,bool fillZero)
{
   memAlloc(size);
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;

   if (fillZero)
   {
      memset(mBasePtr,0,size);
   }
}

ByteBuffer::~ByteBuffer ()
{
  if(mMemAllocCode)
  {
     free((void*)mBasePtr);
  }
}

//******************************************************************************
// Buffer base address

void ByteBuffer::setBaseAddress (char* address,int size)
{
   memFree();

   mBasePtr=address;
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=size;
   mMemAllocCode=0;
   mError=0;
}

char* ByteBuffer::getBaseAddress ()
{
   return mBasePtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Memory management

void ByteBuffer::memAlloc (int size)
{
   mBasePtr = (char*) malloc(size);

   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=size;
   mMemAllocCode=1;
}

//******************************************************************************

void ByteBuffer::memFree ()
{
   if(mMemAllocCode)
   {
      free((void*)mBasePtr);
   }

   mBasePtr=0;
   mWorkingPtr=0;
   mMemAllocCode=0;
   mWorkingLength=0;
   mMaxLength=0;
}

//******************************************************************************

void ByteBuffer::reset ()
{
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
}

//******************************************************************************

void ByteBuffer::rewind ()
{
   mWorkingPtr=mBasePtr;
}

//******************************************************************************

void ByteBuffer::forward (int size)
{
   if (size + mWorkingPtr - mBasePtr > mMaxLength) return;

   if (mCopyDirection==COPY_TO)
   {
      mWorkingPtr    += size;
      mWorkingLength += size;
   }
   else
   {
      mWorkingPtr    += size;
   }
}

//******************************************************************************

void ByteBuffer::advance (int size)
{
   if (size + mWorkingPtr - mBasePtr > mMaxLength) return;

   mWorkingPtr    += size;
   mWorkingLength += size;
}

//******************************************************************************

void ByteBuffer::fillZero (int size)
{
   if (size + mWorkingPtr - mBasePtr > mMaxLength) return;

   if (mCopyDirection==COPY_TO)
   {
      for (int i=0;i<size;i++)
      {
         mWorkingPtr[i] = 0;
      }

      mWorkingPtr    += size;
      mWorkingLength += size;
   }
   else
   {
      mWorkingPtr    += size;
   }
}

//******************************************************************************
// buffer position

bool ByteBuffer::setPosition (char* address)
{
   mWorkingPtr=address;
   return true;
}

bool ByteBuffer::setPosition (int index)
{
   if (index>=mMaxLength) return false;

   mWorkingPtr=&mBasePtr[index];

   return true;
}

char* ByteBuffer::getPosition ()
{
   return mWorkingPtr;
}

char* ByteBuffer::getPositionC ()
{
   return mWorkingPtr;
}

void* ByteBuffer::getPositionV ()
{
   return (void*)mWorkingPtr;
}

//******************************************************************************
void ByteBuffer::show()
{
   int tLength = mMaxLength;
   if (tLength>50) tLength=50;
	printf(" 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9\n");
	for (int i=0;i<tLength;i++)
	{
		printf("%02X",(unsigned char)mBasePtr[i]);
	}
   printf("\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// these only set or get values of some member variables

void ByteBuffer::setCopyTo()
{
   mCopyDirection=COPY_TO;
}

void ByteBuffer::setCopyFrom ()
{
   mCopyDirection=COPY_FROM;
}

bool ByteBuffer::isCopyTo()
{
   return mCopyDirection==COPY_TO;
}

bool ByteBuffer::isCopyFrom ()
{
   return mCopyDirection==COPY_FROM;
}

int ByteBuffer::getError()
{
   return mError;
}

void ByteBuffer::setError(int value)
{
   mError=value;
}

int ByteBuffer::getLength()
{
   return mWorkingLength;
}

void ByteBuffer::setLength(int value)
{
   mWorkingLength=value;
}

int ByteBuffer::getMaxLength()
{
   return mMaxLength;
}

void ByteBuffer::setNetworkOrder (bool aNetworkOrder)
{
   if (gByteBuffer_ThisMachineIsLittleEndian)
   {
      mByteSwapping = aNetworkOrder;
   }
   else
   {
      mByteSwapping = !aNetworkOrder;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// raw data

bool ByteBuffer::putData (void* data,int size) 
{
   // guard
   if(data==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // itemWorkingLength is the length of the fixed size data 
   int itemWorkingLength = size;

   // guard
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   // copy the fixed size data
   memcpy(mWorkingPtr,data,itemWorkingLength);


   // adjust members
   mWorkingPtr    += itemWorkingLength;
   mWorkingLength += itemWorkingLength;

   return true;
}

//---------------------------------------------

bool ByteBuffer::getData (void* data,int size) 
{
   // guard
   if(data==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // itemWorkingLength is the length of the fixed size data 
   int itemWorkingLength = size;

   // guard
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   // copy the fixed size data
   memcpy(data,mWorkingPtr,itemWorkingLength);

   // adjust members
   mWorkingPtr += itemWorkingLength;

   return true;
}

//--------------------------------------

bool ByteBuffer::copyData (void* data,int size) 
{
   if(data==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putData(data,size);
   }
   else
   {
      return getData(data,size);  
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// put and get operations for ByteContent
//
// these call the ByteContent object's supplied member function
// copyToFrom

bool ByteBuffer::put (ByteContent* content) 
{
   // guard
   if(content==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // set copy direction
   setCopyTo();

   // call ByteContent supplied member function to do the put
   content->copyToFrom(this);

   return true;
}

bool ByteBuffer::putToBuffer (ByteContent* content) 
{
   // guard
   if(content==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // set copy direction
   setCopyTo();

   // call ByteContent supplied member function to do the put
   content->copyToFrom(this);

   return true;
}

bool ByteBuffer::get (ByteContent* content) 
{
   if(content==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // set copy direction
   setCopyFrom();

   // call ByteContent supplied member function to do the get
   content->copyToFrom(this);

   return true;
}

bool ByteBuffer::getFromBuffer (ByteContent* content) 
{
   if(content==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // set copy direction
   setCopyFrom();

   // call ByteContent supplied member function to do the get
   content->copyToFrom(this);

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// put and get operations for primitive data types.
//
// put copies an item into the buffer in network order at the
// current value of the buffer working pointer and advances the
// pointer. 
//
// get copies an item out of the buffer in host order at the
// current value of the buffer working pointer and advances the
// pointer. 

//******************************************************************************
// unsigned short

bool ByteBuffer::put (void* pItem, int pItemSizeInBytes) 
{
   const int itemWorkingLength = pItemSizeInBytes;

   // guard
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }
   
   // temp pointer
   char* source = (char *) pItem;

   // copy bytes to buffer, host to network
   if (mByteSwapping)
   {
      for (int byteIndex = 0; byteIndex < pItemSizeInBytes; byteIndex++)
      {
         mWorkingPtr[byteIndex] = source[pItemSizeInBytes - byteIndex - 1];
      }
   }
   else
   {
      for (int byteIndex = 0; byteIndex < pItemSizeInBytes; byteIndex++)
      {
         mWorkingPtr[byteIndex] = source[byteIndex];
      }
   }

   // adjust members
   mWorkingPtr    += itemWorkingLength;
   mWorkingLength += itemWorkingLength;

   return true;
}

bool ByteBuffer::get (void* pItem, int pItemSizeInBytes) 
{
   const int itemWorkingLength = pItemSizeInBytes;

   // guard
   if(pItem==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }
   
   // temp pointer
   char* destin = (char*)pItem;

   // copy bytes from buffer, network to host
   if (mByteSwapping)
   {
      for (int byteIndex = 0; byteIndex < pItemSizeInBytes; byteIndex++)
      {
         destin[byteIndex] = mWorkingPtr[pItemSizeInBytes - byteIndex - 1];
      }
   }
   else
   {
      for (int byteIndex = 0; byteIndex < pItemSizeInBytes; byteIndex++)
      {
         destin[byteIndex] = mWorkingPtr[byteIndex];
      }
   }

   // adjust members
   mWorkingPtr += itemWorkingLength;

   return true;
}


bool ByteBuffer::putItem (unsigned char  item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (unsigned char* item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (unsigned short  item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (unsigned short* item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (unsigned int  item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (unsigned int* item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (unsigned long long  item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (unsigned long long* item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (char   item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (char*  item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (short   item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (short*  item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (int   item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (int*  item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (long long   item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (long long*  item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (float  item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (float* item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (double  item) {return put (&item, sizeof(item));}
bool ByteBuffer::getItem (double* item) {return get ( item, sizeof(*item));}

bool ByteBuffer::putItem (bool    flag) {unsigned char t = flag ? 1 : 0; return put(&t,sizeof(t));}
bool ByteBuffer::getItem (bool*   flag) {unsigned char t; bool s = get(&t,sizeof(t));*flag = (t!=0);return s;}

bool ByteBuffer::putEnum (int     item) {return put (&item, sizeof(item));}
bool ByteBuffer::getEnum (int*    item) {return get ( item, sizeof(*item));}

//******************************************************************************
// null terminated string

bool ByteBuffer::putZString (char* string,int upperBound) 
{
   // guard
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // itemWorkingLength is the length of the string 
   // plus the null character
   int itemWorkingLength = 0;
   while(1)
   {
      if(string[itemWorkingLength++]==0) break;
      if(itemWorkingLength==upperBound) break;
   }

   // guard
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   // copy the string, without the null character
   memcpy(mWorkingPtr,string,itemWorkingLength-1);

   // copy the null character
   mWorkingPtr[itemWorkingLength-1] = 0;

   // adjust members
   mWorkingPtr    += itemWorkingLength;
   mWorkingLength += itemWorkingLength;

   return true;
}

//---------------------------------------------

bool ByteBuffer::getZString (char* string,int upperBound) 
{
   // guard
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // itemWorkingLength is the length of the string 
   // plus the null character
   int itemWorkingLength = 0;
   while(1)
   {
      if(mWorkingPtr[itemWorkingLength++]==0) break;
      if(itemWorkingLength==upperBound) break;
   }

   // guard
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   // copy the string, without the null character
   memcpy(string,mWorkingPtr,itemWorkingLength-1);

   // copy the null character
   string[itemWorkingLength-1] = 0;

   // adjust members
   mWorkingPtr += itemWorkingLength;

   return true;
}

//******************************************************************************
// fixed size string

bool ByteBuffer::putFString (char* string,int fixedSize) 
{
   // guard
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // itemWorkingLength is the length of the fixed size string 
   int itemWorkingLength = fixedSize;

   // guard
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   // copy the fixed size string
   memcpy(mWorkingPtr,string,itemWorkingLength);


   // adjust members
   mWorkingPtr    += itemWorkingLength;
   mWorkingLength += itemWorkingLength;

   return true;
}

//---------------------------------------------

bool ByteBuffer::getFString (char* string,int fixedSize) 
{
   // guard
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // itemWorkingLength is the length of the fixed size string 
   int itemWorkingLength = fixedSize;

   // guard
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   // copy the fixed size string
   memcpy(string,mWorkingPtr,itemWorkingLength);

   // adjust members
   mWorkingPtr += itemWorkingLength;

   return true;
}

//---------------------------------------------

bool ByteBuffer::getFString2(char* string,int fixedSize) 
{
   // guard
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // itemWorkingLength is the length of the fixed size string 
   int itemWorkingLength = fixedSize;

   // guard
   if(mWorkingPtr > mBasePtr + mMaxLength - itemWorkingLength)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   // copy the fixed size string
   memcpy(string,mWorkingPtr,itemWorkingLength);

   // null terminate 
   string[fixedSize]=0;

   // adjust members
   mWorkingPtr += itemWorkingLength;

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations

//--------------------------------------

bool ByteBuffer::copy (unsigned char* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (unsigned short* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}
//--------------------------------------

bool ByteBuffer::copy (unsigned int* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (unsigned long long* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (char* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (short* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (int* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (long long* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (float* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (double* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*item);
   }
   else
   {
      return getItem(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (bool* flag) 
{
   if(flag==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putItem(*flag);
   }
   else
   {
      return getItem(flag);  
   }
}

//--------------------------------------

bool ByteBuffer::copyEnum (int* item) 
{
   if(item==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putEnum(*item);
   }
   else
   {
      return getEnum(item);  
   }
}

//--------------------------------------

bool ByteBuffer::copyZString (char* string,int upperBound) 
{
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putZString(string,upperBound);
   }
   else
   {
      return getZString(string,upperBound);  
   }
}

//--------------------------------------

bool ByteBuffer::copyFString (char* string,int fixedSize) 
{
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putFString(string,fixedSize);
   }
   else
   {
      return getFString(string,fixedSize);  
   }
}

//--------------------------------------

bool ByteBuffer::copyFString2 (char* string,int fixedSize) 
{
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putFString(string,fixedSize);
   }
   else
   {
      return getFString2(string,fixedSize);  
   }
}

//--------------------------------------

bool ByteBuffer::copyZString (unsigned char* string,int upperBound) 
{
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putZString((char*)string,upperBound);
   }
   else
   {
      return getZString((char*)string,upperBound);  
   }
}

//--------------------------------------

bool ByteBuffer::copyFString (unsigned char* string,int fixedSize) 
{
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putFString((char*)string,fixedSize);
   }
   else
   {
      return getFString((char*)string,fixedSize);  
   }
}

//--------------------------------------

bool ByteBuffer::copyFString2 (unsigned char* string,int fixedSize) 
{
   if(string==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return putFString((char*)string,fixedSize);
   }
   else
   {
      return getFString2((char*)string,fixedSize);  
   }
}

//--------------------------------------

bool ByteBuffer::copy (ByteContent* content) 
{
   if(content==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      return put(content);
   }
   else
   {
      return get(content);  
   }
}

//******************************************************************************
}//namespace



