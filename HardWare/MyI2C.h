#ifndef __MyI2C_H__
#define __MyI2C_H__

#include "stm32f10x.h"
#include "Delay.h"
#define I2C_READ 0x01
#define I2C_WRITE 0x00
#define I2C_SCL GPIO_Pin_10
#define I2C_SDA GPIO_Pin_11
#define I2C_Pport GPIOB
#define I2C_GPIO_CLK_ENABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
void MyI2C_Start(void);
void MyI2C_Init(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);
uint8_t MyI2C_ADDWR(uint8_t addr, uint8_t w_r);
#endif
