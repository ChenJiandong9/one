
#include "Key.h"

void Key_Init(void)
{
    KEY_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);

    GPIO_AFIODeInit();
    GPIO_EXTILineConfig(KEY_GPIO_EXTI_PORT_SOURCE, KEY_GPIO_PIN_SOURCE);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = KEY_GPIO_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = KEY_GPIO_EXTI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler()
{
    if (EXTI_GetITStatus(KEY_GPIO_EXTI_LINE) != RESET)
    {
        // delay_ms(10);
        if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == 1)
        {
            // delay_ms(30);
            EXTI_ClearITPendingBit(EXTI_Line0);
            // USART1_SendString((uint8_t *)"KEY_PRESS");
        }
    }
}
