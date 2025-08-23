#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f10x.h" // Device header
#include "delay.h"

#define dht11_high GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define dht11_low GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define Read_Data GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define DHT11_DATA_Pin GPIO_Pin_12
#define DHT11_DATA_GPIO_CLK_ENABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#define DHT11_DATA_GPIO_Port GPIOB
extern uint8_t rec_data[4];
void DHT11_GPIO_Init_OUT(void);

void DHT11_GPIO_Init_IN(void);
void DHT11_Start(void);
unsigned char DHT11_Rec_Byte(void);
void DHT11_Rec_Data(void);

#endif
