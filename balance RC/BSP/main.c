#include "global.h"
//*****************************************************************************
//中断优先级如下：
////////////////抢占式优先级      子优先级
//定时器5： 0x00                  0x01
//串口3:       0x03                  0x02
//NRF:         0x01                  0x02
//*****************************************************************************
//要写入到STM32 FLASH的字符串数组
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//数组长度	
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)
#define FLASH_SAVE_ADDR  0X08010004 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
										//否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.
int main(void)
{
	 u8 datatemp[SIZE];	
     BSP_Init();//硬件初始化
//     HMI_SendData("page sine");
    HMI_SendData("page panel");
	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)TEXT_Buffer,SIZE);
	printf("write finished\n");
	while(1)
     {       				  
		if(Count_2ms>=1)//
		{
			Count_2ms = 0;
	        Task_500HZ();		
		}
		if(Count_10ms>=5)//
		{
			Count_10ms = 0;
			Task_100HZ();
		}
		if(Count_40ms>=20)//250Hz Task
		{
			Count_40ms = 0;
			Task_25HZ();
		}
		if(Count_250ms>=125)//200Hz Task
		{
			Count_250ms = 0;
			Task_4HZ();
//			STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)datatemp,SIZE);
//			printf("read flash: %s\n",datatemp);
		}

     }
}


