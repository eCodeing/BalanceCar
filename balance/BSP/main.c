#include "global.h"
//*****************************************************************************
//中断优先级如下：
////////////////抢占式优先级      子优先级
//定时器5： 0x00                  0x01
//串口3:       0x03                  0x02
//NRF:         0x01                  0x02
//*****************************************************************************
int main(void)
{
	 
     BSP_Init();//硬件初始化

     while(1)
     {       				  
		if(Count_1ms>=2)//1000Hz Task
		{
			Count_1ms = 0;
	        Task_1ms();		
		}
		if(Count_2ms>=4)//500Hz Task
		{
			Count_2ms = 0;
			Task_2ms();
		}
		if(Count_4ms>=8)//250Hz Task
		{
			Count_4ms = 0;
			Task_4ms();
		}
		if(Count_5ms>=10)//200Hz Task
		{
			Count_5ms = 0;
			Task_5ms();
		}
		if(Count_50ms>=100)//20Hz Task
		{
			Count_50ms = 0;
			Task_50ms();
		}
		if(Count_10ms>=20)//100Hz Task
		{
			Count_10ms = 0;
			Task_10ms();
		}	
		if(Count_100ms>=200)//100Hz Task
		{
			Count_100ms = 0;
			Task_100ms();
		}	
		if(Count_200ms>=400)//100Hz Task
		{
			Count_200ms = 0;
			Task_200ms();
		}				
		if(Count_500ms>=1000)//2Hz Task
		{
			Count_500ms = 0;
			Task_500ms();
		}
     }
}


