#include "exti.h"

void EXTIx_Init(void)
{
    NVIC_InitTypeDef    NVIC_InitStructure;
    EXTI_InitTypeDef    EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//普通输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);
    
	//使能SYSCFG时钟 此处与103不大一样
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource12);
	//EXTI_Line0
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//配置NVIC
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
{

  if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
  {
//		NRF24L01_IRQHandler();
	NRF24L01_HandlerIRQ();
  }
   EXTI_ClearITPendingBit(EXTI_Line12);
}
