#ifndef __KEY_TASK_H__
#define __KEY_TASK_H__

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#define KEY1_Port GPIOA
#define KEY1_Pin GPIO_Pin_0
#define KEY2_Port GPIOC
#define KEY2_Pin GPIO_Pin_13
#define KEY1_GPIO_CLK_ENABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define KEY1_GPIO_CLK_DISABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE)
#define KEY2_GPIO_CLK_ENABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define KEY2_GPIO_CLK_DISABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE)
void KeyTask(void *pvParameters);
#endif
