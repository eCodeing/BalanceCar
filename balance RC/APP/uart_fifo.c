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
		//д���ݵ�������
		pUART->pTxBuf[pUART->TxWriteIndex] = buf[i];
		//�ж�дָ���Ƿ񳬹��˻������������򷵻�ͷ��
		if(++pUART->TxWriteIndex >= pUART->TxBufSize)
		{
			pUART->TxWriteIndex = 0;
		}
	}
	//һ������TXE�жϣ������������ISR
	USART_ITConfig(pUART->UART,USART_IT_TXE,ENABLE);

}
//��FIFO�еõ�һ���ֽ�
uint8_t  Get_From_FIFO(gUART_FIFO  *pUART,uint8_t * byte)
{
	if(pUART->RxReadIndex == pUART->RxWriteIndex)
		return 1;
	*byte = pUART->pRxBuf[pUART->RxReadIndex];
	if(++pUART->RxReadIndex >= pUART->RxBufSize)
		pUART->RxReadIndex = 0;
	return 0;
}
//�������봮�ڽ�������
void  UART_ISR(gUART_FIFO *pUART)
{
	if(USART_GetITStatus(pUART->UART,USART_IT_RXNE) != RESET)
	{
		pUART->pRxBuf[pUART->RxWriteIndex] = USART_ReceiveData(pUART->UART);
		if(++pUART->RxWriteIndex >= pUART->RxBufSize )
			pUART->RxWriteIndex = 0;
		
		//�˴��������ûص��������߱�־λ֪ͨӦ�ó����յ�������
	}
//���Ͳ��ִ���	
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
			/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
			USART_SendData(pUART->UART,pUART->pTxBuf[pUART->TxReadIndex]);
			if(++pUART->TxReadIndex >= pUART->TxBufSize)
			{
				pUART->TxReadIndex = 0;
			}
		}
	}	/* ����bitλȫ��������ϵ��ж� */
	else if(USART_GetITStatus(pUART->UART,USART_IT_TC) != RESET)
	{
		if(pUART->TxReadIndex == pUART->TxWriteIndex)
		{
			USART_ITConfig(pUART->UART,USART_IT_TC,DISABLE);
		}
		else
		{
						/* ��������£��������˷�֧ */
			
			/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
			USART_SendData(pUART->UART,pUART->pTxBuf[pUART->TxReadIndex]);
			if(++pUART->TxReadIndex >= pUART->TxBufSize)
			{
				pUART->TxReadIndex = 0;
			}
		}
	}
		   
}

