#ifndef DHT11TASK_H
#define DHT11TASK_H

#include "stm32f10x.h"
#include "DHT11.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "USART1.h"

void DHT11Task(void *pvParameters);

#endif
