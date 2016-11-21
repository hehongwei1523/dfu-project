
#include "usart.h"	  
#include "e_type.h"

//#include "com.h"
    

uint8 uart_buf[250];
uint8 * uart_ptr = uart_buf;
uint8 * uart_end = uart_buf + 250;
uint8 index = 0 ;
uint8 uart_data;

void USART1_IRQHandler(unsigned char *data)                	//串口1中断服务程序
{
                      
    if( uart_ptr != uart_end )
    {
		*uart_ptr++ = data;// USART_ReceiveData(USART1);
        //uart_buf[index]=USART_ReceiveData(USART1);
        //uart_ptr++;
        //printf("%c",uart_buf[index]);
        //printf("%c",*(uart_buf+index));
        //printf("%x",*(uart_ptr-1));
		//printf(" index = %d ",index);
        index++;
    }
    else
    {
        index = 0;
        uart_ptr = uart_buf;
    }
            
            
    if( *(uart_ptr-1) == 0xc0 )
        uart_data++;
              
    if( uart_data == 2 ) 
    {
		printf(" flag \n ");
        uart_data = 0;  
        BCSP_DATA_RCV();
		
		BCSPImplementation_Test();
        //get_task_scheduler() ->set_event(test_task_id,EVENT_BCSP_DATA);
    }
                
} 


