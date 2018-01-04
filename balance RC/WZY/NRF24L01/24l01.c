#include "24l01.h"


uint8_t  NRF24L01_RXDATA[RX_PLOAD_WIDTH];
uint8_t  NRF24L01_TXDATA[TX_PLOAD_WIDTH];
u8 TX_ADDRESS[TX_ADR_WIDTH]={0xAA,0xBB,0xCC,0x00,0x01}; //发送地址
u8 RX_ADDRESS[RX_ADR_WIDTH]={0xAA,0xBB,0xCC,0x00,0x01}; //发送地址
/*
*****************************************************************
* 写寄存器
*****************************************************************
*/
uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
{
	  u8 status;	
	  NRF24L01_CSN=0;                 //使能SPI传输
	  status =SPI2_ReadWriteByte(reg);//发送寄存器号 
	  SPI2_ReadWriteByte(value);      //写入寄存器的值
	  NRF24L01_CSN=1;                 //禁止SPI传输	   
	  return(status);       			//返回状态值
}
/*
*****************************************************************
* 读寄存器
*****************************************************************
*/
uint8_t NRF_Read_Reg(uint8_t reg)
{
	u8 reg_val;	    
  NRF24L01_CSN = 0;          //使能SPI传输		
  SPI2_ReadWriteByte(reg);   //发送寄存器号
  reg_val=SPI2_ReadWriteByte(0XFF);//读取寄存器内容
  NRF24L01_CSN = 1;          //禁止SPI传输		    
  return(reg_val);           //返回状态值
}

/*
*****************************************************************
*
* 写缓冲区
*
*****************************************************************
*/
uint8_t NRF_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
  u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //使能SPI传输
  status = SPI2_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI2_ReadWriteByte(*pBuf++); //写入数据	 
  NRF24L01_CSN = 1;       //关闭SPI传输
  return status;          //返回读到的状态值
}
/*
*****************************************************************
* 读缓冲区
*****************************************************************
*/
uint8_t NRF_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
  u8 status,u8_ctr;	       
  NRF24L01_CSN = 0;           //使能SPI传输
  status=SPI2_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
  for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI2_ReadWriteByte(0XFF);//读出数据
  NRF24L01_CSN=1;       //关闭SPI传输
  return status;        //返回读到的状态值
}
/*
*****************************************************************
* 写数据包
*****************************************************************
*/
void NRF_TxPacket(uint8_t * tx_buf, uint8_t len)
{	
	NRF24L01_CE=0; 	 //StandBy I模式	
	NRF_Write_Buf(WR_TX_PLOAD, tx_buf, len); 			 // 装载数据	
	NRF24L01_CE=1; 		 //置高CE，激发数据发送
}
void NRF_TxPacket_AP(uint8_t * tx_buf, uint8_t len)
{	
	NRF24L01_CE=0; 		 //StandBy I模式	
	NRF_Write_Buf(0xa8, tx_buf, len); 			 // 装载数据
	NRF24L01_CE=1; 	 //置高CE
}
u8 Nrf24l01_Check(void)
{ 
	u8 buf1[5]; 
	u8 i; 
	/*写入5个字节的地址. */ 
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5); 
	/*读出写入的地址 */ 
	NRF_Read_Buf(TX_ADDR,buf1,5); 
	/*比较*/ 
	for(i=0;i<5;i++) 
	{ 
		if(buf1[i]!=TX_ADDRESS[i]) 
			break; 
	} 
	if(i==5)
		return SUCCESS ; //MCU与NRF成功连接 
	else
		return ERROR ; //MCU与NRF不正常连接 
}
 void Nrf24l01_Init(u8 model, u8 ch)
{
	NRF24L01_CE=0;
	NRF_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);	//写RX节点地址 
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH); 		//写TX节点地址  
	NRF_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); 													//使能通道0的自动应答 
	NRF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);											//使能通道0的接收地址 
	NRF_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);											//设置自动重发间隔时间:500us;最大自动重发次数:10次 
	NRF_Write_Reg(NRF_WRITE_REG+RF_CH,80);														//设置RF通道为CHANAL
	NRF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); 												//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	//NRF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07); 												//设置TX发射参数,0db增益,1Mbps,低噪声增益开启
/////////////////////////////////////////////////////////
	if(model==1)				//RX
	{
		NRF_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//选择通道0的有效数据宽度 
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		 // IRQ收发完成中断开启,16位CRC,主接收
	}
	else if(model==2)		//TX
	{
		NRF_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//选择通道0的有效数据宽度 
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断开启,16位CRC,主发送
	}
	else if(model==3)		//RX2
	{
		NRF_Write_Reg(FLUSH_TX,0xff);
		NRF_Write_Reg(FLUSH_RX,0xff);
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		 // IRQ收发完成中断开启,16位CRC,主接收
		
		SPI2_ReadWriteByte(0x50);
		SPI2_ReadWriteByte(0x73);
		NRF_Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		NRF_Write_Reg(NRF_WRITE_REG+0x1d,0x06);
	}
	else								//TX2
	{
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断开启,16位CRC,主发送
		NRF_Write_Reg(FLUSH_TX,0xff);
		NRF_Write_Reg(FLUSH_RX,0xff);
		
		SPI2_ReadWriteByte(0x50);
		SPI2_ReadWriteByte(0x73);
		NRF_Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		NRF_Write_Reg(NRF_WRITE_REG+0x1d,0x06);
	}
	NRF24L01_CE=1;
}		   



