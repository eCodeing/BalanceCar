#ifndef  __IOIIC_H
#define __IOIIC_H
#include  "sys.h"


#define SDA_IN()  {GPIOA->MODER&=~(3<<(15*2));GPIOA->MODER|=0<<15*2;}	//PA15 in
#define SDA_OUT() {GPIOA->MODER&=~(3<<(15*2));GPIOA->MODER|=1<<15*2;} //PA15 out
#define  IIC_SCL        PCout(10)
#define  IIC_SDA_W   PAout(15)
#define  IIC_SDA_R    PAin(15)


void IIC_Init(void);
int  IIC_Start(void);
int  IIC_Start(void);
void  IIC_Stop(void);
void  IIC_Ack(void);
void  IIC_nAck(void);
uint8_t  IIC_Wait_Ack(void);
void  IIC_Send_Byte(uint8_t data);
uint8_t IIC_Read_Byte(uint8_t is_ack);

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 IICreadByte(u8 dev, u8 reg, u8 *data);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data);
#endif

