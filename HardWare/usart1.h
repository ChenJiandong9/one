#ifndef __USART1_H__
#define __USART1_H__

#include "stm32f10x.h"

void USART1_Init(u32 bound);
void USART1_SendString(uint8_t *str);
void USART1_SendByte(uint8_t byte);
uint8_t USART1_ReceiveByte(void);
void USART1_Send(uint8_t *Data, uint16_t len);

#endif
