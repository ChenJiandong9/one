#ifndef __KEY_H__
#define __KEY_H__
#include "FreeRTOS.h"
#include "Task.h"
#include "stm32f10x.h"
#include "delay.h"
#include "usart1.h"
#define KEY_GPIO_CLK_ENABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define KEY_GPIO_CLK_DISABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE)
#define KEY_GPIO_PORT GPIOA
#define KEY_GPIO_PIN GPIO_Pin_0
#define KEY_GPIO_PIN_SOURCE GPIO_PinSource0
#define KEY_GPIO_EXTI_PORT_SOURCE GPIO_PortSourceGPIOA
#define KEY_GPIO_EXTI_LINE EXTI_Line0
#define KEY_GPIO_EXTI_IRQ EXTI0_IRQn

void Key_Init(void);
#endif
