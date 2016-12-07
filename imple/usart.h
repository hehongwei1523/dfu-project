#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "e_type.h"

extern void uart_init(uint32 bound);
extern int8 uart_send(uint8* buf,uint16 len);

extern uint8 uart_buf[];
extern uint8 * uart_ptr;
extern uint8 * uart_end;
extern uint8 * uart_handle;

extern void BCSP_DATA_RCV(void);
#endif


