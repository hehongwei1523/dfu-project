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

extern uint8 download_file;
extern uint8 download_file1;

uint16 BlockNum = 1;
uint8 * abuff = "123454321";
uint16 BufferLength = 8;

uint8 LinkEstablishment_Flag = 0x00;

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
  return &pkt_buff;//����һ����ַ���洢���յ�������
}

void freeMem ( void * envState , void * tofree ) 
{
  //�ͷ��ڴ�
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
  //�ͷ�Packet����
  // if we are going to delete it, chop it out of our list of things
  // to delete...

	free( pkt);
}

struct BCSPImplementation bcspImplementation; //����ṹ�壬����ռ�

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

uint16 rcv_count = 0; //link ���ݰ����ձ�־λ

uint8* download_file_buf;  //Ҫ��ȡ���ַ���  
long download_file_len;    //��ȡ�ĳ���  ������Ϊlong,�������ʾ������ 2016-12-5
const char * download_file_path = "c:\\dfu\\my.dfu";//"c:\\dfu\\no-key\\combined5.dfu";// /*·��Ҫ��˫б��\\ */  

void File_Handle(void)
{
	//����һ���ļ�ָ��  
	FILE* download_file = (FILE*)fopen(download_file_path, "rb"); //�Զ����Ƹ�ʽ��ȡ������fread��ȡ���ȱ���

	if (download_file !=NULL) {

		fseek(download_file, 0, SEEK_END);   //�Ƶ�β��  
		download_file_len = ftell(download_file);          //��ȡ����  
		rewind(download_file);               //�ع�ԭλ  
		//printf("count the file content len = %d \n", download_file_len);
		//����buf�ռ�  
		download_file_buf = (uint8*)malloc(sizeof(uint8) * download_file_len + 1);//�����ڴ�ռ䣬���dfu�ļ�����
		if (!download_file_buf) {
			printf("malloc space is not enough. \n");
			return NULL;
		}

		//��ȡ�ļ�  
		//��ȡ����buf����λ��С�����ȣ��ļ�ָ��  
		long rLen = fread(download_file_buf, sizeof(uint8), download_file_len, download_file);
		download_file_buf[rLen] = '\0';
		printf("has read Length = %d \n", rLen);
		//printf("has read content = %s \n", download_file_buf);
		 
		for (int i = rLen; i > 0; i--) //ȥ��dfu�ļ���׺�ĳ���
		{
			if ((download_file_buf[i+4] == 0x00) && (download_file_buf[i+3] == 0x01) &&
				(download_file_buf[i+2] == 0xff) && (download_file_buf[i+1] == 0xff))
			{		
				download_file_len = i+1;
				//printf("download_file_len = %d ", download_file_len);
			}
		}
#if 0
		for (int i = 0; i < 100; i++)
		{
			printf("buf[%d]= 0x%x \n",i,(unsigned char)download_file_buf[i]);
		}
#endif
		/*  //�ݲ��ͷ��ڴ�
		fclose(download_file);
		free(download_file_buf);
		*/
		return 0;
	}
	else
	{
		printf("open file error.");
		exit(0);
	}

	return NULL;
}

//�����ļ����ȵ�����ȫ����long
Result donwnload(void)
{
	struct DFUStatus status;
	struct DFUFunctionalDescriptor functional;

	uint16 blockNum = 0;

	GetFunct(&functional);
	if (functional.wTransferSize == 0)
	{
		printf("wTransferSize errer \n");
		exit(0);
	}
	uint32 bufferLength = functional.wTransferSize;

	if (download_file_len < bufferLength)
	{
		printf("block = %d,bufferLength= %d \n", blockNum, (uint16)download_file_len);
		Dnload(blockNum, download_file_buf, (uint16)download_file_len);
		DoDownloadStatus(&status);
		blockNum++;
	}
	else
	{
		long times = download_file_len / bufferLength; //������Ҫѭ���Ĵ���
		do
		{
			long address_offset = blockNum * bufferLength;
			if (times == 0) bufferLength = download_file_len % bufferLength;
			printf("block = %d,bufferLength= %d \n", blockNum, bufferLength);
			Dnload(blockNum, download_file_buf + address_offset, bufferLength);
			DoDownloadStatus(&status);
			blockNum++;
		} while (times--);
	}
	printf("\n----------------run in Checking----------------- \n");
	//У��
	// Check the status
	Result result = success;
	if (result) result = DoDownloadStatus(&status);
	if (!result) return result;

	// End the download
	result = ProgressCheck(97);
	printf("block = %d,bufferLength= 0 \n", blockNum);
	if (result) result = Dnload(blockNum, 0, 0);
	if (!result) return result;

	// Wait for the operation to complete (do not check for abort)
	Progress(98);

	// Check the status if manifestation tolerant
	if (functional.bmAttributes & attribute_manifestation_tolerant)
	{
		result = ProgressCheck(99);
		if (result) result = DoDFUIdle(false, false);
		if (!result) return result;
	}

	// Successful if this point reached
	return success;
}

void * Thread4(void * a)
{

	while (1)
	{

#if 1
		if (LinkEstablishment_Flag == 0x02) //��ʾBCSP���ӳɹ�
		{
			connected = 1;

			Sleep(100);
			//Func_Test();
			Do_After_Connect();

			//DoReconfigure(false, false, "abc");

			//DoDownload("abc");

			LinkEstablishment_Flag = 0;
			donwnload();

			Sleep(1000);

			DoManifest();

			printf("Test End !\n");
			exit(0);
		}
#endif
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
		Uart_Rcv(NULL);
		//printf("  2  ");

	}
	return NULL;
}

void main_run(void)
{
	File_Handle();
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
	
	bcspImplementation.mStack = (BCSPStack * ) calloc(1,sizeof(BCSPStack)) ;//��̬�����ڴ�ռ�
	
    initialiseStack(bcspImplementation.mStack) ;
    //BTRACE0(WINERR,"WINERR debug enabled\n") ;
    //add a few packets...
    for ( c = 0; c < NUM_PACKETS; c++)
    {
      
    //��ʼ����������ú����ὫPacket��ֵ����mStack.thePacketPool
      Packet * pkt = (Packet *)calloc(1,sizeof(Packet)); //��̬�����ڴ�ռ�
     
      BCSPaddPacket(bcspImplementation.mStack, pkt);
      // keep track of them so that we can delete them later.
      bcspImplementation.Packets[c] = pkt;
               
    }
}

uint32 ms_clock (void)//ϵͳ���ļ�����ms��
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

				//���ղ��ֺ��Ѵ�����˽�������ֽ����жϣ������������Ϊ0xdd(�ڴ��ͷ�)�����˳������������з��� 2016-11-30
				if ((bcspImplementation.mXMITBuffer[6] == 0xdd) && (bcspImplementation.mXMITBuffer[7] == 0xdd) &&
					(bcspImplementation.mXMITBuffer[8] == 0xdd) && (bcspImplementation.mXMITBuffer[9] == 0xdd))
				{
					return;
				}

				//�����������Ϊ0xcd(�ڴ�δ��ʼ��)�����˳��������з��� 2016-12-5
				if ((bcspImplementation.mXMITBuffer[6] == 0xcd) && (bcspImplementation.mXMITBuffer[7] == 0xcd) &&
					(bcspImplementation.mXMITBuffer[8] == 0xcd) && (bcspImplementation.mXMITBuffer[9] == 0xcd))
				{
					return;
				}

				//������Ƿ���download״̬�£����buff�Ĵ�С����30���ж�Ϊ��Ч��buff���˳�����
				if ((sendpdu_flag != 2) && (bcspImplementation.mXMITBytesAvailable > 30))
				{
					return;
				}

				//�ж��������Ƿ������ڵ�0xc0,���б�ʾ���ݰ����˽�����ֱ�ӷ��ͺ��˳�����
				if (sendpdu_flag == 2)
				for (int i = 1; i < BYTE_BUFFERSIZE; i++) //��ʼ��С����Ϊ��С��ֵ������0xc0�����ڽ�ǰ��λ�ã�����mXMITBuffer�ٷ���һ��
				{
					if ((bcspImplementation.mXMITBuffer[i] == 0xc0) && (bcspImplementation.mXMITBuffer[i + 1] == 0xc0))
					{
						com_write(&bcspImplementation.mXMITBuffer, i+1);
						sendpdu_flag = 4;
						return;
					}
				}

				//download������������buff�������0x00 ���ݣ��򲻷���
				if (sendpdu_flag == 4) 
				{
					if ((bcspImplementation.mXMITBuffer[6] == 0x00) && (bcspImplementation.mXMITBuffer[7] == 0x00) &&
						(bcspImplementation.mXMITBuffer[8] == 0x00) && (bcspImplementation.mXMITBuffer[9] == 0x00))
						return;
				}

				com_write(&bcspImplementation.mXMITBuffer, bcspImplementation.mXMITBytesAvailable);

#if 1
				if ((bcspImplementation.mXMITBuffer[0] == 0xc0) && (bcspImplementation.mXMITBuffer[1] == 0xff))
				{

					BCSPStack * stack = bcspImplementation.mStack;
					
					scheduler(stack, 0);
					//���Σ������SEQInput��MUXunreliableinput�ᵼ��getStatusָ�����ȥ  2016-12-5
/*					disposePacket(stack, stack->SLIPInputPacket);

					disposePacketBuffer(stack, &stack->MUXUnreliableInput);
					disposePacketBuffer(stack, &stack->MUXUnreliableInput);
					disposePacketBuffer(stack, &stack->SEQInput);
*/					
					//now do the receive side ...
					disposePacketBuffer(stack, &stack->PacketDelivererInput);
					disposePacketBuffer(stack, &stack->LinkEstablishmentInput);
					disposePacketBuffer(stack, &stack->RCVInputBuffer);
					//disposePacket(stack, stack->theSLIPState.RCVpkt);

					return;
				}
#endif
            }
        }
		
     //we'll return if we get the stackmustdie signal
    } 

}

void load_the_stack_buffer(void)
{
    //now try loading the stack receive buffer
    if (bcspImplementation.mRCVBytesAvailable 
         &&(bcspImplementation.mRCVBytesAvailable <= numFreeSlotsInReceiveBuffer(bcspImplementation.mStack)))
    {
      //�����յ����ֽ�д�뵽mStack.SLIPByteInput��
      writeToReceiveBuffer(bcspImplementation.mStack,bcspImplementation.mRCVBuffer,(uint8) bcspImplementation.mRCVBytesAvailable) ;
      bcspImplementation.mRCVBytesAvailable = 0 ;
    }
}

uint8 * uart_handle = &uart_buf;
void uart_next(void)
{
   uart_handle++;
   if( uart_handle == uart_end )
      uart_handle = uart_buf;
}
uint8 Packet_Rcv_Flag = 0x00;
uint8 uart_get_data[30] = { 0 };

//�����ڽ��յ������ݰ���0xC0��ʼ��0xC0���� 
void BCSP_DATA_RCV(void)
{
	while (uart_handle != uart_ptr)
	{
		if (*uart_handle == 0xc0)
		{
			bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = 0xc0;

			uart_next();
			while (*uart_handle != 0xc0)
			{

               bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = *uart_handle;	
#if 0
			   if (bcspImplementation.mRCVBytesAvailable > 127)  //ͻȻ��������һ�Ѵ��ҵ����ݣ����³�������쳣
			   {

				   for (int i = 0; i < 120; i++)
				   {
					  printf("mRCVBuffer[%d] = 0x%x \n", i, bcspImplementation.mRCVBuffer[i]);
				   }
				
				   //load_the_stack_buffer();
				   //bcspImplementation.mRCVBytesAvailable = 0; //ǿ������
				   //��windows�������㴦�� -- ��û����� 2016-12-5
				   //memset(bcspImplementation.mRCVBuffer, 0x00, 128);		   
				   //return;
			   }
#endif			   
			   uart_next();
			}
			bcspImplementation.mRCVBuffer[bcspImplementation.mRCVBytesAvailable++] = 0xc0;
			if (uart_handle == uart_end -1)//��uart_ptr�պõ���uart_end��ʱ�򣬽�ptr��ǰһλ������handle׷����ptr 2016-12-8
			{
				//uart_handle++;
				uart_ptr--;
			}

		}

		uart_next();	
	}

}

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

    result = GetDevice(&device);
    printf("(----device---- %x )\n", device.idProduct);
	 
	if (result) result = GetFunct(&functional);
	printf("(----descriptor---- %x )\n", functional.wDetachTimeout);
	
	Sleep(100);
	if (result) result = Detach(min(functional.wDetachTimeout, detachTimeout));//��λǰ���øú�����оƬ��λ����DFUģʽ

	Sleep(100);
	if (result) result = Reset(false); //��λоƬ

	Sleep(100); 
	rcv_count = 0;
	if (result) result = DoConnect(true, false);
	
	if (result) result = GetDevice(&device);
	printf("(----device---- %x )\n", device.idProduct);

	if (result) result = GetFunct(&functional);
	printf("(----descriptor---- %x )\n", functional.wDetachTimeout);
    //DoDownload("abc");
	Sleep(100);
	Dnload(0, &download_file, 1023);// sizeof(download_file) / sizeof(uint8));
	DoDownloadStatus(&status);
	Dnload(1, &download_file1, 221);
	DoDownloadStatus(&status);
	//Dnload(2, &download_file, 1023);
	//DoDownloadStatus(&status);
	//Dnload(3, &download_file1, 221);
	//Sleep(100);
	
	if (result) result = GetDevice(&device);
	printf("(----device---- %x )\n", device.idProduct);

	if (result) result = GetFunct(&functional);
	printf("(----descriptor---- %x )\n", functional.wDetachTimeout);
	
	/*
	Sleep(100);
	DoDownloadStatus(&status);
	Dnload(2, &download_file1, 221);
	DoDownloadStatus(&status);
*/
}
