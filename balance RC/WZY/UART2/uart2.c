#include  "uart2.h"
extern gUART_FIFO gUART2;
void UART2_init(u32 bound){
	
   //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	UartVarInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART3时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOB11复用为USART3
	
	//USART3端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PB10，PB11

   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口3
	
  USART_Cmd(USART2, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x01;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

}
void UART2_SendData(char* buf)
{
//    while(*buf!='\0')
//		{
//		    	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
//	        USART2->DR = *buf;   
//			    buf++;
//		}
	Send_To_FIFO(&gUART2,buf);
}

void USART2_IRQHandler(void)
{
//	  uint8_t ch_uart2;
//	  
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
//		{
//		   if(num_recv2==(BUFFER_UART2_LEN-1))num_recv2 = 0;
//		    ch_uart2 = USART_ReceiveData(USART2);//(USART3->DR);	//读取接收到的数据
//		    Buffer_Uart2[num_recv2++] = ch_uart2;
//			  
//		}
	UART_ISR(&gUART2);
}
