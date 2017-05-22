 
#include "bcsptransport.h"
#include "bcspimplementation.h"

#include "transferrequest.h"
#include "bcsptransport.h"
#include "DFUTransportCOM.h"

#include "transportconfiguration.h"
#include "string.h"
#include "stdlib.h"

typedef struct BCSPTransport
{
    enum bc_channel mTunnelOver;
    enum bc_channel mTunnelBelow;

    //const uint16 mBufferSize;
	uint16 mBufferSize;

    struct BCSPImplementation       mStack;
    StackConfiguration       mStackConfiguration;
    BCSPChannel              channels;
    uint8*                   buffers;
} _BCSPTransport;

_BCSPTransport bcsptransport;

typedef struct BCSPCallBackInfo
{
    BCTransferRequest req;
    struct BCSPTransport * object;

} bcspallbackinfo;


void BCSPTransport_Init( )
{
    //BlueCoreTransportImplementation ( "BCSPTransport" , aConfig.useCSRExtensions() , aRecv ), // tunnelling
    bcsptransport.mBufferSize = 4096;
    //mTunnelOver ( aConfig.getTunnelOver() ),
    //mTunnelBelow ( aConfig.getTunnelBelow() ),
    //mStack ( comPort )
}

uint8 _buffers[128] = {0};

void BCSPTransport( )
{
    BCSPTransport_Init( );
    //BTRACELOG ( "E:\\Work\\bcsplog.txt" );
    BTRACEDISABLE ( ( ( 0xFFFFFFFF ) ) );//^ IO ) ^ LINKEST ) ^ USER0 );
    
    bcsptransport.mStackConfiguration.resendTimeout = getResendTimeout();
    bcsptransport.mStackConfiguration.retryLimit = getRetryLimit();
    bcsptransport.mStackConfiguration.TShy = getTShy();
    bcsptransport.mStackConfiguration.TConf = getTConf();
    bcsptransport.mStackConfiguration.confLimit = getConfLimit();
    bcsptransport.mStackConfiguration.windowSize = getWindowSize();
	bcsptransport.mStackConfiguration.useCRC = true;// getUseCRC();  //CRC使用标志位，清零的话会导致数据包没有CRC
	bcsptransport.mStackConfiguration.useLinkEstablishmentProtocol = true;//getUseLinkEstablishmentProtocol();
	/*
	bcsptransport.mStackConfiguration.resendTimeout = 250;//getResendTimeout();
	bcsptransport.mStackConfiguration.retryLimit = 20;//getRetryLimit();
	bcsptransport.mStackConfiguration.TShy = 300;// getTShy();
	bcsptransport.mStackConfiguration.TConf = 250;// getTConf();
	bcsptransport.mStackConfiguration.confLimit = 2;// getConfLimit();
	bcsptransport.mStackConfiguration.windowSize = 4;// getWindowSize();
	bcsptransport.mStackConfiguration.useCRC = true;// getUseCRC();  //CRC使用标志位，清零的话会导致数据包没有CRC
	bcsptransport.mStackConfiguration.useLinkEstablishmentProtocol = true;// getUseLinkEstablishmentProtocol();
	*/
    BCSPStack * stack = getBCSPStack() ;
    setStackConfiguration ( stack, &bcsptransport.mStackConfiguration) ;
    //  Set up all the channels to receive.
 
    {
        bcsptransport.buffers = &_buffers;

        BCSPopenChannel ( &bcsptransport.channels , stack , 12 , BCSPChannelReliable , BCSPCallbackDeferred ) ;
        
        //struct BCSPCallBackInfo *info ;//= new BCSPCallBackInfo ( PDU::bc_channel(i) );
        //info->object = this;
        //BCinitTransferRequest( &(info->req), bcsptransport.buffers, bcsptransport.mBufferSize, onPacketFromChip , info );
        // BCSPreadPacket( &bcsptransport.channels , &(info->req) ) ;
    }
}

BCTransferRequest *req ;
struct BCSPCallBackInfo *info ; 

static void RPC_Func(uint8 *buff,int size)
{
    Packet * pkt;
    pkt = SYNCBLKgetPacketFromPool(bcspImplementation.mStack, modeXMIT,0);//
    pkt->data = (uint8*) buff ;
    setLength(pkt,size);
    setProtocolId(pkt,0x0c) ;//DFU ID
    setProtocolType(pkt,BCSPChannelReliable) ;//BCSPChannelUnreliable); //可靠数据包
	//2016-11-21  这里只有SEQInput的packet能正常收发，MUX会导致ack永远是1，PacketDeliver和RCVInput直接发送不出数据
    addPacketToBuffer(bcspImplementation.mStack,&bcspImplementation.mStack->SEQInput, pkt);//MUXUnreliableInput //PacketDelivererInput  //RCVInputBuffer,pkt);发送不出 
}

void sendpdu ( uint8 *buff, int size )
{
   
	uint8 *send_buff;
    //printf("size = %d ",size);
    send_buff = (uint8 *)malloc(size * sizeof(uint8));
    memcpy(send_buff,buff,size * sizeof(uint8 ));
#if 0
{
  uint16 i = 0;
  for( i=0; i<size; i++)
   printf("send_buff[%d] = %x \n",i,send_buff[i]);
}
#endif
    RPC_Func(send_buff,size);
	//printf("transport packet start \n");
	/*下载时，文件的buff比较大，需要执行几次runStack函数，因此特殊处理 2016-11-25 */
 
	if (sendpdu_flag == 2)
		sendpdu_download();
	else
	    BCSPImplementation_runStack();

	//printf("transport packet end \n");
    free(send_buff); //释放内存空间后，payload的数据为0xdd
}

void onPacketToChip ( BCTransferRequest * req, BCTransferStatus status, void * info )
{

}

void onPacketFromChip (BCTransferRequest *req , BCTransferStatus status, void *info)
{
     struct BCSPCallBackInfo* lInfo = (struct BCSPCallBackInfo*)info;
    //assert ( lInfo->object );
    
    processDataFromChip( req , upgrade );

    return;
}

void processDataFromChip ( BCTransferRequest * req, enum bc_channel ch )
{

}

void setConfig ( uint32 aResendTimeout , uint32 aRetryLimit , 
                uint32 aTShy , uint32 aTConf , uint16 aConfLimit , 
                uint8 aWindowSize , bool aUseLinkEstablishmentProtocol )
{
    bcsptransport.mStackConfiguration.resendTimeout = aResendTimeout;
    bcsptransport.mStackConfiguration.retryLimit = aRetryLimit;
    bcsptransport.mStackConfiguration.TShy = aTShy;
    bcsptransport.mStackConfiguration.TConf = aTConf;
    bcsptransport.mStackConfiguration.confLimit = aConfLimit;
    bcsptransport.mStackConfiguration.windowSize = aWindowSize;
    bcsptransport.mStackConfiguration.useLinkEstablishmentProtocol = aUseLinkEstablishmentProtocol;
}
/*
enum bc_channel getTunnelOver()
{
    //return tunnel_over;
}

enum bc_channel getTunnelBelow()
{
    //return tunnel_below;
}
*/
uint32 getResendTimeout()
{
    return bcsptransport.mStackConfiguration.resendTimeout;
}

uint32 getRetryLimit()
{
    return bcsptransport.mStackConfiguration.retryLimit;
}

uint32 getTShy()
{
    return bcsptransport.mStackConfiguration.TShy;
}

uint32 getTConf()
{
    return bcsptransport.mStackConfiguration.TConf;
}

uint16 getConfLimit()
{
    return bcsptransport.mStackConfiguration.confLimit;
}

uint8 getWindowSize()
{
    return bcsptransport.mStackConfiguration.windowSize;
}

bool getUseCRC()
{
   return bcsptransport.mStackConfiguration.useCRC;
}

bool getUseLinkEstablishmentProtocol()
{
    return bcsptransport.mStackConfiguration.useLinkEstablishmentProtocol;
}
