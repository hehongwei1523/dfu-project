
#include "usart.h"	  
#include "e_type.h"

//#include "com.h"
    

uint8 uart_buf[250];
uint8 * uart_ptr = uart_buf;
uint8 * uart_end = uart_buf + 250;
uint8 index = 0 ;
uint8 uart_data;


