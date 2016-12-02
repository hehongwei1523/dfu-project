//#include "DFURequest.h"

#define Max_Buff_Size 1050
extern uint8 Send_Buff[];

//typedef enum ResultCode Result;

//Result ControlRequest(const struct Setup setup, void *buffer, uint16 bufferLength, uint16 *replyLength);

extern void sendpdu( uint8 *buff, int size );

//extern uint8 uart_get_data[];
extern uint8 * uart_get;
extern void uart_get_next(void);










