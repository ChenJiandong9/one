#include "RGB.h"
/**
ռ����Դ TIM4ͨ��3  DAMͨ��5 GPIOB8
 */

uint16_t GRB[24];
void RGB_Init(void)
{

    // ��������
    RGB_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = RGB_IN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RGN_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(RGN_GPIO_PORT, RGB_IN);

    // ���ö�ʱ��
    RGB_TIM_CLK_ENABLE();
    // ʱ����Ԫ
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 90 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(RGB_TIM, &TIM_TimeBaseStructure);
    // �Ƚ������Ԫ
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(RGB_TIM, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(RGB_TIM, TIM_OCPreload_Enable);
    TIM_Cmd(RGB_TIM, ENABLE);

    RGB_DMA_Init();
}
void RGB_DMA_Init(void)
{
    RGB_DMA_CLK_ENABLE();
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    // ���䷽��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;          // �Ƿ��ڴ浽�ڴ�,Ӳ�����������������
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;         // ����ģʽ���Ƿ��Զ���װ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // ͨ�����ȼ�
    DMA_InitStructure.DMA_BufferSize = 24;                // ��������������������

    /*
    ����Ĵ�����������
    1.��ַ    2.���ݳ���    3.�Ƿ�����
    */
    DMA_InitStructure.DMA_PeripheralBaseAddr = RGB_PWM_ADDR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /*
    �洢����������
    1.��ַ    2.���ݳ���    3.�Ƿ�����
    */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)GRB;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    // DMA1ͨ��5������ʼ��
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    TIM_DMACmd(TIM4, TIM_DMA_CC3, ENABLE);
    // DMA_Cmd(DMA1_Channel5, DISABLE);
    //    TIM_DMACmd(RGB_TIM, TIM_DMA_Update, ENABLE);
}
void RGB_Set(uint8_t R, uint8_t G, uint8_t B)
{

    // ��R��G��B��ÿһλת����pwm���͵�����֡
    for (uint8_t i = 0; i < 8; i++)
    {
        if (G & (1 << i))
        {
            GRB[i] = 68;
        }
        else
        {
            GRB[i] = 22;
        }
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        if (R & (1 << i))
        {
            GRB[i+8] = 68;
        }
        else
        {
            GRB[i+16] = 22;
        }
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        if (B & (1 << i))
        {
            GRB[i+16] = 68;
        }
        else
        {
            GRB[i+16] = 22;
        }
    }

    // ����dma������

    DMA_SetCurrDataCounter(DMA1_Channel5, 24); // �������ü�����

    DMA_Cmd(DMA1_Channel5, ENABLE); // ʹ��DMA
    TIM_Cmd(RGB_TIM, ENABLE);       // ʹ��TIM
    while (!DMA_GetFlagStatus(DMA1_FLAG_TC5))
        ;                            // �ȴ�DMAͨ���������
    DMA_Cmd(DMA1_Channel5, DISABLE); // ʧ��DMA
    DMA_ClearFlag(DMA1_FLAG_TC5);    // ���DMA������ɱ�־
    // delay_us(2);
    TIM_Cmd(RGB_TIM, DISABLE); // ʧ��TIM
    // ʹ��TIM��DMA����
}
