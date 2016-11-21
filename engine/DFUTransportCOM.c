
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

//extern void sendpdu ( int Packet_size );
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
//uint8 uart_get_data[30] = {0};

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

uint8 Packet_Rdv_Flag = 0x00;
uint8 uart_get_data[30] = { 0 };
void Packet_Rcv(void)
{
	Packet_Rdv_Flag = 0x00;
uart_get = uart_handle;
while (uart_get != uart_ptr)
{
	int i = 0, j = 0, k = 0;

	if (*uart_get == 0xc0)
	{
		uart_get_next();//跳过0xc0

		while (*uart_get != 0xc0)
		{
			j++;
			printf("0x%x  ", *uart_get);
			//校验payload格式： 0x00 0x00 0x0x ,满足条件则赋值给数组
			if ((j == 5) && (*uart_get == 0x00)) k++;
			if ((j == 6) && (*uart_get == 0x00)) k++;
			if ((j == 7) && (*uart_get != 0x00)) k++;

			if (j > 8 && k == 3)
			{
				
				uart_get_data[i++] = *uart_get; 
				Packet_Rdv_Flag = 0x01;
			}
			uart_get_next();
		}

	}
	uart_get_next();
}
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
      
    //BCSPImplementation_Test(); 

    sendpdu( PayLoad_Buff ,requestLength );

    //这个延时处理得不是很好 2016-11-10
    //Delay_Ms(2000); //延时一下，等待芯片发送的数据
	//Sleep(2000);
//*****************************************
    //串口接收处理过程
	
	while (Packet_Rdv_Flag != 1)
	{
		Packet_Rcv();
	}
	printf("bb  ");//死循环，等待数据接收
	//Sleep(2000);
//*****************************************/
      
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
		  uart_get_data[i] = 0x00;
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
 
