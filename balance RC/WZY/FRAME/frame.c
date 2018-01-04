#include  "frame.h"
#include "global.h"


//format:+/-xxx.xxx
void Float2Char(u8 *buf,float num)
{
	int temp = 0;
	char num_isnative = 0;
	if(num>0.0f)
	{
		buf[0] = '+';
	    temp = (int)num;
	} 
	else 
	{
		buf[0] = '-';
	    temp = (int)(-num);
		num_isnative = 1;
	} 
	buf[1] = temp;
	buf[2] = '.';
	if(num_isnative)temp = (int)(((-num) - temp)*100);
	else temp = (int)((num - temp)*100);
	buf[3] = temp;
}
void Int2Char(u8 *buf,int num)
{
	int temp = 0;
	if(num>=0)
	{
		buf[0] = '+';
	    temp = num;
	} 
	else 
	{
		buf[0] = '-';
	    temp = (-num);
	} 
	buf[1] = ((temp>>8)&0xFF);
	buf[2] = ((temp>>0)&0xFF);
	buf[3] = 0xFF;
}

void RX_Frame(void)
{
//		u8 rx_try = 0;
//		NRF24L01_RX_Mode();	    				  
//		while(NRF24L01_RxPacket(tmp_buf)!=0)//一旦接收到信息,则显示出来.
//		{
//			rx_try++;
//			if(rx_try>2)break;//尝试3次接收都失败了
//		} 	
//		if(rx_try!=3)//接收完成
//		{
//			tmp_buf[32]=0;//加入字符串结束符
//			OLED_ShowString(0,48,tmp_buf,16);
//		}	

}
void TX_Frame(void)
{

//			NRF24L01_TX_Mode();
//			NRF24L01_TxPacket((u8*)&tx_frame);
//			{
//				txdata_isfinished = 1;	
//			}
}

void  TX_PackageFrame(void)
{
		Float2Char(tx_frame.Angle_Balance,balance_angle.Pitch);
		Float2Char(tx_frame.Gyro_Balance,Gyro_Balance);
		Int2Char(tx_frame.PWM_Moto,pwm_moto);
	    Float2Char(tx_frame.BatVal,(float)BatVal*7.7f);
		Float2Char(tx_frame.KP,balance_kp);
		Float2Char(tx_frame.KD,balance_kd);
	    txdata_isok = 1;
}
//void Nrf_Check_Event(void)
//{
//	u8 sta = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
//	////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////
//	if(sta & (1<<RX_DR))
//	{
//		u8 rx_len = NRF_Read_Reg(R_RX_PL_WID);
//		if(rx_len<33)
//		{
//			NRF_Read_Buf(RD_RX_PLOAD,NRF24L01_RXDATA,rx_len);// read receive payload from RX_FIFO buffer
//			//LED1_ONOFF();
////			Data_Receive_Anl(NRF24L01_RXDATA,rx_len);//解析数据
//			OLED_ShowString(0,48,"analysis",16);
//		}
//		else 
//		{
//			NRF_Write_Reg(FLUSH_RX,0xff);//清空缓冲区
//		}
//	}
//	////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////
//	if(sta & (1<<TX_DS))
//	{
//		//LED1_ONOFF();
//	}
//	////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////
//	if(sta & (1<<MAX_RT))
//	{
//		if(sta & 0x01)	//TX FIFO FULL
//		{
//			NRF_Write_Reg(FLUSH_TX,0xff);
//		}
//	}
//	////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////
//	NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, sta);
//}
