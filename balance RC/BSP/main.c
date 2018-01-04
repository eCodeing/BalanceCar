#include "global.h"
//*****************************************************************************
//�ж����ȼ����£�
////////////////��ռʽ���ȼ�      �����ȼ�
//��ʱ��5�� 0x00                  0x01
//����3:       0x03                  0x02
//NRF:         0x01                  0x02
//*****************************************************************************
//Ҫд�뵽STM32 FLASH���ַ�������
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//���鳤��	
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)
#define FLASH_SAVE_ADDR  0X08010004 	//����FLASH �����ַ(����Ϊż��������������,Ҫ���ڱ�������ռ�õ�������.
										//����,д������ʱ��,���ܻᵼ�²�����������,�Ӷ����𲿷ֳ���ʧ.��������.
int main(void)
{
	 u8 datatemp[SIZE];	
     BSP_Init();//Ӳ����ʼ��
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


