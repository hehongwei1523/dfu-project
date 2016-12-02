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
		*uart_ptr++ = com_get();  //û�������룬�ú���һֱ����0xcc
		//printf("0x%x ", *(uart_ptr-1));

		if ( (*(uart_ptr - 1) == 0xc0) )//&& (*(uart_ptr-2) != 0xc0) ) //�ŵ������жϣ�����ʱ����˷� 2016-11-18
		{
			uart_data++;
			//printf(" uart_data =%d \n ", uart_data);
		}
	}
	else
	{
		uart_ptr = uart_buf;
	}


	if ((uart_data == 2) )//&& (*(uart_ptr - 2) != 0xc0))//���յ�����0xC0������ǰһ������0xC0 
	{
		//printf(" uart_data =%d \n ", uart_data);
		uart_data = 0;
		
		BCSP_DATA_RCV();
		/*
		  //�ӷ��͵����ݰ����д���
		//�ж��Ƿ���Ӧ�����ݰ�
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

		/* �յ����ݰ�ʱ�����Կ��ٻظ�,��˽���BCSP��ϵʱ�����Ժܿ�ﵽlink״̬��
		�����յ�оƬӦ�����ݰ��󣬻��Զ��ط�һ�Σ�����payload�������0xdd  2016-11-21 */

		if (connected == 1)  //BCSP�������Ӻ�
		{

           BCSPImplementation_Test();//���ǻظ����ݰ�

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
