#include "24l01.h"


uint8_t  NRF24L01_RXDATA[RX_PLOAD_WIDTH];
uint8_t  NRF24L01_TXDATA[TX_PLOAD_WIDTH];
u8 TX_ADDRESS[TX_ADR_WIDTH]={0xAA,0xBB,0xCC,0x00,0x01}; //���͵�ַ
u8 RX_ADDRESS[RX_ADR_WIDTH]={0xAA,0xBB,0xCC,0x00,0x01}; //���͵�ַ
/*
*****************************************************************
* д�Ĵ���
*****************************************************************
*/
uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
{
	  u8 status;	
	  NRF24L01_CSN=0;                 //ʹ��SPI����
	  status =SPI2_ReadWriteByte(reg);//���ͼĴ����� 
	  SPI2_ReadWriteByte(value);      //д��Ĵ�����ֵ
	  NRF24L01_CSN=1;                 //��ֹSPI����	   
	  return(status);       			//����״ֵ̬
}
/*
*****************************************************************
* ���Ĵ���
*****************************************************************
*/
uint8_t NRF_Read_Reg(uint8_t reg)
{
	u8 reg_val;	    
  NRF24L01_CSN = 0;          //ʹ��SPI����		
  SPI2_ReadWriteByte(reg);   //���ͼĴ�����
  reg_val=SPI2_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  NRF24L01_CSN = 1;          //��ֹSPI����		    
  return(reg_val);           //����״ֵ̬
}

/*
*****************************************************************
*
* д������
*
*****************************************************************
*/
uint8_t NRF_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
  u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����
  status = SPI2_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI2_ReadWriteByte(*pBuf++); //д������	 
  NRF24L01_CSN = 1;       //�ر�SPI����
  return status;          //���ض�����״ֵ̬
}
/*
*****************************************************************
* ��������
*****************************************************************
*/
uint8_t NRF_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
  u8 status,u8_ctr;	       
  NRF24L01_CSN = 0;           //ʹ��SPI����
  status=SPI2_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
  for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI2_ReadWriteByte(0XFF);//��������
  NRF24L01_CSN=1;       //�ر�SPI����
  return status;        //���ض�����״ֵ̬
}
/*
*****************************************************************
* д���ݰ�
*****************************************************************
*/
void NRF_TxPacket(uint8_t * tx_buf, uint8_t len)
{	
	NRF24L01_CE=0; 	 //StandBy Iģʽ	
	NRF_Write_Buf(WR_TX_PLOAD, tx_buf, len); 			 // װ������	
	NRF24L01_CE=1; 		 //�ø�CE���������ݷ���
}
void NRF_TxPacket_AP(uint8_t * tx_buf, uint8_t len)
{	
	NRF24L01_CE=0; 		 //StandBy Iģʽ	
	NRF_Write_Buf(0xa8, tx_buf, len); 			 // װ������
	NRF24L01_CE=1; 	 //�ø�CE
}
u8 Nrf24l01_Check(void)
{ 
	u8 buf1[5]; 
	u8 i; 
	/*д��5���ֽڵĵ�ַ. */ 
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5); 
	/*����д��ĵ�ַ */ 
	NRF_Read_Buf(TX_ADDR,buf1,5); 
	/*�Ƚ�*/ 
	for(i=0;i<5;i++) 
	{ 
		if(buf1[i]!=TX_ADDRESS[i]) 
			break; 
	} 
	if(i==5)
		return SUCCESS ; //MCU��NRF�ɹ����� 
	else
		return ERROR ; //MCU��NRF���������� 
}
 void Nrf24l01_Init(u8 model, u8 ch)
{
	NRF24L01_CE=0;
	NRF_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);	//дRX�ڵ��ַ 
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH); 		//дTX�ڵ��ַ  
	NRF_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); 													//ʹ��ͨ��0���Զ�Ӧ�� 
	NRF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);											//ʹ��ͨ��0�Ľ��յ�ַ 
	NRF_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);											//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10�� 
	NRF_Write_Reg(NRF_WRITE_REG+RF_CH,80);														//����RFͨ��ΪCHANAL
	NRF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); 												//����TX�������,0db����,2Mbps,���������濪��
	//NRF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07); 												//����TX�������,0db����,1Mbps,���������濪��
/////////////////////////////////////////////////////////
	if(model==1)				//RX
	{
		NRF_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//ѡ��ͨ��0����Ч���ݿ�� 
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		 // IRQ�շ�����жϿ���,16λCRC,������
	}
	else if(model==2)		//TX
	{
		NRF_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//ѡ��ͨ��0����Ч���ݿ�� 
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����жϿ���,16λCRC,������
	}
	else if(model==3)		//RX2
	{
		NRF_Write_Reg(FLUSH_TX,0xff);
		NRF_Write_Reg(FLUSH_RX,0xff);
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		 // IRQ�շ�����жϿ���,16λCRC,������
		
		SPI2_ReadWriteByte(0x50);
		SPI2_ReadWriteByte(0x73);
		NRF_Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		NRF_Write_Reg(NRF_WRITE_REG+0x1d,0x06);
	}
	else								//TX2
	{
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����жϿ���,16λCRC,������
		NRF_Write_Reg(FLUSH_TX,0xff);
		NRF_Write_Reg(FLUSH_RX,0xff);
		
		SPI2_ReadWriteByte(0x50);
		SPI2_ReadWriteByte(0x73);
		NRF_Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		NRF_Write_Reg(NRF_WRITE_REG+0x1d,0x06);
	}
	NRF24L01_CE=1;
}		   



