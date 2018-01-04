#ifndef  __UART_FIFO_H
#define __UART_FIFO_H

#include "global.h"

typedef struct gUART_FIFO_
{
	USART_TypeDef *UART;
	uint8_t * pTxBuf;
	uint8_t * pRxBuf;
	
	uint16_t TxBufSize;
	uint16_t RxBufSize;
    volatile	uint16_t TxWriteIndex;
    volatile	uint16_t RxWriteIndex;
	volatile uint16_t TxReadIndex;
	volatile uint16_t RxReadIndex;
	
}gUART_FIFO;

typedef enum
{
	UART1_ = 0,
	UART2_,
	UART3_
}UARTn;	
void  UartVarInit(void);
void  UART_ISR(gUART_FIFO *pUART);
void  Send_To_FIFO(gUART_FIFO  *pUART,const char * buf);
uint8_t  Get_From_FIFO(gUART_FIFO  *pUART,uint8_t * byte);
#endif
