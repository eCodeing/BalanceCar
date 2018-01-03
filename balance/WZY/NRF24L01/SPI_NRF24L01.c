//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "global.h"
#include  "SPI_NRF24L01.h"
/******************************************************************************
							�궨��
*******************************************************************************/ 
#define NRF_CE_GPIO		GPIOC	
#define NRF_CE_Pin		GPIO_Pin_8
#define NRF_CSN_GPIO	GPIOC		
#define NRF_CSN_Pin		GPIO_Pin_9	
#define NRF_IRQ_GPIO	GPIOB
#define NRF_IRQ_Pin		GPIO_Pin_12

//#define NRF_CE_H   		NRF_CE_GPIO ->BSRR = NRF_CE_Pin  //CE�ߵ�ƽ
//#define NRF_CE_L   		NRF_CE_GPIO ->BRR  = NRF_CE_Pin  //CE�͵�ƽ
//#define NRF_CSN_H  		NRF_CSN_GPIO->BSRR = NRF_CSN_Pin //CSN�ߵ�ƽ
//#define NRF_CSN_L  		NRF_CSN_GPIO->BRR  = NRF_CSN_Pin //CSN�ߵ�ƽ
//#define	NRF_IRQ_Read	NRF_IRQ_GPIO->IDR  & NRF_IRQ_Pin //IRQ������
#define NRF_CE_H   		GPIO_SetBits(GPIOC,GPIO_Pin_8)  //CE�ߵ�ƽ
#define NRF_CE_L   		GPIO_ResetBits(GPIOC,GPIO_Pin_8)  //CE�͵�ƽ
#define NRF_CSN_H  		GPIO_SetBits(GPIOC,GPIO_Pin_9) //CSN�ߵ�ƽ
#define NRF_CSN_L  		GPIO_ResetBits(GPIOC,GPIO_Pin_9) //CSN�ߵ�ƽ
#define	NRF_IRQ_Read	NRF_IRQ_GPIO->IDR  & NRF_IRQ_Pin //IRQ������
/******************************************************************************
							��������
*******************************************************************************/ 
uint8_t NRF24L01_RXDATA[32];//nrf24l01���յ�������
uint8_t NRF24L01_TXDATA[32];//nrf24l01��Ҫ���͵�����
static uint8_t TX_ADDRESS[5]= {0x1A,0x3B,0x5C,0x7D,0x9E};//���ص�ַ
static uint8_t RX_ADDRESS[5]= {0x1A,0x3B,0x5C,0x7D,0x9E};//���յ�ַ
static uint16_t Nrf_Erro;

/******************************************************************************
����ԭ��:	void SPI1_Init(void)
��������:	��ʼ��SPI����
*******************************************************************************/ 
void SPI2_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI1ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOB,Gʱ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��PC8,9
	
	//GPIOB12��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PG8
  //GPIOFB3,4,5��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB5����Ϊ SPI1

 
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//��λSPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

	SPI_RW(0xff);//��������		
	NRF_CSN_H;	//��ֹNRF����
}
/******************************************************************************
����ԭ��:	uint8_t SPI_RW(uint8_t data) 
��������:	SPI���߶�д
�� �� ֵ��	����SPI���߶�ȡ����
*******************************************************************************/ 
uint8_t SPI_RW(uint8_t data) 
{ 
 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI2, data); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����	
 		    
}

/******************************************************************************
����ԭ�ͣ�	uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
��    �ܣ�	NRFд�Ĵ���
�� �� ֵ��	NRFд�Ĵ�������ֵ
*******************************************************************************/
uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	NRF_CSN_L;		//ѡͨNRF����
	status = SPI_RW(reg);//д�Ĵ�����ַ
	SPI_RW(value);	//д����
	NRF_CSN_H;		//��ֹNRF����
	return 	status;
}

/******************************************************************************
����ԭ�ͣ�	uint8_t NRF_Read_Reg(uint8_t reg)
��    �ܣ�	NRF���Ĵ���
�� �� ֵ��	�Ĵ�������
*******************************************************************************/
uint8_t NRF_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	NRF_CSN_L;		//ѡͨNRF���� 
	SPI_RW(reg);	//д�Ĵ�����ַ
	reg_val = SPI_RW(0);//��ȡ�üĴ�����������
	NRF_CSN_H;		//��ֹNRF���� 
    return 	reg_val;
}

/******************************************************************************
����ԭ�ͣ�	uint8_t NRF_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
��    �ܣ�	NRFд������
�� �� ֵ��	NRFд����������ֵ
*******************************************************************************/
uint8_t NRF_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	NRF_CSN_L;	//ѡͨNRF���� 
	status = SPI_RW(reg);//д�Ĵ�����ַ 
	for(i=0; i<uchars; i++)
	{
		SPI_RW(pBuf[i]);//д���� 
	}
	NRF_CSN_H;	//��ֹNRF����
    return 	status;	
}

/******************************************************************************
����ԭ�ͣ�	uint8_t NRF_Read_Buff(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
��    �ܣ�	NRF��������
�� �� ֵ��	����������
*******************************************************************************/
uint8_t NRF_Read_Buff(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	NRF_CSN_L;	//ѡͨNRF���� 
	status = SPI_RW(reg);//д�Ĵ�����ַ
	for(i=0; i<uchars; i++)
	{
		pBuf[i] = SPI_RW(0);//��ȡ��������	
	}
	NRF_CSN_H;	//��ֹNRF����
    return 	status;
}

/******************************************************************************
����ԭ�ͣ�	static void NRF24L01_Set_TX(void)
��    �ܣ�	��NRF24L01����Ϊ����ģʽ
*******************************************************************************/
static void NRF24L01_Set_TX(void)
{
	NRF_CE_L;
	NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0E);//����
	NRF_CE_H;
}

/******************************************************************************
����ԭ�ͣ�	static void NRF24L01_Set_RX(void)
��    �ܣ�	��NRF24L01����Ϊ����ģʽ
*******************************************************************************/
static void NRF24L01_Set_RX(void)
{
	NRF_CE_L;
	NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0F);//����
	NRF_CE_H;
}

/******************************************************************************
����ԭ�ͣ�	void NRF_Send_TX(uint8_t * tx_buf, uint8_t len)
��    �ܣ�	NRF2401�������ݰ�
*******************************************************************************/
void NRF_Send_TX(uint8_t * tx_buf, uint8_t len)
{		
	NRF24L01_Set_TX();
	NRF_CE_L;//�������ģʽ1	
	NRF_Write_Buf(WR_TX_PLOAD, tx_buf, len);//װ������
	NRF_CE_H;//����CEΪ�ߣ��������䡣CE�ߵ�ƽ����ʱ����СΪ10us
}

/******************************************************************************
����ԭ�ͣ�	void NRF24L01_Check(void)
��    �ܣ�	���NRF�����Ƿ�����
*******************************************************************************/
uint8_t NRF24L01_Check(void)
{ 
	uint8_t buf[5]; 
	uint8_t i; 
	//д��5���ֽڵĵ�ַ 
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5); 
	//����д��ĵ�ַ 
	NRF_Read_Buff(TX_ADDR,buf,5); 
	//�Ƚ�
	for(i=0;i<5;i++) 
	{ 
		if(buf[i]!=TX_ADDRESS[i]) 
			break; 
	} 
	if(i==5)
		return 0;
	else
		return 1;
}

/******************************************************************************
����ԭ�ͣ�	void NRF24L01_Init(uint8_t Chanal,uint8_t Mode)
��    �ܣ�	NRF24L01��ʼ��
��    ����	Chanal��RFͨ��
*******************************************************************************/
void NRF24L01_Init(uint8_t Chanal,uint8_t Mode)
{
	NRF_CE_L;
		
	NRF_Write_Reg(FLUSH_TX,0xff);//��շ��ͻ�����
	NRF_Write_Reg(FLUSH_RX,0xff);//��ս��ջ�����
	NRF_Write_Buf(NRF_WRITE_REG + TX_ADDR,   TX_ADDRESS,5); //дTX�ڵ��ַ  	
	NRF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0,RX_ADDRESS,5);	//дRX�ڵ��ַ 

	NRF_Write_Reg(NRF_WRITE_REG + EN_AA,     0x01); //ʹ��ͨ��0���Զ�Ӧ�� 
	NRF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);	//ʹ��ͨ��0�Ľ��յ�ַ 
	NRF_Write_Reg(NRF_WRITE_REG + SETUP_RETR,0x1a);	//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10�� 
	NRF_Write_Reg(NRF_WRITE_REG + RF_CH,   Chanal);	//����RFͨ��ΪCHANAL
	NRF_Write_Reg(NRF_WRITE_REG + RX_PW_P0,    32);	//����ͨ��0����Ч���ݿ��
	NRF_Write_Reg(NRF_WRITE_REG + RF_SETUP,  0x0f); //����TX�������,0db����,2Mbps,���������濪��
	
	if(Mode==TX)
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0E);//����
	else if(Mode==RX)
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0F);//����
	
	NRF_CE_H;
}
void Task_StorePara(struct __Rc * rc);
void Task_ReadPara(struct __Rc *rc);
///******************************************************************************
//����ԭ�ͣ�	static void NRF24L01_Analyse(void)
//��    �ܣ�	����NRF24L01�յ�������֡
//*******************************************************************************/
static void NRF24L01_Analyse(void)
{
////	if((NRF24L01_RXDATA[12] == 0xff)&&(NRF24L01_RXDATA[15] == 0xaa))
////	{
		Rc = *(struct __Rc*)NRF24L01_RXDATA;
//		Task_StorePara(&Rc);
////	}

}

/******************************************************************************
����ԭ�ͣ�	void Send_Data_Back(void)
��    �ܣ�	��ң�ط�������
*******************************************************************************/
void Send_Data_Back(void)
{
	TX_PackageFrame();//���
	NRF_Send_TX((u8*)&tx_frame,32);
}

/******************************************************************************
����ԭ�ͣ�	void NRF24L01_IRQ(void)
��    �ܣ�	NRF24L01�ж�
*******************************************************************************/
void NRF24L01_HandlerIRQ(void)	
{
	uint8_t status = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	
	if(status & (1<<RX_DR))//�����ж�
	{	
		uint8_t rx_len = NRF_Read_Reg(R_RX_PL_WID);
		if(rx_len==32)
		{
			NRF_Read_Buff(RD_RX_PLOAD,NRF24L01_RXDATA,rx_len);//��ȡ����FIFO����
			Nrf_Erro = 0;
		}
		else
		{
			NRF_Write_Reg(FLUSH_RX,0xff);//��ս��ջ�����
		}
	}
	if(status & (1<<MAX_RT))//�ﵽ�����ط��ж�
	{
		if(status & (1<<TX_FULL))//TX FIFO ���
		{
			NRF_Write_Reg(FLUSH_TX,0xff);//��շ��ͻ�����
		}
	}
//	if(status&(1<<TX_DS))//�������
//	{	
		NRF24L01_Set_RX();//����Nrf2401Ϊ����ģʽ
//	}
	NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, status);//����жϱ�־λ	
}

/******************************************************************************
����ԭ�ͣ�	void Nrf_Connect(void)
��    �ܣ�	NRF24L01���Ӻ���
*******************************************************************************/
void Nrf_Connect(void)//1KHZ
{
	Nrf_Erro ++;
	if(Nrf_Erro==1)
	{
		NRF24L01_Analyse();//����NRF24L01�յ�������֡
		Send_Data_Back();  //��ң�ط������� ���������ݺ�����TX_RD�ж�
	}
	if(Nrf_Erro%100==0)  //0.1sδ����nrf���� ����ͼ����ң��
	{	
		NRF24L01_HandlerIRQ();    //����жϱ�־λ ����Ϊ����ģʽ
	}
	if(Nrf_Erro>=1000)   //1sδ����nrf���� ,�������ᣬ���ͣת����ֹ���ⷢ��
	{	

	}

}
