
#include "stdio.h"
#include "com.h"
#include "bcspimplementation.h"
#include "usart.h"
#include <windows.h>
char * ComName = "COM10";  //not true
char set_event = 0;

uint8 uart_data=0;
void * Uart_Rcv(void* g)
{

	if (uart_ptr != uart_end)
	{
		*uart_ptr++ = com_get();  //没数据输入，该函数不会执行
		//printf("0x%x ", *uart_ptr);

		if (*(uart_ptr - 1) == 0xc0)  //放到这里判断，减少时间的浪费 2016-11-18
			uart_data++;
	}
	else
	{
		uart_ptr = uart_buf;
	}

	if ( (uart_data > 1) && (*(uart_ptr - 2) != 0xc0) )//接收到两个C0，并且前一个不是C0
	{
		printf(" flag \n ");
		uart_data = 0;
		BCSP_DATA_RCV();

		set_event = EVENT_BCSP_DATA;
		//BCSPImplementation_Test();
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
