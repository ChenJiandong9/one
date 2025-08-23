#ifndef __RGB_H__
#define __RGB_H__
#include "stm32f10x.h"
#include "DMA.c"
#define RGB_IN GPIO_Pin_8
#define RGN_GPIO_PORT GPIOB
#define RGB_GPIO_CLK_ENABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#define RGB_TIM TIM4
#define RGB_TIM_CLK_ENABLE() RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
#define RGB_DMA_CLK_ENABLE() RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#define RGB_PWM_ADDR ((uint32_t)&TIM4->CCR3)
extern uint16_t GRB[24];
void RGB_Init(void);
void RGB_Set(uint8_t R, uint8_t G, uint8_t B);
void RGB_DMA_Init(void);
#endif
