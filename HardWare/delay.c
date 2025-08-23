#include "stm32f10x.h"

/**
 * @brief  微秒级延时
 * @param  xus 延时时长，范围：0~233015
 * @retval 无
 */

// void delay_us(uint32_t xus)
// {
// 	SysTick->LOAD = 72 * xus;	// 设置定时器重装值
// 	SysTick->VAL = 0x00;		// 清空当前计数值
// 	SysTick->CTRL = 0x00000005; // 设置时钟源为HCLK，启动定时器
// 	while (!(SysTick->CTRL & 0x00010000))
// 		;						// 等待计数到0
// 	SysTick->CTRL = 0x00000004; // 关闭定时器
// }
uint16_t fac_us;
uint16_t fac_ms;
void delay_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
}
void delay_us(uint32_t nus)
{
	/*
	嘀嗒定时器实现
	*/
	// SysTick->LOAD = fac_us * xus;			  // 设置定时器重装值
	// SysTick->VAL = 0x00;					  // 清空当前计数值
	// SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 启动定时器
	// while (!(SysTick->CTRL & 0x00010000))
	// 	;									 // 等待计数到0
	// SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; // 关闭定时器
	// SysTick->VAL = 0x00;

	// RCC->APB1ENR |= 1 << 2;

	TIM5->CNT = nus - 1;	  // 向计数器装要递减的数，减到0后会触发定时器的TIM_FLAG_UpDate标志位
	TIM5->CR1 |= TIM_CR1_CEN; // 使能计数器， 计数器开始递减
	while ((TIM5->SR & TIM_FLAG_Update) != SET)
		;						  // 等到计数器减到0
	TIM5->CR1 &= (~TIM_CR1_CEN);  // 关闭计数器
	TIM5->SR &= ~TIM_FLAG_Update; // 清除定时器变为0的标志位
}

/**
 * @brief  毫秒级延时
 * @param  xms 延时时长，范围：0~4294967295
 * @retval 无
 */
void delay_ms(uint32_t xms)
{
	while (xms--)
	{
		delay_us(1000);
	}
}

/**
 * @brief  秒级延时
 * @param  xs 延时时长，范围：0~4294967295
 * @retval 无
 */
void delay_s(uint32_t xs)
{
	while (xs--)
	{
		delay_ms(1000);
	}
}
