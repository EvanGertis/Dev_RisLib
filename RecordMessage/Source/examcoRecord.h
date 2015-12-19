#ifndef _EXAMCOMSG_RECORD_H_
#define _EXAMCOMSG_RECORD_H_

#include "risByteBuffer.h"
#include "risByteRecord.h"
#include "risByteRecordCopier.h"

namespace ExampleComm
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************
    // Record Types

    class TypeIdT
    {
    public:
        static const int cUnspecified  = 0;
        static const int cTestRecord   = 1;
        static const int cStatusRecord = 2;
        static const int cData1Record  = 3;
        static const int cData2Record  = 4;
    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class TestRecord : public Ris::ByteRecord
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;

       //***********************************************************************
       // Constructor

       TestRecord();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class StatusRecord : public Ris::ByteRecord
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;

       //***********************************************************************
       // Constructor

       StatusRecord();
    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data1Record : public Ris::ByteRecord
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;

       //***********************************************************************
       // Constructor

       Data1Record();

    };


    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data2Record : public Ris::ByteRecord
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;
       Data1Record mRecord1;

       //***********************************************************************
       // Constructor

       Data2Record();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class RecordCopier : public Ris::BaseRecordCopier
    {
    public:

       //***********************************************************************
       // This creates a new record, based on a record type

       Ris::ByteRecord* createRecord(int aRecordType);

       //***********************************************************************
       // This copies byte buffers to/from records

       void copyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteRecord* aRecord);

       //***********************************************************************
       // These copy byte buffers to/from records

       void copyToFrom (Ris::ByteBuffer* aBuffer, TestRecord*   aRecord);
       void copyToFrom (Ris::ByteBuffer* aBuffer, StatusRecord* aRecord);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data1Record*  aRecord);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data2Record*  aRecord);

    };


}
#endif