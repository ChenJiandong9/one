#include "RGB.h"
/**
占用资源 TIM4通道3  DAM通道5 GPIOB8
 */

uint16_t GRB[24];
void RGB_Init(void)
{

    // 配置引脚
    RGB_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = RGB_IN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RGN_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(RGN_GPIO_PORT, RGB_IN);

    // 配置定时器
    RGB_TIM_CLK_ENABLE();
    // 时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 90 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(RGB_TIM, &TIM_TimeBaseStructure);
    // 比较输出单元
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
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    // 传输方向
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;          // 是否内存到内存,硬件触发还是软件触发
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;         // 传输模式，是否自动重装
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // 通道优先级
    DMA_InitStructure.DMA_BufferSize = 24;                // 传输次数，即传输计数器

    /*
    外设寄存器参数设置
    1.地址    2.数据长度    3.是否自增
    */
    DMA_InitStructure.DMA_PeripheralBaseAddr = RGB_PWM_ADDR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /*
    存储器参数设置
    1.地址    2.数据长度    3.是否自增
    */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)GRB;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    // DMA1通道5参数初始化
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    TIM_DMACmd(TIM4, TIM_DMA_CC3, ENABLE);
    // DMA_Cmd(DMA1_Channel5, DISABLE);
    //    TIM_DMACmd(RGB_TIM, TIM_DMA_Update, ENABLE);
}
void RGB_Set(uint8_t R, uint8_t G, uint8_t B)
{

    // 将R，G，B的每一位转换成pwm发送的数据帧
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

    // 调用dma发数据

    DMA_SetCurrDataCounter(DMA1_Channel5, 24); // 重新设置计数器

    DMA_Cmd(DMA1_Channel5, ENABLE); // 使能DMA
    TIM_Cmd(RGB_TIM, ENABLE);       // 使能TIM
    while (!DMA_GetFlagStatus(DMA1_FLAG_TC5))
        ;                            // 等待DMA通道发送完成
    DMA_Cmd(DMA1_Channel5, DISABLE); // 失能DMA
    DMA_ClearFlag(DMA1_FLAG_TC5);    // 清除DMA发送完成标志
    // delay_us(2);
    TIM_Cmd(RGB_TIM, DISABLE); // 失能TIM
    // 使能TIM的DMA请求
}
