
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
			uart_get_next();//跳过0xc0
			while (*uart_get != 0xc0)
			{
				j++;
				//校验payload格式： 0x00 0x00 0x00 0x00,满足条件
				//if ((j == 5) && ((*uart_get == 0x00)|| (*uart_get == 0x02))) k++;
				if (j == 5) k++;
				if ((j == 6) && (*uart_get == 0x00)) k++;
				if ((j == 7) && (*uart_get == 0x00)) k++;
				if ((j == 8) && (*uart_get == 0x00)) k++;

				if ( k == 4 )
				{			
					printf("("); 
				}
				printf("0x%x  ", *uart_get);  //调试打印 :不打印时，会接收不到数据

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

void Packet_Rcv(void) //这部分内容处理得不是很好
{
	uart_get = uart_handle;
	while (uart_get != uart_ptr)
	{
		int i = 0, j = 0, k = 0;
		//printf("  time  \n");
		Sleep(5);
		if (*uart_get == 0xc0)
		{
			uart_get_next();//跳过0xc0
			while (*uart_get != 0xc0)
			{
				j++;
				
				//校验payload格式： 0x00 0x00 0x0x 0x00,满足条件则赋值给数组
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
				//printf("0x%x  ", *uart_get);  //调试打印 :不打印时，会接收不到数据
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
	Packet_Rcv_Flag = 0x00; //接收函数之前必须清理标志位，否则导致判断直接跳过，接收不到数据
	uint32 time_begin = ms_clock();
	
	int i = 9;
	while (i--) //发收分离
	{
       BCSPImplementation_runStack();
	   if (sendpdu_flag == 4)
		   break;
	}
	Sleep(60);   //不接收download的应答，延迟一段时间，等待uart数据的接收
#if 0
	while (Packet_Rcv_Flag != 1)
	{
		Packet_Rcv_download();//等待芯片返回的应答payload，否则一直循环

		if ((ms_clock() - time_begin) > 5000) //接收超时则跳出循环,时间稍长些 2016-11-25
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
	if (setup.bmRequestTypeDirection == dir_host_to_device)//有数据需要发送时
	{
		// Setup packet and buffer to send
		requestLength = sizeof(setup) + setup.wLength;
		//maxReplyLength = sizeof(requestStatus);
		memcpy((void*)PayLoad_Buff, &setup, sizeof(setup));//将setup数据复制到PayLoad_Buff中，下同
		memcpy((void*)(PayLoad_Buff + sizeof(setup)), buffer, setup.wLength);//将buffer数据复制到PayLoad_Buff偏移sizeof(setup)大小
	}
	else//只发送控制命令
	{
		// Only the setup packet to send
		requestLength = sizeof(setup);
		//maxReplyLength = sizeof(requestStatus) + setup.wLength;
        memcpy((void*)PayLoad_Buff, &setup, sizeof(setup));
	}
      
    sendpdu( PayLoad_Buff ,requestLength );

    if( (sendpdu_flag == 1) || (sendpdu_flag == 2) ) return success; //部分函数不需要接收数据，直接退出接收部分

/*****************************************/
    //串口接收处理过程

	Packet_Rcv_Flag = 0x00; //接收函数之前必须清理标志位，否则导致判断直接跳过，接收不到数据
	uint32 time_begin = ms_clock();
	while (Packet_Rcv_Flag != 1)
	{
		Packet_Rcv(); //Sleep(2);
		if ((ms_clock() - time_begin) > 2000) //接收超时则跳出循环 2016-11-22
		{
			printf("time out \n");
			break;
		}
	}

	printf(" Rcv over \n");//数据接收执行完毕
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
 
      memcpy(buffer, uart_get_data, bufferLength);//注意长度

	  for (int i = 0; i < 30; i++)
		  uart_get_data[i] = 0xff; //清零接收缓存数组
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
 
