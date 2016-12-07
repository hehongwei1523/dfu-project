
#include "usart.h"	  
#include "e_type.h"

//#include "com.h"
    
#define uart_buf_size 128
uint8 uart_buf[uart_buf_size];
uint8 * uart_ptr = uart_buf;
uint8 * uart_end = uart_buf + uart_buf_size;
uint8 uart_data;


