#include "DFURequest.h"
#include "stdio.h"
#include "com.h"
#include "bcspimplementation.h"
#include "usart.h"
#include <windows.h>
char * ComName = "COM10";  //not true
char set_event = 0;

uint8 uart_data=0;
//extern char * uart_get;
uint8 response_packet = 0;
uint8 sendpdu_flag = 0;

void Uart_Reset(void)
{
	uart_ptr = uart_buf;
}

void Clearn_Answer_buff(void)
{
	int i = 0;
	for (i = 0; i < 8; i++)
	{ 
		//printf("[%d]=0x%x ",i, bcspImplementation.mRCVBuffer[i]);
		bcspImplementation.mRCVBuffer[i] = 0x00;
	}
}

void * Uart_Rcv(void* g)
{
	if (uart_ptr != uart_end)
	{
		*uart_ptr++ = com_get();  //没数据输入，该函数一直读到0xcc
		//printf("0x%x ", *(uart_ptr-1));

		if ( (*(uart_ptr - 1) == 0xc0) )//&& (*(uart_ptr-2) != 0xc0) ) //放到这里判断，减少时间的浪费 2016-11-18
		{
			uart_data++;
			//printf(" uart_data =%d \n ", uart_data);
		}
	}
	else
	{
		uart_ptr = uart_buf;
	}


	if ((uart_data == 2) )//&& (*(uart_ptr - 2) != 0xc0))//接收到两个0xC0，并且前一个不是0xC0 
	{
		//printf(" uart_data =%d \n ", uart_data);
		uart_data = 0;
		
		BCSP_DATA_RCV();
		/*
		  //从发送的数据包进行处理
		//判断是否是应答数据包
		if (sendpdu_flag == 4)
		if ((bcspImplementation.mRCVBuffer[0] == 0xc0) && (bcspImplementation.mRCVBuffer[1] != 0x00) &&
			(bcspImplementation.mRCVBuffer[2] == 0x00) && (bcspImplementation.mRCVBuffer[3] == 0x00))
		{
			printf(" response ");
			//uart_handle = uart_ptr;
			Clearn_Answer_buff();
			return;
		}
	    */

		set_event = EVENT_BCSP_DATA;

		/* 收到数据包时，可以快速回复,因此建立BCSP联系时，可以很快达到link状态；
		但接收到芯片应答数据包后，会自动重发一次，并且payload填充数据0xdd  2016-11-21 */

		if (connected == 1)  //BCSP建立连接后
		{

           BCSPImplementation_Test();//不是回复数据包

			//printf("uart_data packet start \n");	

			//printf("uart_data packet end  \n");
		}
		else
		{
			BCSPImplementation_Test();
		}

	}
	
	return 0;
}


uint32 SYSTEM_TIME = 0;
void Device_Init()
{
	int result;
	result = com_init(ComName);

	if (result == 0)
	{
       printf("Com Open ! \n");
	}
	
	BCSPImplementation_Environment();
    BCSPTransport();


	SYSTEM_TIME = GetTickCount();
}
