
//#include "DFUEngine.h"
#include "string.h"
#include "common/types.h"
#include "stdbool.h"
#include "DFURequest.h"
#include "DFURequestCOM.h"
#include  "pdu.h"
#include "DFUTransportCOM.h"
//#include "bcsptransport.h"
#include "usart.h"
#include "bcspimplementation.h"
#include "windows.h"

typedef void *HANDLE;

uint8 PayLoad_Buff[Max_Buff_Size] = { 0 };

// Reply header
struct Status
{
        uint16 wStatus;
        uint16 wReplyLength;
};
typedef struct Status requestStatus;

void Test_Printf_Send_Buff(void)
{
  uint16 i = 0;
  for( i=0; i<0x15; i++)

   printf("PayLoad_Buff[%d] = %x \n",i,PayLoad_Buff[i]);
}

uint8 * uart_get = uart_buf;

void uart_get_next(void)
{
   uart_get++;
   if( uart_get == uart_end )
      uart_get = uart_buf;
}
uint8 reply_test[16] = {
   //0xc0, 0xc8, 0x0c, 0x01, 0x2a, 
   //0x00, 0x00, 0x0c, 0x00, 
   0xe0, 0x00, 0x01, 0x00,
   0x01, 0x00, 0x12, 0x0a, 0xff, 0xff, 0x42, 0x09,
};

void Packet_Rcv_download(void)
{
	uart_get = uart_handle; 
	while (uart_get != uart_ptr)
	{
		int j = 0, k = 0,z=0;

		if (*uart_get == 0xc0)
		{
			uart_get_next();//����0xc0
			while (*uart_get != 0xc0)
			{
				j++;
				//У��payload��ʽ�� 0x00 0x00 0x00 0x00,��������
				//if ((j == 5) && ((*uart_get == 0x00)|| (*uart_get == 0x02))) k++;
				if (j == 5) k++;
				if ((j == 6) && (*uart_get == 0x00)) k++;
				if ((j == 7) && (*uart_get == 0x00)) k++;
				if ((j == 8) && (*uart_get == 0x00)) k++;

				if ( k == 4 )
				{			
					printf("("); 
				}
				printf("0x%x  ", *uart_get);  //���Դ�ӡ :����ӡʱ������ղ�������

				if (k == 4)
				{
					Packet_Rcv_Flag = 1;
					printf(")");
					return;
				}

				uart_get_next();
			}
		}
		if (Packet_Rcv_Flag == 1) return;
		uart_get_next();
	}
}

void Packet_Rcv(void) //�ⲿ�����ݴ���ò��Ǻܺ�
{
	uart_get = uart_handle;
	while (uart_get != uart_ptr)
	{
		int i = 0, j = 0, k = 0;
		//printf("  time  \n");
		Sleep(5);
		if (*uart_get == 0xc0)
		{
			uart_get_next();//����0xc0
			while (*uart_get != 0xc0)
			{
				j++;
				
				//У��payload��ʽ�� 0x00 0x00 0x0x 0x00,����������ֵ������
				if ((j == 5) && (*uart_get == 0x00)) k++;
				if ((j == 6) && (*uart_get == 0x00)) k++;
				if ((j == 7) && (*uart_get != 0x00)) k++;
				if ((j == 8) && (*uart_get == 0x00)) k++;

				//if (k == 4) printf("(");
					
				if ( (j > 8) && (k == 4) )
				{
					Packet_Rcv_Flag =1;
					uart_get_data[i++] = *uart_get; 
					//printf("flag");
				}
				//printf("0x%x  ", *uart_get);  //���Դ�ӡ :����ӡʱ������ղ�������
				//if(k==4) printf(")");
				if (j > 30) break;
				uart_get_next();
			}
		}
		if (Packet_Rcv_Flag == 1) return;
		uart_get_next();	
	}
}

void sendpdu_download(void)
{
	Packet_Rcv_Flag = 0x00; //���պ���֮ǰ���������־λ���������ж�ֱ�����������ղ�������
	uint32 time_begin = ms_clock();
	
	int i = 9;
	while (i--) //���շ���
	{
       BCSPImplementation_runStack();
	   if (sendpdu_flag == 4)
		   break;
	}
	Sleep(60);   //������download��Ӧ���ӳ�һ��ʱ�䣬�ȴ�uart���ݵĽ���
#if 0
	while (Packet_Rcv_Flag != 1)
	{
		Packet_Rcv_download();//�ȴ�оƬ���ص�Ӧ��payload������һֱѭ��

		if ((ms_clock() - time_begin) > 5000) //���ճ�ʱ������ѭ��,ʱ���Գ�Щ 2016-11-25
		{
			printf("download time out \n");
			break;
		}
	}
#endif
	sendpdu_flag = 2;
	//printf("download \n");
}

// Generic control requests
Result ControlRequest(const struct Setup setup, void *buffer, uint16 bufferLength, uint16 *replyLength)
{

	// Check that the buffer is large enough
	if (bufferLength < setup.wLength)
	{
		return fail_com_buffer;
	}

	// Prepare the transmit buffer
	uint16 requestLength = 0;
	//uint16 maxReplyLength = 0;  //not use
	if (setup.bmRequestTypeDirection == dir_host_to_device)//��������Ҫ����ʱ
	{
		// Setup packet and buffer to send
		requestLength = sizeof(setup) + setup.wLength;
		//maxReplyLength = sizeof(requestStatus);
		memcpy((void*)PayLoad_Buff, &setup, sizeof(setup));//��setup���ݸ��Ƶ�PayLoad_Buff�У���ͬ
		memcpy((void*)(PayLoad_Buff + sizeof(setup)), buffer, setup.wLength);//��buffer���ݸ��Ƶ�PayLoad_Buffƫ��sizeof(setup)��С
	}
	else//ֻ���Ϳ�������
	{
		// Only the setup packet to send
		requestLength = sizeof(setup);
		//maxReplyLength = sizeof(requestStatus) + setup.wLength;
        memcpy((void*)PayLoad_Buff, &setup, sizeof(setup));
	}
      
    sendpdu( PayLoad_Buff ,requestLength );

    if( (sendpdu_flag == 1) || (sendpdu_flag == 2) ) return success; //���ֺ�������Ҫ�������ݣ�ֱ���˳����ղ���

/*****************************************/
    //���ڽ��մ������

	Packet_Rcv_Flag = 0x00; //���պ���֮ǰ���������־λ���������ж�ֱ�����������ղ�������
	uint32 time_begin = ms_clock();
	while (Packet_Rcv_Flag != 1)
	{
		Packet_Rcv(); //Sleep(2);
		if ((ms_clock() - time_begin) > 2000) //���ճ�ʱ������ѭ�� 2016-11-22
		{
			printf("time out \n");
			break;
		}
	}

	printf(" Rcv over \n");//���ݽ���ִ�����
/*****************************************/
      
    if (replyLength) *replyLength = bufferLength;
     // Decode the reply
	if (setup.bmRequestTypeDirection == dir_device_to_host)
	{
        // A reply was expected
      if (setup.wLength < bufferLength)
      {
        return fail_com_long;
      }
 
      memcpy(buffer, uart_get_data, bufferLength);//ע�ⳤ��

	  for (int i = 0; i < 30; i++)
		  uart_get_data[i] = 0xff; //������ջ�������
	}
	else
	{
		// There should be no reply data
	  if (bufferLength != 0)
	  {
		return fail_com_long;
	  }
	}
    
	// Successful if this point reached
	return success;
}
 
