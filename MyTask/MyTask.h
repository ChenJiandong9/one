#ifndef __MY_TASK_H__
#define __MY_TASK_H__
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "DHT11Task.h"
#include "BH1750.h"
#define STK_SIZE 256
#define HIGH_PRIORITY 3
#define MID_PRIORITY 2
#define LOW_PRIORITY 1
void ReadLuxTask(void *pvParameters);
#endif
