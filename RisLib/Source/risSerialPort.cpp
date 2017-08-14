//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdio.h>

#include "prnPrint.h"
#include "risCallPointer.h"

#include "risSerialPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mPortHandle = 0;
   mRxEventHandle = 0;
   mTxEventHandle = 0;
}

SerialPort::~SerialPort(void)
{
   doClose();
}

bool SerialPort::isValid(){return mValidFlag;}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doOpen(int aPortNumber,char* aPortSetup,int aRxTimeout)
{
   mPortNumber = aPortNumber;
   strcpy(mPortSetup,aPortSetup);
   mValidFlag=false;

   Prn::print(Prn::SerialInit1,"SerialPort::doOpen %d",mPortNumber);

   //--------------------------------------------------------------------------
   // Create events.

   mRxEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
   mTxEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

   //--------------------------------------------------------------------------
   // Create file
   char tSerialPortName[24];
   sprintf(tSerialPortName, "COM%u", mPortNumber);

   mPortHandle = CreateFile(tSerialPortName,  
     GENERIC_READ | GENERIC_WRITE, 
     0, 
     0, 
     OPEN_EXISTING,
     FILE_FLAG_OVERLAPPED,
     0);

   if (mPortHandle==INVALID_HANDLE_VALUE)
   {
      Prn::print(Prn::SerialInit2,"serial_create_error_1 %d", GetLastError());
      return;
   }

   SetupComm(mPortHandle,2048,2048);
   //--------------------------------------------------------------------------
   // Purge 

   Sleep(100);

   doPurge();

   if (GetLastError() != ERROR_SUCCESS)
   {
      Prn::print(Prn::SerialInit2,"serial_create_error_2 %d", GetLastError());
      CloseHandle(mPortHandle);
      mPortHandle=INVALID_HANDLE_VALUE;
      return;
   }
 
   Sleep(100);

   //--------------------------------------------------------------------------
   // DCB

   DCB dcb;

   memset(&dcb, 0, sizeof(dcb));
   dcb.DCBlength = sizeof(dcb);

   GetCommState(mPortHandle, &dcb);

   BuildCommDCB(mPortSetup, &dcb);
  
   dcb.fNull         = FALSE;
   dcb.fAbortOnError = TRUE;

   // SetCommState
   // This might not work if data is being received while initializing.
   // So loop, retry if not successful
   
   int  count=1;
   bool going=true;
   while (going)
   {
      if (SetCommState(mPortHandle, &dcb))
      {
         // Successful, exit loop
         going=false;
      }
      else
      {
         // Failed, continue to retry
         Prn::print(Prn::SerialInit2,"serial_create_retry %d", GetLastError());
         doPurge();
         Sleep(100);
         // Retry failed, abort initialization
         if(count++ == 10)
         {
            Prn::print(Prn::SerialInit2,"serial_create_error_3 %d", GetLastError());
            CloseHandle(mPortHandle);
            mPortHandle=INVALID_HANDLE_VALUE;
            return;
         }
      }
   }

   //--------------------------------------------------------------------------
   // Set timeout

   COMMTIMEOUTS tComTimeout={0};

   tComTimeout.ReadIntervalTimeout         = 0;
   tComTimeout.ReadTotalTimeoutMultiplier  = 0;
   tComTimeout.ReadTotalTimeoutConstant    = aRxTimeout;
   tComTimeout.WriteTotalTimeoutMultiplier = 0;
   tComTimeout.WriteTotalTimeoutConstant   = 0;

   if(!SetCommTimeouts(mPortHandle, &tComTimeout))
   {
      Prn::print(Prn::SerialInit2,"serial_create_error_4 %d", GetLastError());
      CloseHandle(mPortHandle);
      mPortHandle=INVALID_HANDLE_VALUE;
      return;
   }

   //--------------------------------------------------------------------------
   // Purge 

   Sleep(100);
   doPurge();

   //--------------------------------------------------------------------------
   // Done
 
   mValidFlag=true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialPort::doClose()
{
   if (mValidFlag)
   {
      Prn::print(Prn::SerialInit1,"SerialPort::doClose %d",mPortNumber);
      CancelIoEx(mPortHandle,0);
      CloseHandle(mPortHandle);
      CloseHandle(mRxEventHandle);
      CloseHandle(mTxEventHandle);
      mPortHandle = INVALID_HANDLE_VALUE;
      mRxEventHandle = INVALID_HANDLE_VALUE;
      mTxEventHandle = INVALID_HANDLE_VALUE;
      mValidFlag = false;
   } 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Purge the comm channel

void SerialPort::doPurge()
{
   ClearCommError(mPortHandle,0,0);

   DWORD lFlags;

   lFlags = 
      PURGE_RXABORT | PURGE_RXCLEAR |
      PURGE_TXABORT | PURGE_TXCLEAR;

   PurgeComm(mPortHandle, lFlags);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send fixed number of bytes

int SerialPort::doSendBytes(char* aData, int aNumBytes)
{
   // Guard.
   if (!isValid()) return cRetCodeError;

   // Local variables.
   DWORD tNumWritten;
   DWORD tRet  = 0;

   bool tWriteSuccessful = false;
   bool tWaitingOnWrite = false;
   OVERLAPPED tOverlapped = {0};

   tOverlapped.hEvent = mTxEventHandle;

   // Write bytes to the port.
   if (WriteFile(mPortHandle, aData, aNumBytes, &tNumWritten, &tOverlapped))
   {
      // Write was successful.
      Prn::print(Prn::SerialRun4, "SerialPort::doSendBytes PASS1 %d",aNumBytes);
      return 0;
   }

   // Write is pending.
   tRet = WaitForSingleObject(tOverlapped.hEvent, INFINITE);
   switch(tRet)
   {
      // OVERLAPPED structure's event has been signaled. 
      case WAIT_OBJECT_0:
      {
         if (GetOverlappedResult(mPortHandle, &tOverlapped, &tNumWritten, FALSE))
         {
            Prn::print(Prn::SerialRun4, "SerialPort::doSendBytes PASS2 %d",aNumBytes);
            tWriteSuccessful = true;
            return 0;
         }
         else
         {
            tWriteSuccessful = false;
            Prn::print(Prn::SerialRun1, "SerialPort::doSendBytes ERROR 101, %d", GetLastError());
            return cRetCodeError;
         }
      }
      break;
      default:
      {
         tWriteSuccessful = false;
         Prn::print(Prn::SerialRun1, "SerialPort::doSendBytes ERROR 102, %d", GetLastError());
         return cRetCodeError;
      }
      break;
   }
   Prn::print(Prn::SerialRun4, "SerialPort::doSendBytes PASS3 %d",aNumBytes);
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send null terminated string

int SerialPort::doSendZString(char* aData)
{
   int tNumBytes = (int)strlen(aData);
   return doSendBytes(aData,tNumBytes);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send one byte

int SerialPort::doSendOne(char aData)
{
   return doSendBytes(&aData,1);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// receive until cr/lf termination 

int  SerialPort::doReceiveUntilCRLF(char *aData, int aMaxNumBytes)
{
   int  tStatus=0;
   int  tIndex = 0;
   int  tRxStatus=0;
   char tRxChar=0;
   char tRxCharLast=0;
   bool tGoing=true;

   aData[0]=0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while ( isValid() && tGoing )
   {
      // Store last byte
      tRxCharLast = tRxChar;

      // Read one byte
      tRxStatus=doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      { 
         // Read success
         // Store byte
         aData[tIndex]=tRxChar;
         tIndex++;

         if(tRxCharLast==13 && tRxChar==10)
         {
            // Terminator detected
            tGoing=false;
            aData[tIndex-1]=0;
            tStatus=tIndex-2;
         }
         if(tIndex==aMaxNumBytes-1)
         {
            // NumBytes limit was reached
            tGoing=false;
            aData[tIndex]=0;
            tStatus = tIndex;
         }
      }
      else
      {  
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// receive until cr termination 

int  SerialPort::doReceiveUntilCR(char *aData, int aMaxNumBytes)
{
   int  tStatus=0;
   int  tIndex = 0;
   int  tRxStatus=0;
   char tRxChar=0;
   bool tGoing=true;

   aData[0]=0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while ( isValid() && tGoing )
   {
      // Read one byte
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      { 
         // Read success
         // Store byte
         aData[tIndex] = tRxChar;
         tIndex++;

         // If CR
         if(tRxChar==13)
         {
            // Terminator detected
            tGoing = false;
            aData[tIndex]=0;
            tStatus = tIndex-1;
         }
         if(tIndex == aMaxNumBytes-1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }
      }
      else
      {  
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive one byte

int  SerialPort::doReceiveOne(char *aData)
{
   return doReceiveBytes(aData, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive

int SerialPort::doReceiveBytes(char *aData, int aNumBytes)
{
   //---------------------------------------------------------------------------
   // Locals

   DWORD tRet  = 0;
   DWORD tBytesRead  = 0;
   int tBytesTotal = 0;

   bool tReadSuccessful = false;
   bool tWaitingOnRead = false;
   OVERLAPPED tOverlapped = {0};

   tOverlapped.hEvent = mRxEventHandle;

   
   // Issue read operation.
   tRet = ReadFile(mPortHandle, aData, aNumBytes, &tBytesRead, &tOverlapped);

   if (!tRet)
   {
      if (GetLastError() == ERROR_IO_PENDING)
      {
         tWaitingOnRead = true;
      }
      else
      {
//         Prn::print(Prn::SerialRun1,"SerialPort::doReceiveBytes ERROR 101 %d", GetLastError());
         return cRetCodeError;
      }
   }
   else
   {    
      tWaitingOnRead = false;
      tReadSuccessful = true;
   }

   if (tWaitingOnRead)
   {
      // Wait for overlapped i/o completion.
      tRet = WaitForSingleObject(tOverlapped.hEvent, -1);

      // Select on the returned status code.
      switch(tRet)
      {
         // Read completed.
         case WAIT_OBJECT_0:
         {
            if (!GetOverlappedResult(mPortHandle, &tOverlapped, &tBytesRead, FALSE))
            {
               Prn::print(Prn::SerialRun1, "SerialPort::doReceiveBytes ERROR 102 %d", GetLastError());
               return cRetCodeError;
            }
            else
            {
               tReadSuccessful = true;
            }
            tWaitingOnRead = false;
         }
         break;
         // Read timeput.
         case WAIT_TIMEOUT:
         {
            Prn::print(Prn::SerialRun1, "SerialPort::doReceiveBytes ERROR 103%d", GetLastError());
            return cRetCodeError;
         }
         default:
         {
            Prn::print(Prn::SerialRun1, "SerialPort::doReceiveBytes ERROR 104%d", GetLastError());
            return cRetCodeError;
         }
      }
   }


   // If the read was aborted then clear hardware error.
   if (GetLastError() == ERROR_OPERATION_ABORTED)
   {
      Prn::print(Prn::SerialRun1, "SerialPort::doReceiveBytes ERROR 105 %d", GetLastError());
      ClearCommError(mPortHandle, 0, 0);
      return cRetCodeError;
   }

   // If the number of bytes read was wrong then error.
   if (tBytesRead != aNumBytes)
   {
      Prn::print(Prn::SerialRun1, "SerialPort::doReceiveBytes ERROR 106 %d", GetLastError());
      return cRetCodeTimeout;
   }

   // Done.
   return tBytesRead;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

