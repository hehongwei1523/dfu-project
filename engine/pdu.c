
#include "pdu.h"
#include <string.h>
#include <stdio.h>
#include "thread/critical_section.h"







/*
typedef struct PDU
{
    bc_channel mChannel;
    size_t mLen ;
    uint8* mData ;
    //  smart pointer-ness
    uint32 smartCount;
	
} pData;

void PDU_Create(bc_channel channel , uint16 pduSize )
{
  pData->mChannel = channel;
  pData->mLen = pduSize;
}
    void inc()
    {
        ++(pData->smartCount);
    }
    
    uint32 dec()
    {
        return (--(pData->smartCount));
    }

    void resize ( uint32 newlen )
    {
        if (mLen != newlen)
        {
            uint8 * newData[newlen] ;
            memcpy(newData,mData,mLen < newlen ? mLen : newlen) ;
            if ( mLen < newlen )
                memset(newData+mLen,0,newlen-mLen);

            mData = newData ;
            mLen = newlen ;
        }
    }




const uint8 * PDU_data()
{
    return pData->mData;
}

size_t PDU_size()
{
    return pData->mLen;
}

bc_channel PDU_channel()
{
    return pData->mChannel;
}
*/


