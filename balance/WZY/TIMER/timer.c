#include   "timer.h"
void  TIM3CH1CH2_PWM_Init(u16 arr,u16 psc)
{
	  GPIO_InitTypeDef GPIO_InitStru; 	
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
	  TIM_OCInitTypeDef TIM_OCInitStru;
	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3);
	  GPIO_InitStru.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	  GPIO_InitStru.GPIO_Mode=GPIO_Mode_AF;
	  GPIO_InitStru.GPIO_OType=GPIO_OType_PP;
	  GPIO_InitStru.GPIO_Speed=GPIO_Speed_100MHz;
	  GPIO_InitStru.GPIO_PuPd=GPIO_PuPd_UP;
	  GPIO_Init(GPIOB,&GPIO_InitStru);
	
	  TIM_TimeBaseInitStru.TIM_ClockDivision=TIM_CKD_DIV1;
	  TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;
	  TIM_TimeBaseInitStru.TIM_Period=arr;
	  TIM_TimeBaseInitStru.TIM_Prescaler=psc;
	  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStru);
	   
	  TIM_OCInitStru.TIM_OCMode=TIM_OCMode_PWM1;//PWM模式1
	  TIM_OCInitStru.TIM_OutputState=TIM_OutputState_Enable;
	  TIM_OCInitStru.TIM_OCPolarity=TIM_OCPolarity_High ;//输出极性高
	  TIM_OC1Init(TIM3,&TIM_OCInitStru);
	  TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);

	  TIM_OCInitStru.TIM_OCMode=TIM_OCMode_PWM1;//PWM模式1
	  TIM_OCInitStru.TIM_OutputState=TIM_OutputState_Enable;
	  TIM_OCInitStru.TIM_OCPolarity=TIM_OCPolarity_High ;//输出极性高
	  TIM_OC2Init(TIM3,&TIM_OCInitStru);
	  TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	  
	  TIM_ARRPreloadConfig(TIM3,ENABLE);
	  TIM_Cmd(TIM3,ENABLE);
}
///*
// * arr:自动重装载值 psc:预分频数
// * 定时器溢出时间Tout:((arr+1)*(psc+1))/Ft us
// * Ft:定时器工作频率 84MHz
// *最大定时时长65536*65536/84/1000/1000=51.13s
// */
void  Timer5_Init(u16 arr,u16 psc)
{
	  
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	  NVIC_InitTypeDef  NVIC_InitStructure;
	  
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	  
	  TIM_TimeBaseInitStructure.TIM_Period=arr;
	  TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
	  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
	  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	  
	  TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	  TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	  
	  NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	 
	  TIM_Cmd(TIM5,ENABLE);
}
void TIM5_IRQHandler(void)
{

   if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)//定时时间到
   {
	    Timer5_count++;   //更新中断计数
	    Count_1ms++;
	    Count_2ms++;
	    Count_4ms++; 
	    Count_5ms++; 
	   Count_10ms++;
	   Count_100ms++;
	   Count_200ms++;
	   Count_50ms++;
	   Count_500ms++;
   }
   
   TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
}

int  PWM_AmpLimit(int pwm_temp)
{
		int temp;
		if(pwm_temp>PWM_MAX)
		{
			temp = PWM_MAX;
		}
		else if(pwm_temp<-PWM_MAX)
		{
			temp = -PWM_MAX;
		}
		else 
		{	
			temp = pwm_temp;
		}
		return temp;
}

