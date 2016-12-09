#include "DFURequest.h"
#include "stdio.h"
#include "com.h"
#include "bcspimplementation.h"
#include "usart.h"
#include <windows.h>
char * ComName = "COM10";  //not useful,just for debug
char set_event = 0;

uint8 uart_data=0;
uint8 sendpdu_flag = 0;

#define uart_buf_size 128
uint8 uart_buf[uart_buf_size];
uint8 * uart_ptr = uart_buf;
uint8 * uart_end = uart_buf + uart_buf_size;
uint8 uart_data;

void * Uart_Rcv(void* g)
{
	uint8 data;
	DWORD lRead;

	if (uart_ptr != uart_end)
	{
#if 1
		ReadFile(hCOMHnd, &data, 1, &lRead, NULL);//&mRCVOverlapped);
		if (lRead != 1)
		{
			return NULL; //������������ݾ�ֱ���˳�
		}
		*uart_ptr++ = data;
#else
		*uart_ptr++ = com_get();
#endif
		//printf("(0x%x)", *(uart_ptr-1));

		if ( (*(uart_ptr - 1) == 0xc0) ) //&& (*(uart_ptr-2) != 0xc0) )
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
		set_event = EVENT_BCSP_DATA;
		BCSP_DATA_RCV();
	
#if 0
		  //�ӷ��͵����ݰ����д���
		//�ж��Ƿ���Ӧ�����ݰ�
		if (sendpdu_flag == 4)
		if ((bcspImplementation.mRCVBuffer[0] == 0xc0) && (bcspImplementation.mRCVBuffer[1] != 0x00) &&
			(bcspImplementation.mRCVBuffer[2] == 0x00) && (bcspImplementation.mRCVBuffer[3] == 0x00))
		{
			printf(" response ");
			memset(&bcspImplementation.mXMITBuffer, 0x00, 128 * sizeof(uint8));
			return;
		}
#endif	

		/* �յ����ݰ�ʱ�����Կ��ٻظ�,��˽���BCSP��ϵʱ�����Ժܿ�ﵽlink״̬ */
        BCSPImplementation_Test();
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
