#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f10x.h"
#include "delay.h"

#define CW 0  // 顺时针
#define CCW 1 // 逆时针
#define MOTOR_GPIO_CLK_ENABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE)
#define MOTOR_GPIO_CLK_DISABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, DISABLE)
#define Motor_PWM_Time TIM4
#define Motor_Time_CLK_ENABLE() RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE)

/****************************************
定义四个Motor的引脚
*****************************************/
#define MOTORA_AIN1 GPIO_Pin_6
#define MOTORA_AIN2 GPIO_Pin_7
#define MOTORA_Port GPIOB
//////////////////TIM2对应MotorA的编码器
#define MOTORA_ENCODER_A_PIN GPIO_Pin_0
#define MOTORA_ENCODER_B_PIN GPIO_Pin_1
#define MOTORA_ENCODER_Port GPIOA

#define MOTORB_BIN1 GPIO_Pin_8
#define MOTORB_BIN2 GPIO_Pin_9
#define MOTORB_Port GPIOB
/////////////////TIM3对应MotorB的编码器
#define MOTORB_ENCODER_A_PIN GPIO_Pin_6
#define MOTORB_ENCODER_B_PIN GPIO_Pin_7
#define MOTORB_ENCODER_Port GPIOA
#define e 44 // 单个脉冲前进距离
#define PI 3.1415926535
// #define MOTORC_CIN1 GPIO_Pin_5
// #define MOTORC_CIN2 GPIO_Pin_6
// #define MOTORC_PWM_PIN GPIO_Pin_10
// #define MOTORC_PWM_Port GPIOA
// #define MOTORC_Port GPIOB

// #define MOTORD_DIN1 GPIO_Pin_7
// #define MOTORD_DIN2 GPIO_Pin_8
// #define MOTORD_PWM_PIN GPIO_Pin_11
// #define MOTORD_PWM_Port GPIOA
// #define MOTORD_Port GPIOB

typedef enum
{
    MotorA,
    MotorB,
    MotorC,
    MotorD

} Motor;

typedef struct
{
    uint8_t Motor_Direction; // 0:正转 1:反转
    float Motor_Speed;       // 0~65535
    Motor Motor_Num;

} Motor_TypeDef;
void Motor_Init(void);

void Motor_GetState(Motor_TypeDef *Motor);
int16_t Motor_GetCounter(Motor_TypeDef *Motor);
void Motor_SetSpeed(Motor_TypeDef *Motor);

#endif
