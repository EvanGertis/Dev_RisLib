#ifndef _GREENMSG_RECORD_H_
#define _GREENMSG_RECORD_H_

#include "risByteBuffer.h"
#include "risByteRecord.h"

namespace GreenRecord
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************
    // Message Types

    class MsgIdT
    {
    public:
        static const int cUnspecified  = 0;
        static const int cTestRecord   = 1;
        static const int cStatusRecord = 2;
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

       //***********************************************************************
       // Helpers

       void initialize();
       void show();
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

    class RecordCopier
    {
    public:

       //***********************************************************************
       // This creates a new record, based on a record type

       static Ris::ByteRecord* createRecord(int aRecordType);

       //***********************************************************************
       // This copies byte buffers to/from records

       static void copyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteRecord* aRecord);

       //***********************************************************************
       // These copy byte buffers to/from records

       static void copyToFrom (Ris::ByteBuffer* aBuffer, TestRecord*   aRecord);
       static void copyToFrom (Ris::ByteBuffer* aBuffer, StatusRecord* aRecord);

    };


}
#endif
