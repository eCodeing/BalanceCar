#include  "spi1.h"
void SPI1_Init(void)
{
	  SPI_InitTypeDef  SPI_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1,ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		 /* NSS---->GPIO(LED) */
	  SPI_SSOutputCmd(SPI1, ENABLE);
        //GPIOFB3,4,5初始化设置
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
      GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1
     
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);	
	/* SPI1 Config -------------------------------------------------------------*/
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI1, &SPI_InitStructure);
	  /* Enable SPI1 */
	  SPI_Cmd(SPI1, ENABLE);
}


