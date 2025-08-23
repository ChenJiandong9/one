#ifndef __BH1750_H__
#define __BH1750_H__

#include "stm32f10x.h"
#include "MyI2C.h"
#include "delay.h"

#define BH1750_ADDRESS 0x23          // BH1750�ĵ�ַ
#define BH1750_ORDER_POWER 0x01       // ͨ������
#define BH1750_ORDER_MEASUREMENT 0x10 // ��������
#define BH1750_RESOLUTION 1
float BH1750_ReadLux(void);
void BH1750_Init(void);

#endif
