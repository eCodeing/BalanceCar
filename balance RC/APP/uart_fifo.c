#include "uart_fifo.h"

#define  UART2_TXBUF_SIZE   2*1024
#define  UART2_RXBUF_SIZE   2*1024
//gUART_FIFO  gUART1;
gUART_FIFO  gUART2; 
static uint8_t g_TxBuf2[UART2_TXBUF_SIZE];
static uint8_t g_RxBuf2[UART2_RXBUF_SIZE];

void UartVarInit(void)
{
	gUART2.UART = USART2;
	gUART2.pRxBuf = g_RxBuf2;
	gUART2.pTxBuf = g_TxBuf2;
	gUART2.RxBufSize = UART2_RXBUF_SIZE;
	gUART2.TxBufSize = UART2_TXBUF_SIZE;
	gUART2.RxReadIndex = 0;
	gUART2.RxWriteIndex = 0;
	gUART2.TxReadIndex = 0;
	gUART2.TxWriteIndex = 0;
}
void  Send_To_FIFO(gUART_FIFO  *pUART,const char * buf)
{
	uint16_t i;

	for(i = 0;i < strlen(buf);i++)
	{
		//写数据到缓冲区
		pUART->pTxBuf[pUART->TxWriteIndex] = buf[i];
		//判断写指针是否超过了缓冲区，超过则返回头部
		if(++pUART->TxWriteIndex >= pUART->TxBufSize)
		{
			pUART->TxWriteIndex = 0;
		}
	}
	//一旦配置TXE中断，则会立即进入ISR
	USART_ITConfig(pUART->UART,USART_IT_TXE,ENABLE);

}
//从FIFO中得到一个字节
uint8_t  Get_From_FIFO(gUART_FIFO  *pUART,uint8_t * byte)
{
	if(pUART->RxReadIndex == pUART->RxWriteIndex)
		return 1;
	*byte = pUART->pRxBuf[pUART->RxReadIndex];
	if(++pUART->RxReadIndex >= pUART->RxBufSize)
		pUART->RxReadIndex = 0;
	return 0;
}
//缓冲区与串口交互函数
void  UART_ISR(gUART_FIFO *pUART)
{
	if(USART_GetITStatus(pUART->UART,USART_IT_RXNE) != RESET)
	{
		pUART->pRxBuf[pUART->RxWriteIndex] = USART_ReceiveData(pUART->UART);
		if(++pUART->RxWriteIndex >= pUART->RxBufSize )
			pUART->RxWriteIndex = 0;
		
		//此处可以设置回调函数或者标志位通知应用程序收到了数据
	}
//发送部分处理	
//---------------------------------------------------------------------------------------------------------------------------------//	
	if(USART_GetITStatus(pUART->UART,USART_IT_TXE) != RESET)
	{
		if(pUART->TxReadIndex == pUART->TxWriteIndex)
		{
			USART_ITConfig(pUART->UART,USART_IT_TXE,DISABLE);
			USART_ITConfig(pUART->UART,USART_IT_TC,ENABLE);
		}
		else
		{
			/* 从发送FIFO取1个字节写入串口发送数据寄存器 */
			USART_SendData(pUART->UART,pUART->pTxBuf[pUART->TxReadIndex]);
			if(++pUART->TxReadIndex >= pUART->TxBufSize)
			{
				pUART->TxReadIndex = 0;
			}
		}
	}	/* 数据bit位全部发送完毕的中断 */
	else if(USART_GetITStatus(pUART->UART,USART_IT_TC) != RESET)
	{
		if(pUART->TxReadIndex == pUART->TxWriteIndex)
		{
			USART_ITConfig(pUART->UART,USART_IT_TC,DISABLE);
		}
		else
		{
						/* 正常情况下，不会进入此分支 */
			
			/* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */
			USART_SendData(pUART->UART,pUART->pTxBuf[pUART->TxReadIndex]);
			if(++pUART->TxReadIndex >= pUART->TxBufSize)
			{
				pUART->TxReadIndex = 0;
			}
		}
	}
		   
}

