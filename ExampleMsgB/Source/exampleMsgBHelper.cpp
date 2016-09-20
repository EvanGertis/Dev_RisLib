#include <stdio.h>
#include <string.h>
#include "prnPrint.h"
#include "exampleMsgBHelper.h"

namespace Example
{
   //****************************************************************************

   void MsgBHelper::initialize(TestMsg* aMsg)
   {
      aMsg->mCode1 = 901;
      aMsg->mCode2 = 902;
      aMsg->mCode3 = 903;
      aMsg->mCode4 = 904;
   }

   void MsgBHelper::show(TestMsg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void MsgBHelper::initialize(StatusMsg* aMsg)
   {
      aMsg->mCode1 = 901;
      aMsg->mCode2 = 902;
      aMsg->mCode3 = 903;
      aMsg->mCode4 = 904;
   }

   void MsgBHelper::show(StatusMsg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void MsgBHelper::initialize(Data1Msg* aMsg)
   {
      aMsg->mCode1 = 101;
      aMsg->mCode2 = 102;
      aMsg->mCode3 = 103;
      aMsg->mCode4 = 104;
   }

   void MsgBHelper::show(Data1Msg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }


   //****************************************************************************

   void MsgBHelper::initialize(Data2Msg* aMsg)
   {
      aMsg->mCode1 = 201;
      aMsg->mCode2 = 202;
      aMsg->mCode3 = 203;
      aMsg->mCode4 = 204;
      MsgBHelper::initialize(&aMsg->mData1);
   }

   void MsgBHelper::show(Data2Msg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      MsgBHelper::show(&aMsg->mData1);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void MsgBHelper::initialize(Data3Msg* aMsg)
   {
      aMsg->mCode1 = 201;
      aMsg->mCode2 = 202;
      aMsg->mCode3 = 203;
      aMsg->mCode4 = 204;
      strcpy(aMsg->mString1,"ABCDEFGH");
      aMsg->mCode5Loop = 4;
      for (int i = 0; i < 4; i++)
      {
         aMsg->mCode5[i]=1001+i;
      }
   }

   void MsgBHelper::show(Data3Msg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      Prn::print(Prn::ThreadRun3, "String1    %s", aMsg->mString1);
      aMsg->mCode5Loop = 4;
      for (int i = 0; i < 4; i++)
      {
      Prn::print(Prn::ThreadRun3, "Code5      %d", aMsg->mCode5[i]);
      }
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void MsgBHelper::initialize(Data4Msg* aMsg)
   {
      aMsg->mCode1 = 201;
      aMsg->mCode2 = 202;
      aMsg->mCode3 = 203;
      aMsg->mCode4 = 204;
      aMsg->mData1Loop = 4;
      for (int i = 0; i < 4; i++)
      {
         initialize(&aMsg->mData1[i]);
      }
   }

   void MsgBHelper::show(Data4Msg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      aMsg->mData1Loop = 4;
      for (int i = 0; i < 4; i++)
      {
         show(&aMsg->mData1[i]);
      }
      Prn::print(Prn::ThreadRun3, "");
   }
}//namespace