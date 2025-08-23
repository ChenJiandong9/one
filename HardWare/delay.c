#include "stm32f10x.h"

/**
 * @brief  ΢�뼶��ʱ
 * @param  xus ��ʱʱ������Χ��0~233015
 * @retval ��
 */

// void delay_us(uint32_t xus)
// {
// 	SysTick->LOAD = 72 * xus;	// ���ö�ʱ����װֵ
// 	SysTick->VAL = 0x00;		// ��յ�ǰ����ֵ
// 	SysTick->CTRL = 0x00000005; // ����ʱ��ԴΪHCLK��������ʱ��
// 	while (!(SysTick->CTRL & 0x00010000))
// 		;						// �ȴ�������0
// 	SysTick->CTRL = 0x00000004; // �رն�ʱ��
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
	��શ�ʱ��ʵ��
	*/
	// SysTick->LOAD = fac_us * xus;			  // ���ö�ʱ����װֵ
	// SysTick->VAL = 0x00;					  // ��յ�ǰ����ֵ
	// SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // ������ʱ��
	// while (!(SysTick->CTRL & 0x00010000))
	// 	;									 // �ȴ�������0
	// SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; // �رն�ʱ��
	// SysTick->VAL = 0x00;

	// RCC->APB1ENR |= 1 << 2;

	TIM5->CNT = nus - 1;	  // �������װҪ�ݼ�����������0��ᴥ����ʱ����TIM_FLAG_UpDate��־λ
	TIM5->CR1 |= TIM_CR1_CEN; // ʹ�ܼ������� ��������ʼ�ݼ�
	while ((TIM5->SR & TIM_FLAG_Update) != SET)
		;						  // �ȵ�����������0
	TIM5->CR1 &= (~TIM_CR1_CEN);  // �رռ�����
	TIM5->SR &= ~TIM_FLAG_Update; // �����ʱ����Ϊ0�ı�־λ
}

/**
 * @brief  ���뼶��ʱ
 * @param  xms ��ʱʱ������Χ��0~4294967295
 * @retval ��
 */
void delay_ms(uint32_t xms)
{
	while (xms--)
	{
		delay_us(1000);
	}
}

/**
 * @brief  �뼶��ʱ
 * @param  xs ��ʱʱ������Χ��0~4294967295
 * @retval ��
 */
void delay_s(uint32_t xs)
{
	while (xs--)
	{
		delay_ms(1000);
	}
}
