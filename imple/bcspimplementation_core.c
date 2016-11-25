///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Cambridge Silicon Radio April 2000-2006
//
//  FILE    :   bcspimplementation_core.cpp  -  wrapper for BCSP stack
//
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include <stdlib.h>
#include <string.h>
#include "../bcsp/bcspstack.h"
#include "bcspimplementation.h"
#include "usart.h"
#include "DFUTransportCOM.h"
#include "DFUEngineBase.h"

#include "../mcu_dfu/com.h"

#define null ((void *)0)


void onLinkFailSync( void * aThis )
{
    // alert the user
   printf("onLinkFailSync \n");
}

void onLinkFailRetries( void * aThis )
{
    // alert the user
   printf("onLinkFailRetries \n");
}

void onLinkFailUart()
{
    // alert the user
   printf("onLinkFailUart \n");
}

void setOnLinkFailSync(void (*fn)(void*),void * param)
{
    userOnLinkFailSync = fn ;
    userOnLinkFailParamSync = param ;
}

void setOnLinkFailRetries(void (*fn)(void*),void * param)
{
    userOnLinkFailRetries = fn ;
    userOnLinkFailParamRetries = param ;
}

void setOnLinkFailUart(void (*fn)(void*),void * param)
{
    userOnLinkFailUart = fn ;
    userOnLinkFailParamUart = param ;
}

uint8 pkt_buff[1031] = {0};
void * allocMem ( void * envState , uint32 size ) 
{
  return &pkt_buff;//返回一个地址，存储接收到的数据
}

void freeMem ( void * envState , void * tofree ) 
{
  //释放内存
}

void enterCS ( void * envState )
{
  /*
    ((BCSPImplementation*)envState)->mLockCS =
        new Lock ( ((BCSPImplementation*)envState)->cs );
  */
}

void leaveCS ( void * envState )
{
  /*
    Lock *lock = ((BCSPImplementation*)envState)->mLockCS;
    ((BCSPImplementation*)envState)->mLockCS = 0;
    delete lock;
  */
}

void signal ( void * envState )
{
    //((BCSPImplementation*)envState)->mFile->reqXmit();
}

void releasePacket ( void * envState , Packet *pkt )
{
  //释放Packet数组
}

struct BCSPImplementation bcspImplementation; //定义结构体，分配空间

BCSPStack * getBCSPStack()
{
  return bcspImplementation.mStack;
}

void BCSPImplementation_Init(void)
{
    bcspImplementation.mXMITBusy = false ;
    bcspImplementation.mRCVBusy = false ;
    bcspImplementation.mXMITBytesAvailable = 0 ;
    bcspImplementation.mRCVBytesAvailable = 0 ;
    bcspImplementation.mStackMustDie = false ;
    
    userOnLinkFailSync = 0 ;
    userOnLinkFailRetries = 0 ;
}

void BCSPImplementation_Environment ( )
{

    BCSPImplementation_Init();
    //
    memset(bcspImplementation.Packets,0,sizeof(bcspImplementation.Packets)) ;
    
    //create the initial stack state
    BCSPImplementation_createBCSPStack();
    bcspImplementation.mBCSPenv.allocMem = allocMem;
    bcspImplementation.mBCSPenv.freeMem = freeMem;
    bcspImplementation.mBCSPenv.enterCS = enterCS;
    bcspImplementation.mBCSPenv.leaveCS = leaveCS;
    bcspImplementation.mBCSPenv.signal = signal;
    bcspImplementation.mBCSPenv.onLinkFailureSync = onLinkFailSync;
    bcspImplementation.mBCSPenv.onLinkFailureRetries = onLinkFailRetries;
    bcspImplementation.mBCSPenv.clockMask = 0xFFFFFFFF;
    // call back to reverse BCSPaddPacket below
    bcspImplementation.mBCSPenv.releasePacket = releasePacket; 
    //mBCSPenv.envState = bcspImplementation;
    setEnvironment(bcspImplementation.mStack,&bcspImplementation.mBCSPenv) ;
    
}

static const uint8 GetDevice_data[] = {
	0xc0, 0xdb, 0xdc , 0x8c , 0x00, 0xb3 , 0xc1, 0x08 , 0x00 ,
	0x00, 0x00, 0x00 , 0x0c , 0x00, 0x1f , 0x2a, 0xc0 
	, '\0'
};


uint8 sync_data[13] = { 0xc0, 0x40, 0x41, 0x00, 0x7e, 0xda, 0xdc, 0xed,  0xed, 0xa9, 0x7a, 0xc0, '\0' };

uint16 rcv_count = 0;

void * Thread4(void * a)
{
	while (1)
	{
		
		if (LinkEstablishment_Flag == 0x02) //表示BCSP连接成功
		{
			//connected = 1;
			//printf(" 444444444  ");

			Sleep(100);
			Func_Test();
			//Do_After_Connect();

            //DoReconfigure(false, false, "abc");

			//DoDownload("abc");

			LinkEstablishment_Flag = 0;
			exit(0);
		}
/*
	
		if (getchar() == 's')
		{
			//rcv_count = 0;
			printf(" 444444444  ");
			DoReconfigure(false, false, "abc");
			exit(0);
		}*/	
	}

	return NULL;
}


void * Thread3(void * a)
{	
	
	set_event = 0;

	while(1)
	{ 
 
		//printf("  3  ");
		if (connected != 1)
		{
			if (rcv_count < 2)
			{
				com_write(sync_data, 12);
				printf(" ++%d++ ", rcv_count);

			}
		}

       Sleep(300);
	}

	return NULL;
}
void * Thread2(void * a)
{
	while(1)
	{ 
		Uart_Rcv(NULL);   //这里影响到线程3 ？
		//printf("  2  ");

	}

	return NULL;
}

void main_run(void)
{
	while (1)
	{
		if (set_event == EVENT_BCSP_DATA)
		{
			set_event = 0x00;		

			rcv_count++;
			if (rcv_count == 100)
				rcv_count = 4;

		
		}
	}
}


void BCSPImplementation_Test()
{

	 load_the_stack_buffer();
	 BCSPImplementation_runStack();
	 //Sleep(10);

}


void BCSPImplementation_deleteBCSP()
{
    // Stop the stack
    BCSPshutdownStack(bcspImplementation.mStack) ;

    //free up bcsp packet headers...

    //free the stack
    free(bcspImplementation.mStack);
}


void BCSPImplementation_createBCSPStack()
{
    uint8 c = 0;
	
	bcspImplementation.mStack = (BCSPStack * ) calloc(1,sizeof(BCSPStack)) ;
	
    initialiseStack(bcspImplementation.mStack) ;
    //BTRACE0(WINERR,"WINERR debug enabled\n") ;
    //add a few packets...
    for ( c = 0; c < NUM_PACKETS; c++)
    {
      
    //初始化，下面调用函数会将Packet的值复制mStack.thePacketPool
      Packet * pkt = (Packet *)calloc(1,sizeof(Packet)); 
     
      BCSPaddPacket(bcspImplementation.mStack, pkt);
      // keep track of them so that we can delete them later.
      bcspImplementation.Packets[c] = pkt;
               
    }
    /*
    PacketPool * pool = &bcspImplementation.mStack->thePacketPool ;
    PacketBuffer * buf = &pool->buffer;
    Queue * Tqueue = &buf->queue;
    printf("Tqueue = %x ",Tqueue->head);
    */
}

#define WRITEFILE_PERIOD 500
void wait(uint32 wakeupTime)
{
	//this function just puts the stack thread into idle until some
	//event (or timeout) wakes us up.
	int32 timeout = wakeupTime - ms_clock();
	if (timeout <= 0)
		return;

	// There appears to be a bug in the WriteFile
	// implementation under Win98.  Occasionally, the WriteFile never
	// sets the completion event.  Close down and then reopen the com port 
	// if the writefile hasn't completed after a certain period.
	
	if (bcspImplementation.mXMITBusy && (ms_clock() - bcspImplementation.mLastXmitTime > WRITEFILE_PERIOD))
	{
	//BTRACE3(WINERR,"Cancelling IO at time %d (%d ms from %d)\n", ms_clock(), WRITEFILE_PERIOD, mLastXmitTime);
	bcspImplementation.mLastXmitTime = ms_clock() ;
	bcspImplementation.mXMITBusy = false;
	bcspImplementation.mRCVBusy = false;
	//bcspImplementation.mFile->reopen();
	return;
	}
	
	//on with the real code...

}

uint32 ms_clock (void)//系统节拍计数（ms）
{
	volatile uint32 count = GetTickCount() - SYSTEM_TIME;
  //printf("count = %d \n",count);
  return count;
}

void BCSPImplementation_runStack(void)
{
    //here's where we actually run round in a loop calling the scheduler...
    //while(!bcspImplementation.mStackMustDie)
    {
        uint32 timeNow = ms_clock() ;
        uint32 wakeupTime = scheduler(bcspImplementation.mStack,timeNow) ;
        //now see if we can do anything with the uart...
        //try sending ...
        //if(!bcspImplementation.mXMITBusy)
        {
            //copy stuff out of stack output buffer 
            bcspImplementation.mXMITBytesAvailable = numBytesInTransmitBuffer(bcspImplementation.mStack) ;
            if (bcspImplementation.mXMITBytesAvailable)
            {
                readFromTransmitBuffer(bcspImplementation.mStack,bcspImplementation.mXMITBuffer, bcspImplementation.mXMITBytesAvailable) ;
				com_write(&bcspImplementation.mXMITBuffer, bcspImplementation.mXMITBytesAvailable);
            }
        }

		//wait(wakeupTime);
		
		//load_the_stack_buffer();
     //we'll return if we get the stackmustdie signal
    } 

}

void load_the_stack_buffer(void)
{
   //now try loading the stack receive buffer
   if (bcspImplementation.mRCVBytesAvailable 
       &&(bcspImplementation.mRCVBytesAvailable <= numFreeSlotsInReceiveBuffer(bcspImplementation.mStack)))
   {
      //将接收到得字节写入到mStack.SLIPByteInput中
      writeToReceiveBuffer(bcspImplementation.mStack,bcspImplementation.mRCVBuffer,(uint8) bcspImplementation.mRCVBytesAvailable) ;
      bcspImplementation.mRCVBytesAvailable = 0 ;
   }
  
}

uint8 * uart_handle = uart_buf;
void uart_next(void)
{
   uart_handle++;
   if( uart_handle == uart_end )
      uart_handle = uart_buf;
}
uint8 Packet_Rcv_Flag = 0x00;
uint8 uart_get_data[30] = { 0 };
//处理串口接收到的数据包，0xC0开始，0xC0结束 
#if 0
void BCSP_DATA_RCV(void)  //另一种接收payload数据的方法
{
    //uart_get = uart_handle;
    while( uart_handle != uart_ptr )
     { 
		int i = 0, j = 0, k = 0;
		Packet_Rdv_Flag = 0;
       if( *uart_handle == 0xc0 )
       {
		  
         bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = 0xc0;
         uart_next();        
         while( *uart_handle != 0xc0 )
         {
           bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = *uart_handle;                  
           //printf("0x%2x",*uart_handle); 
		   j++;
		  
		   //校验payload格式： 0x00 0x00 0x0x ,满足条件则赋值给数组
		   if ((j == 5) && (*uart_handle == 0x00)) k++;
		   if ((j == 6) && (*uart_handle == 0x00)) k++;
		   if ((j == 7) && (*uart_handle != 0x00)) k++;

		   if (j > 8 && k == 3)
		   { 
			   printf("0x%x  ", *uart_handle);
			   uart_get_data[i++] = *uart_handle; 
			   Packet_Rcv_Flag = 1;
		   }

           uart_next();
         }
         bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = 0xc0;
       }
       uart_next();
    }
}
#else
void BCSP_DATA_RCV(void)
{
	while (uart_handle != uart_ptr)
	{
		if (*uart_handle == 0xc0)
		{
			bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = *uart_handle;
			uart_next();
			while (*uart_handle != 0xc0)
			{
				//printf(" 0x%2x ", *uart_handle);
				bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = *uart_handle;
				uart_next();
			}
			bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = *uart_handle;
		}
        
		uart_next();		
	}
	//uart_get = uart_handle;  //这个添加后，会将0xcc打印出来
}
#endif

struct DeviceDescriptor descriptor;

static const uint8 GetDevice_Return[] = {
  0xc0, 0xc8, 0x0c , 0x01 , 0x2a , 0x00 , 0x00 , 0x0c ,
  0x00, 0xe0 , 0x00 , 0x01 , 0x00 , 0x01 , 0x00 , 0x12 
, 0x0a , 0xff , 0xff , 0x42 , 0x09 , 0x05 , 0x3a , 0xc0
, '\0'
};

uint8 GetStatus_data[] = { 0xc0, 0xd2, 0x8c, 0x00, 0xa1, 0xa1, 0x03,  0x00, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x60, 0xfe, 0xc0, '\0' };

uint8 GetFunct_data[] = { 0xc0, 0xd2, 0x8c, 0x00, 0xa1, 0xc1, 0x09,  0x00, 0x00, 0x00, 0x00,
0x06, 0x00, 0x0a, 0xa2, 0xc0,'\0' };

//static const uint8 confRespPacketData[] = {0xde,0xad,0xd0,0xd0} ;
//struct DeviceDescriptor descriptor_rcv;
uint8 LinkEstablishment_Flag = 0x00;


uint16 BlockNum = 1;
uint8 * abuff = "123454321";
uint16 BufferLength = 8;

void Func_Test(void)
{
	Result result;
   struct DeviceDescriptor device;
   struct DFUStatus status;
   //struct DFUFunctionalDescriptor descriptor;
   struct InterfaceDescriptor iface;
   struct DFUFunctionalDescriptor functional;
   //com_write(GetDevice_data, 18);

     GetInterfaceDFU(&iface);
  // RPCGetInterfaceDFU(&iface);
     //Connect(false);

    result = GetDevice(&device);
    printf("(----device---- %x )\n", device.idProduct);
	 
	if (result) result = GetFunct(&functional);
	printf("(----descriptor---- %x )\n", functional.wDetachTimeout);
   
	if (result) result = GetFunct(&functional);
	printf("(----descriptor---- %x )\n", functional.wDetachTimeout);

	Sleep(100);
	if (result) result = Detach(min(functional.wDetachTimeout, detachTimeout));//复位前调用该函数，芯片复位后还是DFU模式

	Sleep(100);
	if (result) result = Reset(false); //复位芯片

	Sleep(100); 
	rcv_count = 0;
	if (result) result = DoConnect(true, false);

	if (result) result = GetDevice(&device);
	printf("(----device---- %x )\n", device.idProduct);

	if (result) result = GetDevice(&device);
	printf("(----device---- %x )\n", device.idProduct);

	if (result) result = GetFunct(&functional);
	printf("(----descriptor---- %x )\n", functional.wDetachTimeout);

	if (result) result = GetFunct(&functional);
	printf("(----descriptor---- %x )\n", functional.wDetachTimeout);

	//Sleep(200);
    //Detach(200);
    //Dnload(BlockNum ,abuff ,BufferLength);
}
